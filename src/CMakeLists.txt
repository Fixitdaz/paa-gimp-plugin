# author: fixitdaztv
# date: 2026-02-12

include_directories(${GIMP_INCLUDE_DIRS} ${GEGL_INCLUDE_DIRS} ${GOBJECT_INCLUDE_DIRS})
link_directories(${GIMP_LIBRARY_DIRS} ${GEGL_LIBRARY_DIRS} ${GOBJECT_LIBRARY_DIRS})

add_executable(paa-gimp-plugin paa-gimp-plugin.cpp)
target_link_libraries(paa-gimp-plugin ${GIMP_LIBRARIES} ${GEGL_LIBRARIES} ${GOBJECT_LIBRARIES} grad_aff)