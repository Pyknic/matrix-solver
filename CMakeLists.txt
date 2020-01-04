cmake_minimum_required(VERSION 3.14)
project(matrix_solver_cpp)

set(CMAKE_CXX_STANDARD 17)

file(GLOB ${PROJECT_NAME}_SRC "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

add_executable(${PROJECT_NAME} ${${PROJECT_NAME}_SRC})