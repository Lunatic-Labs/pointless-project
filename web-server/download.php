<?php
session_start();
require_once __DIR__ . '/includes/generate.php';

// Only registered or logged-in players can download their personalized puzzle.
if (!isset($_SESSION["email"])) {
    header("Location: ./index.php");
    exit;
}

$error = "";
if ($_SERVER["REQUEST_METHOD"] == "POST") {
    $zip = pointless_generate_zip($_SESSION["email"], $error);
    if ($zip !== null) {
        header("Content-Type: application/zip");
        header("Content-Disposition: attachment; filename=\"pointless.zip\"");
        header("Content-Length: " . filesize($zip));
        readfile($zip);
        unlink($zip);
        exit;
    }
}
?>
<?php require __DIR__ . '/includes/header.php'; ?>
    <div class="content">
        <div class="container"><h2 style="text-align:center">The Pointless Challenge.</h2>
            <p style="text-align:center">
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
    </div>
<?php require __DIR__ . '/includes/footer.php'; ?>
