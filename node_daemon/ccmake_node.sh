#!/bin/bash
#
# Daemon Name: ipS
#
# chkconfig: - 58 74
# description: Our Script

# Source function library.
. /etc/init.d/functions


prog=ccmake_node
lockfile=/tmp/$prog

start() {
    #Make some checks for requirements before continuing
    [ -x ../cmake-build-debug/$prog ] || exit 5

    # Start our daemon daemon
    echo -n $"Starting $prog: "
    daemon --pidfile /var/run/${proc}.pid $prog
    RETVAL=$?
    echo

    #If all is well touch the lock file
    [ $RETVAL -eq 0 ] && touch $lockfile
    return $RETVAL
}

stop() {
    echo -n $"Shutting down $prog: "
    killproc $prog
    RETVAL=$?
    echo

    #If all is well remove the lockfile
    [ $RETVAL -eq 0 ] && rm -f $lockfile
    return $RETVAL
}

# See how we were called.
case "$1" in
  start)
        start
        ;;
  stop)
        stop
        ;;
  status)
        status $prog
        ;;
  restart)
        stop
        start
        ;;
   *)
        echo $"Usage: $0 {start|stop|status|restart}"
        exit 2
esac