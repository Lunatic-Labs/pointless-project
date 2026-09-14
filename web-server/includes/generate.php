<?php
// Builds a player's personalized Pointless zip by running the C++ puzzle
// generator in puzzle-code/src. The generator writes to shared paths
// (zipfiles/ and html-txt/files-*/), so only one run may happen at a time.

// Directory containing the built generator (`main`) and its Makefile.
// Can be overridden with the POINTLESS_SRC_DIR environment variable.
function pointless_src_dir(): string
{
    $dir = getenv('POINTLESS_SRC_DIR');
    return ($dir !== false && $dir !== '') ? $dir : __DIR__ . '/../../puzzle-code/src';
}

// Derives the puzzle seed from an email.
// NOTE: must stay in sync with seed_gen() in puzzle-code/tests/file.cpp.
function pointless_seed(string $email): int
{
    $seed = 1;
    $length = strlen($email);
    for ($i = 0; $i < $length; $i++) {
        $seed += (ord($email[$i]) - 30) * $i;
        $seed %= 10000000;
    }
    return $seed;
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

// Generates the zip for $email and returns the path to a temporary copy,
// which the caller must unlink. Returns null and sets $error on failure.
function pointless_generate_zip(string $email, ?string &$error = null): ?string
{
    $src = pointless_src_dir();
    if (!is_executable("$src/main")) {
        error_log("pointless: generator not built at $src/main");
        $error = "The puzzle generator is not available. Please try again later.";
        return null;
    }

    // ./main rejects a seed of 0.
    $seed = pointless_seed($email) ?: 1;

    $lock = fopen(sys_get_temp_dir() . '/pointless-generate.lock', 'c');
    if ($lock === false || !flock($lock, LOCK_EX)) {
        $error = "Could not start puzzle generation. Please try again.";
        return null;
    }

    try {
        // utils_zip_files() adds to existing zips, so stale output must be removed first.
        if (pointless_run(['make', 'cleanzip'], $src, $output) !== 0
            || pointless_run(['./main', '-s', (string)$seed], $src, $output) !== 0) {
            error_log("pointless: generation failed: $output");
            $error = "Puzzle generation failed. Please try again later.";
            return null;
        }

        $tmp = tempnam(sys_get_temp_dir(), 'pointless-');
        if ($tmp === false || !copy("$src/zipfiles/puzzle1.zip", $tmp)) {
            if ($tmp !== false) {
                unlink($tmp);
            }
            error_log("pointless: could not copy $src/zipfiles/puzzle1.zip");
            $error = "Puzzle generation failed. Please try again later.";
            return null;
        }
        return $tmp;
    } finally {
        // Don't leave the generated puzzles (and their passwords) on disk.
        pointless_run(['make', 'cleanzip'], $src);
        flock($lock, LOCK_UN);
        fclose($lock);
    }
}
