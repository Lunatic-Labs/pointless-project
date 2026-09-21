<?php
// HTTP tests for index.php, the only way in: it registers new players, signs
// registered ones back in. Proofs of progress are taken only by download.php.

function test_index_get(): void
{
    $page = (new Client())->get('index.php');
    check($page->status === 200, "status is 200 (got $page->status)");
    check($page->contains('<input type="email" id="email" name="email" value="" autocomplete="email" required>'), 'has email input');
    check($page->contains('<input type="text" id="fname" name="fname" value="" autocomplete="given-name">'), 'has an optional first name input');
    check($page->contains('<input type="text" id="lname" name="lname" value="" autocomplete="family-name">'), 'has an optional last name input');
    check(!$page->contains('name="token"'), 'takes no proof of progress');
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
    check(count(event_rows()) === 1, 'only the registration is logged');

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

function test_index_players_file_unwritable(): void
{
    if (posix_getuid() === 0) {
        skip('root can write a read-only directory');
    }
    $data = dirname(players_path());
    mkdir($data, 0500, true);
    try {
        $client = new Client();
        $page = register($client, 'ann@b.com');
        check($page->contains('Registration failed'), 'shows the registration failed error');
        check($client->get('download.php')->status === 302, 'no session is started');
    } finally {
        chmod($data, 0700); // So reset_state() can delete it.
    }
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

function test_index_ignores_token(): void
{
    $client = new Client();
    register($client, 'ann@b.com');
    $client->post('download.php'); // Generate the game, so there are tokens to match.
    $seed = player_seed('ann@b.com');

    $back = new Client();
    $page = $back->post('index.php', ['email' => 'ann@b.com', 'token' => fake_token(3, $seed)]);
    check($page->header('Location') === 'download.php', 'a token field does not stop the sign in');
    check(!$back->get('download.php')->contains('Proof of progress accepted'), 'but it is not submitted');
    check(count(event_rows()) === 2, 'and not logged: register and download only');
}
