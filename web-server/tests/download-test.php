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
    $second->post('login.php', ['email' => 'ann@b.com']);
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
