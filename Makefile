.PHONY: all build test clean

CXXFLAGS=-Wall -Wextra -Wpedantic -std=c++11

all: build

build: out out/TestBadData out/TestTreeList out/TestLargeData

out:
	mkdir -p out

out/TestBadData: TestBadData.cpp RTree.h
	$(CXX) -o $@ ${CXXFLAGS} $<

out/TestTreeList: TestTreeList.cpp RTree.h
	$(CXX) -o $@ ${CXXFLAGS} $<

out/TestLargeData: TestLargeData.cpp RTree.h
	$(CXX) -o $@ ${CXXFLAGS} $<

test: build
	./out/TestBadData baddata.txt
	./out/TestTreeList
	./out/TestLargeData

clean:
	rm -rf out
