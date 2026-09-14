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
<!DOCTYPE html>
<html lang="en">
<head>
    <link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.0.7/css/all.css">
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>
        Pointless Challenge
    </title>
    <link rel="stylesheet" href="includes/styles.css">
</head>
<body>
<div id="theme-btn" class="far fa-moon"></div>
    <div style="display:flex; align-items:center; justify-content: center;">
        <div class="title">
            Pointless Challenge
        </div>
        <div class="imagen">
            <object data="includes/bison.svg" alt="LU_Bison"></object>                    
        </div>
    </div>
    <script> //script for dark mode
        const themeBtn = document.getElementById("theme-btn");
            themeBtn.onclick = () => {
                themeBtn.classList.toggle("fa-sun");
                if (themeBtn.classList.contains("fa-sun")) {
                    document.body.classList.add("changeTheme");
                } else {
                    document.body.classList.remove("changeTheme");
                }
            }
    </script>
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
                <div class="error">
                    <?php echo $error; ?>
                </div>
            <?php endif; ?>
            <form action="<?php echo htmlspecialchars($_SERVER["PHP_SELF"]); ?>" method="post" id="downloadForm">
                <button type="submit">
                    Pointless Download
                </button>
            </form>
        </div>
    </div>
</body>
<footer>
    <div style="text-align:center; margin-top: 34.5px;"><p>Dr. Towell - dtowell@lipscomb.edu</p></div>
<footer>
</html>