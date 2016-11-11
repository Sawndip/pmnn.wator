
CXXFLAGS := -std=c++14
all:
	clang++ $(CXXFLAGS) main.cpp -o move.out
run:all
	./move.out
