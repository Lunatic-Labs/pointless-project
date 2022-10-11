CPPFLAGS := -lboost_iostreams -lzip -lboost_unit_test_framework
DEPS := util/zip/Zipper.cpp util/Puzzle.cpp


test: tests/*
	for file in $^ ; do \
		fileNameNoPath=$$(echo $$file | sed 's/tests\///') ; \
		fileNameNoEnding=$$(echo "$$fileNameNoPath" | sed 's/.cpp//') ; \
		g++ $$file $(DEPS) $(CPPFLAGS) -o "obj/$$fileNameNoEnding.test" ; \
	done
build: PuzzleGen.cpp
	g++ PuzzleGen.cpp $(CPPFLAGS) -o PuzzleGen.out