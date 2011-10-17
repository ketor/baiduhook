#!/bin/bash
BHOOK_TEST_ROOT=../..
source $BHOOK_TEST_ROOT/RunEnv.sh
$BHOOK_ROOT/bin/bhook -e fread -p 80 -i 5 ./readfile
