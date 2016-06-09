cmake_minimum_required(VERSION 2.8)
project(LindaCommunicator)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(
        include
)

add_library(${PROJECT_NAME}
#        src/LindaCommunicator.cpp
#        src/ProccessFileUtils.cpp
#        src/ProccessFileUtils.h
        src/TupleFileUtils.cpp
        src/TupleFileUtils.h src/pid.cpp)


add_executable(main main.cpp)
#target_link_libraries(main ${PROJECT_NAME})