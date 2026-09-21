<?php
// HTTP tests for download.php, using the fake generator from lib.php.

function test_download_requires_session(): void
{
    $client = new Client();
    foreach (['get', 'post'] as $method) {
        $page = $client->$method('download.php');
        check($page->status === 302, "$method: status is 302 (got $page->status)");
        check($page->header('Location') === './index.php', "$method: redirects to index.php");
    }
    check(generator_runs() === [], 'generator does not run');
}

function test_download_zip(): void
{
    $client = new Client();
    register($client, 'ann@b.com');
    $page = $client->post('download.php');
    check($page->status === 200, "status is 200 (got $page->status)");
    check($page->header('Content-Type') === 'application/zip', 'content type is application/zip');
    check($page->header('Content-Disposition') === 'attachment; filename="pointless.zip"', 'downloads as pointless.zip');
    $seed = player_seed('ann@b.com');
    check($page->body === "seed=$seed", "body is the player's generated zip");
    check((int)$page->header('Content-Length') === strlen($page->body), 'content length matches');
    check(list_tree(tmp_path()) === [], 'nothing is left in TMPDIR');
}

function test_download_canceled(): void
{
    fake_generator('big');
    $client = new Client();
    register($client, 'ann@b.com');
    $client->postAndHangUp('download.php');
    // php -S finishes the request after the client is gone; give it time.
    for ($i = 0; $i < 100 && list_tree(tmp_path()) !== []; $i++) {
        usleep(100000);
    }
    check(list_tree(tmp_path()) === [], 'nothing is left in TMPDIR after a canceled download');
    $seed = player_seed('ann@b.com');
    check(filesize(games_path() . "/$seed.zip") === 64 << 20, 'the whole zip is stored');
}

function test_download_same_file(): void
{
    $first = new Client();
    register($first, 'ann@b.com');
    $zip = $first->post('download.php')->body;
    $second = new Client(); // A new session, so the rate limit doesn't apply.
    sign_in($second, 'ann@b.com');
    check($second->post('download.php')->body === $zip, 'downloading again gives the same file');
    check(count(generator_runs()) === 1, 'the generator runs only for the first download');
    check(is_file(games_path() . '/' . player_seed('ann@b.com') . '.zip'), 'the zip stays on the server');
}

function test_download_uses_session_email(): void
{
    $ann = new Client();
    register($ann, 'ann@b.com');
    $bo = new Client();
    register($bo, 'bo@b.com');
    check($bo->post('download.php')->body === 'seed=' . player_seed('bo@b.com'), 'second player gets their own zip');
    check($ann->post('download.php')->body === 'seed=' . player_seed('ann@b.com'), 'first player gets their own zip');
}

function test_download_rate_limited(): void
{
    $client = new Client();
    register($client, 'ann@b.com');
    check($client->post('download.php')->header('Content-Type') === 'application/zip', 'first download works');
    $page = $client->post('download.php');
    check($page->header('Content-Type') !== 'application/zip', 'an immediate second download is refused');
    check($page->contains('Please wait'), 'says to wait');
    check(count(generator_runs()) === 1, 'generator runs only once');
}

function test_download_generator_missing(): void
{
    fake_generator('missing');
    $client = new Client();
    register($client, 'ann@b.com');
    $page = $client->post('download.php');
    check($page->status === 200, "status is 200 (got $page->status)");
    check($page->header('Content-Type') !== 'application/zip', 'no zip is sent');
    check($page->contains('The puzzle generator is not available'), 'shows the not available error');
}

function test_download_generator_fails(): void
{
    fake_generator('fail');
    $client = new Client();
    register($client, 'ann@b.com');
    $page = $client->post('download.php');
    check($page->header('Content-Type') !== 'application/zip', 'no zip is sent');
    check($page->contains('Puzzle generation failed'), 'shows the generation failed error');
    check(list_tree(tmp_path()) === [], 'nothing is left in TMPDIR');
}

// The health bar's aria-valuenow for $level of the fake game's pages reached.
function health(int $level): string
{
    return 'aria-valuenow="' . (int)round(100 * $level / FAKE_PUZZLES) . '"';
}

function test_download_shows_progress(): void
{
    $client = new Client();
    register($client, 'ann@b.com');
    check(!$client->get('download.php')->contains('health-bar'), 'no health bar before the game is downloaded');

    $client->post('download.php');
    $seed = player_seed('ann@b.com');
    check($client->get('download.php')->contains(health(0)), 'a new player has an empty bar');
    $page = $client->post('download.php', ['token' => fake_token(3, $seed)]);
    check($page->contains('Proof of progress accepted'), 'a token is accepted');
    check($page->contains(health(3)), 'the bar shows the new level');
    check($page->header('Content-Type') !== 'application/zip', 'submitting a token does not download the zip');
    check(count(generator_runs()) === 1, 'and does not run the generator again');
}

function test_download_bad_token(): void
{
    $client = new Client();
    register($client, 'ann@b.com');
    $client->post('download.php');
    $page = $client->post('download.php', ['token' => 'NOTATOKEN']);
    check($page->contains('not one of your proofs of progress'), 'a wrong token is refused');
    check($page->contains(health(0)), 'the level does not move');
    $rows = event_rows();
    check($rows[count($rows) - 1][1] === 'token-bad', 'the attempt is logged');
}

function test_download_token_rate_limited(): void
{
    $client = new Client();
    register($client, 'ann@b.com');
    $client->post('download.php');
    $seed = player_seed('ann@b.com');
    check($client->post('download.php', ['token' => fake_token(2, $seed)])->contains('Proof of progress accepted'), 'the first token works');
    $page = $client->post('download.php', ['token' => fake_token(3, $seed)]);
    check($page->contains('Please wait'), 'an immediate second token is refused');
    check($page->contains(health(2)), 'the refused token does not count');
    check(count(event_rows()) === 3, 'and is not logged: register, download, and the accepted token only');
}

function test_download_logs_events(): void
{
    $client = new Client();
    register($client, 'ann@b.com');
    $client->post('download.php');
    $rows = event_rows();
    check(count($rows) === 2, 'registering and downloading are both logged (got ' . count($rows) . ')');
    check(array_slice($rows[0], 1, 3) === ['register', 'ann@b.com', 'Test Player'], 'registration is logged with the name');
    check(array_slice($rows[1], 1, 3) === ['download', 'ann@b.com', player_seed('ann@b.com') . '.zip'],
          'the download is logged with the file served');
}

function test_download_health_bar(): void
{
    $client = new Client();
    register($client, 'ann@b.com');
    $client->post('download.php');
    $seed = player_seed('ann@b.com');
    $page = $client->post('download.php', ['token' => fake_token(3, $seed)]);
    check(substr_count($page->body, 'class="health-bar"') === 1, 'one bar, not one segment per puzzle');
    foreach (['Puzzle ', 'Puzzles solved', 'of ' . FAKE_PUZZLES, 'of ' . (FAKE_PUZZLES - 1), 'You are on puzzle', 'solved 2'] as $count) {
        check(!$page->contains($count), "shows no count or total ($count)");
    }

    $second = new Client(); // A new session, so the token rate limit doesn't apply.
    sign_in($second, 'ann@b.com');
    $page = $second->post('download.php', ['token' => fake_token(FAKE_PUZZLES, $seed)]);
    check($page->contains('aria-valuenow="100"'), 'the fin page\'s token fills the bar');
    check($page->contains('completed the challenge'), 'and finishes the game');
}

function test_download_progress_first(): void
{
    $client = new Client();
    register($client, 'ann@b.com');
    $client->post('download.php');
    $seed = player_seed('ann@b.com');
    $page = $client->get('download.php');
    check(strpos($page->body, 'id="downloadForm"') < strpos($page->body, 'Your Progress'),
          'with no proof of progress, the download comes first');

    $page = $client->post('download.php', ['token' => fake_token(1, $seed)]);
    check(strpos($page->body, 'Your Progress') < strpos($page->body, 'id="downloadForm"'),
          'once the first page\'s proof is in, progress comes first');
    check($page->contains(health(1)), 'and the bar counts it');
}

function test_download_messages_by_their_forms(): void
{
    $client = new Client();
    register($client, 'ann@b.com');
    $client->post('download.php');
    $page = $client->post('download.php', ['token' => 'NOTATOKEN']);
    $message = strpos($page->body, 'not one of your proofs of progress');
    check($message > strpos($page->body, 'Your Progress') && $message < strpos($page->body, 'id="tokenForm"'),
          'a token message is in the progress section, above its form');

    $page = $client->post('download.php'); // Too soon after the first download.
    $message = strpos($page->body, 'Please wait');
    check($message < strpos($page->body, 'id="downloadForm"') && $message > strpos($page->body, 'Pointless Challenge.'),
          'a download message is by the download button');
    check($message < strpos($page->body, 'Your Progress'), 'not in the progress section');
}

function test_download_share_links(): void
{
    $client = new Client();
    register($client, 'ann@b.com');
    $client->post('download.php');
    $seed = player_seed('ann@b.com');
    check(!$client->get('download.php')->contains('Share Your Progress'), 'nothing to share before a proof of progress');
    $page = $client->post('download.php', ['token' => fake_token(1, $seed)]);
    check($page->contains('Share Your Progress'), 'the first page\'s proof can be shared');
    check($page->contains('making%20progress'), 'the post says the player is making progress');
    $id = pointless_share_id($seed);
    check($page->contains(rawurlencode("https://tools.lipscomb-soc.org/pointless/share.php?p=$id")),
          "links point at the player's page on the live site");
    check($page->contains('action="share.php"') && $page->contains("name=\"p\" value=\"$id\""),
          'a button opens the player\'s own page');
    check($page->contains('aria-label="Post on Facebook"') && $page->contains('<svg'), 'the links are labeled icons');
    check(!$page->contains($seed), 'the seed is not shown');
    foreach (['x.com', 'facebook.com', 'linkedin.com', 'bsky.app', 'threads.net', 'reddit.com'] as $site) {
        check($page->contains("https://www.$site/") || $page->contains("https://$site/"), "links to $site");
    }
    check(!$page->contains('connect.facebook.net'), 'loads no social media scripts');

    $second = new Client(); // A new session, so the token rate limit doesn't apply.
    sign_in($second, 'ann@b.com');
    $page = $second->post('download.php', ['token' => fake_token(FAKE_PUZZLES, $seed)]);
    check($page->contains('I%20finished%20the%20Pointless%20Challenge'), 'the post says the game is finished');
}
