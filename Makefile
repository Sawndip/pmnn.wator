CXXFLAGS := -std=c++14
CXXFLAGS += -Iwaveform

SRC := main.cpp
SRC += pulse.cpp
SRC += waveform/wave.cpp


all:
	clang++ $(CXXFLAGS) $(SRC) -o move.out
run:all
	./move.out
