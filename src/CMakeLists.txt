cmake_minimum_required(VERSION 3.7)

project(AvrOscilloscope)

set(CMAKE_VERBOSE_MAKEFILE on)

set(source_files_oscilloscope_api driver.cpp SampleReader.cpp SpiProfiler.cpp)
set(source_files_spi SpiTool.cpp)
set(source_files_server server/main.cpp server/datareader.cpp)

add_library(oscilloscope_lib ${source_files_oscilloscope_api})
target_link_libraries(oscilloscope_lib wiringPi)

add_executable(spitool ${source_files_spi})
target_link_libraries(spitool oscilloscope_lib)

add_executable(sampleserver ${source_files_server})
target_link_libraries(sampleserver oscilloscope_lib boost_system pthread)
target_include_directories(sampleserver PUBLIC /home/pi/avr_oscilloscope/src)
