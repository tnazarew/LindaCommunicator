cmake_minimum_required(VERSION 3.3)
project(LindaCommunicator)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(
        include
)

add_library(${PROJECT_NAME}
        src/LindaCommunicator.cpp
        include/pid.h include/tuple.h src/ProccessFileUtils.cpp src/ProccessFileUtils.h src/TupleFileUtils.cpp src/TupleFileUtils.h)

add_executable(main main.cpp)
target_link_libraries(main ${PROJECT_NAME})