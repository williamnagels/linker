project (Linker)
include( CTest )
set(CMAKE_VERBOSE_MAKEFILE ON)
cmake_minimum_required (VERSION 3.9.2)

if (WIN32)
	set (CMAKE_CXX_FLAGS "/std:c++latest /EHsc /D_SCL_SECURE_NO_WARNINGS")
endif()

set(Boost_DEBUG ON)
find_package(Boost 1.65.0 REQUIRED unit_test_framework) 
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${Boost_INCLUDE_DIRS})

add_library(linker_core STATIC
	src/core/addressable.cpp
	src/core/read_write_blob.cpp
	src/core/section/section.cpp
	src/core/section/header_parse_strategy.cpp

	src/core/symtab/symbol.cpp
	src/core/symtab/symbol_table.cpp
	src/core/symtab/symbol_parse_strategy.cpp
)

add_executable(linker src/main.cpp) 
add_executable(linker_unit_tests src/unittest_main.cpp) 
target_link_libraries(linker_unit_tests ${Boost_LIBRARIES})
target_link_libraries(linker linker_core)
add_test( testname linker_unit_tests)