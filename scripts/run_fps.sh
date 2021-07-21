#!/bin/bash

DATAFILE="../data/datafile"
LOGFILE="../log/fps.log"
LISTENPORT=15158	# port number
MAXCLIENTS=3    	# number of clients
MAXMEMBLOCK=10  	# number of blocks in cash memory
BLOCKSIZE=4 		# block's size in bytes
MAXRECV=16	    	# msg length

echo " arguments " $DATAFILE $LOGFILE $LISTENPORT $MAXRECV $MAXCLIENTS $MAXMEMBLOCK $BLOCKSIZE

../bin/fps $DATAFILE $LOGFILE $LISTENPORT $MAXRECV $MAXCLIENTS $MAXMEMBLOCK $BLOCKSIZE

exit 0
