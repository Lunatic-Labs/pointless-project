<?php
// The only way in. An email that is already registered signs that player back in;
// any other valid email registers a new player. Names are optional, asked for and
// stored only at registration: the players file is append-only, so what a returning
// player types in them is ignored. A token typed here is recorded on the way through
// (download.php shows the result), so a player can report progress in one step.
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
    $token = trim($_POST["token"] ?? "");

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
        // Handed to download.php, which is about to show this player their progress.
        [$accepted, $message] = pointless_submit_token_throttled($email, $token);
        if ($message !== "") {
            $_SESSION["token_message"] = [$accepted, $message];
        }
        header("Location: download.php");
        exit;
    }
}
?>
<?php require __DIR__ . '/includes/header.php'; ?>
    <div class="container">
        <?php /* TODO(game.md "Website copy"): the MacGuffin framing below is in; what is
           still missing is the points/items joke -- "Between you and it: eleven puzzles, a
           number of items you will never need, and exactly 0 points." Ignore game.md's
           "Log in here" link: login.php is gone and index.php is the only way in. */ ?>
        <h2>The Pointless Challenge.</h2>
        <p>
            At the end of this challenge there is a <b>MacGuffin</b>. We will not tell
            you what it is. That is what makes it a MacGuffin.
        </p>
        <p>
            A series of puzzles lie before you, along the way you will collect items and
            score points. Register your email and we'll send you on your way.
        </p>
        <p>
            Been here before? The same email brings you straight back to your download,
            and you can report proof of progress while you are at it.
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

            <label for="token">Proof of progress: (optional)</label>
            <input type="text" id="token" name="token" value="" autocomplete="off" spellcheck="false">

            <button type="submit">Submit</button>
        </form>
    </div>
<?php require __DIR__ . '/includes/footer.php'; ?>
