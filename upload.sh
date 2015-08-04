#!/bin/sh
for i in ./*.deb
do
echo $i
curl -F file=@$i http://rcm.rborisov.me/debup.php
done
