<?php
// HTTP tests for login.php.

function test_login_get(): void
{
    $page = (new Client())->get('login.php');
    check($page->status === 200, "status is 200 (got $page->status)");
    check($page->contains('<input type="email" id="email" name="email" value="" autocomplete="email" required>'), 'has email input');
    check($page->contains('<button type="submit">Submit</button>'), 'has submit button');
}

function test_login_registered(): void
{
    register(new Client(), 'ann@b.com');
    $client = new Client();
    $page = $client->post('login.php', ['email' => 'ann@b.com']);
    check($page->status === 302, "status is 302 (got $page->status)");
    check($page->header('Location') === 'download.php', 'redirects to download.php');
    check($client->get('download.php')->status === 200, 'session allows the download page');
}

function test_login_ignores_case(): void
{
    register(new Client(), 'ann@b.com');
    $client = new Client();
    $page = $client->post('login.php', ['email' => ' ANN@B.com ']);
    check($page->status === 302, "status is 302 (got $page->status)");
    check($client->post('download.php')->body === 'email=ann@b.com', 'the download uses the normalized email');
}

function test_login_unregistered(): void
{
    $client = new Client();
    $page = $client->post('login.php', ['email' => 'nobody@b.com']);
    check($page->status === 200, "status is 200 (got $page->status)");
    check($page->contains('Email not registered'), 'shows the not registered error');
    check($client->get('download.php')->status === 302, 'no session is started');
}

function test_login_invalid_email(): void
{
    $page = (new Client())->post('login.php', ['email' => 'not-an-email']);
    check($page->contains('INVALID EMAIL'), 'shows the invalid email error');
}
