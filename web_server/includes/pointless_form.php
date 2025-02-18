<?php
if ($_SERVER["REQUEST_METHOD"] == "POST") {
    $fname = htmlspecialchars(string: $_POST["fname"]);
    $lname = htmlspecialchars(string: $_POST["lname"]);
    $email = htmlspecialchars(string: $_POST["email"]);

    if (empty($email)) {
      exit();
    }

    $formdata = array(
      "fname"=> $fname,
      "lname"=> $lname,
      "email"=> $email
    );

    $file_open = fopen(filename: "./contact_data.csv",mode: "a");

    echo "hi1";
    fputcsv(stream: $file_open, fields: $formdata);
    echo "hi2";
    fclose(stream: $file_open);
    echo "hi3";

    header("Location: ../index.html");
}
else {
    header("Location: ../index.html");
}