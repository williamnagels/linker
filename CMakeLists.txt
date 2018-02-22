project (Linker)
include( CTest )
include(ExternalProject)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_GENERATOR_PLATFORM x86-64)
cmake_minimum_required (VERSION 3.9.2)


if (WIN32)
	set (CMAKE_CXX_FLAGS "/std:c++latest /EHsc")
endif()

set(Boost_DEBUG ON)
set(Boost_USE_STATIC_LIBS ON)
find_package(Boost 1.65.0 REQUIRED unit_test_framework date_time filesystem) 
find_package(Git REQUIRED) 

set(EXTERNAL_INSTALL_LOCATION ${CMAKE_BINARY_DIR}/external)

ExternalProject_Add(
    mmcow
    GIT_REPOSITORY https://github.com/williamnagels/mmcow.git
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
	CONFIGURE_COMMAND ""
	UPDATE_COMMAND ${GIT_EXECUTABLE} pull
	BUILD_IN_SOURCE ON
	BUILD_ALWAYS OFF
    )


	
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${Boost_INCLUDE_DIRS})

ExternalProject_Get_Property(mmcow source_dir)
set(mmcowIncludeDir ${source_dir}/include)


MESSAGE( STATUS "mmcow header files:" ${mmcowIncludeDir} )
include_directories(${mmcowIncludeDir})
add_library(linker_core STATIC
	src/core/addressable.cpp
	src/core/general.cpp
	src/core/elf.cpp


	# symbol table
	src/core/symtab/symbol.cpp
	src/core/symtab/symbol_table.cpp

	#section
	src/core/section/helpers.cpp

	src/core/linker/link_struct.cpp
)

add_executable(linker src/main.cpp) 

add_executable(linker_unit_tests 
	src/unittest_main.cpp
	src/core/symtab/symbol_table_unittest.cpp
	src/core/reltab/relocation_table_unittest.cpp
	src/core/header/header_unittest.cpp
	src/core/section/section_unittest.cpp

) 
add_dependencies(linker_unit_tests mmcow)
add_dependencies(linker mmcow)
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
configure_file(testfiles/global_and_local_symbol testfiles/ COPYONLY)
configure_file(testfiles/data_empty_bss_global_and_local_symbol testfiles/ COPYONLY)
