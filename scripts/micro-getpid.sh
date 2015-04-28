#!/bin/bash
cd "$(dirname "$0")"
helpers/prepare.sh
APPS=../

helpers/execute.sh getpid-base.csv $APPS/tk-getpid

helpers/lttngprofilemodules.sh emptyprobes
helpers/profile.sh getpid-emptyprobes.csv $APPS/tk-getpid

helpers/lttngprofile.sh syscall_empty
helpers/lttngprofilemodules.sh nosignal
helpers/profile.sh getpid-bookkeeping.csv $APPS/tk-getpid

helpers/lttngprofile.sh syscall_empty
helpers/lttngprofilemodules.sh signal_getpid
helpers/profile.sh getpid-signal.csv $APPS/tk-getpid
