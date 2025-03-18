<?php
if ($_SERVER["REQUEST_METHOD"] == "POST") {
    $usr_fname = htmlspecialchars(string: $_POST["fname"]);
    $usr_lname = htmlspecialchars(string: $_POST["lname"]);

    $formdata = [
      "fname"=> $usr_fname,
      "lname"=> $usr_lname
    ];

    $file = fopen(filename: './test-data.csv',mode: 'r');
    fgetcsv(stream: $file); //skips first line in csv
    $line = fgetcsv(stream: $file);
    while ($line != false) {
      if ($formdata == $line){
        
      }
    }
}
else {
  header("Location: ../test.html");
}