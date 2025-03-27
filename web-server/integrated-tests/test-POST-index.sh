#!/bin/bash
cd ~/Desktop/pointless-project/web-server
touch ./integrated-tests/output.txt

# Send POST request to index.php
wget --post-data "fname=j&lname=j&email=j@j.com" http://localhost:8000/index.php -O ./integrated-tests/output.txt

# Check for if input went through
if ! grep 'j,j,j@j.com' ./includes/contact-data.csv; then
    echo 'Error: input not received'
else
    echo 'Input received. No errors.'
fi

# Delete wget output and remove test data from csv
rm ./integrated-tests/output.txt
sed -i '$d' ./includes/contact-data.csv