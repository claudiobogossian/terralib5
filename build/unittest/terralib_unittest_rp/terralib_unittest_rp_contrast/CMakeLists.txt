# This script builds the rp unit test 
# The variable ROOT contains the root directory where terralib is installed (ex: terralib5/trunk)

cmake_minimum_required(VERSION 2.8)

# IMPORTANT - Controls for the source and build tree locations
set(ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../../../..")     # the terralib root directory (change it if it is in a different directory)
set(SRCDIR ${ROOT}/unittest/rp)                         # directory where the source files of the rp unit test are located
set(INCLDIR ${SRCDIR})                                  # directory where the header files of the rp unit test are located
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../../Modules")

# Set the project name
set(PROJ_NAME "terralib_unittest_rp_contrast")
project(${PROJ_NAME})           # the project name is terralib_unittest_rp_contrast
set(LIB_NAME ${PROJ_NAME})      # the library name is terralib_unittest_rp_contrast

if(WIN32)
	if(MSVC)
		set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${ROOT}/bin32/msvc2010")
		set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${ROOT}/bin32/msvc2010")
		set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${ROOT}/lib32/msvc2010")
		set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${ROOT}/lib32/msvc2010")
	endif(MSVC)
else(WIN32)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${ROOT}/bin32")
endif(WIN32)

find_package(Boost 1.44 REQUIRED)
if(Boost_FOUND)
  set(DEP_INCLUDES ${DEP_INCLUDES} ${Boost_INCLUDE_DIRS})
endif(Boost_FOUND)

find_package(CppUnit)
if(CPPUNIT_FOUND)
  set(DEP_INCLUDES ${DEP_INCLUDES} ${CPPUNIT_INCLUDE_DIR})

  if(WIN32)
    set(CPPUNIT_LIBRARY
      optimized ${CPPUNIT_LIBRARY_RELEASE}
      debug     ${CPPUNIT_LIBRARY_DEBUG})
  else(WIN32)
    set(CPPUNIT_LIBRARY ${CPPUNIT_LIBRARY})
  endif(WIN32)
  
  set(DEP_LIBS ${DEP_LIBS} ${CPPUNIT_LIBRARY})	
endif(CPPUNIT_FOUND)

#Definitions for windows compiling
if(WIN32)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS -DBOOST_ALL_NO_LIB)
endif(WIN32)

#Set the TerraLib modules to be linked
if(WIN32)
  link_directories(${ROOT}/lib32/msvc2010)
elseif(APPLE)
  link_directories(${ROOT}/lib32/Release ${ROOT}/lib32/Debug)
elseif(UNIX)
  link_directories(${ROOT}/lib32)
endif(WIN32)

set(DEP_LIBS  ${DEP_LIBS}
  optimized terralib_common
  optimized terralib_dataaccess
  optimized terralib_plugin
  optimized terralib_raster
  optimized terralib_rp
  debug     terralib_common_d
  debug     terralib_dataaccess_d
  debug     terralib_plugin_d
  debug     terralib_raster_d
  debug     terralib_rp_d)

# Select the source and header files
set(SRCS ${SRCDIR}/TsContrast.cpp ${SRCDIR}/TsContrastMain.cpp)
set(HDRS ${SRCDIR}/Config.h ${SRCDIR}/LoadModules.h ${SRCDIR}/TsContrast.h)

# Select the include directories
include_directories(${ROOT}/src ${DEP_INCLUDES})

link_libraries(${DEP_LIBS})

add_executable(${PROJ_NAME} ${SRCS} ${HDRS})

set_target_properties(${PROJ_NAME} PROPERTIES DEBUG_POSTFIX _d)

if(WIN32)
  set(terralib-3rdparty-libs $ENV{TERRALIB_DEP_MSVC2010})
  string(REPLACE "\\" "/" terralib-3rdparty-libs ${terralib-3rdparty-libs})
  
  file(COPY
       "${terralib-3rdparty-libs}/cppunit/src/cppunit/x86/ReleaseDll/cppunit_dll.dll"
       "${terralib-3rdparty-libs}/cppunit/src/cppunit/x86/DebugDll/cppunitd_dll.dll"
       "${ROOT}/unittest/report.xsl"
       DESTINATION "${ROOT}/bin32/msvc2010")
else(WIN32)
  file(COPY
       "${ROOT}/unittest/report.xsl"
       DESTINATION "${ROOT}/bin32")      
endif(WIN32)
 