<?php
// Unit tests for includes/events.php (no server), using the fake generator from lib.php.
require_once __DIR__ . '/../includes/events.php';

// Registers a player, generates their game, and returns their seed.
function player_with_game(string $email = 'ann@b.com'): string
{
    pointless_add_player('Ann', 'Lee', $email);
    $error = '';
    check(pointless_player_zip($email, $error) !== null, "the game is generated (error: $error)");
    return player_seed($email);
}

function test_events_file_from_env(): void
{
    check(pointless_events_file() === events_path(), 'the events file sits next to the players file');
}

function test_events_normalize_token(): void
{
    check(pointless_normalize_token(' acde-fhjk ') === 'ACDEFHJK', 'case, spaces, and hyphens do not matter');
    check(pointless_normalize_token('ACDEFHJK') === 'ACDEFHJK', 'a plain token is unchanged');
    check(pointless_normalize_token('!?') === '', 'a token of nothing but punctuation is empty');
}

function test_events_log_and_level(): void
{
    check(pointless_player_level('ann@b.com') === 0, 'an unknown player is at level 0');
    check(pointless_log_event('register', ' Ann@B.com', 'Ann Lee', 0), 'an event is logged');
    check(pointless_log_event('token-ok', 'ann@b.com', 'ACDEFHJK', 3), 'a second event is logged');
    check(pointless_log_event('token-ok', 'bo@b.com', 'ACDEFHJK', 7), "another player's event is logged");

    $rows = event_rows();
    check(count($rows) === 3, 'every event is kept (got ' . count($rows) . ')');
    check(array_slice($rows[0], 1) === ['register', 'ann@b.com', 'Ann Lee', '0'], 'the row holds the event, email, detail, and level');
    check($rows[0][0] !== '', 'the row is timed');
    check(pointless_player_level('Ann@b.com ') === 3, "the newest event gives the player's level");
    check(pointless_player_level('bo@b.com') === 7, 'each player has their own level');
    check(pointless_player_level('cy@b.com') === 0, 'a player with no events is at level 0');
}

function test_events_progress(): void
{
    pointless_add_player('Ann', 'Lee', 'ann@b.com');
    check(pointless_player_progress('ann@b.com') === [0, 0], 'no page count before the game is generated');
    $error = '';
    pointless_player_zip('ann@b.com', $error);
    check(pointless_player_progress('ann@b.com') === [0, FAKE_PUZZLES], 'the answer key gives the page count');
}

function test_events_submit_token(): void
{
    $seed = player_with_game();

    // Token N is inside the zip that puzzle N-1's answer opens, so it proves page N was reached.
    [$ok, $level, $message] = pointless_submit_token('ann@b.com', fake_token(1, $seed));
    check($ok && $level === 1, "the first token counts too (got level $level: $message)");
    check($message === 'Proof of progress accepted.', "and gets the same message (got: $message)");
    [$ok, $level] = pointless_submit_token('ann@b.com', fake_token(3, $seed));
    check($ok && $level === 3, "token 3 means page 3 was reached (got level $level)");

    check(pointless_player_level('ann@b.com') === 3, 'the level is recorded');
    $rows = event_rows();
    check(count($rows) === 2, 'both submissions are logged (got ' . count($rows) . ')');
    check(array_slice($rows[1], 1) === ['token-ok', 'ann@b.com', fake_token(3, $seed), '3'], 'the row holds the token and the new level');
}

function test_events_submit_token_case_and_spaces(): void
{
    $seed = player_with_game();
    [$ok, $level] = pointless_submit_token('Ann@B.com ', ' ' . strtolower(fake_token(2, $seed)) . ' ');
    check($ok && $level === 2, 'a token is matched however it is typed');
}

function test_events_submit_token_rejected(): void
{
    $seed = player_with_game();
    [$ok, $level, $message] = pointless_submit_token('ann@b.com', 'NOTATOKEN');
    check(!$ok && $level === 0, 'a wrong token is rejected');
    check(str_contains($message, 'not one of your proofs of progress'), "the message says so (got: $message)");
    [$ok] = pointless_submit_token('ann@b.com', '');
    check(!$ok, 'an empty token is rejected');

    $rows = event_rows();
    check(count($rows) === 2, 'rejections are logged too: they are the brute-force signal');
    check(array_slice($rows[0], 1) === ['token-bad', 'ann@b.com', 'NOTATOKEN', '0'], 'the rejected token is recorded');

    // Another player's token is no better than a made-up one.
    $bo = player_with_game('bo@b.com');
    [$ok] = pointless_submit_token('ann@b.com', fake_token(3, $bo));
    check(!$ok, "another player's token is rejected");
}

function test_events_level_never_drops(): void
{
    $seed = player_with_game();
    pointless_submit_token('ann@b.com', fake_token(4, $seed));
    check(pointless_player_level('ann@b.com') === 4, 'the last token means every page was reached');
    [$ok, $level] = pointless_submit_token('ann@b.com', fake_token(2, $seed));
    check($ok && $level === 4, 'submitting an earlier token again does not lower the level');
    [, $level] = pointless_submit_token('ann@b.com', 'NOTATOKEN');
    check($level === 4, 'nor does a rejected one');
    check(event_rows()[2][4] === '4', 'the rejection records the level the player still has');
}

function test_events_submit_before_download(): void
{
    pointless_add_player('Ann', 'Lee', 'ann@b.com');
    [$ok, $level, $message] = pointless_submit_token('ann@b.com', 'ACDEFHJK');
    check(!$ok && $level === 0, 'a token cannot be submitted before the game exists');
    check(str_contains($message, 'Download your puzzle first'), "the message says so (got: $message)");
    check(event_rows() === [], 'there is nothing to compare against, so it is not logged as a wrong answer');
}
