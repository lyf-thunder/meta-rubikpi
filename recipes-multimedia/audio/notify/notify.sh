#!/bin/sh

start() {
    sleep 2
    /usr/sbin/notify
}

stop() {
    killall notify
}

if [[ $1 == "start" ]]; then
    start
elif [[ $1 == "stop" ]]; then
    stop
else
    exit 1
fi

