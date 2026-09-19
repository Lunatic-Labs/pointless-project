<?php
require_once __DIR__ . '/includes/session.php';
require_once __DIR__ . '/includes/players.php';

$error = "";
$email = "";

if ($_SERVER["REQUEST_METHOD"] == "POST") {
    $email = pointless_normalize_email($_POST["email"] ?? "");

    if (filter_var($email, FILTER_VALIDATE_EMAIL) === false) {
        $error = "!! INVALID EMAIL !! Please enter a valid email...";
    } elseif (!pointless_player_exists($email)) {
        $error = "Email not registered. Perhaps you haven't signed up yet? <a href='./index.php'>Click here.</a>";
    } else {
        session_regenerate_id(true);
        $_SESSION["email"] = $email;
        header("Location: download.php");
        exit;
    }
}
?>
<?php require __DIR__ . '/includes/header.php'; ?>
    <div class="container">
        <h2>Log in</h2>
        <p>Enter the email you registered with to download your puzzle.</p>
        <?php if ($error): ?>
            <div class="warning">
                <?php echo $error; ?>
            </div>
        <?php endif; ?>
        <form action="<?php echo htmlspecialchars($_SERVER["PHP_SELF"]); ?>" method="post" id="stringForm">
            <label for="email">Email:</label>
            <input type="email" id="email" name="email" value="<?php echo htmlspecialchars($email); ?>" autocomplete="email" required>
            <button type="submit">Submit</button>
        </form>
    </div>
<?php require __DIR__ . '/includes/footer.php'; ?>
