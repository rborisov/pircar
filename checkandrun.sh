#!/bin/sh

PROCESS="$1"
PROCANDARGS=$*

while :
do
    RESULT=`pgrep ${PROCESS}`

    if [ "${RESULT:-null}" = null ]; then
        echo "${PROCESS} not running, starting "$PROCANDARGS
        $PROCANDARGS &
    else
        echo "running"
    fi
    sleep 10
done
