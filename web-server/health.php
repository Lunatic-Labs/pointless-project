<?php
// Deploy check. `bin/deploy pointless` (in the dtowell/tools repo) requests this
// page after switching releases. It answers "ok" only if Apache passed both
// environment variables and the files and directories they name can be used. A
// missing SetEnv then fails the deploy, instead of quietly storing players inside
// the release, where a later deploy deletes them. It says what is wrong but gives
// no paths.
require_once __DIR__ . '/includes/events.php';

$problems = [];
foreach (['POINTLESS_PLAYERS_FILE', 'POINTLESS_GENERATOR_DIR'] as $name) {
    if ((string)getenv($name) === '') {
        $problems[] = "$name is not set";
    }
}
$data = dirname(pointless_players_file());
if (!is_dir($data) || !is_writable($data)) {
    $problems[] = 'the players directory is not writable';
}
foreach (['players file' => pointless_players_file(), 'events file' => pointless_events_file(),
          'games directory' => pointless_games_dir()] as $what => $path) {
    if (file_exists($path) && !is_writable($path)) {
        $problems[] = "the $what is not writable";
    }
}
$gen = pointless_generator_dir();
if (!is_executable("$gen/src/main") || !is_dir("$gen/resources")) {
    $problems[] = 'the puzzle generator is not available';
}

header('Content-Type: text/plain');
header('Cache-Control: no-store');
if ($problems) {
    http_response_code(503);
    echo implode("\n", $problems), "\n";
} else {
    echo "ok\n";
}
