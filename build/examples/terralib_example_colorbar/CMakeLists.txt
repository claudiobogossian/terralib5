# This script builds the colorbar example
# The variable ROOT contains the root directory where terralib is installed (ex: terralib5/trunk)

cmake_minimum_required(VERSION 2.8)

# IMPORTANT - Controls for the source and build tree locations
set(ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../../..")  # change this if the terralib root directory is somewhere else
set(SRCDIR ${ROOT}/examples/colorbar)                # directory where the source files of the colorbar example are located
set(INCLDIR ${SRCDIR})                               # directory where the header files of the colorbar example are located

# Set the project name
set(PROJ_NAME "terralib_example_colorbar")
project(${PROJ_NAME})         # the project name is terralib_example_colorbar
set(LIB_NAME ${PROJ_NAME})    # the library name is terralib_example_colorbar

set(DEP_LIBS terralib_color
		terralib_common
    ${Boost_LIBRARIES}
    ${QT_LIBRARIES}
		)
	
# Select the source and header files
file(GLOB SRCS ${SRCDIR}/*.cpp)
file(GLOB HDRS ${SRCDIR}/*.h)

if(WIN32)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS ${QT_DEFINITIONS})
endif(WIN32)
	
# Select the include directories
include_directories(
    ${TE_INCLUDE_DRIS}
    ${DEP_INCLUDES}
    ${TE_INCLUDE_DIRS}
    ${Boost_INCLUDE_DIRS}
)
add_executable (${PROJ_NAME} ${HDRS} ${SRCS})
                    
set_target_properties(${PROJ_NAME} PROPERTIES DEBUG_POSTFIX _d)
 
target_link_libraries(${PROJ_NAME} ${DEP_LIBS})
                 