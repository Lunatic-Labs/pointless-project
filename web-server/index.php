<?php
$file = fopen(filename: './includes/contact-data.csv', mode:'r');

// $test_input = "f@gmail.com";
$test_input = "m@c.com";

fgetcsv(stream: $file); //skips first line in csv
$line = fgetcsv(stream: $file);
while($line != false){
    $email = $line[2];
    echo "Email: $email \n";
    if ($test_input == $email) {
        echo"<script>alert('invalid email')
        window.location.href='../index.php'</script>";
        exit;
        };
    $line = fgetcsv($file);
}

fclose($file);