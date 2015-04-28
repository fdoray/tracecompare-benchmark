#!/bin/bash
cd "$(dirname "$0")"
helpers/prepare.sh

sudo rm -rf ../results/current
LOGFILE=../results/current/log.txt
mkdir -p "${LOGFILE%/*}"

#echo '  Doing getpid.'
#echo '  Doing getpid.' >> $LOGFILE
#./micro-getpid.sh >> $LOGFILE 2>> $LOGFILE
echo '  Doing libunwind.'
echo '  Doing libunwind.' >> $LOGFILE
./micro-libunwind.sh >> $LOGFILE 2>> $LOGFILE
#echo '  Doing ust.'
#echo '  Doing ust.' >> $LOGFILE
#./micro-ust.sh >> $LOGFILE 2>> $LOGFILE
#echo '  Doing overhead.'
#echo '  Doing overhead.' >> $LOGFILE
#./overhead-all.sh >> $LOGFILE 2>> $LOGFILE
