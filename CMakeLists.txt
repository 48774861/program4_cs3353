cmake_minimum_required(VERSION 3.0.0)
project(Project VERSION 0.1.0)

# show compiler output
set(CMAKE_VERBOSE_MAKEFILE ON)
add_compile_options(-Wall -Wextra -pedantic)

# executables
add_executable(main hw4.cpp hw4test.cpp)

set(CMAKE_VERBOSE_MAKEFILE OFF)