<?php
$usr_fname = "";
$usr_lname = "";
$usr_email = "";
$token = "n\\a";
$error = "";
$email_valid = true;
$no_duplicate = true;

if ($_SERVER["REQUEST_METHOD"] == "POST") {
    $usr_fname = htmlspecialchars(string: $_POST["fname"]);
    $usr_lname = htmlspecialchars(string: $_POST["lname"]);
    $usr_email = htmlspecialchars(string: $_POST["email"]);

    // invalid email error
    if (filter_var($usr_email, FILTER_VALIDATE_EMAIL) === false) {
        $error = "!! INVALID EMAIL !! Please enter a vaild email...";
        $email_valid = false;
    } else {
        // duplicate email in csv error check
        $file = fopen(filename: './includes/contact-data.csv', mode:'r');
        fgetcsv(stream: $file); //skips first line in csv
        $line = fgetcsv(stream: $file);
        while($line != false){
            $email = $line[2];
            if ($usr_email == $email) {
                $error = "!! DUPLICATE EMAIL !! Please enter a different email...";
                $no_duplicate = false;
                break;
            }
            $line = fgetcsv($file);
        };
        fclose($file);
    };

    if ($email_valid && $no_duplicate){
        $formdata = array(
        "fname"=> $usr_fname,
        "lname"=> $usr_lname,
        "email"=> $usr_email,
        $token
        );
        $file_open = fopen(filename: "./includes/contact-data.csv",mode: "a");

        // puts data into csv and closes connection
        fputcsv(stream: $file_open, fields: $formdata);
        fclose(stream: $file_open);
        header("Location: ../download.php");
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
    <div class="container"><h2 style="text-align:center">The Pointless Challenge.</h2>
        <p style="text-align:center">
            Hi, welcome to the Pointless Project!<br> 
            Please register your email, so we can get you to the download page.<br>
            Already registered? Login <a href="./login.php">here</a>.
        </p>
        <?php if ($error): ?>
            <div class="error">
                <?php echo $error; ?>
            </div>
        <?php endif; ?>
        <form action="<?php echo htmlspecialchars($_SERVER["PHP_SELF"]); ?>" method="post" id="stringForm"> 
            <label for="fname">
                First Name:
            </label>
            <input type="text" id="fname" name="fname" required>
            
            <label for="lname">
                Last Name:
            </label>
            <input type="text" id="lname" name="lname" required>
            
            <label for="email">
                Email:
            </label>
            <input type="text" id="email" name="email" required>
            
            <button type="submit">
                Submit
            </button>
        </form>
    </div>
</body>
<footer>
    <div style="text-align:center; margin-top: 34.5px;"><p>Dr. Towell - dtowell@lipscomb.edu</p></div>
<footer>
</html>