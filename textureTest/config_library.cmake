if(WIN32)
    # for windows find_package(GTest REQUIRED) DONOT work
    set(GTEST_ROOT "C:/Program Files/googletest-distribution")
	include_directories("C:/Program Files/googletest-distribution/include")
    set(GTEST_MSVC_SEARCH MT)
else()
    set(GTEST_ROOT /usr/local)
    find_package(GTest REQUIRED)
endif()


set(EC_extra_libs "")
############################
# for assimp
###########################

#if(WIN32)
#    set(ASSIMP_ROOT_DIR "D:/libraries/assimp/assimp-4.1.0-vs2017x64/inst")

#    set(CMAKE_PREFIX_PATH
#        ${ASSIMP_ROOT_DIR}
#        ${CMAKE_PREFIX_PATH}
#    )

#    set(CMAKE_MODULE_PATH
#        "${ASSIMP_ROOT_DIR}/lib/cmake/assimp-4.1"
#        ${CMAKE_MODULE_PATH})
#endif()
#find_package(assimp REQUIRED)
#message(STATUS "assimp libs : " ${ASSIMP_LIBRARIES})
#message("assimp include dir: " ${ASSIMP_INCLUDE_DIRS})

#include_directories(${ASSIMP_INCLUDE_DIRS})
#link_directories(${ASSIMP_LIBRARY_DIRS})
#list(APPEND EC_extra_libs ${ASSIMP_LIBRARIES} )

####################################
#  find Boost
####################################
# set(Boost_DEBUG ON) # set ON to see debug minfo when finding boost

# !!! very important definition !!!
# that will avoid undefined references in linking
add_definitions(-DBOOST_ALL_DYN_LINK)

set(BOOST_ROOT "C:/local/boost_1_68_0")

set(Boost_USE_DEBUG_LIBS ON)
set(Boost_USE_RELEASE_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.54.0 REQUIRED COMPONENTS filesystem thread log log_setup)
#message(STATUS "found libs : " ${Boost_LIBRARIES})
foreach( var in ${Boost_LIBRARIES})
    message(${var} )
endforeach()
include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})
list(APPEND EC_extra_libs ${Boost_LIBRARIES})


####################################
#  find OpenMP
####################################
#FIND_PACKAGE( OpenMP REQUIRED)
#if(OPENMP_FOUND)
#    message("OPENMP FOUND")
#    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
#    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
#    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
#endif()

####################################
#  find Eigen3
####################################
if(WIN32)
    set(Eigen3_DIR "D:/lib2/Eigen3/include/eigen3")
    message(STATUS  "eigen3 cmake module path : " ${CMAKE_MODULE_PATH})
endif(WIN32)

set(CMAKE_PREFIX_PATH ${Eigen3_DIR} ${CMAKE_PREFIX_PATH})
set(CMAKE_MODULE_PATH "${Eigen3_DIR}/share" ${CMAKE_MODULE_PATH})

find_package (Eigen3 REQUIRED NO_MODULE)
include_directories(
  ${EIGEN3_INCLUDE_DIR}
  ${EIGEN3_LIBRARIES}
)

####################################
#  find Qt5
####################################
cmake_policy(SET CMP0020 NEW)
cmake_policy(SET CMP0071 NEW)

if(WIN32)
    set(CMAKE_PREFIX_PATH
        "D:/Qt/Qt5.10.0/5.12.3/msvc2017_64"
        ${CMAKE_PREFIX_PATH}
        )
# elseif(UNIX)
endif(WIN32)

find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)

####################################
#  find Xerces
####################################
#find_package(XercesC)
#message(STATUS "xerces libs:" ${XercesC_LIBRARIES})
#include_directories(${XercesC_INCLUDE_DIRS})
#list(APPEND EC_extra_libs ${XercesC_LIBRARIES})

###################################
# rapidjson
###################################
if(WIN32)
    set(RapidJson_DIR "D:/lib2/rapidjson/include")
endif(WIN32)
#find_package(RapidJSON)
include_directories(${RapidJson_DIR})


#####################################
# VTK
#####################################
if(WIN32)
    list(APPEND CMAKE_PREFIX_PATH
        D:/libraries/vtk/VTK-8.1.2-build2017x64/inst/lib/cmake/vtk-8.1)

    set(VTK_DIR C:/Program Files/VTK/bin)
endif()
find_package(VTK REQUIRED)
#include(${VTK_USE_FILE})
INCLUDE_DIRECTORIES(${VTK_INCLUDE_DIRS})
link_libraries(${VTK_LIBRARIES})
#list(APPEND EC_extra_libs ${VTK_LIBRARIES})


#####################################
# google test
#####################################
if(WIN32)
    # for windows find_package(GTest REQUIRED) DONOT work
#    set($ENV{GTEST_ROOT} "D:/libraries/gtest/googletest-release-1.8.1-inst")
    set(GTEST_ROOT "C:/Program Files/googletest-distribution")
    set(GTEST_MSVC_SEARCH MT)
else()
    set(GTEST_ROOT /usr/local)
    find_package(GTest REQUIRED)
endif()
include(GoogleTest)
include_directories(${GTEST_ROOT}/include)
link_directories(${GTEST_ROOT}/lib)

message(STATUS "added extra libs:")
foreach( var ${EC_extra_libs})
    message(${var} )
endforeach()

#################################
# find all needed packages
#################################

message(STATUS "added extra libs:")
foreach( var ${EC_extra_libs})
    message(${var} )
endforeach()
#set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/inst")
set(CMAKE_INSTALL_PREFIX "D:/TEST/release/0.1.2")

##################################
# ECComponents
##################################
if(WIN32)
    set(ECComponents_ROOT D:/TEST/release/0.1.4 )
endif()
set(CMAKE_MODULE_PATH "${ECComponents_ROOT}/cmake" ${CMAKE_MODULE_PATH})

message(STATUS "CMAKE_MODULE_PATH" )
foreach(var ${CMAKE_MODULE_PATH})
    message(STATUS ${var})
endforeach()
find_package(ECComponents REQUIRED)


message(STATUS "find libraries ")
foreach(var ${ECComponents_LIBRARIES})
    message(STATUS ${var})
endforeach()

include_directories(${ECComponents_INCLUDE_DIRS})

message(STATUS "find libraries ")
foreach(var ${ECJoint_LIBRARIES})
    message(STATUS ${var})
endforeach()

include_directories(${ECJoint_INCLUDE_DIRS})
