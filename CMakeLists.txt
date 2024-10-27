cmake_minimum_required (VERSION 3.1)
project (TakeHomeAssignment)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIR})

add_executable(logParser src/main.cpp src/webServerAnalyser.cpp)
add_executable(test_webServerAnalyser tests/test_webServerAnalyser.cpp src/webServerAnalyser.cpp)

target_link_libraries(test_webServerAnalyser ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES} pthread)
