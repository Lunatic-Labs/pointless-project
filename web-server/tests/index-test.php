<?php
// HTTP tests for index.php (registration).

function test_index_get(): void
{
    $page = (new Client())->get('index.php');
    check($page->status === 200, "status is 200 (got $page->status)");
    check($page->contains('<input type="text" id="fname" name="fname" value="" autocomplete="given-name" required>'), 'has first name input');
    check($page->contains('<input type="text" id="lname" name="lname" value="" autocomplete="family-name" required>'), 'has last name input');
    check($page->contains('<input type="email" id="email" name="email" value="" autocomplete="email" required>'), 'has email input');
    check($page->contains('<button type="submit">Submit</button>'), 'has submit button');
    check($page->contains('href="./login.php"'), 'links to login');
}

function test_index_register(): void
{
    $client = new Client();
    $page = register($client, 'ann@b.com', 'Ann', 'Lee');
    check($page->status === 302, "status is 302 (got $page->status)");
    check($page->header('Location') === 'download.php', 'redirects to download.php');
    check(player_rows() === [['FName', 'LName', 'Email'], ['Ann', 'Lee', 'ann@b.com']], 'player is saved');

    $download = $client->get('download.php');
    check($download->status === 200, 'session allows the download page');
    check($download->contains('Pointless Download'), 'download page has the download button');
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

function test_index_requires_names(): void
{
    $page = register(new Client(), 'ann@b.com', '  ', 'Lee');
    check($page->status === 200, "status is 200 (got $page->status)");
    check($page->contains('Please enter your first and last name.'), 'shows the missing name error');
    check(player_rows() === [], 'nothing is saved');
}

function test_index_duplicate_email(): void
{
    register(new Client(), 'ann@b.com', 'Ann', 'Lee');
    $client = new Client();
    $page = register($client, ' ANN@b.com', 'Other', 'Person');
    check($page->status === 200, "status is 200 (got $page->status)");
    check($page->contains('DUPLICATE EMAIL'), 'shows the duplicate email error, ignoring case and spaces');
    check(count(player_rows()) === 2, 'the player is saved only once');
    check($client->get('download.php')->status === 302, 'no session is started');
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
