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

// Returns whether $email is registered.
function pointless_player_exists(string $email): bool
{
    $file = @fopen(pointless_players_file(), 'r');
    if ($file === false) {
        return false; // No one has registered yet.
    }
    fgetcsv($file); // Skip the header.
    $found = false;
    while (($line = fgetcsv($file)) !== false) {
        if (isset($line[2]) && $line[2] === $email) {
            $found = true;
            break;
        }
    }
    fclose($file);
    return $found;
}

// Appends a player, creating the file (and its header) if needed.
// Returns false on failure.
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
    $ok = fputcsv($file, [$fname, $lname, $email]) !== false;
    fflush($file);
    flock($file, LOCK_UN);
    fclose($file);
    return $ok;
}
