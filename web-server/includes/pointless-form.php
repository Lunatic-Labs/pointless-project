<?php
$usr_fname = "";
$usr_lname = "";
$usr_email = "";
$error = "";
$success = false;

if ($_SERVER["REQUEST_METHOD"] == "POST") {
    $usr_fname = htmlspecialchars(string: $_POST["fname"]);
    $usr_lname = htmlspecialchars(string: $_POST["lname"]);
    $usr_email = htmlspecialchars(string: $_POST["email"]);

    // invalid email error
    if (filter_var($usr_email, FILTER_VALIDATE_EMAIL) === false) {
    $error = "!! INVALID EMAIL !! Please enter a vaild email..."
    }; elseif
    
    // duplicate email in csv error check
    $file = fopen(filename: './contact-data.csv', mode:'r');
    fgetcsv(stream: $file); //skips first line in csv
    $line = fgetcsv(stream: $file);
    while($line != false){
        $email = $line[2];
        if ($usr_email == $email) {
            echo"<script>alert('!! DUPLICATE EMAIL !! Please enter a different email...')
            window.location.href='../index.html'</script>";
            exit;
            };
        $line = fgetcsv($file);
    };
    fclose($file);

    $formdata = array(
      "fname"=> $usr_fname,
      "lname"=> $usr_lname,
      "email"=> $usr_email
    );
    $file_open = fopen(filename: "./contact-data.csv",mode: "a");

    // puts data into csv and closes connection
    fputcsv(stream: $file_open, fields: $formdata);
    fclose(stream: $file_open);

    header("Location: ../download.html");
}
else {
    header("Location: ../index.html");
}