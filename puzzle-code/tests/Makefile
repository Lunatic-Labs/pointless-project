CFLAGS := -ggdb -pedantic -std=c++17 -Wextra -Wall
INCLUDES := -I../src/include -I./include
CLINKS := -lzip
CC := g++

SRCS := $(wildcard *.cpp) $(wildcard ../src/*.cpp)
OBJDIR := ./obj
OBJS := $(addprefix $(OBJDIR)/,$(notdir $(SRCS:.cpp=.o)))
TARGET := main

make-main: $(OBJDIR) $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^ $(CLINKS)

all: clean make-main
	./main

$(OBJDIR)/%.o: %.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<

$(OBJDIR)/%.o: ../src/%.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<

$(OBJDIR):
	mkdir -p $(OBJDIR)

clean:
	rm -rf $(OBJDIR)/*
	rm -f $(TARGET)
