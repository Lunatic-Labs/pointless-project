<?php
// HTTP tests for index.php (registration).

function test_index_get(): void
{
    $page = (new Client())->get('index.php');
    check($page->status === 200, "status is 200 (got $page->status)");
    check($page->contains('<input type="text" id="fname" name="fname" required>'), 'has first name input');
    check($page->contains('<input type="text" id="lname" name="lname" required>'), 'has last name input');
    check($page->contains('<input type="text" id="email" name="email" required>'), 'has email input');
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

function test_index_duplicate_email(): void
{
    register(new Client(), 'ann@b.com', 'Ann', 'Lee');
    $client = new Client();
    $page = register($client, 'ann@b.com', 'Other', 'Person');
    check($page->status === 200, "status is 200 (got $page->status)");
    check($page->contains('DUPLICATE EMAIL'), 'shows the duplicate email error');
    check(count(player_rows()) === 2, 'the player is saved only once');
    check($client->get('download.php')->status === 302, 'no session is started');
}

function test_index_escapes_names(): void
{
    register(new Client(), 'x@b.com', '<b>Ann</b>', 'Lee');
    check(player_rows()[1][0] === '&lt;b&gt;Ann&lt;/b&gt;', 'names are stored HTML-escaped');
}
