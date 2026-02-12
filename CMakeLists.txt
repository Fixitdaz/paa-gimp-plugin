# author: fixitdaztv
# date: 2026-02-12

cmake_minimum_required(VERSION 3.16)
project(paa-gimp-plugin)

# Set C++17 standard for GIMP 3.0 compatibility
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Force static macros for the grad_aff library to fix the __imp__ errors
add_definitions(-DGRAD_AFF_STATIC=1 -DGRAD_AFF_API=)

# 1. Find GIMP 3.0 and OpenMP dependencies
find_package(PkgConfig REQUIRED)
pkg_check_modules(GIMP REQUIRED gimp-3.0 gimpui-3.0)
find_package(OpenMP REQUIRED)

# 2. Tell CMake where to find your .cpp and .h files
# Note we point to the src/ folder here
include_directories(src ${GIMP_INCLUDE_DIRS})

# 3. Create the executable
add_executable(paa-gimp-plugin src/paa-gimp-plugin.cpp)

# 4. Link Libraries
# We link GIMP and the specific static libraries you have in UCRT64
set(LIB_PATH "C:/msys64/ucrt64/lib")
set_target_properties(paa-gimp-plugin PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(paa-gimp-plugin
    PRIVATE
    ${GIMP_LIBRARIES}
    "${LIB_PATH}/libgrad_aff.a"
    "${LIB_PATH}/liblzokay.a"
    "${LIB_PATH}/libsquish.a"
    OpenMP::OpenMP_CXX
)