#!/bin/bash

set -e

CXXFLAGS="-ggdb -pedantic -std=c++20 -Wextra -Wall -o main"
CXXLINKS="-lboost_iostreams -lzip -lboost_unit_test_framework"
CXXDEPS="./*.cpp"

if [ "$1" == "c" ];
then
  echo "Cleaning up..."
  rm -rf main *.zip ./zipfiles/*
  rm ./files*/instructions.html
elif [ "$1" == "a" ];
then
  ./build.sh c
  ./build.sh
  ./build.sh r
elif [ "$1" == "r" ];
then
  echo "Running..."
  ./main
else
  echo "Building..."
  g++ $CXXFLAGS $CXXDEPS $CXXLINKS
fi
