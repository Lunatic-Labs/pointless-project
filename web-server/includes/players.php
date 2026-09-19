<?php
// Registered players are stored in a CSV file (columns: FName, LName, Email, Seed).
// Seed is the player's puzzle generator seed, chosen at random when they register.
// Players who registered before seeds were stored have no Seed until their first
// download records it (see pointless_player_zip() in generate.php).
// It lives outside web-server/ so the web server can never serve it, and it is
// not tracked in git. Default: data/contact-data.csv at the repository root.
// Can be overridden with the POINTLESS_PLAYERS_FILE environment variable.
function pointless_players_file(): string
{
    $file = getenv('POINTLESS_PLAYERS_FILE');
    return ($file !== false && $file !== '') ? $file : __DIR__ . '/../../data/contact-data.csv';
}

// Returns $email as it is stored and compared: trimmed and lowercased.
// The puzzle generator's utils_seed_from_email() ignores the same differences.
function pointless_normalize_email(string $email): string
{
    return strtolower(trim($email));
}

// Returns $email's row (FName, LName, Email, Seed), ignoring case and surrounding
// whitespace, or null if $email is not registered. Seed is '' if none is stored yet.
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
            $found = array_pad($line, 4, '');
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

// Stores $seed for $email if the player has no seed yet (they registered before
// seeds were stored), and updates an old file to have a Seed column. An
// existing seed is never changed. Returns false if the player is not registered
// or the file could not be rewritten.
function pointless_set_player_seed(string $email, string $seed): bool
{
    $email = pointless_normalize_email($email);
    $file = @fopen(pointless_players_file(), 'r+');
    if ($file === false) {
        return false;
    }
    // Rewritten in place, under the same lock pointless_add_player() takes.
    flock($file, LOCK_EX);
    $rows = [];
    $found = false;
    while (($line = fgetcsv($file)) !== false) {
        if ($line === [null]) {
            continue; // A blank line.
        }
        $line = array_pad($line, 4, '');
        if ($rows === []) {
            $line = ['FName', 'LName', 'Email', 'Seed'];
        } elseif (pointless_normalize_email($line[2]) === $email) {
            $found = true;
            if ($line[3] === '') {
                $line[3] = $seed;
            }
        }
        $rows[] = $line;
    }
    $ok = $found && rewind($file) && ftruncate($file, 0);
    foreach ($ok ? $rows : [] as $line) {
        $ok = $ok && fputcsv($file, $line) !== false;
    }
    fflush($file);
    flock($file, LOCK_UN);
    fclose($file);
    return $ok;
}
