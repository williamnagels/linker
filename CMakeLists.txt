project (Linker)
set(CMAKE_VERBOSE_MAKEFILE ON)
cmake_minimum_required (VERSION 2.6)


if (WIN32)
set(BOOST_ROOT "C:/program files/boost_1_64_0")
set(BOOST_LIBRARYDIR ${BOOST_ROOT}/stage/lib/)
set (CMAKE_CXX_FLAGS "/std:c++latest /EHsc /D_SCL_SECURE_NO_WARNINGS")
set(Boost_DEBUG ON)
set(Boost_USE_STATIC_LIBS OFF)
add_definitions( -DBOOST_ALL_NO_LIB ) 
endif()


find_package(Boost 1.64.0 ) #REQUIRED COMPONENTS serialization

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
  add_definitions("-DUSE_BOOST_KARMA")
endif() 

add_library(linker_core STATIC
	src/core/component.cpp
)

add_executable(linker src/main.cpp) 

target_link_libraries(linker linker_core)