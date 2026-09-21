<?php
// HTTP tests for index.php, the only way in: it registers new players, signs
// registered ones back in, and takes an optional token on the way through.

function test_index_get(): void
{
    $page = (new Client())->get('index.php');
    check($page->status === 200, "status is 200 (got $page->status)");
    check($page->contains('<input type="email" id="email" name="email" value="" autocomplete="email" required>'), 'has email input');
    check($page->contains('<input type="text" id="fname" name="fname" value="" autocomplete="given-name">'), 'has an optional first name input');
    check($page->contains('<input type="text" id="lname" name="lname" value="" autocomplete="family-name">'), 'has an optional last name input');
    check($page->contains('<input type="text" id="token" name="token" value="" autocomplete="off" spellcheck="false">'), 'has an optional token input');
    check($page->contains('<button type="submit">Submit</button>'), 'has submit button');
    check(!$page->contains('login.php'), 'there is no separate login page to link to');
}

function test_index_register(): void
{
    $client = new Client();
    $page = register($client, 'ann@b.com', 'Ann', 'Lee');
    check($page->status === 302, "status is 302 (got $page->status)");
    check($page->header('Location') === 'download.php', 'redirects to download.php');
    check(count(player_rows()) === 2 && array_slice(player_rows()[1], 0, 3) === ['Ann', 'Lee', 'ann@b.com'], 'player is saved');
    check(preg_match('/^\d+$/', player_seed('ann@b.com')) === 1, 'player gets a seed');
    check(generator_runs() === [], 'registering does not run the generator');
    check(count(event_rows()) === 1, 'only the registration is logged: an empty token submits nothing');

    $download = $client->get('download.php');
    check($download->status === 200, 'session allows the download page');
    check($download->contains('Pointless Download'), 'download page has the download button');
}

function test_index_session_cookie(): void
{
    $page = register(new Client(), 'ann@b.com');
    $cookies = $page->headers('Set-Cookie');
    check(count($cookies) >= 1 && str_starts_with($cookies[0], 'pointless='), 'session cookie is named pointless, not PHPSESSID');
    foreach ($cookies as $cookie) {
        check(stripos($cookie, 'httponly') !== false, "cookie is HttpOnly ($cookie)");
        check(stripos($cookie, 'samesite=lax') !== false, "cookie is SameSite=Lax ($cookie)");
    }
}

function test_index_invalid_email(): void
{
    $client = new Client();
    $page = register($client, 'not-an-email');
    check($page->status === 200, "status is 200 (got $page->status)");
    check($page->contains('INVALID EMAIL'), 'shows the invalid email error');
    check(player_rows() === [], 'nothing is saved');
    check($client->get('download.php')->status === 302, 'no session is started');
}

function test_index_email_with_apostrophe(): void
{
    $client = new Client();
    $page = register($client, "o'brien@b.com");
    check($page->status === 302, "a valid email with ' is accepted (got $page->status)");
    check(player_rows()[1][2] === "o'brien@b.com", 'the email is stored unescaped');
}

function test_index_names_optional(): void
{
    $client = new Client();
    $page = sign_in($client, 'ann@b.com');
    check($page->status === 302, "a nameless registration is accepted (got $page->status)");
    check(array_slice(player_rows()[1], 0, 3) === ['', '', 'ann@b.com'], 'the player is saved with empty names');
    check($client->get('download.php')->status === 200, 'session allows the download page');
    check(array_slice(event_rows()[0], 1, 3) === ['register', 'ann@b.com', ''], 'the registration is logged with no name');
}

function test_index_normalizes_email(): void
{
    register(new Client(), ' Ann@B.COM ', 'Ann', 'Lee');
    check(player_rows()[1][2] === 'ann@b.com', 'the email is stored trimmed and lowercased');
}

function test_index_names_stored_as_typed(): void
{
    register(new Client(), 'x@b.com', '<b>Ann</b>', 'Lee');
    check(player_rows()[1][0] === '<b>Ann</b>', 'names are stored as typed');

    $page = register(new Client(), 'not-an-email', '<b>Ann</b>', 'Lee');
    check($page->contains('value="&lt;b&gt;Ann&lt;/b&gt;"'), 'names are escaped when shown again');
    check(!$page->contains('<b>Ann</b>'), 'names are never shown unescaped');
}

function test_index_returning_player(): void
{
    register(new Client(), 'ann@b.com', 'Ann', 'Lee');
    $client = new Client();
    $page = sign_in($client, 'ann@b.com');
    check($page->status === 302, "status is 302 (got $page->status)");
    check($page->header('Location') === 'download.php', 'a registered email goes straight to the download page');
    check(!$page->contains('DUPLICATE EMAIL'), 'a second visit is not an error');
    check(count(player_rows()) === 2, 'the player is saved only once');
    check($client->get('download.php')->status === 200, 'session allows the download page');
    check(count(event_rows()) === 1, 'signing back in is not a second registration');

    // The players file is append-only, so a returning player's names are ignored.
    register(new Client(), 'ann@b.com', 'Other', 'Person');
    check(count(player_rows()) === 2 && player_rows()[1][0] === 'Ann', 'names typed on a later visit change nothing');
}

function test_index_returning_player_ignores_case(): void
{
    register(new Client(), 'ann@b.com');
    $client = new Client();
    $page = sign_in($client, ' ANN@B.com ');
    check($page->status === 302, "status is 302 (got $page->status)");
    check(count(player_rows()) === 2, 'the same player is not registered twice');
    check($client->post('download.php')->body === 'seed=' . player_seed('ann@b.com'), 'the download uses the normalized email');
}

function test_index_token(): void
{
    $client = new Client();
    register($client, 'ann@b.com');
    $client->post('download.php'); // Generate the game, so there are tokens to match.
    $seed = player_seed('ann@b.com');

    $back = new Client(); // A returning player: email and token in one step.
    $page = sign_in($back, 'ann@b.com', fake_token(3, $seed));
    check($page->status === 302, "status is 302 (got $page->status)");
    check($page->header('Location') === 'download.php', 'a token does not change where the form goes');

    $download = $back->get('download.php');
    check($download->contains('Proof of progress accepted'), 'the download page reports the token');
    check($download->contains('<b>2 of ' . (FAKE_PUZZLES - 1) . '</b>'), 'the level went up');
    check(!$back->get('download.php')->contains('Proof of progress accepted'), 'the message is shown only once');
    $rows = event_rows();
    check(array_slice($rows[count($rows) - 1], 1, 3) === ['token-ok', 'ann@b.com', fake_token(3, $seed)], 'the token is logged');
}

function test_index_bad_token(): void
{
    $client = new Client();
    register($client, 'ann@b.com');
    $client->post('download.php');

    $back = new Client();
    check(sign_in($back, 'ann@b.com', 'NOTATOKEN')->status === 302, 'a wrong token still signs the player in');
    $download = $back->get('download.php');
    check($download->contains('not one of your proofs of progress'), 'the download page says the token is wrong');
    check($download->contains('<b>0 of ' . (FAKE_PUZZLES - 1) . '</b>'), 'the level does not move');
    $rows = event_rows();
    check($rows[count($rows) - 1][1] === 'token-bad', 'the attempt is logged');
}

function test_index_token_rate_limited(): void
{
    $client = new Client();
    register($client, 'ann@b.com');
    $client->post('download.php');
    $seed = player_seed('ann@b.com');

    // One session, so index.php and download.php share the token throttle.
    check(sign_in($client, 'ann@b.com', fake_token(2, $seed))->status === 302, 'the first token is taken');
    check($client->get('download.php')->contains('Proof of progress accepted'), 'and accepted');
    $page = $client->post('download.php', ['token' => fake_token(3, $seed)]);
    check($page->contains('Please wait'), 'an immediate second token is refused');
    check($page->contains('<b>1 of ' . (FAKE_PUZZLES - 1) . '</b>'), 'the refused token does not count');
    check(count(event_rows()) === 3, 'and is not logged: register, download, and the accepted token only');
}
