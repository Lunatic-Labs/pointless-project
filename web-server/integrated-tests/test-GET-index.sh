#!/bin/bash
cd ~/Desktop/pointless-project/web-server
touch ./integrated-tests/output.txt

# GET to retrieve index page and put into txt file
wget http://localhost:8000/index.php -O ./integrated-tests/output.txt

# Retreived webpage vs Expetcted webpage
if ! grep -q '<input type="text" id="fname" name="fname" required>' ./integrated-tests/output.txt; then
    echo 'Error: No First Name input element.'
elif ! grep -q '<input type="text" id="lname" name="lname" required>' ./integrated-tests/output.txt; then
    echo 'Error: No Last Name input element.'
elif ! grep -q '<input type="text" id="email" name="email" required>' ./integrated-tests/output.txt; then
    echo 'Error: Email input element.'
elif ! grep -q '<button type="submit">Submit</button>' ./integrated-tests/output.txt; then
    echo 'Error: No submit button.'
else
    echo 'No Errors. Elements present!'
fi

# Delete wget output
rm ./integrated-tests/output.txt