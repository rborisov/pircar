#!/bin/bash

while true; do
    LC_ALL=C nmcli -t -f TYPE,STATE dev | grep -q "^gsm:disconnected$"
    if [ $? -eq 0 ]; then
        #jdownloader is still in the download status so stop it because
        #internet is disconnected and jdownloader won't resume download 
        #when connected again
        #jdownloader --stop-download
        #sometimes I can not get connected after disconnection when 
        #I click on <name of the network connection>. I have to disable
        #and enable Mobile Broadband
        nmcli -t nm wwan off
        sleep 1
        nmcli -t nm wwan on
        sleep 1
        nmcli -t con up id "Tata Docomo Internet"
        #wait approximately 15 sec to get connected
        #if anyone can add better command to check for it just comment it :-p 
        sleep 15
        #now connected to internet so start download
        #jdownloader --start-download
    fi
    #it does not worth keep it checking every millisecond.
    #my connection will be reestablished within 5-15 seconds
    sleep 2
    #if anyone can code it better please feel free to comment
    #TO-DO:: check for data received. if data < 15 KB after 20 seconds of connection
    #reconnect mobile broadband connection  
done
