cmake_minimum_required (VERSION 3.19)
project(CVUI)
set(CMAKE_BUILD_TYPE "RELEASE")

find_package(PkgConfig)
find_package (JPEG REQUIRED)

# GTKMM setup
pkg_check_modules(GTKMM gtkmm-4.0)
link_directories(${GTKMM_LIBRARY_DIRS})
include_directories(${GTKMM_INCLUDE_DIRS})
include_directories(${JPEG_INCLUDE_DIR})

# includes
include_directories("include/")
include_directories("include/utils/")
include_directories("include/image_processing/")
include_directories("include/image_processing/segmentation/")
include_directories("include/pipeline/")
include_directories("include/pipeline/image_processing/")
include_directories("include/pipeline/image_processing/segmentation")
include_directories("include/ui/")

# sources files
file(GLOB SOURCES "src/*.cpp")

file(GLOB SOURCES_UTILS "src/utils/*.cpp")
list(APPEND SOURCES ${SOURCES_UTILS})

file(GLOB SOURCES_IMAGE_PROCESSING "src/image_processing/*.cpp")
list(APPEND SOURCES ${SOURCES_IMAGE_PROCESSING})

file(GLOB SOURCES_IMAGE_PROCESSING_SEGMENTATION "src/image_processing/segmentation/*.cpp")
list(APPEND SOURCES ${SOURCES_IMAGE_PROCESSING_SEGMENTATION})

file(GLOB SOURCES_PIPELINE "src/pipeline/*.cpp")
list(APPEND SOURCES ${SOURCES_PIPELINE})

file(GLOB SOURCES_PIPELINE_IP "src/pipeline/image_processing/*.cpp")
list(APPEND SOURCES ${SOURCES_PIPELINE_IP})

file(GLOB SOURCES_PIPELINE_IP_SEGMENTATION "src/pipeline/image_processing/segmentation/*.cpp")
list(APPEND SOURCES ${SOURCES_PIPELINE_IP_SEGMENTATION})

file(GLOB SOURCES_UI "src/ui/*.cpp")
list(APPEND SOURCES ${SOURCES_UI})

# Compiler flag
set(GCC_COMPILE_FLAGS "--std=gnu++17 -O3 -Wall -pedantic")
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COMPILE_FLAGS}")

add_executable(CVUI ${SOURCES})
target_link_libraries(CVUI ${GTKMM_LIBRARIES})
target_link_libraries(CVUI ${JPEG_LIBRARIES})
