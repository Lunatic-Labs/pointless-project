<?php
session_start();
require_once __DIR__ . '/includes/players.php';

$error = "";

if ($_SERVER["REQUEST_METHOD"] == "POST") {
    $usr_email = htmlspecialchars($_POST["email"] ?? "");

    if (filter_var($usr_email, FILTER_VALIDATE_EMAIL) === false) {
        $error = "!! INVALID EMAIL !! Please enter a vaild email...";
    } elseif (!pointless_player_exists($usr_email)) {
        $error = "Email not registered. Perhaps you haven't signed up yet? <a href='./index.php'>Click here.</a>";
    } else {
        $_SESSION["email"] = $usr_email;
        header("Location: download.php");
        exit;
    }
}
?>
<?php require __DIR__ . '/includes/header.php'; ?>
    <div class="container">
        <p style="text-align:center">
            This is the login page.<br>
            Enter your email to download your puzzle.
        </p>
        <?php if ($error): ?>
            <div class="warning">
                <?php echo $error; ?>
            </div>
        <?php endif; ?>
        <form action="<?php echo htmlspecialchars($_SERVER["PHP_SELF"]); ?>" method="post" id="stringForm">
            <label for="email">Email:</label>
            <input type="text" id="email" name="email" required>
            <button type="submit">Submit</button>
        </form>
    </div>
<?php require __DIR__ . '/includes/footer.php'; ?>
