COMPILE=$(CXX) -c $(CXXFLAGS) $^ -o $@
LINK=$(CXX) $^ -o $@
CXX=c++
CXXFLAGS=-std=c++1z -Wall -Iinclude

.PHONY: setup
setup:
	-mkdir build

.PHONY: test-either
test-either: build/either.test
	$<

build/either.test: build/either.o
	$(LINK)

build/either.o: test/either.cpp
	$(COMPILE)

.PHONY: clean
clean:
	rm build/*

