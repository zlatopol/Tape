cmake_minimum_required(VERSION 3.10)
project(main)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


include_directories(include)

find_package(nlohmann_json REQUIRED)
find_package(GTest REQUIRED)

add_executable(main src/main.cpp src/TmpTapeGenerator.cpp src/Tape.cpp src/TapeSorter.cpp)

add_executable(test src/test.cpp src/TmpTapeGenerator.cpp src/Tape.cpp src/TapeSorter.cpp)

target_link_libraries(main nlohmann_json::nlohmann_json)
target_link_libraries(test gtest::gtest)