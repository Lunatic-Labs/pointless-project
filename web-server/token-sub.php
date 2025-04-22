<?php
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
    <div class="container">
        <form action="<?php echo htmlspecialchars($_SERVER["PHP_SELF"]); ?>" method="post" id="stringForm"> 
            <label for="token">
                Token:
            </label>
            <input type="text" id="token" name="token" required>
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