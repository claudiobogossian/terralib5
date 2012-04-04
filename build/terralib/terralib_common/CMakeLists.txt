# This script builds the terralib common module
# The variable ROOT contains the root directory where terralib is installed (ex: terralib5/trunk)

cmake_minimum_required(VERSION 2.8)

# IMPORTANT - Controls for the source and build tree locations
set(ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../../..")    # the terralib root directory (change it if it is in another directory)
set(SRCDIR ${ROOT}/src/terralib/common)             # directory where the source files of the terralib common module are located
set(INCLDIR ${SRCDIR})                              # directory where the header files of the terralib common module are located
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../Modules")

# Set the project name
set(PROJ_NAME "terralib_common")
project(${PROJ_NAME})      # the project name is terralib_common
set(LIB_NAME ${PROJ_NAME}) # the library name is terralib_common

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

find_package(Log4Cxx)
if(Log4cxx_FOUND)
  set(DEP_INCLUDES ${DEP_INCLUDES} ${LOG4CXX_INCLUDE_DIR})
  
  if(WIN32)
    set(LOG4CXX_LIBRARY
        optimized ${LOG4CXX_LIBRARY_RELEASE}
        debug ${LOG4CXX_LIBRARY_DEBUG})
  else(WIN32)
    set(LOG4CXX_LIBRARY ${LOG4CXX_LIBRARY})
  endif(WIN32)
  
  set(DEP_LIBS ${DEP_LIBS} ${LOG4CXX_LIBRARY})	
endif(Log4cxx_FOUND)

find_package(Boost 1.44.0 COMPONENTS filesystem system REQUIRED)
if(Boost_FOUND)
  if(WIN32)
    set(BOOST_LIBRARY
        optimized ${Boost_FILESYSTEM_LIBRARY_RELEASE}
        optimized ${Boost_SYSTEM_LIBRARY_RELEASE}
        debug     ${Boost_FILESYSTEM_LIBRARY_DEBUG}
        debug     ${Boost_SYSTEM_LIBRARY_DEBUG})
    set (DEP_INCLUDES ${DEP_INCLUDES} ${Boost_INCLUDE_DIR})
    set (DEP_LIBS     ${DEP_LIBS} ${BOOST_LIBRARY})
  else(WIN32)
    set(DEP_LIBS     ${DEP_LIBS} ${Boost_LIBRARIES})
    set(DEP_INCLUDES ${DEP_INCLUDES} ${Boost_INCLUDE_DIRS})
  endif(WIN32)
endif(Boost_FOUND)

if(WIN32)
  find_package(GNU)
  if(GNU_FOUND)
	  set(DEP_LIBS     ${DEP_LIBS} ${GNU_LIBRARY})
	  set(DEP_INCLUDES ${DEP_INCLUDES} ${GNU_INCLUDE_DIR})
  endif(GNU_FOUND)
endif(WIN32)

#Add Ws2_32.lib for Windows
if(WIN32)
  set(DEP_LIBS ${DEP_LIBS} Ws2_32.lib)
endif(WIN32)

#Definitions for windows compiling
if(WIN32)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS -DTECOMMONDLL -DBOOST_ALL_NO_LIB)
endif(WIN32)

if(UNIX)
  find_package(DL)
  if(DL_FOUND)
	  set(DEP_LIBS     ${DEP_LIBS} ${DL_LIBRARY})
	  set(DEP_INCLUDES ${DEP_INCLUDES} ${DL_INCLUDE_DIR})
  endif(DL_FOUND)
endif(UNIX)

# Select the source files
file(GLOB SRCS     ${SRCDIR}/*.cpp)
file(GLOB URI_SRCS ${SRCDIR}/URI/*.cpp)

# Select the header files
file(GLOB HDRS     ${SRCDIR}/*.h)
file(GLOB URI_HDRS ${SRCDIR}/URI/*.h)

# Group the source and header files
source_group("Source Files"      FILES ${SRCDIR})
source_group("URI\\Source Files" FILES ${URI_SRCS})
source_group("Header Files"      FILES ${HDRS})
source_group("URI\\Header Files" FILES ${URI_HDRS})

# Select the include directories
include_directories(${ROOT}/src ${INCLDIR}/URI ${DEP_INCLUDES})

link_libraries(${DEP_LIBS})

add_library(${LIB_NAME} SHARED ${SRCS} ${URI_SRCS} ${HDRS} ${URI_HDRS})

set_target_properties(${PROJ_NAME} PROPERTIES DEBUG_POSTFIX _d)

if(WIN32)
  set(CompilerVersion vc100)
  set(terralib-3rdparty-libs $ENV{TERRALIB_DEP_MSVC2010})
  string(REPLACE "\\" "/" terralib-3rdparty-libs ${terralib-3rdparty-libs})
  
  file(MAKE_DIRECTORY "${ROOT}/bin32/msvc2010")
  file(MAKE_DIRECTORY "${ROOT}/lib32/msvc2010")

  file(COPY
       "${terralib-3rdparty-libs}/boost/lib/win32/boost_filesystem-${CompilerVersion}-mt-gd-${Boost_LIB_VERSION}.dll"
       "${terralib-3rdparty-libs}/boost/lib/win32/boost_filesystem-${CompilerVersion}-mt-${Boost_LIB_VERSION}.dll"
       "${terralib-3rdparty-libs}/boost/lib/win32/boost_system-${CompilerVersion}-mt-gd-${Boost_LIB_VERSION}.dll"
       "${terralib-3rdparty-libs}/boost/lib/win32/boost_system-${CompilerVersion}-mt-${Boost_LIB_VERSION}.dll"
       "${terralib-3rdparty-libs}/boost/lib/win32/boost_thread-${CompilerVersion}-mt-gd-${Boost_LIB_VERSION}.dll"
       "${terralib-3rdparty-libs}/boost/lib/win32/boost_thread-${CompilerVersion}-mt-${Boost_LIB_VERSION}.dll"
       "${terralib-3rdparty-libs}/boost/lib/win32/boost_date_time-${CompilerVersion}-mt-gd-${Boost_LIB_VERSION}.dll"
       "${terralib-3rdparty-libs}/boost/lib/win32/boost_date_time-${CompilerVersion}-mt-${Boost_LIB_VERSION}.dll"
       "${terralib-3rdparty-libs}/log4cxx/win32/debug/log4cxx_d.dll"
       "${terralib-3rdparty-libs}/log4cxx/win32/release/log4cxx.dll"
       "${ROOT}/src/terralib/common/URI/protocolPorts.txt"
       DESTINATION "${ROOT}/bin32/msvc2010")
              
  file(COPY
       "${terralib-3rdparty-libs}/gnuWin32/bin/"
       DESTINATION "${ROOT}/bin32/msvc2010"
       FILES_MATCHING PATTERN "*.dll")
 
   file(MAKE_DIRECTORY "${ROOT}/bin32/msvc2010/conf")

   file(COPY "${ROOT}/log/te-log.conf" DESTINATION "${ROOT}/bin32/msvc2010/conf")
   
   file(RENAME "${ROOT}/bin32/msvc2010/protocolPorts.txt" "${ROOT}/bin32/msvc2010/protocol")
   
else(WIN32)
  file(MAKE_DIRECTORY "${ROOT}/lib32")
  file(MAKE_DIRECTORY "${ROOT}/bin32")
  file(MAKE_DIRECTORY "${ROOT}/bin32/conf")
  
  file(COPY "${ROOT}/log/te-log.conf" DESTINATION "${ROOT}/bin32/conf")
  
  file(COPY "${ROOT}/src/terralib/common/URI/protocolPorts.txt" 
       DESTINATION "${ROOT}/bin32")

  file(RENAME "${ROOT}/bin32/protocolPorts.txt" "${ROOT}/bin32/protocol")                
endif(WIN32)
