#!/bin/bash
cd ..
cdt=$(date +"%Y-%m-%d_%H:%M:%S")
touch ./integrated-tests/test-login-GET-output-$cdt.txt

# GET to retrieve index page and put into txt file
wget http://localhost:8000/index.php -O ./integrated-tests/test-login-GET-output-$cdt.txt

# Retreived webpage vs Expetcted webpage
if ! grep -q '<input type="text" id="email" name="email" required>' ./integrated-tests/test-login-GET-output-$cdt.txt; then
    echo 'Error: Email input element.'
elif ! grep -q '<button type="submit">Submit</button>' ./integrated-tests/test-login-GET-output-$cdt.txt; then
    echo 'Error: No submit button.'
else
    # Delete wget output, if successful
    echo 'No Errors. Elements present!'
    rm ./integrated-tests/test-login-GET-output-$cdt.txt
fi