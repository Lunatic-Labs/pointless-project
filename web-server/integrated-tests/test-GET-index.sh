#!/bin/bash
cd ~/Desktop/pointless-project/web-server
touch ./integrated-tests/output.txt

# get copy of contact-data.csv prior to input
cp ./includes/contact-data.csv ./integrated-tests/ideal-csv/test-duplicate-user.csv

# GET to retrieve index page
wget http://localhost:8000/index.php -O ./integrated-tests/output.txt

# Retreived webpage vs Expetcted webpage
diff -y ./integrated-tests/output.txt index.php

rm ./integrated-tests/output.txt