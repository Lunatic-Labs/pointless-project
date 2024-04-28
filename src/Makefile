CFLAGS := -ggdb -pedantic -std=c++17 -Wextra -Wall
CLINKS := -lboost_iostreams -lzip -lboost_unit_test_framework
CC := g++

SRCS := $(wildcard *.cpp) $(wildcard *./include/.h)
OBJDIR := ./obj
ZIP_DIR := ./zipfiles
OBJS := $(addprefix $(OBJDIR)/,$(notdir $(SRCS:.cpp=.o)))
TARGET := main

.PHONY: all clean cleanzip make-main

make-main: $(ZIP_DIR) $(OBJDIR) $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(CLINKS)

all: make-main cleanzip
	./main

$(OBJDIR)/%.o: %.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(ZIP_DIR):
	mkdir -p $(ZIP_DIR)

cleanzip:
	rm -f ./zipfiles/*
	rm -rf ./files-ast/AST
	rm -rf ./files-rematch/*
	rm -rf ./files-rematch/.passwords
	rm -f ./files*/instructions.html
	rm -f ./files-rematch-based/____________.html

clean: cleanzip
	rm -rf $(OBJDIR)/*
	rm -f $(TARGET)
