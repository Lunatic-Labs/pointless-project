<?php
// Tests for privacy.php, and the links to it.
require_once __DIR__ . '/../includes/progress.php';

function test_privacy_page(): void
{
    $page = (new Client())->get('privacy.php');
    check($page->status === 200, "status is 200 (got $page->status)");
    check($page->contains('<title>Privacy Policy - Pointless Challenge</title>'), 'the page has its title');
    check(preg_match('/Last updated: \w+ \d+, \d{4}/', $page->body) === 1, 'the page is dated');
    check($page->contains('mailto:dtowell@lipscomb.edu'), 'the page says whom to contact');
    check($page->headers('Set-Cookie') === [], 'reading the policy sets no cookie');
}

function test_privacy_linked(): void
{
    $client = new Client();
    $index = $client->get('index.php');
    check($index->contains('For ages 13 and up. See our <a href="privacy.php">privacy policy</a>.'),
          'the sign-in form points to the policy');
    register($client, 'ann@b.com');
    $seed = player_seed('ann@b.com');
    $pages = ['index.php' => $index, 'download.php' => $client->get('download.php'),
              'share.php' => (new Client())->get('share.php?p=' . pointless_share_id($seed)),
              'privacy.php' => $client->get('privacy.php')];
    foreach ($pages as $url => $page) {
        check($page->contains('<a href="privacy.php">Privacy</a>'), "$url: the footer links to the policy, relatively");
    }
}
