#!/bin/bash
filename=$1
len=${#filename}
echo len
file=${filename:0:len-4}
echo $file
ppmtobmp $file.ppm > $file.bmp && open $file.bmp
echo 'OK'
exit 0
