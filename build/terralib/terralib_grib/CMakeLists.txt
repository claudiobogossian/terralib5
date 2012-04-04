# This script builds the terralib grib module
# The variable ROOT contains the root directory where terralib is installed (ex: terralib5/trunk)

cmake_minimum_required(VERSION 2.8)

# IMPORTANT - Controls for the source and build tree locations
set(ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../../..")    # the terralib root directory (change it if it is in another directory)
set(SRCDIR ${ROOT}/src/terralib/grib)               # directory where the source files of the terralib grib module are located
set(INCLDIR ${SRCDIR})                              # directory where the header files of the terralib grib module are located
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../Modules")

# Set the project name
set(PROJ_NAME "terralib_grib")
project(${PROJ_NAME})      # the project name is terralib_grib
set(LIB_NAME ${PROJ_NAME}) # the library name is terralib_grib

find_package(Boost 1.44.0 COMPONENTS filesystem system REQUIRED)
if(Boost_FOUND AND !WIN32 AND !APPLE)
  set (DEP_LIBS ${DEP_LIBS} ${Boost_LIBRARIES})
  set (DEP_INCLUDES ${DEP_INCLUDES} ${Boost_INCLUDE_DIRS})
endif(Boost_FOUND AND !WIN32 AND !APPLE)

find_package(Grib)
if(GRIB_FOUND)
  set(DEP_LIBS ${DEP_LIBS} ${GRIB_LIBRARY})
  set(DEP_INCLUDES ${DEP_INCLUDES} ${GRIB_INCLUDE_DIR})
endif(GRIB_FOUND)

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
    optimized terralib_datatype
    optimized terralib_geometry
    optimized terralib_plugin
    optimized terralib_raster
    optimized terralib_srs
    debug     terralib_common_d
    debug     terralib_dataaccess_d
    debug     terralib_datatype_d
    debug     terralib_geometry_d
    debug     terralib_plugin_d
    debug     terralib_raster_d
    debug     terralib_srs_d)

link_libraries(${DEP_LIBS})

# Select the source files
file(GLOB SRCS ${SRCDIR}/*.cpp)
file(GLOB HDRS ${SRCDIR}/*.h)

# Select the include directories
include_directories(${DEP_INCLUDES})

add_library(${LIB_NAME} SHARED ${SRCS} ${HDRS})

set_target_properties(${PROJ_NAME} PROPERTIES DEBUG_POSTFIX _d)

add_dependencies(${PROJ_NAME}
                 terralib_common 
                 terralib_dataaccess
                 terralib_datatype
                 terralib_geometry
                 terralib_plugin
                 terralib_raster
                 terralib_srs)

