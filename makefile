CPPFLAGS := -g -lcurl -lboost_iostreams -lzip -lboost_unit_test_framework
DEPS := util/json/WebScraper.cpp util/zip/Zipper.cpp util/Puzzle.cpp util/HtmlGenerator.cpp util/rng/RandomNumber.cpp


test: tests/*
	for file in $^ ; do \
		fileNameNoPath=$$(echo $$file | sed 's/tests\///') ; \
		fileNameNoEnding=$$(echo "$$fileNameNoPath" | sed 's/.cpp//') ; \
		g++ $$file $(DEPS) $(CPPFLAGS) -o "obj/$$fileNameNoEnding.test" ; \
	done
build: PuzzleGen.cpp
	g++ Program.cpp ${DEPS} $(CPPFLAGS) -o PuzzleGen.out