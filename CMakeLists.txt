cmake_minimum_required(VERSION 2.8)
project(LindaCommunicator)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g")

include_directories(
        include
)

add_library(${PROJECT_NAME}
        src/LindaCommunicator.cpp
        src/ProcessFileUtils.cpp
        src/TupleFileUtils.cpp
        src/PatternComparator.cpp
        src/MatchesFinder.cpp
        src/SearchProcessFile.cpp
        src/signals.cpp)


add_executable(main main.cpp)
target_link_libraries(main ${PROJECT_NAME})
