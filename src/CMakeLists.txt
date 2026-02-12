# author: fixitdaztv
# date: 2026-02-12

# Add the include directories for UI and GTK
include_directories(
    ${GIMP_INCLUDE_DIRS} 
    ${GIMPUI_INCLUDE_DIRS} 
    ${GTK3_INCLUDE_DIRS} 
    ${GEGL_INCLUDE_DIRS} 
    ${GOBJECT_INCLUDE_DIRS}
)

link_directories(
    ${GIMP_LIBRARY_DIRS} 
    ${GIMPUI_LIBRARY_DIRS} 
    ${GTK3_LIBRARY_DIRS} 
    ${GEGL_LIBRARY_DIRS} 
    ${GOBJECT_LIBRARY_DIRS}
)

add_executable(paa-gimp-plugin paa-gimp-plugin.cpp)

# Link against the UI and GTK libraries
target_link_libraries(paa-gimp-plugin 
    ${GIMP_LIBRARIES} 
    ${GIMPUI_LIBRARIES} 
    ${GTK3_LIBRARIES} 
    ${GEGL_LIBRARIES} 
    ${GOBJECT_LIBRARIES} 
    grad_aff
)