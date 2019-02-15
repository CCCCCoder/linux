#!/bin/bash
#make clean
make 
rm /tftpboot/uImage
mkimage -n 'uImage' -A arm -O linux -T kernel -C none -a 0x30008000 -e 0x30008040 -d arch/arm/boot/zImage /tftpboot/uImage

