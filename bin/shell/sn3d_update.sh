#!/bin/sh

cd /SN3D/sn3d-project

echo "\n -- SN3D 3D Printer Firmware Update Start --\n"
echo "Image pull from remote server...\n"
echo "Current branch check\n"
echo "Branchs 		  \n"
git branch

echo "Checkout to master branch\n"
git checkout master

echo "git pull master\n"
git pull origin master

echo "Start Compile...\n"
make all_clean
make all

echo "Compile done.\n"

while true;
do 
    echo -n "firmware update completed. are you want reboot system? (yes/no): "
    read ANSWER

    if [ $ANSWER = y ] || [ $ANSWER = yes ];
    then
	    echo "Start reboot\n"
	    sudo reboot 
	    break
    elif [ $ANSWER = n ] || [ $ANSWER = no ];
    then
	    break

    else
	    echo "plase input 'y' or 'yes' and 'n' or 'no'.\n" 
    fi
done

echo "\n -- SN3D Firmware Update Commplte -- \n"
