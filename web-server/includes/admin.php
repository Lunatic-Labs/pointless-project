<?php
// The staff report, admin.php: every player's progress, sorted into what to do about
// it (congratulate, help, encourage, recruit). It is the only part of the site with
// passwords, because it is the only part that shows players' names, emails, and
// events to someone else.
//
// Admins are listed in admins.htpasswd next to the players file, one `name:hash` per
// line, as `htpasswd -B` writes it (bcrypt, which password_verify() reads); lines
// starting with # are ignored. There is no page for managing them: anyone who can
// write that file on the server is effectively an admin already. Without the file,
// admin.php answers 404. Sign-ins and sign-outs go to admin-log.csv, also next to
// the players file.
require_once __DIR__ . '/events.php';
require_once __DIR__ . '/progress.php';

// An admin who has loaded no page for this many seconds is signed out.
const POINTLESS_ADMIN_IDLE = 2 * 3600;
// This many failed sign-ins, by anyone, within POINTLESS_ADMIN_LOCKOUT seconds lock
// sign-in for everyone until the oldest of them is that old. Per-session limits are
// useless here (an attacker drops the cookie), and with one or two admins a global
// lock costs little.
const POINTLESS_ADMIN_FAILURES = 10;
const POINTLESS_ADMIN_LOCKOUT = 15 * 60;

// A player with no events for this many days has stalled.
const POINTLESS_STALLED_DAYS = 7;
// A player is struggling after this many rejected tokens since their last progress, ...
const POINTLESS_STRUGGLING_TOKENS = 3;
// ... or after coming back at least this many hours after their last progress without making more.
const POINTLESS_STRUGGLING_HOURS = 24;

// Each status: [label, what to do about it]. The order is the order of the report's tabs.
const POINTLESS_STATUSES = [
    'finished' => ['Finished', 'Reached the final page. Congratulate them.'],
    'struggling' => ['Struggling', 'Still coming back, but not getting further. Offer help.'],
    'stalled' => ['Stalled', 'Nothing for ' . POINTLESS_STALLED_DAYS . ' days or more. Encourage them.'],
    'not-started' => ['Not started', 'Registered, but never downloaded the game.'],
    'active' => ['Active', 'Making progress.'],
];

// Can be overridden with the POINTLESS_ADMINS_FILE environment variable.
function pointless_admins_file(): string
{
    $file = getenv('POINTLESS_ADMINS_FILE');
    return ($file !== false && $file !== '') ? $file : dirname(pointless_players_file()) . '/admins.htpasswd';
}

function pointless_admin_log_file(): string
{
    return dirname(pointless_players_file()) . '/admin-log.csv';
}

// Returns the admins as [name => password hash]; [] if the admins file is missing.
function pointless_admins(): array
{
    $admins = [];
    foreach (@file(pointless_admins_file(), FILE_IGNORE_NEW_LINES | FILE_SKIP_EMPTY_LINES) ?: [] as $line) {
        $line = trim($line);
        if ($line === '' || $line[0] === '#' || !str_contains($line, ':')) {
            continue;
        }
        [$name, $hash] = explode(':', $line, 2);
        if ($name !== '' && $hash !== '') {
            $admins[$name] = $hash;
        }
    }
    return $admins;
}

// Appends $event ("login-ok", "login-bad", or "logout") for the admin $name to the
// admin log, with the request's address. The name may be anything someone typed, so
// it is shortened and made safe for spreadsheets.
function pointless_admin_log(string $event, string $name): void
{
    $path = pointless_admin_log_file();
    if (!is_dir(dirname($path)) && !mkdir(dirname($path), 0700, true)) {
        return;
    }
    $file = fopen($path, 'a');
    if ($file === false) {
        return;
    }
    flock($file, LOCK_EX);
    if (fstat($file)['size'] === 0) {
        fputcsv($file, ['Time', 'Event', 'Name', 'Address']);
    }
    fputcsv($file, [date('Y-m-d H:i:s T'), $event, pointless_safe_name(substr($name, 0, 64)), $_SERVER['REMOTE_ADDR'] ?? '']);
    fflush($file);
    flock($file, LOCK_UN);
    fclose($file);
}

// Returns the times of the failed sign-ins in the last POINTLESS_ADMIN_LOCKOUT seconds before $now.
function pointless_admin_recent_failures(int $now): array
{
    $times = [];
    $file = @fopen(pointless_admin_log_file(), 'r');
    if ($file === false) {
        return $times;
    }
    fgetcsv($file); // Skip the header.
    while (($line = fgetcsv($file)) !== false) {
        $time = isset($line[1]) && $line[1] === 'login-bad' ? strtotime($line[0]) : false;
        if ($time !== false && $time > $now - POINTLESS_ADMIN_LOCKOUT) {
            $times[] = $time;
        }
    }
    fclose($file);
    return $times;
}

// Signs $name in, if $password is theirs. Returns an error message, or null on success.
// The caller must have started the session.
function pointless_admin_sign_in(string $name, string $password): ?string
{
    $failures = pointless_admin_recent_failures(time());
    if (count($failures) >= POINTLESS_ADMIN_FAILURES) {
        $minutes = (int)ceil((min($failures) + POINTLESS_ADMIN_LOCKOUT - time()) / 60);
        return "Too many failed sign-ins. Try again in $minutes minute" . ($minutes === 1 ? '' : 's') . '.';
    }
    $admins = pointless_admins();
    // An unknown name is checked against a hash of nothing in particular, so it takes as
    // long as a known one and the timing doesn't tell which names exist.
    $hash = $admins[$name] ?? '$2y$10$/H2MRkiWSuHE8ByMCJBpN.3T37fH92rXz73sxc3hxeP0PH4uHmlBS';
    if (!password_verify($password, $hash) || !isset($admins[$name])) {
        pointless_admin_log('login-bad', $name);
        return 'Wrong name or password.';
    }
    session_regenerate_id(true);
    $_SESSION['admin'] = $name;
    $_SESSION['admin_seen'] = time();
    pointless_admin_log('login-ok', $name);
    return null;
}

function pointless_admin_sign_out(): void
{
    if (isset($_SESSION['admin'])) {
        pointless_admin_log('logout', $_SESSION['admin']);
    }
    unset($_SESSION['admin'], $_SESSION['admin_seen']);
    session_regenerate_id(true);
}

// Returns the signed-in admin's name, or null. An admin removed from the admins file,
// or idle for POINTLESS_ADMIN_IDLE seconds, is signed out.
function pointless_admin_signed_in(): ?string
{
    $name = $_SESSION['admin'] ?? null;
    if ($name === null) {
        return null;
    }
    if (!isset(pointless_admins()[$name]) || ($_SESSION['admin_seen'] ?? 0) + POINTLESS_ADMIN_IDLE < time()) {
        unset($_SESSION['admin'], $_SESSION['admin_seen']);
        return null;
    }
    $_SESSION['admin_seen'] = time();
    return $name;
}

// Returns every event as [email => [[time, event, detail, level, platform], ...]],
// oldest first, with time as a Unix time.
function pointless_events_by_player(): array
{
    $events = [];
    $file = @fopen(pointless_events_file(), 'r');
    if ($file === false) {
        return $events;
    }
    fgetcsv($file); // Skip the header.
    while (($line = fgetcsv($file)) !== false) {
        if (!isset($line[4])) {
            continue;
        }
        $events[pointless_normalize_email($line[2])][] =
            [(int)strtotime($line[0]), $line[1], $line[3], (int)$line[4], $line[5] ?? ''];
    }
    fclose($file);
    return $events;
}

// Returns one summary per registered player, in the order they registered, as of $now
// (default: now). Each is an array with:
//   player      their row of the players file (FName, LName, Email, Seed)
//   name        pointless_display_name()
//   email, seed
//   puzzles     their answer key's puzzles, or [] before their first download
//   events      pointless_events_by_player()'s list for them
//   level       the highest page they have reached (0 before any token)
//   pages       how many pages their game has (0 before their first download)
//   stuck_on    the name of the puzzle on page `level`, which they have not proved they
//               solved; '' when finished, not downloaded, or before any token
//   registered, downloaded, progressed, last_seen
//               Unix times (or null) of registering, the first download, the latest
//               event that raised their level, and their latest event
//   rejected    rejected tokens since their last progress (or download)
//   platform    the platform of their latest download, or else of their latest event
//   status      a key of POINTLESS_STATUSES
// A player can solve puzzles without submitting their tokens, so level is only how far
// they have *proved* they got.
function pointless_admin_report(?int $now = null): array
{
    $now ??= time();
    $events = pointless_events_by_player();
    $report = [];
    $file = @fopen(pointless_players_file(), 'r');
    if ($file === false) {
        return $report;
    }
    fgetcsv($file); // Skip the header.
    while (($player = fgetcsv($file)) !== false) {
        if (!isset($player[3])) {
            continue;
        }
        $email = pointless_normalize_email($player[2]);
        $mine = $events[$email] ?? [];
        $puzzles = pointless_game_puzzles($player[3]) ?? [];

        $registered = $downloaded = $progressed = $last_seen = null;
        $level = 0;
        $rejected = 0;
        $platform = '';
        foreach ($mine as [$time, $event, , $after, $where]) {
            $last_seen = $time;
            $platform = $where;
            if ($event === 'register') {
                $registered ??= $time;
            } elseif ($event === 'download') {
                $downloaded ??= $time;
                $progressed ??= $time; // The download starts the clock on the first puzzle.
            } elseif ($event === 'token-bad') {
                $rejected++;
            } elseif ($event === 'token-ok' && $after > $level) {
                $level = $after;
                $progressed = $time;
                $rejected = 0;
            }
        }
        if ($downloaded !== null) {
            // The platform that has to unzip the game, unless they downloaded again elsewhere.
            foreach (array_reverse($mine) as [, $event, , , $where]) {
                if ($event === 'download') {
                    $platform = $where;
                    break;
                }
            }
        }
        $pages = count($puzzles);
        $finished = $pages > 0 && $level >= $pages;

        if ($finished) {
            $status = 'finished';
        } elseif ($pages === 0) {
            $status = 'not-started';
        } elseif ($last_seen === null || $now - $last_seen >= POINTLESS_STALLED_DAYS * 86400) {
            $status = 'stalled';
        } elseif ($rejected >= POINTLESS_STRUGGLING_TOKENS
                  || ($progressed !== null && $last_seen - $progressed >= POINTLESS_STRUGGLING_HOURS * 3600)) {
            $status = 'struggling';
        } else {
            $status = 'active';
        }

        $report[] = [
            'player' => $player,
            'name' => pointless_display_name($player),
            'email' => $email,
            'seed' => $player[3],
            'puzzles' => $puzzles,
            'events' => $mine,
            'level' => $level,
            'pages' => $pages,
            'stuck_on' => !$finished && $level > 0 ? (string)($puzzles[$level - 1]['name'] ?? '') : '',
            'registered' => $registered,
            'downloaded' => $downloaded,
            'progressed' => $progressed,
            'last_seen' => $last_seen,
            'rejected' => $rejected,
            'platform' => $platform,
            'status' => $status,
        ];
    }
    fclose($file);
    return $report;
}

// Sorts $report (from pointless_admin_report()) furthest first: by the fraction of their
// game reached, then by who got there first. The top of it is the recruiting list.
function pointless_admin_sort(array $report): array
{
    usort($report, function (array $a, array $b): int {
        $progress = fn(array $r) => $r['pages'] > 0 ? min($r['level'], $r['pages']) / $r['pages'] : 0;
        return [$progress($b), $a['progressed'] ?? PHP_INT_MAX, $a['registered'] ?? PHP_INT_MAX]
           <=> [$progress($a), $b['progressed'] ?? PHP_INT_MAX, $b['registered'] ?? PHP_INT_MAX];
    });
    return $report;
}
