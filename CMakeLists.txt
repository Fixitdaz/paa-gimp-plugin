# author: fixitdaztv
# date: 2026-02-12
cmake_minimum_required (VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# UCRT64/MinGW preferred linker flags
set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++") 

project ("paa-gimp-plugin")

find_package(PkgConfig REQUIRED)

# We must explicitly look for GIMP UI and GTK3 now
pkg_check_modules(GIMP REQUIRED gimp-3.0)
pkg_check_modules(GIMPUI REQUIRED gimp-ui-3.0)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
pkg_check_modules(GEGL REQUIRED gegl-0.4)
pkg_check_modules(GOBJECT REQUIRED gobject-2.0)

add_subdirectory ("src")