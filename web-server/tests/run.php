<?php
// Runs the web-server tests: `php web-server/tests/run.php [name-filter]`.
//
// Starts its own `php -S` on a free port with a temporary players file (and
// games directory next to it), a fake puzzle generator, and a temporary TMPDIR, so it never touches
// data/contact-data.csv or puzzle-code/production. State is reset before
// each test. Exits with status 1 if any test fails.

// Set before anything reads sys_get_temp_dir(), which caches it.
require_once __DIR__ . '/lib.php';
putenv('TMPDIR=' . tmp_path());
putenv('POINTLESS_PLAYERS_FILE=' . players_path());
putenv('POINTLESS_GENERATOR_DIR=' . generator_path());
// The failure tests make includes/ log errors; keep them out of the test output.
ini_set('display_errors', '0');
ini_set('log_errors', '1');
ini_set('error_log', test_dir() . '/php-errors.log');

require_once __DIR__ . '/players-test.php';
require_once __DIR__ . '/generate-test.php';
require_once __DIR__ . '/events-test.php';
require_once __DIR__ . '/index-test.php';
require_once __DIR__ . '/download-test.php';

$tests = [
    'test_players_file_from_env',
    'test_players_missing_file',
    'test_players_add_creates_file_and_header',
    'test_players_random_seed',
    'test_players_exists_matches_whole_email',
    'test_players_normalizes_email',
    'test_players_csv_quoting',
    'test_players_formula_names',

    'test_generate_dir_from_env',
    'test_generate_success',
    'test_generate_missing_generator',
    'test_generate_generator_fails',
    'test_generate_no_zip',
    'test_generate_player_zip_stored',
    'test_generate_player_answers',
    'test_generate_player_zip_unregistered',
    'test_generate_player_zip_fails',
    'test_generate_real_generator',

    'test_events_file_from_env',
    'test_events_normalize_token',
    'test_events_log_and_level',
    'test_events_progress',
    'test_events_submit_token',
    'test_events_submit_token_case_and_spaces',
    'test_events_submit_token_rejected',
    'test_events_level_never_drops',
    'test_events_submit_before_download',

    'test_index_get',
    'test_index_register',
    'test_index_session_cookie',
    'test_index_invalid_email',
    'test_index_email_with_apostrophe',
    'test_index_names_optional',
    'test_index_normalizes_email',
    'test_index_names_stored_as_typed',
    'test_index_returning_player',
    'test_index_returning_player_ignores_case',
    'test_index_token',
    'test_index_bad_token',
    'test_index_token_rate_limited',

    'test_download_requires_session',
    'test_download_zip',
    'test_download_canceled',
    'test_download_same_file',
    'test_download_uses_session_email',
    'test_download_rate_limited',
    'test_download_generator_missing',
    'test_download_generator_fails',
    'test_download_shows_progress',
    'test_download_bad_token',
    'test_download_token_rate_limited',
    'test_download_logs_events',
];

$filter = $argv[1] ?? '';
$keepDir = false;
$counts = ['PASS' => 0, 'FAIL' => 0, 'SKIP' => 0];

register_shutdown_function(function () use (&$keepDir) {
    if (!$keepDir) {
        remove_tree(test_dir());
    }
});

foreach ($tests as $test) {
    if ($filter !== '' && !str_contains($test, $filter)) {
        continue;
    }
    reset_state();
    try {
        $test();
        $result = 'PASS';
        $detail = '';
    } catch (TestSkipped $e) {
        $result = 'SKIP';
        $detail = ': ' . $e->getMessage();
    } catch (Throwable $e) {
        $result = 'FAIL';
        $detail = ': ' . ($e instanceof TestFailure ? $e->getMessage()
                         : get_class($e) . ': ' . $e->getMessage() . ' (' . basename($e->getFile()) . ':' . $e->getLine() . ')');
    }
    $counts[$result]++;
    echo "$result $test$detail\n";
}

echo "\n{$counts['PASS']} passed, {$counts['FAIL']} failed, {$counts['SKIP']} skipped\n";
if ($counts['PASS'] + $counts['FAIL'] + $counts['SKIP'] === 0) {
    echo "no tests match '$filter'\n";
    exit(1);
}
if ($counts['FAIL'] > 0) {
    $keepDir = true;
    echo 'Test files (including server.log and php-errors.log) kept in ' . test_dir() . "\n";
    exit(1);
}
