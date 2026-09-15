# Builds the puzzle generator (src/) and its tests (tests/).
#
# Run make from puzzle-code/. The binaries themselves must run from their own
# directory (src/ or tests/) because resource paths are relative to it, for
# example ../html-txt/files-math/.desc.txt. The run and test targets do that.

CXX      := g++
CXXFLAGS := -ggdb -pedantic -std=c++17 -Wextra -Wall -O0 --coverage -MMD -MP
LDFLAGS  := --coverage
LDLIBS   := -lzip

BUILD    := build
GEN_BIN  := src/main
TEST_BIN := tests/main
ZIP_DIR  := src/zipfiles

SRC_OBJS  := $(patsubst %.cpp,$(BUILD)/%.o,$(wildcard src/*.cpp))
TEST_OBJS := $(patsubst %.cpp,$(BUILD)/%.o,$(wildcard tests/*.cpp))
# The tests define their own main() and FLAGS, so they link everything except src/main.o.
LIB_OBJS  := $(filter-out $(BUILD)/src/main.o,$(SRC_OBJS))

.PHONY: build run test coverage cleanzip clean

build: $(GEN_BIN) | $(ZIP_DIR)

run: build cleanzip
	cd src && ./main

test: $(TEST_BIN)
	cd tests && ./main

# Requires `make run` or `make test` to have produced coverage data.
coverage:
	cd $(BUILD)/src && gcov -o . $(abspath $(wildcard src/*.cpp))

$(GEN_BIN): $(SRC_OBJS)
	$(CXX) $(LDFLAGS) -o $@ $^ $(LDLIBS)

$(TEST_BIN): $(TEST_OBJS) $(LIB_OBJS)
	$(CXX) $(LDFLAGS) -o $@ $^ $(LDLIBS)

$(BUILD)/src/%.o: src/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(BUILD)/tests/%.o: tests/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -Itests/include -c -o $@ $<

$(ZIP_DIR):
	mkdir -p $@

# Removes generated zips and the files the generator writes into html-txt/.
cleanzip:
	rm -f $(ZIP_DIR)/*
	rm -rf html-txt/files-bst/tree
	rm -rf html-txt/files-rematch/*
	rm -rf html-txt/files-rematch/.passwords
	rm -f html-txt/files*/instructions.html
	rm -f html-txt/files-rematch-based/____________.html

clean: cleanzip
	rm -rf $(BUILD)
	rm -f $(GEN_BIN) $(TEST_BIN)

-include $(SRC_OBJS:.o=.d) $(TEST_OBJS:.o=.d)
