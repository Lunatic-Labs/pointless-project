#!/bin/bash

set -xe

CXXFLAGS="-ggdb -pedantic -std=c++20 -Wextra -Wall -o main"
CXXLINKS="-lboost_iostreams -lzip -lboost_unit_test_framework"
CXXDEPS="./utils.cpp ./puzzle.cpp"

if [ "$1" == "c" ];
then
  rm -rf main *.zip ./zipfiles/*
elif [ "$1" == "a" ];
then
  ./build.sh c
  g++ $CXXFLAGS $CXXDEPS main.cpp $CXXLINKS
else
  g++ $CXXFLAGS $CXXDEPS main.cpp $CXXLINKS
fi
