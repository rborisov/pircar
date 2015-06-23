#!/bin/bash
RESULT=`pgrep indi-pirc`
if [ "${RESULT:-null}" != null ]; then
    killall indi-pirc
fi

./watchdog.sh -daemon ./build/indi-pirc/indi-pirc &

RESULT=`pgrep radio-pirc`
if [ "${RESULT:-null}" != null ]; then
    killall radio-pirc
fi

./watchdog.sh -daemon ./build/radio-pirc/radio-pirc &

RESULT=`pgrep browser-pirc`
if [ "${RESULT:-null}" != null ]; then
    killall browser-pirc
fi

./watchdog.sh -daemon ./build/browser-pirc/browser-pirc &

tail -f /var/log/syslog
