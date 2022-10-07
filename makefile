CPPFLAGS := -lboost_iostreams -lzip -lboost_unit_test_framework


# These variables are from https://stackoverflow.com/a/2908351
SRC_TEST_DIR := ./tests
TEST_DIR := ./obj
SRCTEST := $(wildcard $(SRC_TEST_DIR)/*.cpp)
DSTTEST := $(patsubst $(SRC_TEST_DIR)/%.cpp,$(TEST_DIR)/%.o,$(SRCTEST))

$(TEST_DIR)/%.test: $(SRC_TEST_DIR)/%.cpp
	g++ $< $(CPPFLAGS) -o $@

build: PuzzleGen.cpp
	g++ PuzzleGen.cpp $(CPPFLAGS) -o PuzzleGen.out