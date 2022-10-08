CPPFLAGS := -lboost_iostreams -lzip -lboost_unit_test_framework



test: tests/UtilityTests.cpp tests/PuzzleTests.cpp
	g++ tests/PuzzleTests.cpp util/Puzzle.cpp $(CPPFLAGS) -o obj/PuzzleTests.test
	g++ tests/UtilityTests.cpp $(CPPFLAGS) -o obj/UtilityTests.test

build: PuzzleGen.cpp
	g++ PuzzleGen.cpp $(CPPFLAGS) -o PuzzleGen.out