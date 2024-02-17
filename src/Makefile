CFLAGS := -ggdb -pedantic -std=c++20 -Wextra -Wall
CLINKS := -lboost_iostreams -lzip -lboost_unit_test_framework
CC := g++

SRCS := $(wildcard *.cpp)
OBJDIR := ./obj
OBJS := $(addprefix $(OBJDIR)/,$(notdir $(SRCS:.cpp=.o)))
TARGET := main

.PHONY: all clean cleanzip

all: $(TARGET) cleanzip
	./main

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(CLINKS)

$(OBJDIR)/%.o: %.cpp
	mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c -o $@ $<

cleanzip:
	rm -f ./zipfiles/*

clean: cleanzip
	rm -rf $(OBJDIR)/*
	rm -f $(TARGET)
	rm -f ./files*/instructions.html
