<?php

if ($_SERVER["REQUEST_METHOD"] == "POST") {
    $firstname = htmlspecialchars(string: $_POST["firstname"]);
    $lastname = htmlspecialchars(string: $_POST["lastname"]);

    if (empty($firstname) || empty($lastname)) {
      exit();
    }

    $file_open = fopen(filename: "./contact_data.csv",mode: "a");

    echo "hi1";
    fputcsv($file_open, [$firstname, $lastname]);
    echo "hi2";
    fclose(stream: $file_open);
    echo "hi3";

    header("Location: ../index.php");
}
else {
    header("Location: ../index.php");
}