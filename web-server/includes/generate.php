<?php
// Builds a player's personalized Pointless zip by running the production puzzle
// generator. The web server never runs make: a person runs `make production` in
// puzzle-code/, which creates puzzle-code/production/ (src/main and resources/).
// That directory is only read. Each download runs the generator in its own
// temporary copy of resources/, so downloads can't interfere with each other.

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

// Generates the zip for $email and returns the path to a temporary file,
// which the caller must unlink. Returns null and sets $error on failure.
function pointless_generate_zip(string $email, ?string &$error = null): ?string
{
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
        $output = '';
        if (!pointless_copy_dir("$gen/resources", "$work/resources")
            || pointless_run([$main, '-e', $email], "$work/src", $output) !== 0) {
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
