#!/bin/sh 

echo "Starting Mobile Broadband Connection. Tej"
while true; do
  # testing...to see if gsm is on the list of active devices
  LC_ALL=C nmcli -t -f TYPE,STATE dev | grep -q "^gsm:disconnected$"
  if [ $? -eq 0 ]; then
    break
  else
    # not connected, sleeping for a second
    sleep 1
  fi
done

# now once GSM modem shows up, run these commands
while true; do
  # Enable Mobile Broadband
  nmcli -t nm wwan on

  # Connect to network
  nmcli -t con up id "BSNL/CellOne New GPRS/3G 1"

  # Check status if connected or not
  nmcli -f device,state -t dev | grep ttyACM0 | awk -F':' '{print $2}' | { read status; }

  echo $status;

  if [$status == "connected"]; then
    break
  else
    # not connected, sleeping for a second
    nmcli -t nm wwan off
    sleep 1
  fi
done
