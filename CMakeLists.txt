cmake_minimum_required(VERSION 3.14)
project(matrix_solver_cpp)

set(CMAKE_CXX_STANDARD 17)
set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(TEST_DIR "${CMAKE_CURRENT_SOURCE_DIR}/test")
set(THIRDPARTY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty")

file(GLOB ${PROJECT_NAME}_SRC "${SRC_DIR}/*.cpp")
file(GLOB ${PROJECT_NAME}_TESTS "${TEST_DIR}/*.cpp")

# Create a library from the sources to make testing easier
add_library(${PROJECT_NAME}_lib ${${PROJECT_NAME}_SRC})

# Create the test target
add_subdirectory(${THIRDPARTY_DIR}/googletest)
add_executable(${PROJECT_NAME}_test ${${PROJECT_NAME}_TESTS})
add_test(NAME ${PROJECT_NAME}_test COMMAND ${PROJECT_NAME}_test)
target_link_libraries(${PROJECT_NAME}_test PUBLIC ${PROJECT_NAME}_lib gtest)

# Create an executable
add_executable(${PROJECT_NAME} ${${PROJECT_NAME}_SRC})