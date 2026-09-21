<?php
// The event log: what each player has done, appended one line at a time to
// data/events.csv (next to the players file, so it is outside web-server/ and
// not in git). Columns: Time, Event, Email, Detail, Level.
//
// Level is the player's level *after* the event, so the newest line for an email
// gives their level without replaying anything, while the lines underneath still
// reconstruct it. Nothing here ever rewrites a line: the file is append-only, like
// the players file.
//
// A token is not a secret and is not hidden on its page. It doesn't have to be:
// puzzle N's page is inside puzzleN.zip, which only puzzle N-1's answer opens, so
// submitting token N is already proof that the player reached page N. The level is
// the highest page reached: 0 until the first token (which ships unencrypted in the
// download, so it proves only that they opened it), and the fin page's at the end.
require_once __DIR__ . '/generate.php';
require_once __DIR__ . '/players.php';

// The fewest seconds between one session's token submissions. Tokens are the only
// record of how far a player got, so guessing at them has to be slow.
const POINTLESS_TOKEN_INTERVAL = 5;

// Can be overridden with the POINTLESS_EVENTS_FILE environment variable.
function pointless_events_file(): string
{
    $file = getenv('POINTLESS_EVENTS_FILE');
    return ($file !== false && $file !== '') ? $file : dirname(pointless_players_file()) . '/events.csv';
}

// Returns $token as it is stored and compared: uppercase, with everything that is
// not a token character removed, so spaces, hyphens, and lowercase all still match.
function pointless_normalize_token(string $token): string
{
    return preg_replace('/[^A-Z0-9]/', '', strtoupper($token));
}

// Appends one event, creating the file (and its header) if needed. $event is
// "register", "download", "token-ok", or "token-bad"; $level is the player's level
// after it. Returns false on failure.
function pointless_log_event(string $event, string $email, string $detail, int $level): bool
{
    $path = pointless_events_file();
    if (!is_dir(dirname($path)) && !mkdir(dirname($path), 0700, true)) {
        return false;
    }
    $file = fopen($path, 'a');
    if ($file === false) {
        return false;
    }
    flock($file, LOCK_EX);
    if (fstat($file)['size'] === 0) {
        fputcsv($file, ['Time', 'Event', 'Email', 'Detail', 'Level']);
    }
    $row = [date('Y-m-d H:i:s T'), $event, pointless_normalize_email($email), $detail, $level];
    $ok = fputcsv($file, $row) !== false;
    fflush($file);
    flock($file, LOCK_UN);
    fclose($file);
    return $ok;
}

// Returns $email's level: the Level of their newest event, or 0 if they have none.
function pointless_player_level(string $email): int
{
    $email = pointless_normalize_email($email);
    $file = @fopen(pointless_events_file(), 'r');
    if ($file === false) {
        return 0; // Nothing has happened yet.
    }
    fgetcsv($file); // Skip the header.
    $level = 0;
    while (($line = fgetcsv($file)) !== false) {
        if (isset($line[2], $line[4]) && pointless_normalize_email($line[2]) === $email) {
            $level = (int)$line[4]; // The last match wins: events are appended in order.
        }
    }
    fclose($file);
    return $level;
}

// Returns $email's [level, pages], where pages is how many pages (and tokens) their
// game has, fin page included (0 until they have downloaded it).
function pointless_player_progress(string $email): array
{
    $answers = pointless_player_answers($email);
    return [pointless_player_level($email), $answers === null ? 0 : count($answers)];
}

// Records $email's submission of $token and returns [accepted, level, message].
// Every submission is logged, accepted or not: the rejections are what a brute-force
// attempt would look like, and both are participation data.
function pointless_submit_token(string $email, string $token): array
{
    $email = pointless_normalize_email($email);
    $token = pointless_normalize_token($token);
    $level = pointless_player_level($email);

    $answers = pointless_player_answers($email);
    if ($answers === null) {
        // Nothing to compare against, so this isn't a wrong answer; don't log it as one.
        return [false, $level, 'Download your puzzle first. Every puzzle page in it shows a proof of progress.'];
    }

    $n = 0;
    foreach ($answers as $puzzle) {
        if ($token !== '' && pointless_normalize_token((string)($puzzle['token'] ?? '')) === $token) {
            $n = (int)$puzzle['n'];
            break;
        }
    }
    if ($n === 0) {
        pointless_log_event('token-bad', $email, $token, $level);
        return [false, $level, 'That is not one of your proofs of progress. Check it and try again.'];
    }

    // Token N is inside the zip that puzzle N-1's answer opens, so it proves the player
    // reached page N. The level only ever goes up.
    $level = max($level, $n);
    pointless_log_event('token-ok', $email, $token, $level);
    return [true, $level, 'Proof of progress accepted.'];
}

// pointless_submit_token(), rate limited per session: the caller must have started
// one (includes/session.php). Returns [accepted, message]; an empty $token submits
// nothing and gives an empty message, so a page can offer the field as optional.
function pointless_submit_token_throttled(string $email, string $token): array
{
    if (pointless_normalize_token($token) === '') {
        return [false, ''];
    }
    $wait = ($_SESSION['last_token'] ?? 0) + POINTLESS_TOKEN_INTERVAL - time();
    if ($wait > 0) {
        return [false, "Please wait $wait more second" . ($wait === 1 ? '' : 's') . ' before submitting another proof of progress.'];
    }
    $_SESSION['last_token'] = time();
    [$accepted, , $message] = pointless_submit_token($email, $token);
    return [$accepted, $message];
}
