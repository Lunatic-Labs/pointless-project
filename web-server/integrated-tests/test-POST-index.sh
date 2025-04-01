#!/bin/bash
cd ~/Desktop/pointless-project/web-server
touch ./integrated-tests/output.txt
cp ./includes/contact-data.csv ./integrated-tests/original-data.csv

# Send POST request to index.php
wget --post-data "fname=j&lname=j&email=j@j.com" http://localhost:8000/index.php -O ./integrated-tests/output.txt

# Check for if input went through
if ! grep 'j,j,j@j.com' ./includes/contact-data.csv; then
    echo 'Error: input not received'
else
    echo 'Input received. No errors.'
fi

# Check integrity of contact-data.csv 
if diff ./includes/contact-data.csv ./integrated-tests/original-data.csv; then
    echo 'Changes to contact-data.csv?: No, data is intact.'
else
    echo 'Changes to contact-data.csv?: !!contact-data.csv compromised!! Check for changes!!'
fi

# Delete wget output
rm ./integrated-tests/output.txt
rm ./integrated-tests/original-data.csv