# author: fixitdaztv
# date: 2026-02-12
# Top-level CMake project file for GIMP 3.0 compatibility

cmake_minimum_required (VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# GIMP 3.0 dynamic linking is preferred due to GObject introspection
set(CMAKE_EXE_LINKER_FLAGS "") 

project ("paa-gimp-plugin")

# Find required GIMP 3.0 and GEGL development packages
find_package(PkgConfig REQUIRED)
pkg_check_modules(GIMP REQUIRED gimp-3.0)
pkg_check_modules(GEGL REQUIRED gegl-0.4)
pkg_check_modules(GOBJECT REQUIRED gobject-2.0)

# Include sub-projects
add_subdirectory ("src")