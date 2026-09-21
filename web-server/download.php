<?php
require_once __DIR__ . '/includes/session.php';
require_once __DIR__ . '/includes/generate.php';
require_once __DIR__ . '/includes/events.php';
require_once __DIR__ . '/includes/progress.php';

// A player's first download runs the puzzle generator, so a session must wait this many seconds between downloads.
const POINTLESS_DOWNLOAD_INTERVAL = 10;

// Only signed-in players can download their personalized puzzle. (Tokens are
// throttled too, by POINTLESS_TOKEN_INTERVAL in includes/events.php.)
if (!isset($_SESSION["email"])) {
    header("Location: ./index.php");
    exit;
}

$error = "";         // About the download, shown by its button.
$token_error = "";   // About a proof of progress, shown by its form.
$token_notice = "";

if ($_SERVER["REQUEST_METHOD"] == "POST" && isset($_POST["token"])) {
    [$accepted, $message] = pointless_submit_token_throttled($_SESSION["email"], $_POST["token"]);
    if ($accepted) {
        $token_notice = $message;
    } else {
        $token_error = $message;
    }
} elseif ($_SERVER["REQUEST_METHOD"] == "POST") {
    $wait = ($_SESSION["last_download"] ?? 0) + POINTLESS_DOWNLOAD_INTERVAL - time();
    if ($wait > 0) {
        $error = "Please wait $wait more second" . ($wait === 1 ? "" : "s") . " before downloading again.";
    } else {
        $_SESSION["last_download"] = time();
        session_write_close(); // Don't hold the session lock while the generator runs.
        // The player's permanent zip (generated on their first download); never delete it.
        $zip = pointless_player_zip($_SESSION["email"], $error);
        if ($zip !== null) {
            $handle = fopen($zip, 'rb');
            if ($handle !== false) {
                pointless_log_event("download", $_SESSION["email"], basename($zip),
                                    pointless_player_level($_SESSION["email"]));
                header("Content-Type: application/zip");
                header("Content-Disposition: attachment; filename=\"pointless.zip\"");
                header("Content-Length: " . fstat($handle)['size']);
                fpassthru($handle);
                exit;
            }
            error_log("pointless: could not open $zip");
            $error = "Puzzle generation failed. Please try again later.";
        }
    }
}

// How far this player has come: the level is the highest page reached. $pages is 0
// until they have downloaded their game.
[$level, $pages] = pointless_player_progress($_SESSION["email"]);
$finished = $pages > 0 && $level >= $pages;

// Posts link to the player's own share page, which shows their name and health bar.
$share = [];
if ($level > 0) {
    $player = pointless_find_player($_SESSION["email"]);
    $share = pointless_share_links(pointless_share_url($player[3]), $finished);
    $share_id = pointless_share_id($player[3]); // For the button to their page, relative so it works on any server.
}
?>
<?php ob_start(); // The progress section, which goes first once there is any progress. ?>
    <div class="container">
        <h2>Your Progress</h2>
        <?php if ($pages): ?>
            <?php echo pointless_health_bar($level, $pages); ?>
            <?php if ($finished): ?>
                <p class="step">You completed the challenge. Congratulations!</p>
            <?php endif; ?>
        <?php endif; ?>
        <?php if ($token_error): ?>
            <div class="warning">
                <?php echo $token_error; ?>
            </div>
        <?php endif; ?>
        <?php if ($token_notice): ?>
            <div class="notice">
                <?php echo $token_notice; ?>
            </div>
        <?php endif; ?>
        <form action="<?php echo htmlspecialchars($_SERVER["PHP_SELF"]); ?>" method="post" id="tokenForm">
            <label for="token">Proof of progress:</label>
            <input type="text" id="token" name="token" autocomplete="off" spellcheck="false" required>

            <button type="submit">Submit Proof</button>
        </form>
    </div>
    <?php if ($share): ?>
        <div class="container">
            <h2>Share Your Progress</h2>
            <p>
                <?php echo $finished ? "You completed the challenge!" : "You are making progress."; ?>
                Tell your friends, and dare them to catch up.
            </p>
            <p class="share">
                <?php foreach ($share as $site => $url): ?>
                    <a href="<?php echo htmlspecialchars($url); ?>" target="_blank" rel="noopener noreferrer"
                       title="<?php echo $site; ?>" aria-label="Post on <?php echo $site; ?>"><?php echo pointless_share_icon($site); ?></a>
                <?php endforeach; ?>
            </p>
            <form action="share.php" method="get">
                <input type="hidden" name="p" value="<?php echo htmlspecialchars($share_id); ?>">
                <button type="submit">View Your Page</button>
            </form>
        </div>
    <?php endif; ?>
<?php $progress_section = ob_get_clean(); ?>
<?php require __DIR__ . '/includes/header.php'; ?>
    <?php if ($level > 0) echo $progress_section; ?>
    <div class="container">
        <p>Your personal puzzle is ready. </p>
        <?php if ($error): ?>
            <div class="warning">
                <?php echo $error; ?>
            </div>
        <?php endif; ?>
        <form action="<?php echo htmlspecialchars($_SERVER["PHP_SELF"]); ?>" method="post" id="downloadForm">
            <button type="submit">Pointless Download</button>
        </form>
    </div>
    <?php if ($level <= 0) echo $progress_section; ?>
<?php require __DIR__ . '/includes/footer.php'; ?>
