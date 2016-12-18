CXXFLAGS := -std=c++14
CXXFLAGS += -Iwaveform
CXXFLAGS += -g
CXXFLAGS += -I/usr/local/include
CXXFLAGS += -DBOOST_LOG_DYN_LINK

LDFLAGS += -L/usr/local/lib
LDFLAGS += -lopencv_imgproc
LDFLAGS += -lopencv_highgui
LDFLAGS += -lopencv_core
LDFLAGS += -lboost_log
LDFLAGS += -lboost_log_setup
LDFLAGS += -lpthread

SRC := main.cpp
SRC += pulse.cpp
SRC += base_layer.cpp
SRC += audio_layer.cpp
SRC += net.cpp
SRC += waveform/wave.cpp

OBJ := $(SRC:.cpp=.o)

Target := pmmn.out

.PHONY: all run dump-clean clean

all:$(Target)

$(Target):$(OBJ)
	clang++ $(CXXFLAGS) $^ $(LDFLAGS) -o $@

run:all dump-clean
	./pmmn.out
clean:
	rm -rf *.out
dump-clean:
	rm -rf dump/*.png


