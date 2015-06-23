#!/usr/bin/env bash
b=$2
a=${b##*/}
echo ${a}
PIDFILE=~/.${a}.pid
if [ x"$1" = x-daemon ]; then
    if test -f "$PIDFILE"; then exit; fi
    echo $$ > "$PIDFILE"
    trap "rm '$PIDFILE'" EXIT SIGTERM
    while true; do
        #launch your app here
        #echo $2 $3 $4 $5 $6
        "$2"
        wait # needed for trap to work
    done
elif [ x"$1" = x-stop ]; then
    kill `cat "$PIDFILE"`
else
    echo use script -daemon[-stop] app
#    nohup "$0" -daemon
fi
