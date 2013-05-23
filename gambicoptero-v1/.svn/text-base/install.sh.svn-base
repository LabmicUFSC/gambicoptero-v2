#!/bin/bash
APP=aeromain
if [ -n "$1" ]; then
   APP=$1
fi 


make APPLICATION=$APP
/usr/local/arm/gcc/bin/arm-objcopy -I elf32-little -O binary img/$APP.img img/$APP.bin
python tools/emote/red-bsl.py -t /dev/ttyUSB0 -f img/$APP.bin
