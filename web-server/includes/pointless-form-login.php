<?php
if ($_SERVER["REQUEST_METHOD"] == "POST") {
    $usr_fname = htmlspecialchars(string: $_POST["fname"]);
    $usr_lname = htmlspecialchars(string: $_POST["lname"]);
    $usr_email = htmlspecialchars(string: $_POST["email"]);

    // invalid email error
    if (filter_var($usr_email, FILTER_VALIDATE_EMAIL) === false) {
    echo"<script>alert('!! INVALID EMAIL !! Please enter a vaild email...')
    window.location.href='../login.html'</script>";
    exit;
    };
    
    // duplicate email in csv error check
    $file = fopen(filename: './contact-data.csv', mode:'r');
    fgetcsv(stream: $file); //skips first line in csv
    $line = fgetcsv(stream: $file);
    while($line != false){
        $usr_input = [$usr_fname,$usr_lname,$usr_email];
        if ($usr_input == $line) {
            echo"<script>alert('!! DUPLICATE EMAIL !! Please enter a different email...')
            window.location.href='../login.html'</script>";
            exit;
            };
        $line = fgetcsv($file);
    };
    fclose($file);

    header("Location: ../download.html");
}
else {
    header("Location: ../login.html");
}