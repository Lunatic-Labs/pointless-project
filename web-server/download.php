<?php
require_once __DIR__ . '/includes/session.php';
require_once __DIR__ . '/includes/generate.php';

// Each download runs the puzzle generator, so a session must wait this many seconds between downloads.
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
        $zip = pointless_generate_zip($_SESSION["email"], $error);
        if ($zip !== null) {
            // Delete the zip before sending it from the open handle: if the player
            // cancels, PHP stops this script partway and nothing is left behind.
            $handle = fopen($zip, 'rb');
            unlink($zip);
            if ($handle !== false) {
                header("Content-Type: application/zip");
                header("Content-Disposition: attachment; filename=\"pointless.zip\"");
                header("Content-Length: " . fstat($handle)['size']);
                fpassthru($handle);
                exit;
            }
            error_log("pointless: could not open the generated zip");
            $error = "Puzzle generation failed. Please try again later.";
        }
    }
}
?>
<?php require __DIR__ . '/includes/header.php'; ?>
    <div class="container"><h2>The Pointless Challenge.</h2>
        <p>
            Before you can use the Pointless Puzzle zip, <strong>you need to download <a target="_blank" rel="noopener noreferrer" href="https://www.7-zip.org/">7-Zip.</a></strong><br>
            Regular zip openers cannot properly open the pointless project,<br>
            <strong>so please download 7-Zip before opening the Pointless zip file.</strong>
        </p>
    </div>
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
