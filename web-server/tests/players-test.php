<?php
// Unit tests for includes/players.php (no server).
require_once __DIR__ . '/../includes/players.php';

function test_players_file_from_env(): void
{
    check(pointless_players_file() === players_path(), 'POINTLESS_PLAYERS_FILE is used');
}

function test_players_missing_file(): void
{
    check(!pointless_player_exists('a@b.com'), 'no one is registered when the file does not exist');
}

function test_players_add_creates_file_and_header(): void
{
    check(pointless_add_player('Ann', 'Lee', 'ann@b.com'), 'first player is added');
    check(pointless_add_player('Bo', 'Ray', 'bo@b.com'), 'second player is added');
    $rows = player_rows();
    check(count($rows) === 3, 'file has one header and both players');
    check($rows[0] === ['FName', 'LName', 'Email', 'Seed'], 'header names the columns');
    check(array_slice($rows[1], 0, 3) === ['Ann', 'Lee', 'ann@b.com'], 'first player is stored');
    check(array_slice($rows[2], 0, 3) === ['Bo', 'Ray', 'bo@b.com'], 'second player is stored');
}

function test_players_random_seed(): void
{
    pointless_add_player('Ann', 'Lee', 'ann@b.com');
    pointless_add_player('Bo', 'Ray', 'bo@b.com');
    $ann = player_seed('ann@b.com');
    $bo = player_seed('bo@b.com');
    check(preg_match('/^\d+$/', $ann) === 1, "a seed is a decimal number (got $ann)");
    check($ann !== $bo, 'players get different seeds');
    check(pointless_find_player('ANN@b.com') === ['Ann', 'Lee', 'ann@b.com', $ann], 'find_player returns the row, seed included');
    check(pointless_find_player('cy@b.com') === null, 'find_player returns null for an unregistered email');
}

function test_players_exists_matches_whole_email(): void
{
    pointless_add_player('Ann', 'Lee', 'ann@b.com');
    check(pointless_player_exists('ann@b.com'), 'registered email is found');
    check(!pointless_player_exists('ann@b.co'), 'prefix of an email is not found');
    check(!pointless_player_exists('Email'), 'header is not a player');
    check(!pointless_player_exists('Ann'), 'names are not emails');
}

function test_players_normalizes_email(): void
{
    check(pointless_normalize_email(" Ann@B.COM\n") === 'ann@b.com', 'emails are trimmed and lowercased');
    pointless_add_player('Ann', 'Lee', ' Ann@B.com ');
    check(player_rows()[1][2] === 'ann@b.com', 'the email is stored normalized');
    check(pointless_player_exists("ANN@b.com\t"), 'lookups ignore case and surrounding whitespace');
}

function test_players_csv_quoting(): void
{
    pointless_add_player('Smith, Jr', 'say "hi"', 'q@b.com');
    check(array_slice(player_rows()[1], 0, 3) === ['Smith, Jr', 'say "hi"', 'q@b.com'], 'commas and quotes round-trip');
    check(pointless_player_exists('q@b.com'), 'email after a quoted field is found');
}

function test_players_formula_names(): void
{
    foreach (['=1+2', '+1', '-1', '@SUM(A1)', "\tx", "\rx"] as $i => $name) {
        pointless_add_player($name, 'Lee', "p$i@b.com");
        check(player_rows()[$i + 1][0] === "'$name", 'formula-like name ' . json_encode($name) . " gets a leading '");
    }
    pointless_add_player('Ann', 'O-Neil', 'ann@b.com');
    check(array_slice(player_rows()[7], 0, 3) === ['Ann', 'O-Neil', 'ann@b.com'], 'other names are stored as given');
}
