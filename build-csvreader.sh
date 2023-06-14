#!/bin/bash
g++ -c tablemodel.cpp
g++ -c tableview.cpp
g++ -c main.cpp
g++ -o csvreader main.o tablemodel.o tableview.o
echo "Csvreader succesfully built!"
echo "For example enter:"
echo "$ ./csvreader file.csv"
echo "to use it."