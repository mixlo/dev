#!/usr/bin/env bash

if [ $# -eq 0 ]; then
    echo "Usage: runtests <.tst-file>"
else
    echo $@
fi
