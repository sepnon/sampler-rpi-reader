cmake_minimum_required(VERSION 3.7)

project(AvrOscilloscope)

set(CMAKE_VERBOSE_MAKEFILE on)

include_directories(${PROJECT_SOURCE_DIR})

set(source_files_oscilloscope_lib avr_io/Spi.cpp oscilloscope/SampleReader.cpp oscilloscope/SpiProfiler.cpp)
set(source_files_spitool tools/SpiTool.cpp)
set(source_files_server server/main.cpp server/datareader.cpp)

add_library(oscilloscope_lib ${source_files_oscilloscope_lib})
target_link_libraries(oscilloscope_lib wiringPi)

add_executable(spitool ${source_files_spitool})
target_link_libraries(spitool oscilloscope_lib)

add_executable(sampleserver ${source_files_server})
target_link_libraries(sampleserver oscilloscope_lib boost_system pthread)
target_include_directories(sampleserver PUBLIC /home/pi/avr_oscilloscope/src)
