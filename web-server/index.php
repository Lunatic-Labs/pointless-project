<?php
session_start();
require_once __DIR__ . '/includes/players.php';

$error = "";
$fname = "";
$lname = "";
$email = "";

if ($_SERVER["REQUEST_METHOD"] == "POST") {
    // Stored as typed (see pointless_safe_name()); escaped only when shown.
    $fname = trim($_POST["fname"] ?? "");
    $lname = trim($_POST["lname"] ?? "");
    $email = pointless_normalize_email($_POST["email"] ?? "");

    if ($fname === "" || $lname === "") {
        $error = "Please enter your first and last name.";
    } elseif (filter_var($email, FILTER_VALIDATE_EMAIL) === false) {
        $error = "!! INVALID EMAIL !! Please enter a valid email...";
    } elseif (pointless_player_exists($email)) {
        $error = "!! DUPLICATE EMAIL !! Please enter a different email...";
    } elseif (!pointless_add_player($fname, $lname, $email)) {
        error_log("pointless: could not write " . pointless_players_file());
        $error = "Registration failed. Please try again later.";
    } else {
        session_regenerate_id(true);
        $_SESSION["email"] = $email;
        header("Location: download.php");
        exit;
    }
}
?>
<?php require __DIR__ . '/includes/header.php'; ?>
    <div class="container"><h2>The Pointless Challenge.</h2>
        <p>
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
            <input type="text" id="fname" name="fname" value="<?php echo htmlspecialchars($fname); ?>" autocomplete="given-name" required>

            <label for="lname">Last Name:</label>
            <input type="text" id="lname" name="lname" value="<?php echo htmlspecialchars($lname); ?>" autocomplete="family-name" required>

            <label for="email">Email:</label>
            <input type="email" id="email" name="email" value="<?php echo htmlspecialchars($email); ?>" autocomplete="email" required>

            <button type="submit">Submit</button>
        </form>
    </div>
<?php require __DIR__ . '/includes/footer.php'; ?>
