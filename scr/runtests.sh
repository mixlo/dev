#!/usr/bin/env bash
FILES=./bin/*.tst

echo $0
echo $(dirname $0)

for test_file in ../bin/*.tst; do
    $test_file
done

#if [ $# -eq 0 ]; then
#    echo "Usage: runtests <.tst-file>"
#else
#    ./bin/bst_test.tst
#fi
