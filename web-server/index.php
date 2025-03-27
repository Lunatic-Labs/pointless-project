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

    // invalid email error *fix; not showing up
    if (filter_var($usr_email, FILTER_VALIDATE_EMAIL) === false) {
        $error = "!! INVALID EMAIL !! Please enter a vaild email...";
    } elseif ($success = false){
        // duplicate email in csv error check *fix; not showing up
        $file = fopen(filename: './includes/contact-data.csv', mode:'r');
        fgetcsv(stream: $file); //skips first line in csv
        $line = fgetcsv(stream: $file);
        while($line != false){
            $email = $line[2];
            if ($usr_email == $email) {
                $error = "!! DUPLICATE EMAIL !! Please enter a different email...";
                exit;
            }
            $line = fgetcsv($file);
        };
        fclose($file);
    } else {
        $formdata = array(
        "fname"=> $usr_fname,
        "lname"=> $usr_lname,
        "email"=> $usr_email
        );
        $file_open = fopen(filename: "./includes/contact-data.csv",mode: "a");

        // puts data into csv and closes connection
        fputcsv(stream: $file_open, fields: $formdata);
        fclose(stream: $file_open);
        $success = true;
    }
    header("Location: ../download.php");
}
?>

<!DOCTYPE html>
<html lang="en">
<head>
    <link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.0.7/css/all.css">
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Pointless Challenge</title>
    <style>
        #theme-btn {
            font-size: 1.5rem;
            cursor: pointer;
            color: #596AFF;
        }
        #theme-btn:hover {
            color: #BB86FC;
        }
        .changeTheme {
            background: #1D1E22; 
            color: #eee;
        }        
        .maindiv{
            display: flex;
            flex-direction: column;
            height: 95vh;
        }
        .title{ 
            font-size: 72px; 
            font-weight: bold; 
            color: #331E54; 
            text-shadow:4px  4px 0 #F4AA00;
                }
        .imagen{
                    height:9rem; 
                    padding-left: 28px;
                }
        .container {
                width: 80%;
                margin: -4px auto 0; /* This makes the bison stand on the box. We were cookin bro.*/
                padding: 20px;
                border: 1px solid #ccc;
                border-radius: 8px;
                text-align: center;
            }
        .container ul {
                text-align: center;
                list-style-position: inside;
            }
        .container p {
                max-width: 60%;
                margin: 10px auto;
            }
    </style>
</head>
<body>
    <div id="theme-btn" class="far fa-moon"></div>
    <div style="display:flex; align-items:center; justify-content: center;">
        <div class="title">Pointless Challenge</div>
        <div class="imagen">
            <svg width="78.750000" height="141.750000" xmlns="http://www.w3.org/2000/svg" version="1.1">
                <rect x="31.500000" y="0.000000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="36.750000" y="0.000000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="42.000000" y="0.000000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="47.250000" y="0.000000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="5.250000" y="5.250000" width="5.250000" height="5.250000" fill="#D2BB8D"   />
                <rect x="21.000000" y="5.250000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="26.250000" y="5.250000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="31.500000" y="5.250000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="36.750000" y="5.250000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="42.000000" y="5.250000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="47.250000" y="5.250000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="52.500000" y="5.250000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="57.750000" y="5.250000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="68.250000" y="5.250000" width="5.250000" height="5.250000" fill="#D2BB8D"   />
                <rect x="5.250000" y="10.500000" width="5.250000" height="5.250000" fill="#D2BB8D"   />
                <rect x="10.500000" y="10.500000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="15.750000" y="10.500000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="21.000000" y="10.500000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="26.250000" y="10.500000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="31.500000" y="10.500000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="36.750000" y="10.500000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="42.000000" y="10.500000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="47.250000" y="10.500000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="52.500000" y="10.500000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="57.750000" y="10.500000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="63.000000" y="10.500000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="68.250000" y="10.500000" width="5.250000" height="5.250000" fill="#D2BB8D"   />
                <rect x="5.250000" y="15.750000" width="5.250000" height="5.250000" fill="#D2BB8D"   />
                <rect x="10.500000" y="15.750000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="15.750000" y="15.750000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="21.000000" y="15.750000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="26.250000" y="15.750000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="31.500000" y="15.750000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="36.750000" y="15.750000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="42.000000" y="15.750000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="47.250000" y="15.750000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="52.500000" y="15.750000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="57.750000" y="15.750000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="63.000000" y="15.750000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="68.250000" y="15.750000" width="5.250000" height="5.250000" fill="#D2BB8D"   />
                <rect x="5.250000" y="21.000000" width="5.250000" height="5.250000" fill="#D2BB8D"   />
                <rect x="10.500000" y="21.000000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="15.750000" y="21.000000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="21.000000" y="21.000000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="26.250000" y="21.000000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="31.500000" y="21.000000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="36.750000" y="21.000000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="42.000000" y="21.000000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="47.250000" y="21.000000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="52.500000" y="21.000000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="57.750000" y="21.000000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="63.000000" y="21.000000" width="5.250000" height="5.250000" fill="#D2BB8D"   />
                <rect x="68.250000" y="21.000000" width="5.250000" height="5.250000" fill="#D2BB8D"   />
                <rect x="5.250000" y="26.250000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="10.500000" y="26.250000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="15.750000" y="26.250000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="21.000000" y="26.250000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="26.250000" y="26.250000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="31.500000" y="26.250000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="36.750000" y="26.250000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="42.000000" y="26.250000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="47.250000" y="26.250000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="52.500000" y="26.250000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="57.750000" y="26.250000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="63.000000" y="26.250000" width="5.250000" height="5.250000" fill="#D2BB8D"   />
                <rect x="68.250000" y="26.250000" width="5.250000" height="5.250000" fill="#D2BB8D"   />
                <rect x="73.500000" y="26.250000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="5.250000" y="31.500000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="10.500000" y="31.500000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="15.750000" y="31.500000" width="5.250000" height="5.250000" fill="#FFFFFF"   />
                <rect x="21.000000" y="31.500000" width="5.250000" height="5.250000" fill="#000000"   />
                <rect x="26.250000" y="31.500000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="31.500000" y="31.500000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="36.750000" y="31.500000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="42.000000" y="31.500000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="47.250000" y="31.500000" width="5.250000" height="5.250000" fill="#000000"   />
                <rect x="52.500000" y="31.500000" width="5.250000" height="5.250000" fill="#FFFFFF"   />
                <rect x="57.750000" y="31.500000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="63.000000" y="31.500000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="68.250000" y="31.500000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="73.500000" y="31.500000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="5.250000" y="36.750000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="10.500000" y="36.750000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="15.750000" y="36.750000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="21.000000" y="36.750000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="26.250000" y="36.750000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="31.500000" y="36.750000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="36.750000" y="36.750000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="42.000000" y="36.750000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="47.250000" y="36.750000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="52.500000" y="36.750000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="57.750000" y="36.750000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="63.000000" y="36.750000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="68.250000" y="36.750000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="73.500000" y="36.750000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="10.500000" y="42.000000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="15.750000" y="42.000000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="21.000000" y="42.000000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="26.250000" y="42.000000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="31.500000" y="42.000000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="36.750000" y="42.000000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="42.000000" y="42.000000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="47.250000" y="42.000000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="52.500000" y="42.000000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="57.750000" y="42.000000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="63.000000" y="42.000000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="68.250000" y="42.000000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="10.500000" y="47.250000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="15.750000" y="47.250000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="21.000000" y="47.250000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="26.250000" y="47.250000" width="5.250000" height="5.250000" fill="#000000"   />
                <rect x="31.500000" y="47.250000" width="5.250000" height="5.250000" fill="#000000"   />
                <rect x="36.750000" y="47.250000" width="5.250000" height="5.250000" fill="#000000"   />
                <rect x="42.000000" y="47.250000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="47.250000" y="47.250000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="52.500000" y="47.250000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="57.750000" y="47.250000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="63.000000" y="47.250000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="68.250000" y="47.250000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="15.750000" y="52.500000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="21.000000" y="52.500000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="26.250000" y="52.500000" width="5.250000" height="5.250000" fill="#000000"   />
                <rect x="31.500000" y="52.500000" width="5.250000" height="5.250000" fill="#000000"   />
                <rect x="36.750000" y="52.500000" width="5.250000" height="5.250000" fill="#000000"   />
                <rect x="42.000000" y="52.500000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="47.250000" y="52.500000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="52.500000" y="52.500000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="57.750000" y="52.500000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="63.000000" y="52.500000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="21.000000" y="57.750000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="26.250000" y="57.750000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="31.500000" y="57.750000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="36.750000" y="57.750000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="42.000000" y="57.750000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="47.250000" y="57.750000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="52.500000" y="57.750000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="57.750000" y="57.750000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="15.750000" y="63.000000" width="5.250000" height="5.250000" fill="#331E54"   />
                <rect x="21.000000" y="63.000000" width="5.250000" height="5.250000" fill="#331E54"   />
                <rect x="26.250000" y="63.000000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="31.500000" y="63.000000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="36.750000" y="63.000000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="42.000000" y="63.000000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="47.250000" y="63.000000" width="5.250000" height="5.250000" fill="#331E54"   />
                <rect x="52.500000" y="63.000000" width="5.250000" height="5.250000" fill="#331E54"   />
                <rect x="57.750000" y="63.000000" width="5.250000" height="5.250000" fill="#331E54"   />
                <rect x="5.250000" y="68.250000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="10.500000" y="68.250000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="15.750000" y="68.250000" width="5.250000" height="5.250000" fill="#331E54"   />
                <rect x="21.000000" y="68.250000" width="5.250000" height="5.250000" fill="#331E54"   />
                <rect x="26.250000" y="68.250000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="31.500000" y="68.250000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="36.750000" y="68.250000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="42.000000" y="68.250000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="47.250000" y="68.250000" width="5.250000" height="5.250000" fill="#331E54"   />
                <rect x="52.500000" y="68.250000" width="5.250000" height="5.250000" fill="#331E54"   />
                <rect x="57.750000" y="68.250000" width="5.250000" height="5.250000" fill="#331E54"   />
                <rect x="63.000000" y="68.250000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="68.250000" y="68.250000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="0.000000" y="73.500000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="5.250000" y="73.500000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="10.500000" y="73.500000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="15.750000" y="73.500000" width="5.250000" height="5.250000" fill="#331E54"   />
                <rect x="21.000000" y="73.500000" width="5.250000" height="5.250000" fill="#331E54"   />
                <rect x="26.250000" y="73.500000" width="5.250000" height="5.250000" fill="#331E54"   />
                <rect x="31.500000" y="73.500000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="36.750000" y="73.500000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="42.000000" y="73.500000" width="5.250000" height="5.250000" fill="#331E54"   />
                <rect x="47.250000" y="73.500000" width="5.250000" height="5.250000" fill="#331E54"   />
                <rect x="52.500000" y="73.500000" width="5.250000" height="5.250000" fill="#331E54"   />
                <rect x="57.750000" y="73.500000" width="5.250000" height="5.250000" fill="#331E54"   />
                <rect x="63.000000" y="73.500000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="68.250000" y="73.500000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="73.500000" y="73.500000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="0.000000" y="78.750000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="5.250000" y="78.750000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="10.500000" y="78.750000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="15.750000" y="78.750000" width="5.250000" height="5.250000" fill="#331E54"   />
                <rect x="21.000000" y="78.750000" width="5.250000" height="5.250000" fill="#331E54"   />
                <rect x="26.250000" y="78.750000" width="5.250000" height="5.250000" fill="#331E54"   />
                <rect x="31.500000" y="78.750000" width="5.250000" height="5.250000" fill="#F4AA00"   />
                <rect x="36.750000" y="78.750000" width="5.250000" height="5.250000" fill="#331E54"   />
                <rect x="42.000000" y="78.750000" width="5.250000" height="5.250000" fill="#331E54"   />
                <rect x="47.250000" y="78.750000" width="5.250000" height="5.250000" fill="#331E54"   />
                <rect x="52.500000" y="78.750000" width="5.250000" height="5.250000" fill="#331E54"   />
                <rect x="57.750000" y="78.750000" width="5.250000" height="5.250000" fill="#331E54"   />
                <rect x="63.000000" y="78.750000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="68.250000" y="78.750000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="73.500000" y="78.750000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="0.000000" y="84.000000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="5.250000" y="84.000000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="10.500000" y="84.000000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="15.750000" y="84.000000" width="5.250000" height="5.250000" fill="#331E54"   />
                <rect x="21.000000" y="84.000000" width="5.250000" height="5.250000" fill="#331E54"   />
                <rect x="26.250000" y="84.000000" width="5.250000" height="5.250000" fill="#331E54"   />
                <rect x="31.500000" y="84.000000" width="5.250000" height="5.250000" fill="#F4AA00"   />
                <rect x="36.750000" y="84.000000" width="5.250000" height="5.250000" fill="#331E54"   />
                <rect x="42.000000" y="84.000000" width="5.250000" height="5.250000" fill="#331E54"   />
                <rect x="47.250000" y="84.000000" width="5.250000" height="5.250000" fill="#331E54"   />
                <rect x="52.500000" y="84.000000" width="5.250000" height="5.250000" fill="#331E54"   />
                <rect x="57.750000" y="84.000000" width="5.250000" height="5.250000" fill="#331E54"   />
                <rect x="63.000000" y="84.000000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="68.250000" y="84.000000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="73.500000" y="84.000000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="5.250000" y="89.250000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="10.500000" y="89.250000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="15.750000" y="89.250000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="21.000000" y="89.250000" width="5.250000" height="5.250000" fill="#000000"   />
                <rect x="26.250000" y="89.250000" width="5.250000" height="5.250000" fill="#331E54"   />
                <rect x="31.500000" y="89.250000" width="5.250000" height="5.250000" fill="#F4AA00"   />
                <rect x="36.750000" y="89.250000" width="5.250000" height="5.250000" fill="#331E54"   />
                <rect x="42.000000" y="89.250000" width="5.250000" height="5.250000" fill="#331E54"   />
                <rect x="47.250000" y="89.250000" width="5.250000" height="5.250000" fill="#331E54"   />
                <rect x="52.500000" y="89.250000" width="5.250000" height="5.250000" fill="#000000"   />
                <rect x="57.750000" y="89.250000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="63.000000" y="89.250000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="68.250000" y="89.250000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="10.500000" y="94.500000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="15.750000" y="94.500000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="21.000000" y="94.500000" width="5.250000" height="5.250000" fill="#000000"   />
                <rect x="26.250000" y="94.500000" width="5.250000" height="5.250000" fill="#331E54"   />
                <rect x="31.500000" y="94.500000" width="5.250000" height="5.250000" fill="#F4AA00"   />
                <rect x="36.750000" y="94.500000" width="5.250000" height="5.250000" fill="#F4AA00"   />
                <rect x="42.000000" y="94.500000" width="5.250000" height="5.250000" fill="#F4AA00"   />
                <rect x="47.250000" y="94.500000" width="5.250000" height="5.250000" fill="#331E54"   />
                <rect x="52.500000" y="94.500000" width="5.250000" height="5.250000" fill="#000000"   />
                <rect x="57.750000" y="94.500000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="63.000000" y="94.500000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="15.750000" y="99.750000" width="5.250000" height="5.250000" fill="#331E54"   />
                <rect x="21.000000" y="99.750000" width="5.250000" height="5.250000" fill="#331E54"   />
                <rect x="26.250000" y="99.750000" width="5.250000" height="5.250000" fill="#331E54"   />
                <rect x="31.500000" y="99.750000" width="5.250000" height="5.250000" fill="#331E54"   />
                <rect x="36.750000" y="99.750000" width="5.250000" height="5.250000" fill="#331E54"   />
                <rect x="42.000000" y="99.750000" width="5.250000" height="5.250000" fill="#331E54"   />
                <rect x="47.250000" y="99.750000" width="5.250000" height="5.250000" fill="#331E54"   />
                <rect x="52.500000" y="99.750000" width="5.250000" height="5.250000" fill="#331E54"   />
                <rect x="57.750000" y="99.750000" width="5.250000" height="5.250000" fill="#331E54"   />
                <rect x="15.750000" y="105.000000" width="5.250000" height="5.250000" fill="#331E54"   />
                <rect x="21.000000" y="105.000000" width="5.250000" height="5.250000" fill="#331E54"   />
                <rect x="26.250000" y="105.000000" width="5.250000" height="5.250000" fill="#331E54"   />
                <rect x="31.500000" y="105.000000" width="5.250000" height="5.250000" fill="#331E54"   />
                <rect x="36.750000" y="105.000000" width="5.250000" height="5.250000" fill="#331E54"   />
                <rect x="42.000000" y="105.000000" width="5.250000" height="5.250000" fill="#331E54"   />
                <rect x="47.250000" y="105.000000" width="5.250000" height="5.250000" fill="#331E54"   />
                <rect x="52.500000" y="105.000000" width="5.250000" height="5.250000" fill="#331E54"   />
                <rect x="57.750000" y="105.000000" width="5.250000" height="5.250000" fill="#331E54"   />
                <rect x="15.750000" y="110.250000" width="5.250000" height="5.250000" fill="#331E54"   />
                <rect x="21.000000" y="110.250000" width="5.250000" height="5.250000" fill="#331E54"   />
                <rect x="26.250000" y="110.250000" width="5.250000" height="5.250000" fill="#331E54"   />
                <rect x="31.500000" y="110.250000" width="5.250000" height="5.250000" fill="#331E54"   />
                <rect x="36.750000" y="110.250000" width="5.250000" height="5.250000" fill="#331E54"   />
                <rect x="42.000000" y="110.250000" width="5.250000" height="5.250000" fill="#331E54"   />
                <rect x="47.250000" y="110.250000" width="5.250000" height="5.250000" fill="#331E54"   />
                <rect x="52.500000" y="110.250000" width="5.250000" height="5.250000" fill="#331E54"   />
                <rect x="57.750000" y="110.250000" width="5.250000" height="5.250000" fill="#331E54"   />
                <rect x="15.750000" y="115.500000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="21.000000" y="115.500000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="26.250000" y="115.500000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="31.500000" y="115.500000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="36.750000" y="115.500000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="42.000000" y="115.500000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="47.250000" y="115.500000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="52.500000" y="115.500000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="57.750000" y="115.500000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="15.750000" y="120.750000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="21.000000" y="120.750000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="26.250000" y="120.750000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="31.500000" y="120.750000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="42.000000" y="120.750000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="47.250000" y="120.750000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="52.500000" y="120.750000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="57.750000" y="120.750000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="10.500000" y="126.000000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="15.750000" y="126.000000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="21.000000" y="126.000000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="26.250000" y="126.000000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="47.250000" y="126.000000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="52.500000" y="126.000000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="57.750000" y="126.000000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="63.000000" y="126.000000" width="5.250000" height="5.250000" fill="#552D1B"   />
                <rect x="5.250000" y="131.250000" width="5.250000" height="5.250000" fill="#000000"   />
                <rect x="10.500000" y="131.250000" width="5.250000" height="5.250000" fill="#000000"   />
                <rect x="15.750000" y="131.250000" width="5.250000" height="5.250000" fill="#000000"   />
                <rect x="21.000000" y="131.250000" width="5.250000" height="5.250000" fill="#000000"   />
                <rect x="52.500000" y="131.250000" width="5.250000" height="5.250000" fill="#000000"   />
                <rect x="57.750000" y="131.250000" width="5.250000" height="5.250000" fill="#000000"   />
                <rect x="63.000000" y="131.250000" width="5.250000" height="5.250000" fill="#000000"   />
                <rect x="68.250000" y="131.250000" width="5.250000" height="5.250000" fill="#000000"   />
                <rect x="0.000000" y="136.500000" width="5.250000" height="5.250000" fill="#000000"   />
                <rect x="5.250000" y="136.500000" width="5.250000" height="5.250000" fill="#000000"   />
                <rect x="10.500000" y="136.500000" width="5.250000" height="5.250000" fill="#000000"   />
                <rect x="15.750000" y="136.500000" width="5.250000" height="5.250000" fill="#000000"   />
                <rect x="21.000000" y="136.500000" width="5.250000" height="5.250000" fill="#000000"   />
                <rect x="52.500000" y="136.500000" width="5.250000" height="5.250000" fill="#000000"   />
                <rect x="57.750000" y="136.500000" width="5.250000" height="5.250000" fill="#000000"   />
                <rect x="63.000000" y="136.500000" width="5.250000" height="5.250000" fill="#000000"   />
                <rect x="68.250000" y="136.500000" width="5.250000" height="5.250000" fill="#000000"   />
                <rect x="73.500000" y="136.500000" width="5.250000" height="5.250000" fill="#000000"   />
                </svg>                    
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
    <div class="container"><h2 style="text-align:center">The Pointless Challenge.</h2><p style="text-align:center">
        Hi, welcome to the Pointless Project!<br> 
        Please register your email, so we can get you to the download page.<br>
        Already registered? Login <a href="./login.html">here</a>.
        <?php if ($error): ?>
        <div class="error"><?php echo $error; ?></div>
        <?php endif; ?>
        <form action="<?php echo htmlspecialchars($_SERVER["PHP_SELF"]); ?>" method="post" id="stringForm"> 
            <label for="fname">First Name:</label>
            <input type="text" id="fname" name="fname" required>
            
            <label for="lname">Last Name:</label>
            <input type="text" id="lname" name="lname" required>
            
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