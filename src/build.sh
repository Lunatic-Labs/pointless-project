#!/bin/bash

CXXFLAGS="-ggdb -pedantic -std=c++20 -Wextra -Wall -O3 -o main"
CXXLINKS="-lboost_iostreams -lzip -lboost_unit_test_framework"
CXXDEPS="./*.cpp"

if [ "$1" == "c" ];
then
  echo "Cleaning..."
  rm -rf *.zip ./zipfiles/* >/dev/null 2>&1
  rm ./files*/instructions.html >/dev/null 2>&1
  rm ./files*/*.ppm >/dev/null 2>&1
  rm ./files*/*.svg >/dev/null 2>&1
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
  mkdir -p ./zipfiles
  g++ $CXXFLAGS $CXXDEPS $CXXLINKS
fi
