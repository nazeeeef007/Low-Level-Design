cmake_minimum_required(VERSION 3.16)
project(elevator_system LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Threads REQUIRED)

add_library(elevator
    Elevator.cpp
    ElevatorSelectionStrategy.cpp
    ElevatorSystem.cpp
    Display.cpp)
target_include_directories(elevator PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(elevator PUBLIC Threads::Threads)

add_executable(elevator_demo main.cpp)
target_link_libraries(elevator_demo PRIVATE elevator)

add_executable(elevator_tests tests.cpp)
target_link_libraries(elevator_tests PRIVATE elevator)

enable_testing()
add_test(NAME elevator_tests COMMAND elevator_tests)
