#!/usr/bin/bash

while true; do
        echo "Starting Ravi"
	./Raven > /dev/null  2>&1
	sleep 1
	echo "Restarting Ravi"
done
