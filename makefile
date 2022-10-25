CPPFLAGS := -I /Users/aldiux/Documents/Boost/boost_1_80_0 -I "/Users/aldiux/Documents/Boost/boost_1_80_0/boost/test" -I /Users/aldiux/Documents/Boost/boost_1_80_0/libs/spirit/classic/test/actor -I /Users/aldiux/Documents/Boost/boost_1_80_0/libs/iostreams -I /Users/aldiux/Documents/homebrew/Cellar/libzip/1.9.2/include -I /Users/aldiux/Documents/Boost/boost_1_80_0/stage/lib/cmake
DEPS := util/zip/Zipper.cpp util/Puzzle.cpp util/HtmlGenerator.cpp


test: tests/*
	 for file in $^/Users/aldiux/Downloads/Pointless/pointless-project/; do \
	 	fileNameNoPath=$$(echo $$file | sed 's/tests\///') ; \
	 	fileNameNoEnding=$$(echo "$$fileNameNoPath" | sed 's/.cpp//') ; \
		g++ -std=c++11 $$file $(DEPS) $(CPPFLAGS) -o "obj/$$fileNameNoEnding.test" ; \
	done
build: PuzzleGen.cpp
	g++ PuzzleGen.cpp $(CPPFLAGS) -o PuzzleGen.out 