#!/bin/bash
#make clean
make 
rm /tftpboot/uImage
mkimage -n 'uImage' -A arm -O linux -T kernel -C none -a 0x30008000 -e 0x30008040 -d /home/look/Desktop/linux-2.6.32.2/arch/arm/boot/zImage /tftpboot/uImage
