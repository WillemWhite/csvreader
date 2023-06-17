#!/bin/bash
mkdir build
cd build
cmake -G "Unix Makefiles" ..
cmake --build . --config Release --target csvreader
echo "--------------------------------------------------------------------------------"
echo "Csvreader succesfully built!"
echo "For example enter:"
echo "$ ./build/csvreader test-csv-files/test-success-1.csv"
echo "to use it."