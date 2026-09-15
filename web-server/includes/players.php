<?php
// Registered players are stored in a CSV file (columns: FName, LName, Email).
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

// Returns whether $email is registered, ignoring case and surrounding whitespace.
function pointless_player_exists(string $email): bool
{
    $email = pointless_normalize_email($email);
    $file = @fopen(pointless_players_file(), 'r');
    if ($file === false) {
        return false; // No one has registered yet.
    }
    fgetcsv($file); // Skip the header.
    $found = false;
    while (($line = fgetcsv($file)) !== false) {
        if (isset($line[2]) && pointless_normalize_email($line[2]) === $email) {
            $found = true;
            break;
        }
    }
    fclose($file);
    return $found;
}

// Appends a player, creating the file (and its header) if needed.
// Names are stored as given and the email normalized. Returns false on failure.
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
        fputcsv($file, ['FName', 'LName', 'Email']);
    }
    $ok = fputcsv($file, [$fname, $lname, pointless_normalize_email($email)]) !== false;
    fflush($file);
    flock($file, LOCK_UN);
    fclose($file);
    return $ok;
}
