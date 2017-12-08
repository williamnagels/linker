project (Linker)
include( CTest )
set(CMAKE_VERBOSE_MAKEFILE ON)
cmake_minimum_required (VERSION 3.9.2)

if (WIN32)
	set (CMAKE_CXX_FLAGS "/std:c++latest /EHsc")

endif()

set(Boost_DEBUG ON)
set(Boost_USE_STATIC_LIBS ON)
find_package(Boost 1.65.0 REQUIRED unit_test_framework date_time) 


include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${Boost_INCLUDE_DIRS})

add_library(linker_core STATIC
	src/core/addressable.cpp
	src/core/read_write_blob.cpp
	src/core/general.cpp
	src/core/elf.cpp
	src/core/section/section.cpp

	# symbol table
	src/core/symtab/symbol.cpp
	src/core/symtab/symbol_table.cpp
	src/core/symtab/symbol_parse_strategy.cpp

	# header
	src/core/header/header.cpp
)

add_executable(linker src/main.cpp) 

add_executable(linker_unit_tests 
	src/unittest_main.cpp
	src/core/symtab/symbol_table_unittest.cpp	
	src/core/header/header_unittest.cpp
	src/core/section/section_unittest.cpp
) 

add_custom_command(TARGET linker_unit_tests
                   POST_BUILD
                   COMMAND ctest -C $<CONFIGURATION> -VV --output-on-failure)

target_link_libraries(linker_unit_tests linker_core ${Boost_LIBRARIES})
target_link_libraries(linker linker_core ${Boost_LIBRARIES})
add_test( linker linker_unit_tests)

set(directory ${CMAKE_BINARY_DIR}/testfiles)
file(MAKE_DIRECTORY  ${directory})
execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory ${directory})
configure_file(testfiles/wrong_magic_bytes testfiles/ COPYONLY)
configure_file(testfiles/sleep testfiles/ COPYONLY)