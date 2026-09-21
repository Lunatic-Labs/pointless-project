<?php
// A player's public progress page, share.php?p=<share id> (see includes/progress.php).
// Their social media posts link here, so it needs no session, and its og: tags give
// Facebook and LinkedIn the player's name. It shows only the name and the health bar.
require_once __DIR__ . '/includes/session.php';
require_once __DIR__ . '/includes/progress.php';
require_once __DIR__ . '/includes/events.php';

$player = pointless_find_player_by_share_id((string)($_GET['p'] ?? ''));
if ($player === null) {
    http_response_code(404);
    $name = '';
} else {
    $name = pointless_display_name($player);
    [$level, $pages] = pointless_player_progress($player[2]);
    $finished = $pages > 0 && $level >= $pages;
    $headline = $finished ? "$name finished the Pointless Challenge!" : "$name is taking the Pointless Challenge.";
    $page_title = "$name - Pointless Challenge";
    $page_meta = [
        'og:type' => 'website',
        'og:site_name' => 'Pointless Challenge',
        'og:title' => $headline,
        'og:description' => 'A chain of puzzles, each one locking the next. How far can you get?',
        'og:url' => pointless_share_url($player[3]),
    ];
}
?>
<?php require __DIR__ . '/includes/header.php'; ?>
    <div class="container">
        <?php if ($player === null): ?>
            <h2>The Pointless Challenge.</h2>
            <p>There is no player here.</p>
        <?php else: ?>
            <h2><?php echo htmlspecialchars($headline); ?></h2>
            <?php if ($pages): ?>
                <?php echo pointless_health_bar($level, $pages); ?>
            <?php endif; ?>
        <?php endif; ?>
        <form action="index.php" method="get">
            <button type="submit">Take the Challenge Yourself</button>
        </form>
    </div>
<?php require __DIR__ . '/includes/footer.php'; ?>
