<?php
// Unit tests for includes/generate.php (no server), using the fake generator from lib.php.
require_once __DIR__ . '/../includes/generate.php';

function test_generate_dir_from_env(): void
{
    check(pointless_generator_dir() === generator_path(), 'POINTLESS_GENERATOR_DIR is used');
}

function test_generate_success(): void
{
    $before = list_tree(generator_path());
    $error = '';
    $zip = pointless_generate_zip('ann@b.com', $error);
    check($zip !== null, "zip is generated (error: $error)");
    check(file_get_contents($zip) === 'email=ann@b.com', 'returned file is the generated puzzle1.zip');
    unlink($zip);

    $runs = generator_runs();
    check(count($runs) === 1, 'generator runs once');
    check($runs[0]['args'] === ['-e', 'ann@b.com'], 'generator gets -e <email>');
    check(basename($runs[0]['cwd']) === 'src', 'generator runs in a src/ directory');
    check(!str_starts_with($runs[0]['cwd'], generator_path()), 'generator does not run inside the production tree');
    check($runs[0]['has_resources'], 'generator sees a copy of resources/ at ../resources');
    check(!file_exists(dirname($runs[0]['cwd'])), 'work directory is removed');
    check(list_tree(tmp_path()) === [], 'nothing is left in TMPDIR');
    check(list_tree(generator_path()) === $before, 'production tree is unchanged');
}

function test_generate_missing_generator(): void
{
    fake_generator('missing');
    $error = '';
    check(pointless_generate_zip('ann@b.com', $error) === null, 'no zip without a generator');
    check(str_contains($error, 'not available'), "error says the generator is not available (got: $error)");
}

function test_generate_generator_fails(): void
{
    fake_generator('fail');
    $error = '';
    check(pointless_generate_zip('ann@b.com', $error) === null, 'no zip when the generator fails');
    check(str_contains($error, 'failed'), "error says generation failed (got: $error)");
    check(count(generator_runs()) === 1, 'generator ran');
    check(list_tree(tmp_path()) === [], 'nothing is left in TMPDIR');
}

function test_generate_no_zip(): void
{
    fake_generator('no-zip');
    $error = '';
    check(pointless_generate_zip('ann@b.com', $error) === null, 'no zip when the generator writes none');
    check(str_contains($error, 'failed'), "error says generation failed (got: $error)");
    check(list_tree(tmp_path()) === [], 'nothing is left in TMPDIR');
}

// Runs the real generator: puzzle-code/production (or $POINTLESS_TEST_GENERATOR_DIR).
// Skipped when it hasn't been built; fails if src/main exists but the tree is broken
// (for example, left over from an older layout), since downloads would fail too.
function test_generate_real_generator(): void
{
    $dir = getenv('POINTLESS_TEST_GENERATOR_DIR') ?: __DIR__ . '/../../puzzle-code/production';
    if (!is_executable("$dir/src/main")) {
        skip("no generator in $dir (run `make production` in puzzle-code/)");
    }
    putenv("POINTLESS_GENERATOR_DIR=$dir");
    try {
        $error = '';
        $zip = pointless_generate_zip('real-test@example.com', $error);
    } finally {
        putenv('POINTLESS_GENERATOR_DIR=' . generator_path());
    }
    check($zip !== null, "zip is generated (error: $error)");
    $bytes = file_get_contents($zip);
    unlink($zip);
    check(str_starts_with($bytes, "PK\x03\x04"), 'result is a zip file');
    // Entry names are stored unencrypted, so they can be found without the password.
    check(str_contains($bytes, 'instructions.html'), 'zip contains instructions.html');
    check(str_contains($bytes, 'puzzle2.zip'), 'zip contains puzzle2.zip');
    check(list_tree(tmp_path()) === [], 'nothing is left in TMPDIR');
}
