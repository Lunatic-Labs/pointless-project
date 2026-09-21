<?php
// Tests for share.php, each player's public progress page, and includes/progress.php.
require_once __DIR__ . '/../includes/progress.php';

function test_share_page(): void
{
    $client = new Client();
    register($client, 'ann@b.com', 'Ann', 'Lee');
    $seed = player_seed('ann@b.com');
    $id = pointless_share_id($seed);
    check(preg_match('/^[0-9a-f]{16}$/', $id) === 1, "the share id is 16 hex digits (got $id)");
    check(pointless_find_player_by_share_id($id)[2] === 'ann@b.com', 'the share id finds the player');

    $visitor = new Client(); // Anyone can see it: no session.
    $page = $visitor->get("share.php?p=$id");
    check($page->status === 200, "status is 200 (got $page->status)");
    check($page->contains('<title>Ann Lee - Pointless Challenge</title>'), 'the title has the name');
    check($page->contains('Ann Lee is taking the Pointless Challenge.'), 'the page has the name');
    check($page->contains('<meta property="og:title" content="Ann Lee is taking the Pointless Challenge.">'),
          'so does the preview social media sites show');
    check($page->contains("content=\"https://tools.lipscomb-soc.org/pointless/share.php?p=$id\""), 'og:url is the live page');
    check(!$page->contains('health-bar'), 'no health bar before the game is downloaded');
    check(!$page->contains('ann@b.com') && !$page->contains($seed), 'neither the email nor the seed is shown');

    $client->post('download.php');
    $client->post('download.php', ['token' => fake_token(3, $seed)]);
    $page = $visitor->get("share.php?p=$id");
    check($page->contains(health(3)), 'the page shows the health bar');
    check(!$page->contains('of ' . FAKE_PUZZLES), 'without a count');

    $second = new Client();
    sign_in($second, 'ann@b.com');
    $second->post('download.php', ['token' => fake_token(FAKE_PUZZLES, $seed)]);
    check($visitor->get("share.php?p=$id")->contains('Ann Lee finished the Pointless Challenge!'), 'a finished game says so');
}

function test_share_page_unknown(): void
{
    register(new Client(), 'ann@b.com');
    foreach (['share.php', 'share.php?p=0123456789abcdef', 'share.php?p=' . player_seed('ann@b.com')] as $url) {
        $page = (new Client())->get($url);
        check($page->status === 404, "$url: status is 404 (got $page->status)");
        check($page->contains('There is no player here.'), "$url: says so");
    }
}

function test_share_page_names(): void
{
    $cases = [
        ['nameless@b.com', '', '', 'A player is taking'],
        ['first@b.com', 'Ann', '', 'Ann is taking'],
        ['html@b.com', '<b>Ann</b>', 'Lee', '&lt;b&gt;Ann&lt;/b&gt; Lee is taking'],
        ['formula@b.com', '=Ann', '-Lee', '=Ann -Lee is taking'],
    ];
    foreach ($cases as [$email, $fname, $lname, $shown]) {
        register(new Client(), $email, $fname, $lname);
        $page = (new Client())->get('share.php?p=' . pointless_share_id(player_seed($email)));
        check($page->contains($shown), "shows \"$shown\"");
    }
    check(!(new Client())->get('share.php?p=' . pointless_share_id(player_seed('html@b.com')))->contains('<b>Ann</b>'),
          'names are never shown unescaped');
}
