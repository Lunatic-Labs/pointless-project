<?php
// Tests for health.php, the page the deploy checks after switching releases.

function test_health_ok(): void
{
    mkdir(dirname(players_path()), 0700, true);
    $page = (new Client())->get('health.php');
    check($page->status === 200, "status is 200 (got $page->status)");
    check($page->body === "ok\n", "body is ok (got '$page->body')");
}

function test_health_players_dir_missing(): void
{
    $page = (new Client())->get('health.php');
    check($page->status === 503, "status is 503 (got $page->status)");
    check($page->contains('the players directory is not writable'), 'names the players directory');
    check(!$page->contains(players_path()), 'does not show the path');
}

function test_health_players_file_unwritable(): void
{
    if (posix_getuid() === 0) {
        skip('root can write a read-only file');
    }
    mkdir(dirname(players_path()), 0700, true);
    file_put_contents(players_path(), "FName,LName,Email,Seed\n");
    chmod(players_path(), 0400);
    $page = (new Client())->get('health.php');
    check($page->status === 503, "status is 503 (got $page->status)");
    check($page->contains('the players file is not writable'), 'names the players file');
}

function test_health_generator_missing(): void
{
    mkdir(dirname(players_path()), 0700, true);
    fake_generator('missing');
    $page = (new Client())->get('health.php');
    check($page->status === 503, "status is 503 (got $page->status)");
    check($page->contains('the puzzle generator is not available'), 'names the generator');
}
