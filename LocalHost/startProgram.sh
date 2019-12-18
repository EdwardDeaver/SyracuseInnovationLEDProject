#!/bin/bash
# Script that runs openframeworks then python udp server script. 
# Uses caffinate to force conputer to not sleep
# If using non-mac os system comment out caffeinate
echo "Starting OpenFrameworks App, then UDP Server"
#GETS CURRENT DIRECTORY
SCRIPTPATH="$( cd "$(dirname "$0")" ; pwd -P )" 
# SETS OPENFRAMEWORKS APP PATH
OPENFRAMEWORKSPATH="/OpenFrameworks/apps/myApps/PublicInstallationProject/bin/PublicInstallationProjectDebug.app"
# SETS PYTHON APP PATH
PYTHONAPPPATH="/python/UDPSERVER.py"
#OPENS OPENFRAMEWORKS APP
open "$SCRIPTPATH$OPENFRAMEWORKSPATH"
#RUNS MONGODB
mongod --config /usr/local/etc/mongod.conf --fork
#RUNS PYTHON3 APP 
python3 "$SCRIPTPATH$PYTHONAPPPATH"
#FORCES MAC TO NOT SLEEP
caffeinate

