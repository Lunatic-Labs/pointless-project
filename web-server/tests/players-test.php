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
    check(player_rows() === [['FName', 'LName', 'Email'], ['Ann', 'Lee', 'ann@b.com'], ['Bo', 'Ray', 'bo@b.com']],
          'file has one header and both players');
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
    check(player_rows()[1] === ['Smith, Jr', 'say "hi"', 'q@b.com'], 'commas and quotes round-trip');
    check(pointless_player_exists('q@b.com'), 'email after a quoted field is found');
}
