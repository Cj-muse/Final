#!/bin/bash

#use fresh floppy disk
rm vdisk
cp vdiskCOPY vdisk


./mku init
./mku login
./mku sh
#./mku test

# add my own files
PARTITION=1
VDISK=vdisk
SECTOR=$(sudo ./ptable vdisk $PARTITION)
OFFSET=$(sudo expr $SECTOR \* 512)

sudo mount -o loop,offset=$OFFSET $VDISK /mnt

echo copying accounts into $VDISK/bin/accounts
sudo cp -av accounts /mnt/bin/accounts

#echo copying shell into $VDISK/bin/shell
#sudo cp -av sh.o /mnt/bin/shell

sudo umount /mnt



