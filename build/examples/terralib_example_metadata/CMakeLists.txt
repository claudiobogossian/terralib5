# This script builds the metadata example
# The variable ROOT contains the root directory where terralib is installed (ex: terralib5/trunk)

cmake_minimum_required(VERSION 2.8)

# IMPORTANT - Controls for the source and build tree locations
set(ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../../..")  # change this if the terralib root directory is somewhere else
set(SRCDIR ${ROOT}/examples/metadata)                # directory where the source files of the metadata example are located
set(INCLDIR ${SRCDIR})                               # directory where the header files of the metadata example are located

# Set the project name
set(PROJ_NAME "terralib_example_metadata")
project(${PROJ_NAME})         # the project name is terralib_example_metadata
set(LIB_NAME ${PROJ_NAME})    # the library name is terralib_example_metadata

find_package(Boost ${_Boost_VERSION} COMPONENTS system REQUIRED)

if(WIN32)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS -DBOOST_ALL_NO_LIB)
endif()

	
set(DEP_LIBS 
    terralib_common 
    terralib_dataaccess
    terralib_metadata
    terralib_plugin
    terralib_serialization
    terralib_xml
	${Boost_LIBRARIES}
)
	
# Select the source and header files
file(GLOB SRCS ${SRCDIR}/*.cpp)
file(GLOB HDRS ${SRCDIR}/*.h)

# Select the include directories
include_directories(
    ${TE_INCLUDE_DIRS}
    ${DEP_INCLUDES}
    ${Boost_INCLUDE_DIRS}
)

add_executable (${PROJ_NAME} ${HDRS} ${SRCS})
                    
set_target_properties(${PROJ_NAME} PROPERTIES DEBUG_POSTFIX _d)
 
target_link_libraries(${PROJ_NAME} ${DEP_LIBS})

copyLogFiles(${CMAKE_CURRENT_BINARY_DIR})
                 