<?php
// Showing a player's progress: the health bar, and each player's public share page
// (share.php?p=<share id>), which is what their social media posts link to.
//
// The health bar is deliberately unannotated: it shows how full it is, never how
// many pages there are or how many are reached.
require_once __DIR__ . '/players.php';

// The live site, which share links point at. It is fixed rather than built from the request, so a
// post made from a test server still sends friends to the real site.
const POINTLESS_SITE_URL = 'https://tools.lipscomb-soc.org/pointless/';

// Returns the id in $seed's player's share URL. The seed itself must stay private (it
// rebuilds the player's answers), so the id is a one-way hash of it: public, permanent
// (the seed never changes), and useless for guessing the seed.
function pointless_share_id(string $seed): string
{
    return substr(hash_hmac('sha256', $seed, 'pointless share'), 0, 16);
}

// Returns the row (FName, LName, Email, Seed) of the player whose share id is $id,
// or null if there is none.
function pointless_find_player_by_share_id(string $id): ?array
{
    if (preg_match('/^[0-9a-f]{16}$/', $id) !== 1) {
        return null;
    }
    $file = @fopen(pointless_players_file(), 'r');
    if ($file === false) {
        return null;
    }
    fgetcsv($file); // Skip the header.
    $found = null;
    while (($line = fgetcsv($file)) !== false) {
        if (isset($line[3]) && hash_equals(pointless_share_id($line[3]), $id)) {
            $found = $line;
            break;
        }
    }
    fclose($file);
    return $found;
}

// Returns $player's name for showing to others (not escaped): first and last name,
// without the ' that pointless_safe_name() stored in front of a formula character.
// A player who gave no name is "A player".
function pointless_display_name(array $player): string
{
    $unsafe = fn(string $name) => preg_match("/^'[=+\\-@\t\r]/", $name) === 1 ? substr($name, 1) : $name;
    $name = trim($unsafe($player[0] ?? '') . ' ' . $unsafe($player[1] ?? ''));
    return $name === '' ? 'A player' : $name;
}

// Returns the URL of $seed's player's share page on the live site.
function pointless_share_url(string $seed): string
{
    return POINTLESS_SITE_URL . 'share.php?p=' . pointless_share_id($seed);
}

// Returns the health bar for $level of $pages reached, as HTML.
function pointless_health_bar(int $level, int $pages): string
{
    $percent = $pages > 0 ? (int)round(100 * min($level, $pages) / $pages) : 0;
    return '<div class="health-bar" role="progressbar" aria-label="Progress" aria-valuemin="0" aria-valuemax="100"'
         . " aria-valuenow=\"$percent\"><span style=\"width: $percent%\"></span></div>";
}

// Returns $site's icon (a key of pointless_share_links()) as inline SVG, colored by the
// text color. The icons are Simple Icons (simpleicons.org, CC0), stored in includes/icons/
// so the page loads nothing from the sites themselves.
function pointless_share_icon(string $site): string
{
    return trim((string)file_get_contents(__DIR__ . '/icons/' . strtolower($site) . '.svg'));
}

// Returns links for posting $url (a share page) as [site => link]. Plain share URLs,
// with no SDKs, so a page loads nothing from the sites until the player clicks.
// Facebook and LinkedIn take only the URL; they read the page's og: tags.
function pointless_share_links(string $url, bool $finished): array
{
    $text = $finished ? 'I finished the Pointless Challenge!' : 'I am making progress in the Pointless Challenge.';
    $u = rawurlencode($url);
    $t = rawurlencode($text);
    $both = rawurlencode("$text $url");
    return [
        'X' => "https://x.com/intent/tweet?text=$t&url=$u&hashtags=Pointless,LU",
        'Facebook' => "https://www.facebook.com/sharer/sharer.php?u=$u",
        'LinkedIn' => "https://www.linkedin.com/sharing/share-offsite/?url=$u",
        'Bluesky' => "https://bsky.app/intent/compose?text=$both",
        'Threads' => "https://www.threads.net/intent/post?text=$both",
        'Reddit' => "https://www.reddit.com/submit?url=$u&title=$t",
    ];
}
