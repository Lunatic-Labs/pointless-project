<?php
// The staff report (see includes/admin.php). Signed-in admins see every player, sorted
// furthest first, with tabs for each status; admin.php?p=<share id> shows one player's
// events and answer key; &csv=1 downloads the current tab as a spreadsheet.
require_once __DIR__ . '/includes/session.php';
require_once __DIR__ . '/includes/admin.php';

// Names and emails: never cache, never index, and never send this URL to a site linked from here.
header('Cache-Control: no-store');
header('X-Robots-Tag: noindex, nofollow');
header('Referrer-Policy: no-referrer');

$h = fn($text) => htmlspecialchars((string)$text);
$when = fn(?int $time) => $time === null ? '' : date('Y-m-d H:i', $time);
$page_title = 'Players - Pointless Challenge';

if (!pointless_admins()) {
    http_response_code(404);
    require __DIR__ . '/includes/header.php';
    echo "    <div class=\"container\">\n        <p>There is nothing here.</p>\n    </div>\n";
    require __DIR__ . '/includes/footer.php';
    exit;
}

$error = '';
if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    if (isset($_POST['logout'])) {
        pointless_admin_sign_out();
        header('Location: admin.php');
        exit;
    }
    $error = pointless_admin_sign_in(trim($_POST['name'] ?? ''), $_POST['password'] ?? '') ?? '';
    if ($error === '') {
        header('Location: admin.php');
        exit;
    }
}

$admin = pointless_admin_signed_in();
if ($admin === null) {
    require __DIR__ . '/includes/header.php'; ?>
    <div class="container">
        <h2>Staff Sign In</h2>
        <?php if ($error): ?>
            <div class="warning"><?php echo $h($error); ?></div>
        <?php endif; ?>
        <form action="admin.php" method="post">
            <label for="name">Name:</label>
            <input type="text" id="name" name="name" autocomplete="username" required>
            <label for="password">Password:</label>
            <input type="password" id="password" name="password" autocomplete="current-password" required>
            <button type="submit">Sign In</button>
        </form>
    </div>
<?php
    require __DIR__ . '/includes/footer.php';
    exit;
}

$report = pointless_admin_sort(pointless_admin_report());
$view = (string)($_GET['view'] ?? 'all');
if (!isset(POINTLESS_STATUSES[$view])) {
    $view = 'all';
}
$rows = $view === 'all' ? $report : array_values(array_filter($report, fn($r) => $r['status'] === $view));

if (isset($_GET['csv'])) {
    header('Content-Type: text/csv; charset=utf-8');
    header('Content-Disposition: attachment; filename="pointless-' . $view . '-' . date('Y-m-d') . '.csv"');
    $out = fopen('php://output', 'w');
    fputcsv($out, ['FName', 'LName', 'Email', 'Status', 'Level', 'Pages', 'Stuck On', 'Registered', 'Downloaded',
                   'Last Progress', 'Last Seen', 'Rejected Tokens', 'Platform']);
    foreach ($rows as $r) {
        // Names are already stored spreadsheet-safe; the rest is made so here.
        $cells = [$r['email'], POINTLESS_STATUSES[$r['status']][0], $r['level'], $r['pages'], $r['stuck_on'],
                  $when($r['registered']), $when($r['downloaded']), $when($r['progressed']), $when($r['last_seen']),
                  $r['rejected'], $r['platform']];
        fputcsv($out, array_merge([$r['player'][0], $r['player'][1]], array_map(fn($c) => pointless_safe_name((string)$c), $cells)));
    }
    fclose($out);
    exit;
}

$detail = null;
if (isset($_GET['p'])) {
    foreach ($report as $r) {
        if (hash_equals(pointless_share_id($r['seed']), (string)$_GET['p'])) {
            $detail = $r;
        }
    }
    if ($detail === null) {
        http_response_code(404);
    }
}

$progress = fn(array $r) => $r['pages'] > 0 ? "$r[level] / $r[pages]" : '';
$status = fn(array $r) => '<span class="status status-' . $r['status'] . '">' . POINTLESS_STATUSES[$r['status']][0] . '</span>';
require __DIR__ . '/includes/header.php';
?>
    <div class="container admin">
        <form action="admin.php" method="post" class="admin-signout">
            Signed in as <?php echo $h($admin); ?>.
            <button type="submit" name="logout" value="1">Sign Out</button>
        </form>
<?php if (isset($_GET['p'])): ?>
        <p><a href="admin.php?view=<?php echo $h($view); ?>">&larr; All players</a></p>
    <?php if ($detail === null): ?>
        <p>There is no such player.</p>
    <?php else: $r = $detail; ?>
        <h2><?php echo $h($r['name']); ?></h2>
        <table class="admin-facts">
            <tr><th>Email</th><td><a href="mailto:<?php echo $h($r['email']); ?>"><?php echo $h($r['email']); ?></a></td></tr>
            <tr><th>Status</th><td><?php echo $status($r); ?> <?php echo $h(POINTLESS_STATUSES[$r['status']][1]); ?></td></tr>
            <tr><th>Progress</th><td><?php echo $progress($r); ?><?php echo $r['stuck_on'] !== '' ? ', on ' . $h($r['stuck_on']) : ''; ?></td></tr>
            <tr><th>Registered</th><td><?php echo $when($r['registered']); ?></td></tr>
            <tr><th>Last seen</th><td><?php echo $when($r['last_seen']); ?> (<?php echo $h($r['platform']); ?>)</td></tr>
            <tr><th>Seed</th><td><?php echo $h($r['seed']); ?></td></tr>
            <tr><th>Share page</th><td><a href="share.php?p=<?php echo $h(pointless_share_id($r['seed'])); ?>">share.php</a></td></tr>
        </table>
        <?php if ($r['puzzles']): ?>
            <h3>Answer key</h3>
            <div class="admin-table">
                <table>
                    <tr><th>Page</th><th>Puzzle</th><th>Password</th><th>Proof of progress</th><th>Reached</th></tr>
                    <?php foreach ($r['puzzles'] as $puzzle): ?>
                        <tr>
                            <td><?php echo $h($puzzle['n'] ?? ''); ?></td>
                            <td><?php echo $h($puzzle['name'] ?? ''); ?></td>
                            <td><code><?php echo $h($puzzle['password'] ?? ''); ?></code></td>
                            <td><code><?php echo $h($puzzle['token'] ?? ''); ?></code></td>
                            <td><?php echo (int)($puzzle['n'] ?? 0) <= $r['level'] ? '&#10003;' : ''; ?></td>
                        </tr>
                    <?php endforeach; ?>
                </table>
            </div>
        <?php endif; ?>
        <h3>Events</h3>
        <div class="admin-table">
            <table>
                <tr><th>Time</th><th>Event</th><th>Detail</th><th>Level</th><th>Platform</th></tr>
                <?php foreach (array_reverse($r['events']) as [$time, $event, $what, $level, $where]): ?>
                    <tr>
                        <td><?php echo $when($time); ?></td>
                        <td><?php echo $h($event); ?></td>
                        <td><?php echo $h($what); ?></td>
                        <td><?php echo $level; ?></td>
                        <td><?php echo $h($where); ?></td>
                    </tr>
                <?php endforeach; ?>
            </table>
        </div>
    <?php endif; ?>
<?php else: ?>
        <h2>Players</h2>
        <p class="admin-tabs">
            <?php foreach (['all' => ['All', '']] + POINTLESS_STATUSES as $key => [$label]): ?>
                <?php $count = $key === 'all' ? count($report) : count(array_filter($report, fn($r) => $r['status'] === $key)); ?>
                <a href="admin.php?view=<?php echo $key; ?>"<?php echo $key === $view ? ' class="current"' : ''; ?>><?php echo "$label ($count)"; ?></a>
            <?php endforeach; ?>
        </p>
        <p>
            <?php echo $view === 'all' ? 'Everyone, furthest first: the top of this list is who to recruit.' : $h(POINTLESS_STATUSES[$view][1]); ?>
            Progress counts only the proofs of progress a player submitted, so some are further along than it shows.
        </p>
        <?php if ($view === 'all'): ?>
            <?php
            // Where players who have not finished are, by page: which puzzles stop people.
            $stops = [];
            foreach ($report as $r) {
                if ($r['pages'] > 0 && $r['status'] !== 'finished') {
                    $stops[$r['level']] ??= [$r['level'] > 0 ? $r['stuck_on'] : 'no proof of progress yet', 0];
                    $stops[$r['level']][1]++;
                }
            }
            ksort($stops);
            ?>
            <?php if ($stops): ?>
                <h3>Where players are</h3>
                <table class="admin-stops">
                    <?php foreach ($stops as $level => [$name, $count]): ?>
                        <tr>
                            <td><?php echo $level > 0 ? "Page $level" : ''; ?></td>
                            <td><?php echo $h($name); ?></td>
                            <td><span class="admin-bar" style="width: <?php echo 0.75 * $count; ?>em"></span> <?php echo $count; ?></td>
                        </tr>
                    <?php endforeach; ?>
                </table>
            <?php endif; ?>
        <?php endif; ?>
        <?php if ($rows): ?>
            <div class="admin-table">
                <table>
                    <tr><th>Name</th><th>Email</th><th>Status</th><th>Progress</th><th>Stuck on</th><th>Registered</th>
                        <th>Last progress</th><th>Last seen</th><th>Rejected</th><th>Platform</th></tr>
                    <?php foreach ($rows as $r): ?>
                        <tr>
                            <td><a href="admin.php?view=<?php echo $view; ?>&amp;p=<?php echo pointless_share_id($r['seed']); ?>"><?php echo $h($r['name']); ?></a></td>
                            <td><a href="mailto:<?php echo $h($r['email']); ?>"><?php echo $h($r['email']); ?></a></td>
                            <td><?php echo $status($r); ?></td>
                            <td><?php echo $progress($r); ?></td>
                            <td><?php echo $h($r['stuck_on']); ?></td>
                            <td><?php echo $when($r['registered']); ?></td>
                            <td><?php echo $when($r['progressed']); ?></td>
                            <td><?php echo $when($r['last_seen']); ?></td>
                            <td><?php echo $r['rejected'] ?: ''; ?></td>
                            <td><?php echo $h($r['platform']); ?></td>
                        </tr>
                    <?php endforeach; ?>
                </table>
            </div>
            <label for="emails">Emails on this tab, to paste into Bcc:</label>
            <textarea id="emails" class="admin-emails" rows="3" readonly><?php echo $h(implode(', ', array_column($rows, 'email'))); ?></textarea>
            <p><a href="admin.php?view=<?php echo $view; ?>&amp;csv=1">Download this tab as CSV</a></p>
        <?php else: ?>
            <p>No players.</p>
        <?php endif; ?>
<?php endif; ?>
    </div>
<?php require __DIR__ . '/includes/footer.php'; ?>
