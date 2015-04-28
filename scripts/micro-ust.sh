#!/bin/bash
cd "$(dirname "$0")"
helpers/prepare.sh
APPS=../

helpers/execute.sh ust-base.csv $APPS/tk-ust empty

helpers/trace-ust.sh
helpers/execute.sh ust-tracepoint.csv $APPS/tk-ust
lttng destroy
