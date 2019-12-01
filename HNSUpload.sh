#!/bin/bash
HOST='192.168.178.116'
USER='pi'
PASS='raspberry'
TARGETFOLDER='/home/pi/Workspace/HomeNetSystem'
SOURCEFOLDER='/home/gio/Workspace/HomeNetSystem'

lftp -f "
open $HOST
user $USER $PASS
lcd $SOURCEFOLDER
mirror --reverse --delete --verbose $SOURCEFOLDER $TARGETFOLDER
bye
"
