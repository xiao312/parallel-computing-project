#!/bin/bash

echo ""

rm MyExecutable 2>/dev/null
rm -rf build 2>/dev/null
rm *.log 2>/dev/null

mkdir build ; cd build
cmake -DOpenBLAS_DIR=/home/xk/OpenBLAS-develop/ .. && make

echo ""

mv MyExecutable ..; cd .. ; /usr/bin/time -v ./MyExecutable

rm MyExecutable 2>/dev/null
rm -rf build 2>/dev/null

echo ""