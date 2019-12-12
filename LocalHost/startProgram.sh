#!/bin/bash
# Script that runs openframeworks then python udp server script. 
# Uses caffinate to force conputer to not sleep
# If using non-mac os system comment out caffeinate
echo "Starting OpenFrameworks App, then UDP Server"
open OpenFrameworks/apps/myApps/PublicInstallationProject/bin/PublicInstallationProjectDebug.app 
python3 python/UDPSERVER.py
caffeinate

