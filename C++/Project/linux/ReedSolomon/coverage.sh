#!/bin/bash
make clean
make all
cd dist/Debug/GNU-Linux-x86/
./reedsolomon
cd ../../../
lcov -d . -o coverage.info -b . -c
 genhtml -o result coverage.info
