cmake_minimum_required (VERSION 3.22)
project(PuzzleMaker)

find_package(PkgConfig)

# GTKMM setup
pkg_check_modules(GTKMM gtkmm-3.0)
link_directories(${GTKMM_LIBRARY_DIRS})
include_directories(${GTKMM_INCLUDE_DIRS})

# includes
include_directories("include/")
include_directories("include/utils/")

# sources files
file(GLOB SOURCES "src/*.cpp")

file(GLOB SOURCES_UTILS "src/utils/*.cpp")
list(APPEND SOURCES ${SOURCES_UTILS})

# Compiler flag
set(GCC_COMPILE_FLAGS "--std=gnu++17 -O2 -Wall -pedantic")
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COMPILE_FLAGS}")

add_executable(PuzzleMaker ${SOURCES})
target_link_libraries(PuzzleMaker ${GTKMM_LIBRARIES})
