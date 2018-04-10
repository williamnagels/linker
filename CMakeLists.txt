cmake_minimum_required (VERSION 3.9.1)

project (Linker)
include( CTest )
include(ExternalProject)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_GENERATOR_PLATFORM x86-64)
set(CMAKE_BUILD_TYPE Debug)
set (CMAKE_CXX_STANDARD 17)

if (WIN32)
	set (CMAKE_CXX_FLAGS "/std:c++latest /EHsc")
endif()

set(Boost_DEBUG ON)
set(Boost_USE_STATIC_LIBS ON)
find_package(Boost 1.65.0 REQUIRED unit_test_framework date_time filesystem system) 
find_package(Git REQUIRED) 

ExternalProject_Add(
    mmcow
    GIT_REPOSITORY https://github.com/williamnagels/mmcow.git
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
	CONFIGURE_COMMAND ""
	UPDATE_COMMAND ${GIT_EXECUTABLE} pull
	BUILD_IN_SOURCE ON
	BUILD_ALWAYS ON
)

ExternalProject_Add(
    ranges
    GIT_REPOSITORY https://github.com/ericniebler/range-v3.git
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
	CONFIGURE_COMMAND ""
	UPDATE_COMMAND ${GIT_EXECUTABLE} checkout
	BUILD_IN_SOURCE ON
	BUILD_ALWAYS ON
	GIT_TAG 0.3.5
)

ExternalProject_Get_Property(mmcow source_dir)
set(mmcowIncludeDir ${source_dir}/include)
ExternalProject_Get_Property(ranges source_dir)
set(rangesIncludeDir ${source_dir}/include)

MESSAGE( STATUS "mmcow header files:" ${mmcowIncludeDir} )
MESSAGE( STATUS "ranges header files:" ${rangesIncludeDir} )



add_library(linker_core STATIC
	src/core/addressable.cpp
	src/core/general.cpp
	src/core/elf.cpp


	# symbol table
	src/core/symtab/symbol.cpp
	src/core/symtab/symbol_table.cpp

	#section
	src/core/section/helpers.cpp
)
target_include_directories(linker_core
	PUBLIC
        $<INSTALL_INTERFACE:include>    
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}
		${mmcowIncludeDir}
		${rangesIncludeDir}
)

add_dependencies(linker_core mmcow)
add_dependencies(linker_core ranges)

add_executable(linker src/main.cpp) 

add_executable(linker_unit_tests 
	src/unittest_main.cpp
	src/core/symtab/symbol_table_unittest.cpp
	src/core/reltab/relocation_table_unittest.cpp
	src/core/header/header_unittest.cpp
	src/core/section/section_unittest.cpp
	src/core/linker/link_struct_unittest.cpp
) 
target_include_directories(linker
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}
		${mmcowIncludeDir}
		${rangesIncludeDir}
)

target_include_directories(linker_unit_tests
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}
		${mmcowIncludeDir}
		${rangesIncludeDir}
)

target_link_libraries(linker_unit_tests linker_core ${Boost_LIBRARIES})
target_link_libraries(linker linker_core ${Boost_LIBRARIES})

# creating custom target for the unit test in order for the build to fail but the
# the test executable to not be deleted by make. The deletion is annoying if the
# test binary crashed, making the build fail but no debugger can be attached
# since make deleted the binary
#https://stackoverflow.com/questions/43885692/build-custom-target-by-default-in-cmake
#https://stackoverflow.com/questions/39622247/cmake-add-custom-command-failure-target-gets-deleted
add_custom_target(run_linker_unit_tests ALL
		COMMAND ctest -C $<CONFIGURATION> -VV --output-on-failure 
		DEPENDS linker_unit_tests)

add_test( linker_test linker_unit_tests)


set(directory ${CMAKE_BINARY_DIR}/testfiles)
file(MAKE_DIRECTORY  ${directory})
execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory ${directory})
configure_file(testfiles/wrong_magic_bytes testfiles/ COPYONLY)
configure_file(testfiles/sleep testfiles/ COPYONLY)
configure_file(testfiles/global_and_local_symbol testfiles/ COPYONLY)
configure_file(testfiles/data_empty_bss_global_and_local_symbol testfiles/ COPYONLY)
configure_file(testfiles/simple_main_no_data testfiles/ COPYONLY)
configure_file(testfiles/simple_main_local_symbol testfiles/ COPYONLY)
