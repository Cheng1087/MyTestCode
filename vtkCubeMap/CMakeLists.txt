cmake_minimum_required(VERSION 3.3 FATAL_ERROR)

project(cubemap)
set(VTK_DIR C:/Program Files/VTK/bin)
find_package(VTK REQUIRED)
include(${VTK_USE_FILE})
INCLUDE_DIRECTORIES(${VTK_INCLUDE_DIRS})
link_libraries(${VTK_LIBRARIES})
add_executable(cubemap main.cpp vtkECPolyDataNormals.h vtkECPolyDataNormals.cpp)
