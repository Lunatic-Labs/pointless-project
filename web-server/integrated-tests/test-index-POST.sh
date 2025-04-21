#!/bin/bash
cd ..
current_datetime=$(date +"%Y-%m-%d_%H:%M:%S")
echo $current_datetime
touch ./integrated-tests/test-index-POST_output_$current_datetime.txt
cp ./includes/contact-data.csv ./integrated-tests/original-data_$current_datetime.csv

# Send POST request to index.php
wget --post-data "fname=j&lname=j&email=j@j.com" http://localhost:8000/index.php -O ./integrated-tests/test-index-POST_output_$current_datetime.txt

# Check for if input went through, delete output txt if successful
if ! grep 'j,j,j@j.com' ./includes/contact-data.csv; then
    echo 'Error: input not received'
else
    echo 'Input received. No errors.'
    sed -i '$d' ./includes/contact-data.csv
fi

# Check integrity of contact-data.csv, delete output csv if successful 
if diff ./includes/contact-data.csv ./integrated-tests/original-data_$current_datetime.csv; then
    echo 'Changes to contact-data.csv?: No, data is intact.'
    rm ./integrated-tests/original-data_$current_datetime.csv
else
    echo 'Changes to contact-data.csv?: !!contact-data.csv compromised!! Check for changes!!'
fi

# Delete output txt
rm ./integrated-tests/test-index-POST_output_$current_datetime.txt