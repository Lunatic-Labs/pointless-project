#!/bin/bash
cd ..
cdt=$(date +"%Y-%m-%d_%H:%M:%S")
echo $cdt
# Player data file used by the server (the default location; see includes/players.php).
data=../data/contact-data.csv
touch ./integrated-tests/test-index-POST-output-$cdt.txt

# Back up the player data. It may not exist yet if no one has registered.
had_data=false
if [ -f $data ]; then
    cp $data ./integrated-tests/original-data-$cdt.csv
    had_data=true
fi

# Send POST request to index.php
wget --post-data "fname=j&lname=j&email=j@j.com" http://localhost:8000/index.php -O ./integrated-tests/test-index-POST-output-$cdt.txt

# Check for if input went through
if ! grep -q 'j,j,j@j.com' $data; then
    echo 'Error: input not received'
else
    echo 'Input received. No errors.'
fi

# Restore the player data
if $had_data; then
    cp ./integrated-tests/original-data-$cdt.csv $data && rm ./integrated-tests/original-data-$cdt.csv
    echo 'Player data restored.'
else
    rm -f $data
fi

# Delete output txt
rm ./integrated-tests/test-index-POST-output-$cdt.txt
