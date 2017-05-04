#!/bin/sh
# Linux service controller script

service=W3CServer

case "$1" in
    start)
        start-stop-daemon --start --quiet --background --exec /usr/bin/$service
        ;;
    stop)
        killall $service
        ;;
    restart)
        killall $service
        start-stop-daemon --start --quiet --background --exec /usr/bin/$service
        ;;
    *)
        echo "Usage: $0 {start|stop|restart}"
        exit 1
        ;;
esac

