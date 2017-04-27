#!/bin/bash
# Linux service controller script

service=W3CServer
params="-secure"

case "$1" in
    start)
        start-stop-daemon --start --quiet --background --exec /usr/bin/$service -- -secure
        ;;
    stop)
        killall $service
        ;;
    restart)
        killall $service
        start-stop-daemon --start --quiet --background --exec /usr/bin/$service -- -secure
        ;;
    *)
        echo "Usage: $0 {start|stop|restart}"
        exit 1
        ;;
esac
