<?php
// Tests for admin.php, the staff report, and includes/admin.php.
require_once __DIR__ . '/../includes/admin.php';

// Writes the admins file with $admins ([name => password]).
function make_admins(array $admins): void
{
    @mkdir(dirname(players_path()), 0700, true);
    $lines = ["# Pointless admins"];
    foreach ($admins as $name => $password) {
        $lines[] = "$name:" . password_hash($password, PASSWORD_BCRYPT, ['cost' => 4]); // Cheap, for speed.
    }
    file_put_contents(dirname(players_path()) . '/admins.htpasswd', implode("\n", $lines) . "\n");
}

// Returns a client signed in as an admin.
function admin_client(): Client
{
    make_admins(['dwayne' => 'correct horse']);
    $client = new Client();
    $page = $client->post('admin.php', ['name' => 'dwayne', 'password' => 'correct horse']);
    check($page->status === 302, "signing in redirects (got $page->status)");
    return $client;
}

// Replaces the events file with $events: [unix time, event, email, detail, level, platform] each.
function write_events(array $events): void
{
    @mkdir(dirname(events_path()), 0700, true);
    $file = fopen(events_path(), 'w');
    fputcsv($file, ['Time', 'Event', 'Email', 'Detail', 'Level', 'Platform']);
    foreach ($events as [$time, $event, $email, $detail, $level, $platform]) {
        fputcsv($file, [date('Y-m-d H:i:s T', $time), $event, $email, $detail, $level, $platform]);
    }
    fclose($file);
}

// Column $n of each row of the admin log, without the header.
function admin_log_column(int $n): array
{
    $lines = @file(dirname(players_path()) . '/admin-log.csv', FILE_IGNORE_NEW_LINES) ?: [];
    return array_map(fn($line) => str_getcsv($line)[$n], array_slice($lines, 1));
}

function test_admin_not_set_up(): void
{
    register(new Client(), 'ann@b.com');
    $page = (new Client())->get('admin.php');
    check($page->status === 404, "without an admins file, status is 404 (got $page->status)");
    check(!$page->contains('password'), 'there is no sign-in form');
    check(!$page->contains('ann@b.com'), 'no player is shown');
}

function test_admin_requires_sign_in(): void
{
    make_admins(['dwayne' => 'correct horse']);
    register(new Client(), 'ann@b.com');
    foreach (['admin.php', 'admin.php?csv=1', 'admin.php?p=' . pointless_share_id(player_seed('ann@b.com'))] as $url) {
        $page = (new Client())->get($url);
        check($page->status === 200, "$url: status is 200 (got $page->status)");
        check($page->contains('type="password"'), "$url: shows the sign-in form");
        check(!$page->contains('ann@b.com'), "$url: shows no player");
    }
    check($page->header('Cache-Control') === 'no-store', 'the page is never cached');
    check($page->header('Referrer-Policy') === 'no-referrer', 'and its URL is never sent to other sites');
}

function test_admin_sign_in_and_out(): void
{
    make_admins(['dwayne' => 'correct horse', 'bo' => 'battery staple']);
    register(new Client(), 'ann@b.com', 'Ann', 'Lee');
    $client = new Client();
    foreach ([['dwayne', 'wrong'], ['nobody', 'correct horse'], ['bo', 'correct horse']] as [$name, $password]) {
        $page = $client->post('admin.php', ['name' => $name, 'password' => $password]);
        check($page->contains('Wrong name or password.'), "$name/$password is refused");
        check(!$page->contains('ann@b.com'), "$name/$password shows no player");
    }
    $page = $client->post('admin.php', ['name' => 'bo', 'password' => 'battery staple']);
    check($page->status === 302 && $page->header('Location') === 'admin.php', 'the right password redirects to the report');
    $page = $client->get('admin.php');
    check($page->contains('Signed in as bo.'), 'the report names the admin');
    check($page->contains('ann@b.com'), 'the report shows the player');

    $client->post('admin.php', ['logout' => '1']);
    check(!$client->get('admin.php')->contains('ann@b.com'), 'after signing out, the report is gone');
    check(admin_log_column(1) === ['login-bad', 'login-bad', 'login-bad', 'login-ok', 'logout'], 'sign-ins and outs are logged');
    check(admin_log_column(2) === ['dwayne', 'nobody', 'bo', 'bo', 'bo'], 'with the name typed');
}

function test_admin_lockout(): void
{
    make_admins(['dwayne' => 'correct horse']);
    $client = new Client();
    for ($i = 0; $i < POINTLESS_ADMIN_FAILURES; $i++) {
        check(pointless_admin_sign_in('dwayne', 'guess') === 'Wrong name or password.', "guess $i is refused");
    }
    $page = $client->post('admin.php', ['name' => 'dwayne', 'password' => 'correct horse']);
    check($page->contains('Too many failed sign-ins. Try again in 15 minutes.'), 'then even the right password is refused');

    // Failures older than the lockout no longer count.
    $log = dirname(players_path()) . '/admin-log.csv';
    $rows = array_map('str_getcsv', file($log, FILE_IGNORE_NEW_LINES));
    $file = fopen($log, 'w');
    fputcsv($file, array_shift($rows));
    foreach ($rows as $row) {
        $row[0] = date('Y-m-d H:i:s T', time() - POINTLESS_ADMIN_LOCKOUT - 60);
        fputcsv($file, $row);
    }
    fclose($file);
    $page = $client->post('admin.php', ['name' => 'dwayne', 'password' => 'correct horse']);
    check($page->status === 302, 'after the lockout, the right password works');
}

function test_admin_removed_admin(): void
{
    $client = admin_client();
    check($client->get('admin.php')->contains('Signed in as dwayne.'), 'the admin is signed in');
    make_admins(['bo' => 'battery staple']);
    check($client->get('admin.php')->contains('type="password"'), 'removing them from the admins file signs them out');
}

function test_admin_report_statuses(): void
{
    $now = time();
    $day = 86400;
    foreach (['fin', 'new', 'quiet', 'guessing', 'returning', 'busy'] as $name) {
        $name === 'new' ? pointless_add_player('New', '', "$name@b.com") : player_with_game("$name@b.com");
    }
    $seed = player_seed('fin@b.com');
    write_events([
        [$now - 9 * $day, 'register', 'fin@b.com', '', 0, 'Windows'],
        [$now - 9 * $day, 'download', 'fin@b.com', "$seed.zip", 0, 'Windows'],
        [$now - 8 * $day, 'token-ok', 'fin@b.com', 'X', FAKE_PUZZLES, 'Android'],
        [$now - 5 * $day, 'register', 'new@b.com', '', 0, 'iOS'],
        [$now - 9 * $day, 'download', 'quiet@b.com', 'x.zip', 0, 'macOS'],
        [$now - 8 * $day, 'token-ok', 'quiet@b.com', 'X', 2, 'macOS'],
        [$now - 3 * $day, 'download', 'guessing@b.com', 'x.zip', 0, 'Linux'],
        [$now - 3 * $day, 'token-bad', 'guessing@b.com', 'A', 0, 'Linux'],   // Before their progress: forgiven.
        [$now - 2 * $day, 'token-ok', 'guessing@b.com', 'X', 1, 'Linux'],
        [$now - 2 * $day + 60, 'token-bad', 'guessing@b.com', 'B', 1, 'Linux'], // Too soon to count as returning.
        [$now - 2 * $day + 60, 'token-bad', 'guessing@b.com', 'C', 1, 'Linux'],
        [$now - 2 * $day + 60, 'token-bad', 'guessing@b.com', 'D', 1, 'Linux'],
        [$now - 4 * $day, 'download', 'returning@b.com', 'x.zip', 0, 'Windows'],
        [$now - 3 * $day, 'token-ok', 'returning@b.com', 'X', 2, 'Windows'],
        [$now - 1 * $day, 'download', 'returning@b.com', 'x.zip', 2, 'ChromeOS'],
        [$now - 3 * $day, 'download', 'busy@b.com', 'x.zip', 0, 'Windows'],
        [$now - 3600, 'token-ok', 'busy@b.com', 'X', 3, 'Windows'],
        [$now - 1800, 'token-ok', 'busy@b.com', 'X', 1, 'Windows'], // A lower token doesn't lower the level.
    ]);
    $report = [];
    foreach (pointless_admin_report($now) as $r) {
        $report[$r['email']] = $r;
    }
    check(array_keys($report) === ['fin@b.com', 'new@b.com', 'quiet@b.com', 'guessing@b.com', 'returning@b.com', 'busy@b.com'],
          'every player is reported, in the order they registered');
    $expect = [
        'fin@b.com' => ['finished', FAKE_PUZZLES, ''],
        'new@b.com' => ['not-started', 0, ''],
        'quiet@b.com' => ['stalled', 2, 'puzzle2'],
        'guessing@b.com' => ['struggling', 1, 'puzzle1'],
        'returning@b.com' => ['struggling', 2, 'puzzle2'],
        'busy@b.com' => ['active', 3, 'puzzle3'],
    ];
    foreach ($expect as $email => [$status, $level, $stuck]) {
        $r = $report[$email];
        check($r['status'] === $status, "$email is $status (got $r[status])");
        check($r['level'] === $level, "$email is at level $level (got $r[level])");
        check($r['stuck_on'] === $stuck, "$email is stuck on '$stuck' (got '$r[stuck_on]')");
    }
    check($report['fin@b.com']['pages'] === FAKE_PUZZLES, 'pages come from the answer key');
    check($report['new@b.com']['pages'] === 0 && $report['new@b.com']['registered'] === strtotime(date('Y-m-d H:i:s T', $now - 5 * $day)),
          'a player who has not downloaded has no pages, but a registration time');
    check($report['guessing@b.com']['rejected'] === 3, 'rejected tokens count from the last progress');
    check($report['fin@b.com']['platform'] === 'Windows', "the platform is the download's");
    check($report['returning@b.com']['platform'] === 'ChromeOS', "the latest download's");
    check($report['new@b.com']['platform'] === 'iOS', "or else the latest event's");

    $order = array_column(pointless_admin_sort(array_values($report)), 'email');
    check($order === ['fin@b.com', 'busy@b.com', 'quiet@b.com', 'returning@b.com', 'guessing@b.com', 'new@b.com'],
          'sorted furthest first, then by who got there first (got ' . implode(' ', $order) . ')');
}

function test_admin_report_page(): void
{
    $ann = new Client();
    register($ann, 'ann@b.com', '<b>Ann</b>', 'Lee');
    $ann->post('download.php');
    $seed = player_seed('ann@b.com');
    $ann->post('download.php', ['token' => fake_token(2, $seed)]);
    register(new Client(), 'bo@b.com', '=Bo', '');

    $admin = admin_client();
    $page = $admin->get('admin.php');
    check($page->contains('&lt;b&gt;Ann&lt;/b&gt; Lee') && !$page->contains('<b>Ann</b>'), 'names are escaped');
    check($page->contains('All (2)') && $page->contains('Active (1)') && $page->contains('Not started (1)'), 'the tabs count players');
    check($page->contains('2 / ' . FAKE_PUZZLES), "shows Ann's progress");
    check($page->contains('Where players are') && $page->contains('puzzle2'), 'shows where players are');
    check($page->contains('ann@b.com, bo@b.com'), 'lists the emails for Bcc');

    $page = $admin->get('admin.php?view=not-started');
    check($page->contains('bo@b.com') && !$page->contains('ann@b.com'), 'a tab shows only its players');
    check($admin->get('admin.php?view=bogus')->contains('All (2)'), 'an unknown tab shows everyone');

    $page = $admin->get('admin.php?p=' . pointless_share_id($seed));
    check($page->contains("pw1$seed") && $page->contains(fake_token(3, $seed)), "a player's page shows their answer key");
    check($page->contains('token-ok') && $page->contains(fake_token(2, $seed)), 'and their events');
    check($admin->get('admin.php?p=0123456789abcdef')->status === 404, 'an unknown player is a 404');

    $csv = $admin->get('admin.php?csv=1');
    check(str_starts_with((string)$csv->header('Content-Type'), 'text/csv'), 'the CSV is a CSV');
    $rows = array_map('str_getcsv', explode("\n", trim($csv->body)));
    check(count($rows) === 3 && $rows[0][2] === 'Email', 'it has a header and one row per player');
    check($rows[1][0] === '<b>Ann</b>' && $rows[1][3] === 'Active' && $rows[1][4] === '2', "Ann's row");
    check($rows[2][0] === "'=Bo", 'names stay spreadsheet-safe');
    check(count(array_map('str_getcsv', explode("\n", trim($admin->get('admin.php?view=active&csv=1')->body)))) === 2,
          'a tab exports only its players');
}
