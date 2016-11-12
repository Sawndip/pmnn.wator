
CXXFLAGS := -std=c++14
CXXFLAGS += -g
CXXFLAGS += -I/usr/local/include

LDFLAGS += -L/usr/local/lib
LDFLAGS += -lopencv_imgproc
LDFLAGS += -lopencv_highgui
LDFLAGS += -lopencv_core

all:
	clang++ $(CXXFLAGS) main.cpp wave.cpp $(LDFLAGS) -o wave.out
run:all
	./wave.out
