<?php
require_once __DIR__ . '/includes/session.php';
require_once __DIR__ . '/includes/generate.php';

// A player's first download runs the puzzle generator, so a session must wait this many seconds between downloads.
const POINTLESS_DOWNLOAD_INTERVAL = 10;

// Only registered or logged-in players can download their personalized puzzle.
if (!isset($_SESSION["email"])) {
    header("Location: ./index.php");
    exit;
}

$error = "";
if ($_SERVER["REQUEST_METHOD"] == "POST") {
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
?>
<?php require __DIR__ . '/includes/header.php'; ?>
    <div class="container"><h2>The Pointless Challenge.</h2></div>
    <div class="container">
        <?php if ($error): ?>
            <div class="warning">
                <?php echo $error; ?>
            </div>
        <?php endif; ?>
        <form action="<?php echo htmlspecialchars($_SERVER["PHP_SELF"]); ?>" method="post" id="downloadForm">
            <button type="submit">Pointless Download</button>
        </form>
    </div>
<?php require __DIR__ . '/includes/footer.php'; ?>
