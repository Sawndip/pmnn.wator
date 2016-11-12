CXXFLAGS := -std=c++14
SRC := main.cpp
SRC += pulse.cpp


all:
	clang++ $(CXXFLAGS) $(SRC) -o move.out
run:all
	./move.out
