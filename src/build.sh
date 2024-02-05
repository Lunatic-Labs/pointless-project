#!/bin/bash

CXXFLAGS="-ggdb -pedantic -std=c++20 -Wextra -Wall -o main"
CXXLINKS="-lboost_iostreams -lzip -lboost_unit_test_framework"
CXXDEPS="./*.cpp"

# Load with demo seed
if [ "$1" == "demo" ];
then
    # Seed is optional. If empty, seed will default to 1.
    SEED="$2"
    if [ "$SEED" == "" ];
    then
        SEED="1"
    fi
    echo "Loading demo with seed: $SEED"
    ./build.sh c
    ./build.sh
    ./main $SEED
elif [ "$1" == "c" ];
then
  echo "Cleaning..."
  rm -rf *.zip ./zipfiles/* >/dev/null
  rm -rf ./files*/instructions.html >/dev/null
  rm -rf ./files*/*.ppm >/dev/null
  rm -rf ./files*/*.svg >/dev/null
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
