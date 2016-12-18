cmake_minimum_required(VERSION 2.8)
project (pmmnn)

set (CMAKE_CXX_FLAGS "-std=c++14 -g")
include_directories(waveform)
include_directories(/usr/local/include)
add_definitions(-DBOOST_LOG_DYN_LINK)


set (SRC main.cpp
  pulse.cpp
  base_layer.cpp
  audio_layer.cpp
  net.cpp
  waveform/wave.cpp
)

add_executable(pmmn.out ${SRC})
target_link_libraries(pmmn.out
  opencv_imgproc
  opencv_highgui
  opencv_core
  boost_log
  boost_log_setup
  pthread
)

