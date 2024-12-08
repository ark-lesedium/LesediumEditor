cmake_minimum_required(VERSION 3.10)

project(LesediumEditor)

set(CMAKE_CXX_STANDARD 14)

# Find GTK3 package
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)

# Include GTK3 headers
include_directories(${GTK3_INCLUDE_DIRS})

# Add source files
add_executable(LesediumEditor main.cpp)

# Link GTK3 libraries
target_link_libraries(LesediumEditor ${GTK3_LIBRARIES})

# For debugging
message(STATUS "GTK3_INCLUDE_DIRS = ${GTK3_INCLUDE_DIRS}")
message(STATUS "GTK3_LIBRARIES = ${GTK3_LIBRARIES}")
