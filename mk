#!/bin/bash

#use fresh floppy disk
rm vdisk
cp vdiskCOPY vdisk


./mku init
./mku login
#./mku test

# add my own password file
PARTITION=1
VDISK=vdisk
SECTOR=$(sudo ./ptable vdisk $PARTITION)
OFFSET=$(sudo expr $SECTOR \* 512)

sudo mount -o loop,offset=$OFFSET $VDISK /mnt

echo copying accounts into $VDISK/bin/accounts
sudo cp -av accounts /mnt/bin/accounts
sudo umount /mnt



