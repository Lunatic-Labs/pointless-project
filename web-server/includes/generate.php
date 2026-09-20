<?php
// Builds a player's personalized Pointless zip by running the production puzzle
// generator. The web server never runs make: a person runs `make production` in
// puzzle-code/, which creates puzzle-code/production/ (src/main and resources/).
// That directory is only read. Each generation runs in its own temporary copy of
// resources/, so generations can't interfere with each other.
//
// Each player's zip is generated once and kept permanently in the games
// directory, with an answer key, so every download of it is the same file.
require_once __DIR__ . '/players.php';

// Directory created by `make production`.
// Can be overridden with the POINTLESS_GENERATOR_DIR environment variable.
function pointless_generator_dir(): string
{
    $dir = getenv('POINTLESS_GENERATOR_DIR');
    return ($dir !== false && $dir !== '') ? $dir : __DIR__ . '/../../puzzle-code/production';
}

// Runs a command (no shell) in $cwd and returns its exit code.
// Combined stdout/stderr is stored in $output.
function pointless_run(array $cmd, string $cwd, ?string &$output = null): int
{
    $descriptors = [0 => ['file', '/dev/null', 'r'], 1 => ['pipe', 'w'], 2 => ['redirect', 1]];
    $proc = proc_open($cmd, $descriptors, $pipes, $cwd);
    if (!is_resource($proc)) {
        $output = '';
        return -1;
    }
    $output = stream_get_contents($pipes[1]);
    fclose($pipes[1]);
    return proc_close($proc);
}

// Directory holding each player's zip (<seed>.zip) and answer key (<seed>.txt):
// games/ next to the players file, so it is outside web-server/ and the deploy tree.
function pointless_games_dir(): string
{
    return dirname(pointless_players_file()) . '/games';
}

// Recursively copies the directory $from to $to (which must not exist).
function pointless_copy_dir(string $from, string $to): bool
{
    $names = scandir($from);
    if ($names === false || !mkdir($to, 0700)) {
        return false;
    }
    foreach ($names as $name) {
        if ($name === '.' || $name === '..') {
            continue;
        }
        $ok = is_dir("$from/$name") ? pointless_copy_dir("$from/$name", "$to/$name")
                                    : copy("$from/$name", "$to/$name");
        if (!$ok) {
            return false;
        }
    }
    return true;
}

// Recursively removes $path.
function pointless_remove_path(string $path): void
{
    if (is_link($path) || !is_dir($path)) {
        @unlink($path);
        return;
    }
    foreach (scandir($path) ?: [] as $name) {
        if ($name !== '.' && $name !== '..') {
            pointless_remove_path("$path/$name");
        }
    }
    @rmdir($path);
}

// Runs the generator with $args and returns the path to a temporary copy of the
// puzzle1.zip it wrote, which the caller must unlink. The generator's output (the
// seed and passwords) is stored in $output. Returns null and sets $error on failure.
function pointless_generate_zip(array $args, ?string &$output = null, ?string &$error = null): ?string
{
    $output = '';
    $gen = pointless_generator_dir();
    $main = realpath("$gen/src/main");
    if ($main === false || !is_executable($main) || !is_dir("$gen/resources")) {
        error_log("pointless: no production generator in $gen (run `make production` in puzzle-code/)");
        $error = "The puzzle generator is not available. Please try again later.";
        return null;
    }

    // The generator reads and writes ../resources/ and zipfiles/ relative to its
    // working directory, so give it a private tree laid out like production/.
    $work = tempnam(sys_get_temp_dir(), 'pointless-');
    if ($work === false || !unlink($work) || !mkdir("$work/src/zipfiles", 0700, true)) {
        error_log("pointless: could not create a work directory");
        $error = "Puzzle generation failed. Please try again later.";
        return null;
    }

    try {
        if (!pointless_copy_dir("$gen/resources", "$work/resources")
            || pointless_run(array_merge([$main], $args), "$work/src", $output) !== 0) {
            error_log("pointless: generation failed: $output");
            $error = "Puzzle generation failed. Please try again later.";
            return null;
        }

        $zip = tempnam(sys_get_temp_dir(), 'pointless-zip-');
        if ($zip === false || !rename("$work/src/zipfiles/puzzle1.zip", $zip)) {
            if ($zip !== false) {
                unlink($zip);
            }
            error_log("pointless: generator did not produce puzzle1.zip");
            $error = "Puzzle generation failed. Please try again later.";
            return null;
        }
        return $zip;
    } finally {
        // Don't leave the generated puzzles (and their passwords) on disk.
        pointless_remove_path($work);
    }
}

// Copies the file $from to $path atomically (a copy next to $path, then a rename),
// so no one ever sees a partial file, even if $from is on another file system.
function pointless_store_file(string $from, string $path): bool
{
    $tmp = "$path.tmp-" . getmypid();
    if (!copy($from, $tmp) || !rename($tmp, $path)) {
        @unlink($tmp);
        return false;
    }
    return true;
}

// Returns the path to $email's stored zip, generating it (and its answer key) on
// the first request. The file is permanent: the caller must not change or delete
// it. Returns null and sets $error if the player is not registered or generation fails.
function pointless_player_zip(string $email, ?string &$error = null): ?string
{
    $player = pointless_find_player($email);
    if ($player === null) {
        error_log("pointless: download for unregistered email $email");
        $error = "Email not registered. Please register again.";
        return null;
    }
    [$fname, $lname, $email, $seed] = $player;
    $games = pointless_games_dir();
    if (is_file("$games/$seed.zip")) {
        return "$games/$seed.zip";
    }

    $output = '';
    $zip = pointless_generate_zip(['-s', $seed], $output, $error);
    if ($zip === null) {
        return null;
    }
    try {
        // The answer key is stored first, so a stored zip always has one.
        $key = "Name: $fname $lname\nEmail: $email\nGenerated: " . date('Y-m-d H:i:s T') . "\n$output";
        if ((!is_dir($games) && !@mkdir($games, 0700, true) && !is_dir($games))
            || file_put_contents("$zip.txt", $key) === false
            || !pointless_store_file("$zip.txt", "$games/$seed.txt")
            || !pointless_store_file($zip, "$games/$seed.zip")) {
            error_log("pointless: could not store the game in $games");
            $error = "Puzzle generation failed. Please try again later.";
            return null;
        }
        return "$games/$seed.zip";
    } finally {
        unlink($zip);
        @unlink("$zip.txt");
    }
}
