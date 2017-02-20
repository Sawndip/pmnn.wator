cmake_minimum_required(VERSION 2.8)
project (pmnn)

set (CMAKE_CXX_FLAGS "-std=c++14 -g")
include_directories(waveform)
include_directories(./)
include_directories(/usr/local/include)
add_definitions(-DBOOST_LOG_DYN_LINK)

link_directories(/usr/local/lib)


set (SRC main.cpp
  pulse.cpp
  base_layer.cpp
  audio_layer.cpp
  net.cpp
  core/base_layer.cpp
  core/base_blob.cpp
  core/audio_layer.cpp
  core/net.cpp
  waveform/wave.cpp
)

add_executable(pmnn.out ${SRC})

if(APPLE)
  set(BOOST boost_log-mt  boost_log_setup-mt )
else(APPLE)
  set(BOOST boost_log  boost_log_setup )
endif(APPLE)

target_link_libraries(pmnn.out
  opencv_imgproc
  opencv_highgui
  opencv_core
  ${BOOST}
  pthread
)
