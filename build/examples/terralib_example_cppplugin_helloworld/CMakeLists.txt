# This script builds the cppplugin_helloworld example
# The variable ROOT contains the root directory where terralib is installed (ex: terralib5/trunk)

cmake_minimum_required(VERSION 2.8)

# IMPORTANT - Controls for the source and build tree locations
set(ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../../..")      # the terralib root directory (change it if it is located in another place)
set(SRCDIR ${ROOT}/examples/cppplugin/helloworld)     # directory where the source files of the cppplugin_hellowworld example are located
set(INCLDIR ${SRCDIR})                                # directory where the header files of the cppplugin_hellowworld example are located

# Set the project name
set(PROJ_NAME "terralib_example_cppplugin_helloworld")
project(${PROJ_NAME})           # the project name is terralib_example_cppplugin_helloworld
set(LIB_NAME ${PROJ_NAME})      # the library name is terralib_example_cppplugin_helloworld

#Definitions for windows compiling
if(WIN32)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS -DTESRSDLL -DBOOST_ALL_NO_LIB)
endif(WIN32)

set(DEP_LIBS 
    terralib_common
		terralib_plugin
    )

# Select the source and header files
file(GLOB SRCS ${SRCDIR}/*.cpp)
file(GLOB HDRS ${SRCDIR}/*.h)

# Select the include directories
include_directories(
    ${TE_INCLUDE_DIRS}
    ${DEP_INCLUDES}
)

link_libraries(${DEP_LIBS})

set(CMAKE_DEBUG_POSTFIX _d)

add_library(${LIB_NAME} SHARED ${SRCS} ${HDRS})

set_target_properties(${PROJ_NAME} PROPERTIES DEBUG_POSTFIX _d)

copyLogFiles(${CMAKE_CURRENT_BINARY_DIR})
