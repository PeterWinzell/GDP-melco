#!/bin/sh
# Linux service controller script

service=OpenDSAdapter
params=""

case "$1" in
    start)
        start-stop-daemon --start --quiet --background --exec /usr/bin/$service -- $params
        ;;
    stop)
        killall $service
        ;;
    restart)
        killall $service
        start-stop-daemon --start --quiet --background --exec /usr/bin/$service -- $params
        ;;
    *)
        echo "Usage: $0 {start|stop|restart}"
        exit 1
        ;;
esac
