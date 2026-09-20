<?php
// Registered players are stored in a CSV file (columns: FName, LName, Email, Seed).
// Seed is the player's puzzle generator seed, chosen at random when they register
// and never changed. It lives outside web-server/ so the web server can never serve it, and it is
// not tracked in git. Default: data/contact-data.csv at the repository root.
// Can be overridden with the POINTLESS_PLAYERS_FILE environment variable.
function pointless_players_file(): string
{
    $file = getenv('POINTLESS_PLAYERS_FILE');
    return ($file !== false && $file !== '') ? $file : __DIR__ . '/../../data/contact-data.csv';
}

// Returns $email as it is stored and compared: trimmed and lowercased.
function pointless_normalize_email(string $email): string
{
    return strtolower(trim($email));
}

// Returns $email's row (FName, LName, Email, Seed), ignoring case and surrounding
// whitespace, or null if $email is not registered.
function pointless_find_player(string $email): ?array
{
    $email = pointless_normalize_email($email);
    $file = @fopen(pointless_players_file(), 'r');
    if ($file === false) {
        return null; // No one has registered yet.
    }
    fgetcsv($file); // Skip the header.
    $found = null;
    while (($line = fgetcsv($file)) !== false) {
        if (isset($line[2]) && pointless_normalize_email($line[2]) === $email) {
            $found = $line;
            break;
        }
    }
    fclose($file);
    return $found;
}

// Returns whether $email is registered, ignoring case and surrounding whitespace.
function pointless_player_exists(string $email): bool
{
    return pointless_find_player($email) !== null;
}

// Returns a new random seed, as a decimal string (the generator's `-s` argument).
function pointless_random_seed(): string
{
    return (string)random_int(0, PHP_INT_MAX);
}

// Returns $name as it is stored. A spreadsheet opening the players file would run
// a cell that starts with =, +, -, @, tab, or carriage return as a formula, so
// such names get a leading ' (which spreadsheets show as text). Otherwise unchanged.
function pointless_safe_name(string $name): string
{
    return ($name !== '' && strpos("=+-@\t\r", $name[0]) !== false) ? "'$name" : $name;
}

// Appends a player with a new random seed, creating the file (and its header) if
// needed. Names are stored as given except for pointless_safe_name(), and the
// email normalized. Returns false on failure.
function pointless_add_player(string $fname, string $lname, string $email): bool
{
    $path = pointless_players_file();
    if (!is_dir(dirname($path)) && !mkdir(dirname($path), 0700, true)) {
        return false;
    }
    $file = fopen($path, 'a');
    if ($file === false) {
        return false;
    }
    flock($file, LOCK_EX);
    if (fstat($file)['size'] === 0) {
        fputcsv($file, ['FName', 'LName', 'Email', 'Seed']);
    }
    $ok = fputcsv($file, [pointless_safe_name($fname), pointless_safe_name($lname), pointless_normalize_email($email), pointless_random_seed()]) !== false;
    fflush($file);
    flock($file, LOCK_UN);
    fclose($file);
    return $ok;
}
