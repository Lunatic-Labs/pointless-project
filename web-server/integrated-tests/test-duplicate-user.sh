#!/bin/bash

cp ~/Desktop/pointless-project/web-server/includes/contact-data.csv ~/Desktop/pointless-project/web-server/integrated-tests/ideal-csv/test-duplicate-user.csv
wget --post-data 'usr_fname=j&usr_lname=j&usr_email=j@j.com' http://localhost:8000/includes/pointless-form.php

diff ~/Desktop/pointless-project/web-server/includes/contact-data.csv ~/Desktop/pointless-project/web-server/integrated-tests/ideal-csv/test-duplicate-user.csv
rm ideal-csv/test-duplicate-user.csv
