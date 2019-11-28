cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
cmake_policy(SET CMP0074 NEW)

PROJECT(textureTest)
enable_testing()
#find_package(GTest REQUIRED)
include(config_library.cmake)
set(CMAKE_AUTOMOC TRUE)
set(CMAKE_AUTORCC TRUE)
set(srcTest
    src/PrepareBlock3D.h
    src/PrepareBlock3D.cpp
    src/BlockMaterialVisualObject.h
    src/BlockMaterialVisualObject.cpp
    src/vtkTextureMapToECObject.h
    src/vtkTextureMapToECObject.cpp
#    vtkTextureMapToBox.h
#    vtkTextureMapToBox.cpp
    src/vtkTexturingHelper.h
    src/vtkTexturingHelper.cpp
    src/vtkECPolyDataNormals.h
    src/vtkECPolyDataNormals.cpp
    main.cpp
    textures.qrc
    )
add_executable(textureTest
    ${srcTest}
    )

target_link_libraries(textureTest
    ${EC_extra_libs}
    ${ECComponents_LIBRARIES}
    gtestd
    gtest_maind
    gmockd
    gmock_maind
    )


