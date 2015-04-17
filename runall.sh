#!/bin/bash
RESULT=`pgrep indi-pirc`
if [ "${RESULT:-null}" = null ]; then
    ./indi-pirc/build/indi-pirc &
else
    echo "indi-pirc already running"
fi
RESULT=`pgrep radio-pirc`
if [ "${RESULT:-null}" = null ]; then
    ./radio-pirc/build/radio-pirc &
else
    echo "radio-pirc already running"
fi
RESULT=`pgrep browser-pirc`
if [ "${RESULT:-null}" = null ]; then
    ./browser-pirc/build/browser-pirc -w 320 -h 240 &
else
    echo "browser-pirc already running"
fi
tail -f /var/log/syslog
