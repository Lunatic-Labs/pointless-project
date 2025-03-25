#!/bin/bash
cd ~/Desktop/pointless-project/web-server
touch ./integrated-tests/output.txt

cp ./includes/contact-data.csv ./integrated-tests/output.csv

wget --post-data 'usr_fname=j&usr_lname=j&usr_email=j@j.com' http://localhost:8000/index.php -O ./integrated-tests/output.txt

# Before vs After
diff -y ./integrated-tests/output.txt download.html # webpage
diff -y ./integrated-tests/output.csv ./includes/contact-data.csv 

rm ./integrated-tests/output.csv
rm ./integrated-tests/output.txt