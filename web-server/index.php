<?php
// The only way in. An email that is already registered signs that player back in;
// any other valid email registers a new player. Names are optional, asked for and
// stored only at registration: the players file is append-only, so what a returning
// player types in them is ignored.
require_once __DIR__ . '/includes/session.php';
require_once __DIR__ . '/includes/players.php';
require_once __DIR__ . '/includes/events.php';

$error = "";
$fname = "";
$lname = "";
$email = "";

if ($_SERVER["REQUEST_METHOD"] == "POST") {
    // Stored as typed (see pointless_safe_name()); escaped only when shown.
    $fname = trim($_POST["fname"] ?? "");
    $lname = trim($_POST["lname"] ?? "");
    $email = pointless_normalize_email($_POST["email"] ?? "");

    $registered = filter_var($email, FILTER_VALIDATE_EMAIL) !== false && pointless_player_exists($email);

    if (filter_var($email, FILTER_VALIDATE_EMAIL) === false) {
        $error = "!! INVALID EMAIL !! Please enter a valid email...";
    } elseif (!$registered && !pointless_add_player($fname, $lname, $email)) {
        error_log("pointless: could not write " . pointless_players_file());
        $error = "Registration failed. Please try again later.";
    } else {
        session_regenerate_id(true);
        $_SESSION["email"] = $email;
        if (!$registered) {
            pointless_log_event("register", $email, trim("$fname $lname"), 0);
        }
        header("Location: download.php");
        exit;
    }
}
?>
<?php require __DIR__ . '/includes/header.php'; ?>
    <div class="container">
        <h2>The Pointless Challenge.</h2>
        <p>
            A series of puzzles lie before you, along the way you will collect items and
            score points. Register and we'll send you on your way to find the
            <b>MacGuffin</b>. Don't worry, we'll explain it along the way.
        </p>
        <p><br />
            Been here before? Your personal challenge is still available.
        </p>
        <?php if ($error): ?>
            <div class="warning">
                <?php echo $error; ?>
            </div>
        <?php endif; ?>
        <form action="<?php echo htmlspecialchars($_SERVER["PHP_SELF"]); ?>" method="post" id="stringForm">
            <label for="email">Email:</label>
            <input type="email" id="email" name="email" value="<?php echo htmlspecialchars($email); ?>" autocomplete="email" required>

            <label for="fname">First Name: (optional)</label>
            <input type="text" id="fname" name="fname" value="<?php echo htmlspecialchars($fname); ?>" autocomplete="given-name">

            <label for="lname">Last Name: (optional)</label>
            <input type="text" id="lname" name="lname" value="<?php echo htmlspecialchars($lname); ?>" autocomplete="family-name">

            <button type="submit">Submit</button>
        </form>
    </div>
<?php require __DIR__ . '/includes/footer.php'; ?>
