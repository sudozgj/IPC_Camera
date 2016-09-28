#!/bin/bash

while true
do
	echo `date +%F" "%H:%M:%S`
	java -jar doorHandler.jar /home/shc/DZG/IPC_Camera/doorLog
	sleep 30
done
