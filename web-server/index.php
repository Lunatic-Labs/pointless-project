<?php
session_start();
require_once __DIR__ . '/includes/players.php';

$error = "";

if ($_SERVER["REQUEST_METHOD"] == "POST") {
    $usr_fname = htmlspecialchars($_POST["fname"] ?? "");
    $usr_lname = htmlspecialchars($_POST["lname"] ?? "");
    $usr_email = htmlspecialchars($_POST["email"] ?? "");

    if (filter_var($usr_email, FILTER_VALIDATE_EMAIL) === false) {
        $error = "!! INVALID EMAIL !! Please enter a vaild email...";
    } elseif (pointless_player_exists($usr_email)) {
        $error = "!! DUPLICATE EMAIL !! Please enter a different email...";
    } elseif (!pointless_add_player($usr_fname, $usr_lname, $usr_email)) {
        error_log("pointless: could not write " . pointless_players_file());
        $error = "Registration failed. Please try again later.";
    } else {
        $_SESSION["email"] = $usr_email;
        header("Location: download.php");
        exit;
    }
}
?>
<?php require __DIR__ . '/includes/header.php'; ?>
    <div class="container"><h2 style="text-align:center">The Pointless Challenge.</h2>
        <p style="text-align:center">
            Hi, welcome to the Pointless Project!<br>
            Please register your email, so we can get you to the download page.<br>
            Already registered? Login <a href="./login.php">here</a>.
        </p>
        <?php if ($error): ?>
            <div class="warning">
                <?php echo $error; ?>
            </div>
        <?php endif; ?>
        <form action="<?php echo htmlspecialchars($_SERVER["PHP_SELF"]); ?>" method="post" id="stringForm">
            <label for="fname">First Name:</label>
            <input type="text" id="fname" name="fname" required>

            <label for="lname">Last Name:</label>
            <input type="text" id="lname" name="lname" required>

            <label for="email">Email:</label>
            <input type="text" id="email" name="email" required>

            <button type="submit">Submit</button>
        </form>
    </div>
<?php require __DIR__ . '/includes/footer.php'; ?>
