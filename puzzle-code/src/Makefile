CFLAGS := -ggdb -pedantic -std=c++17 -Wextra -Wall
CLINKS := -lzip
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
	rm -rf ../html-txt/files-bst/tree
	rm -rf ../html-txt/files-rematch/*
	rm -rf ../html-txt/files-rematch/.passwords
	rm -f ../html-txt/files*/instructions.html
	rm -f ../html-txt/files-rematch-based/____________.html

clean: cleanzip
	rm -rf $(OBJDIR)/*
	rm -f $(TARGET)
