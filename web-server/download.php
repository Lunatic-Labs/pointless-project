<?php
require_once __DIR__ . '/includes/session.php';
require_once __DIR__ . '/includes/generate.php';
require_once __DIR__ . '/includes/events.php';

// A player's first download runs the puzzle generator, so a session must wait this many seconds between downloads.
const POINTLESS_DOWNLOAD_INTERVAL = 10;

// Tokens are the only record of how far a player got, so guessing at them has to be slow.
const POINTLESS_TOKEN_INTERVAL = 5;

// Only registered or logged-in players can download their personalized puzzle.
if (!isset($_SESSION["email"])) {
    header("Location: ./index.php");
    exit;
}

$error = "";
$notice = "";
if ($_SERVER["REQUEST_METHOD"] == "POST" && isset($_POST["token"])) {
    $wait = ($_SESSION["last_token"] ?? 0) + POINTLESS_TOKEN_INTERVAL - time();
    if ($wait > 0) {
        $error = "Please wait $wait more second" . ($wait === 1 ? "" : "s") . " before submitting another token.";
    } else {
        $_SESSION["last_token"] = time();
        [$accepted, , $message] = pointless_submit_token($_SESSION["email"], $_POST["token"]);
        if ($accepted) {
            $notice = $message;
        } else {
            $error = $message;
        }
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

// How far this player has come. $puzzles is 0 until they have downloaded their game.
[$level, $puzzles] = pointless_player_progress($_SESSION["email"]);
?>
<?php require __DIR__ . '/includes/header.php'; ?>
    <div class="container">
        <h2>The Pointless Challenge.</h2>
        <p>Your puzzle is ready. The download gives you <code>pointless.zip</code>, the first layer of your personal challenge.</p>
        <?php if ($error): ?>
            <div class="warning">
                <?php echo $error; ?>
            </div>
        <?php endif; ?>
        <?php if ($notice): ?>
            <div class="notice">
                <?php echo $notice; ?>
            </div>
        <?php endif; ?>
        <form action="<?php echo htmlspecialchars($_SERVER["PHP_SELF"]); ?>" method="post" id="downloadForm">
            <button type="submit">Pointless Download</button>
        </form>
    </div>
    <div class="container">
        <h2>Your Progress</h2>
        <p>
            Every puzzle page shows a <b>token</b>. Type each one in here as you reach it, so we know how far you have come.
            A token is eight letters and digits; capitals, spaces, and hyphens don't matter.
        </p>
        <p class="progress">
            Puzzles solved:
            <b><?php echo $level; ?><?php echo $puzzles ? " of $puzzles" : ""; ?></b>
        </p>
        <form action="<?php echo htmlspecialchars($_SERVER["PHP_SELF"]); ?>" method="post" id="tokenForm">
            <label for="token">Token:</label>
            <input type="text" id="token" name="token" autocomplete="off" spellcheck="false" required>

            <button type="submit">Submit Token</button>
        </form>
    </div>
<?php require __DIR__ . '/includes/footer.php'; ?>
