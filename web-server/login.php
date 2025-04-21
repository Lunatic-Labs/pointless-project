<?php
$usr_email = "";
$email_valid = true;
$registered = false;
$error = "";
if ($_SERVER["REQUEST_METHOD"] == "POST") {
    $usr_email = htmlspecialchars(string: $_POST["email"]);

    // invalid email error *fix; not showing up
    if (filter_var($usr_email, FILTER_VALIDATE_EMAIL) === false) {
        $error = "!! INVALID EMAIL !! Please enter a vaild email...";
        $email_valid = false;
    } else {
        // duplicate email in csv error check *fix; not showing up
        $file = fopen(filename: './includes/contact-data.csv', mode:'r');
        fgetcsv(stream: $file); //skips first line in csv
        $line = fgetcsv(stream: $file);
        while($line != false){
            $email = $line[2];
            if ($usr_email == $email) {
                $error = "";
                $registered = true;
                break;
            } else {
                $error = "Email not registered. Perhaps you haven't signed up yet? <a href='./index.php'>Click here.</a>";
            };
            $line = fgetcsv($file);
        };
        fclose($file);
    };
    if ($email_valid && $registered) {
        header("Location: ../token_sub.php");
    };
};
?>
<!DOCTYPE html>
<html lang="en">
<head>
    <link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.0.7/css/all.css">
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Pointless Challenge</title>
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
    <div class="container">
        This is the login page. <br>
        Enter your email to access the tokens page.
        <?php if ($error): ?>
            <div class="error"><?php echo $error; ?></div>
        <?php endif; ?>
        <form action="<?php echo htmlspecialchars($_SERVER["PHP_SELF"]); ?>" method="post" id="stringForm"> 
                
                <label for="email">Email:</label>
                <input type="text" id="email" name="email" required>
                <button type="submit">Submit</button>
        </form>
    </div>
</body>
<footer>
    <div style="text-align:center; margin-top: 34.5px;"><p>Dr. Towell - dtowell@lipscomb.edu</p></div>
<footer>
</html>