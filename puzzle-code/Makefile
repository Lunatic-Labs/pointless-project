# Builds the puzzle generator (src/) and its tests (tests/).
#
# Run make from puzzle-code/. The binaries themselves must run from their own
# directory (src/ or tests/) because resource paths are relative to it, for
# example ../resources/files-math/.desc.txt. The run and test targets do that.
#
# `make test T=bst` runs only the tests whose names contain "bst".
#
# `make production` builds an optimized generator without coverage into
# production/ (src/main plus a clean copy of resources/). The web server runs
# that copy and never runs make; a person rebuilds it after changing the code.

CXX      := g++
CXXFLAGS := -ggdb -pedantic -std=c++17 -Wextra -Wall -Wshadow -O0 --coverage -MMD -MP
LDFLAGS  := --coverage
LDLIBS   := -lzip

BUILD    := build
GEN_BIN  := src/main
TEST_BIN := tests/main
ZIP_DIR  := src/zipfiles

PROD_DIR      := production
PROD_BUILD    := $(BUILD)/production
PROD_CXXFLAGS := -pedantic -std=c++17 -Wextra -Wall -Wshadow -O2 -MMD -MP
PROD_BIN      := $(PROD_BUILD)/main

SRC_OBJS  := $(patsubst %.cpp,$(BUILD)/%.o,$(wildcard src/*.cpp))
TEST_OBJS := $(patsubst %.cpp,$(BUILD)/%.o,$(wildcard tests/*.cpp))
PROD_OBJS := $(patsubst src/%.cpp,$(PROD_BUILD)/%.o,$(wildcard src/*.cpp))
# The tests define their own main() and FLAGS, so they link everything except src/main.o.
LIB_OBJS  := $(filter-out $(BUILD)/src/main.o,$(SRC_OBJS))

# Removes generated zips and the files the generator writes into resources/,
# under the tree rooted at $(1).
define clean-generated
	rm -f $(1)/$(ZIP_DIR)/*
	rm -rf $(1)/resources/files-bst/tree
	rm -rf $(1)/resources/files-rematch/*
	rm -rf $(1)/resources/files-rematch/.passwords
	rm -f $(1)/resources/files*/instructions.html
endef

.PHONY: build run test coverage production cleanzip clean

build: $(GEN_BIN) | $(ZIP_DIR)

run: build cleanzip
	cd src && ./main

test: $(TEST_BIN)
	cd tests && ./main $(T)

# Requires `make run` or `make test` to have produced coverage data.
coverage:
	cd $(BUILD)/src && gcov -o . $(abspath $(wildcard src/*.cpp))

# Replaces production/ with a fresh optimized build and clean resources.
production: $(PROD_BIN)
	rm -rf $(PROD_DIR)
	mkdir -p $(PROD_DIR)/$(ZIP_DIR)
	cp $(PROD_BIN) $(PROD_DIR)/$(GEN_BIN)
	cp -r resources $(PROD_DIR)/resources
	$(call clean-generated,$(PROD_DIR))

$(GEN_BIN): $(SRC_OBJS)
	$(CXX) $(LDFLAGS) -o $@ $^ $(LDLIBS)

$(TEST_BIN): $(TEST_OBJS) $(LIB_OBJS)
	$(CXX) $(LDFLAGS) -o $@ $^ $(LDLIBS)

$(PROD_BIN): $(PROD_OBJS)
	$(CXX) -o $@ $^ $(LDLIBS)

$(BUILD)/src/%.o: src/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(BUILD)/tests/%.o: tests/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -Itests/include -c -o $@ $<

$(PROD_BUILD)/%.o: src/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(PROD_CXXFLAGS) -c -o $@ $<

$(ZIP_DIR):
	mkdir -p $@

# Also removes the zips a failed game_zipfiles test may leave in tests/.
cleanzip:
	$(call clean-generated,.)
	rm -rf tests/zipfiles

# Does not remove production/, which a running web server may be using.
clean: cleanzip
	rm -rf $(BUILD)
	rm -f $(GEN_BIN) $(TEST_BIN)

-include $(SRC_OBJS:.o=.d) $(TEST_OBJS:.o=.d) $(PROD_OBJS:.o=.d)
