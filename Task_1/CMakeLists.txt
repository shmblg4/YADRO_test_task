cmake_minimum_required(VERSION 3.20)
project(Task_1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)
file(GLOB SOURCES "src/*.cpp")

add_executable(Task_1_QPSK tests/test_QPSK.cpp ${SOURCES})
add_executable(Task_1_QAM16 tests/test_QAM16.cpp ${SOURCES})
add_executable(Task_1_QAM64 tests/test_QAM64.cpp ${SOURCES})
add_executable(Task_1_test tests/task_4_test.cpp ${SOURCES})