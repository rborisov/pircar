#!/bin/sh
path=`curl rcm.rborisov.me/debdw.php?cpu=i686`
file=`basename $path`
echo $file
[ ! -f $file ] && wget $path
