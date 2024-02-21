CFLAGS := -ggdb -pedantic -std=c++17 -Wextra -Wall
CLINKS := -lboost_iostreams -lzip -lboost_unit_test_framework
CC := g++

SRCS := $(wildcard *.cpp)
OBJDIR := ./obj
OBJS := $(addprefix $(OBJDIR)/,$(notdir $(SRCS:.cpp=.o)))
TARGET := main

.PHONY: all clean cleanzip

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(CLINKS)

all: $(TARGET) cleanzip
	./main

$(OBJDIR)/%.o: %.cpp
	mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c -o $@ $<

cleanzip:
	rm -f ./zipfiles/*
	rm -rf ./files-ast/AST

clean: cleanzip
	rm -rf $(OBJDIR)/*
	rm -f $(TARGET)
	rm -f ./files*/instructions.html
