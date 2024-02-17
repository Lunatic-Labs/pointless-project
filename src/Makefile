CFLAGS= -ggdb -pedantic -std=c++20 -Wextra -Wall -o main
CLINKS= -lboost_iostreams -lzip -lboost_unit_test_framework
CC=g++

SRCS := $(wildcard *.cpp)
OBJS := $(SRCS:.cpp=.o)
TARGET := main

.PHONY: all clean

$(TARGET): $(OBJS)
	mkdir -p ./zipfiles
	$(CC) $(CFLAGS) $(CLINKS) -o $@ $^

%.o: %.cpp
	g++ $(CFLAGS) -c -o $@ $<

clean:
	rm -rf main *.zip ./zipfiles/*
	rm ./files*/instructions.html