#!/usr/bin/env bash

set -euox pipefail

if [ -f $1 ]; then
    echo "$1 exists"
elif [ -d "$1" ]; then 
    echo "$1 is a directory"
else 
    echo "$1 not found"
fi
