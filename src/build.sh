#!/bin/bash

set -xe

CXXFLAGS="-ggdb -pedantic -std=c++20 -Wextra -Wall -o main"
CXXLINKS="-lboost_iostreams -lzip -lboost_unit_test_framework"
CXXDEPS="./*.cpp"

if [ "$1" == "c" ];
then
  rm -rf main *.zip ./zipfiles/*
elif [ "$1" == "a" ];
then
  ./build.sh c
  g++ $CXXFLAGS $CXXDEPS $CXXLINKS
else
  g++ $CXXFLAGS $CXXDEPS $CXXLINKS
fi
