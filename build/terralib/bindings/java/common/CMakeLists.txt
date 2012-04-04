# This script builds the terralib common java binding
# The variable ROOT contains the root directory where terralib is installed (ex: terralib5/trunk)

cmake_minimum_required(VERSION 2.8)

# IMPORTANT - Controls for the source and build tree locations
set(ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../../../../..")  # the terralib root directory (change it if it is in another directory)
set(SRCDIR ${ROOT}/src/terralib/binding/java/common)       # directory where the source files of the terralib common java binding are located
set(INCLDIR ${SRCDIR})                                     # directory where the header files of the terralib common java binding are located
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../../../Modules")

# Set the project name
set(PROJ_NAME "terralib_binding_java_common")
project(${PROJ_NAME})           # the project name is terralib_binding_java_common
set(LIB_NAME ${PROJ_NAME})      # the library name is terralib_binding_java_common

if(WIN32)
	if(MSVC)
		set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${ROOT}/bin32/msvc2010")
		set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${ROOT}/bin32/msvc2010")
		set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${ROOT}/lib32/msvc2010")
		set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${ROOT}/lib32/msvc2010")
	endif(MSVC)
else(WIN32)
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${ROOT}/lib32")
endif(WIN32)

find_package(Boost 1.44 REQUIRED)
if(Boost_FOUND)
  set(DEP_INCLUDES ${DEP_INCLUDES} ${Boost_INCLUDE_DIRS})
endif(Boost_FOUND)

find_package(JNI)
if(JNI_FOUND)
  set(DEP_INCLUDES ${DEP_INCLUDES} ${JAVA_INCLUDE_PATH})
  set(DEP_INCLUDES ${DEP_INCLUDES} ${JAVA_INCLUDE_PATH2})
endif(JNI_FOUND)

#Definitions for windows compiling
if(WIN32)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS -DTEJAVACOMMONDLL -DBOOST_ALL_DYN_LINK -DBOOST_ALL_NO_LIB)
endif(WIN32)

if(WIN32)
	set(DEP_LIBS ${DEP_LIBS}
		optimized "${ROOT}/lib32/msvc2010/terralib_common.lib"
		debug     "${ROOT}/lib32/msvc2010/terralib_common_d.lib")
endif(WIN32)  

# Select the source and header files
file(GLOB SRCS ${SRCDIR}/*.cpp)
file(GLOB HDRS ${SRCDIR}/*.h)

# Select the include directories
include_directories(${ROOT}/src ${DEP_INCLUDES})

link_libraries(${DEP_LIBS})
		
add_library(${LIB_NAME} SHARED ${SRCS} ${HDRS})

set_target_properties(${PROJ_NAME} PROPERTIES DEBUG_POSTFIX _d)

add_dependencies(${PROJ_NAME} terralib_common)