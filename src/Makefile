CFLAGS := -ggdb -pedantic -std=c++20 -Wextra -Wall
CLINKS := -lboost_iostreams -lzip -lboost_unit_test_framework
CC := g++

SRCS := $(wildcard *.cpp)
OBJDIR := ./obj
ZIP_DIR := ./zipfiles
OBJS := $(addprefix $(OBJDIR)/,$(notdir $(SRCS:.cpp=.o)))
TARGET := main

.PHONY: all clean cleanzip

$(TARGET): $(OBJS)
	mkdir -p $(ZIP_DIR)
	$(CC) $(CFLAGS) -o $@ $^ $(CLINKS)
	
all: $(TARGET) cleanzip
	./main

$(OBJDIR)/%.o: %.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

cleanzip:
	rm -f ./zipfiles/*
	rm -rf ./files-ast/AST

clean: cleanzip
	rm -rf $(OBJDIR)/*
	rm -f $(TARGET)
	rm -f ./files*/instructions.html
