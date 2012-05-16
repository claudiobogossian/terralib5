# This script builds the terralib sam module
# The variable ROOT contains the root directory where terralib is installed (ex: terralib5/trunk)

cmake_minimum_required(VERSION 2.8)

# IMPORTANT - Controls for the source and build tree locations

set(ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../../..")     # the terralib root directory (change it if it is in another directory)
set(SRCDIR ${ROOT}/src/terralib/sam)                 # directory where the source files of the terralib sam module are located
set(INCLDIR ${SRCDIR})                               # directory where the header files of the terralib sam module are located
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../Modules")

if(MSVC)
	SET(MSVC_INCREMENTAL_DEFAULT ON)
endif(MSVC)

# Set the project name
set(PROJ_NAME "terralib_sam")
project(${PROJ_NAME})       # the project name is terralib_sam
set(LIB_NAME ${PROJ_NAME})  # the library name is terralib_sam

if(WIN32)
	if(MSVC)
		set (CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${ROOT}/bin32/msvc2010")
		set (CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${ROOT}/bin32/msvc2010")
		set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${ROOT}/lib32/msvc2010")
		set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${ROOT}/lib32/msvc2010")
	endif(MSVC)
else(WIN32)
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${ROOT}/lib32")
endif(WIN32)

# This part disables the incremental linker
if(MSVC)
	string(REPLACE "INCREMENTAL:YES" "INCREMENTAL:NO" replacementFlags
	${CMAKE_EXE_LINKER_FLAGS_DEBUG})
	set(CMAKE_SHARED_LINKER_FLAGS_DEBUG "/INCREMENTAL:NO ${replacementFlags}")
endif(MSVC)

find_package(Boost 1.44 REQUIRED)
if(Boost_FOUND)
  set(DEP_INCLUDES ${DEP_INCLUDES} ${Boost_INCLUDE_DIRS})
endif(Boost_FOUND)

#Set the TerraLib modules to be linked
if(WIN32)
  link_directories(${ROOT}/lib32/msvc2010)
else(WIN32)
  link_directories(${ROOT}/lib32)
endif(WIN32)

set(DEP_LIBS  ${DEP_LIBS}
		optimized terralib_common
		debug     terralib_common_d)

link_libraries(${DEP_LIBS})

# Select the header files
file(GLOB HDRS ${SRCDIR}/*.h)
file(GLOB RTREE_HDRS ${SRCDIR}/rtree/*.h)

# Group the header files
source_group("Header Files" FILES ${HDRS})
source_group("Header Files\\rtree" FILES ${RTREE_HDRS})

# Select the include directories
include_directories(${DEP_INCLUDES})
		
add_library(${LIB_NAME} MODULE ${HDRS} ${RTREE_HDRS})

set_target_properties(${LIB_NAME} PROPERTIES LINKER_LANGUAGE C)
set_target_properties(${PROJ_NAME} PROPERTIES DEBUG_POSTFIX _d)

add_dependencies(${PROJ_NAME} terralib_common)
