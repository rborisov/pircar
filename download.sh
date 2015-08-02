#!/bin/sh
cpuname=`uname -m`
echo $cpuname
path=`curl rcm.rborisov.me/debdw.php?cpu=$cpuname`
file=`basename $path`
echo $file
[ ! -f $file ] && wget $path
