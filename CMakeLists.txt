cmake_minimum_required(VERSION 3.13)
project(Assignment)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_custom_target(Assignment COMMAND make -C ${Assignment_SOURCE_DIR} CLION_EXE_DIr=${PROJECT_BINARY_DIR})


