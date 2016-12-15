CXXFLAGS := -std=c++14
CXXFLAGS += -Iwaveform
CXXFLAGS += -g
CXXFLAGS += -I/usr/local/include
CXXFLAGS += -DBOOST_LOG_DYN_LINK

LDFLAGS += -L/usr/local/lib
LDFLAGS += -lopencv_imgproc
LDFLAGS += -lopencv_highgui
LDFLAGS += -lopencv_core
LDFLAGS += -lboost_log-mt
LDFLAGS += -lboost_log_setup-mt

SRC := main.cpp
SRC += pulse.cpp
SRC += base_layer.cpp
SRC += audio_layer.cpp
SRC += net.cpp
SRC += waveform/wave.cpp


all:
	clang++ $(CXXFLAGS) $(SRC) $(LDFLAGS) -o move.out
run:all dump-clean
	./move.out
dump-clean:
	rm -rf dump/*.png
