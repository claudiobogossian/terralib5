#
# Note: This file is based on the original FindGDAL shipped with CMake.
#       We have added support for debug and release libraries for Windows
#       DLLs.
#
# Locate gdal
#
# This module accepts the following environment variables:
#
#    GDAL_DIR or GDAL_ROOT - Specify the location of GDAL
#
# This module defines the following CMake variables:
#
#    GDAL_FOUND - True if libgdal is found
#    GDAL_LIBRARY - A variable pointing to the GDAL library
#    GDAL_INCLUDE_DIR - Where to find the headers
#    GDAL_DATA_DIR - Where to find GDAL data.

#=============================================================================
# Copyright 2007-2009 Kitware, Inc.
#
# Distributed under the OSI-approved BSD License (the "License");
# see accompanying file Copyright.txt for details.
#
# This software is distributed WITHOUT ANY WARRANTY; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the License for more information.
#=============================================================================
# (To distribute this file outside of CMake, substitute the full
#  License text for the above reference.)

#
# $GDALDIR is an environment variable that would
# correspond to the ./configure --prefix=$GDAL_DIR
# used in building gdal.
#
# Created by Eric Wing. I'm not a gdal user, but OpenSceneGraph uses it
# for osgTerrain so I whipped this module together for completeness.
# I actually don't know the conventions or where files are typically
# placed in distros.
# Any real gdal users are encouraged to correct this (but please don't
# break the OS X framework stuff when doing so which is what usually seems
# to happen).

# This makes the presumption that you are include gdal.h like
#
#include "gdal.h"

find_path(GDAL_INCLUDE_DIR gdal.h
  HINTS
    ENV GDAL_DIR
    ENV GDAL_ROOT
  PATH_SUFFIXES
     include/gdal
     include/GDAL
     include
  PATHS
      ~/Library/Frameworks/gdal.framework/Headers
      /Library/Frameworks/gdal.framework/Headers
      /sw # Fink
      /opt/local # DarwinPorts
      /opt/csw # Blastwave
      /opt
)

find_path(GDAL_DATA_DIR
          NAMES gdal_datum.csv
          HINTS ENV GDAL_DIR
                ENV GDAL_ROOT
          PATHS /usr
                /usr/local
                /opt
                /opt/local
          PATH_SUFFIXES share
                        share/gdal
                        data
  
)

if(UNIX)
    # Use gdal-config to obtain the library version (this should hopefully
    # allow us to -lgdal1.x.y where x.y are correct version)
    # For some reason, libgdal development packages do not contain
    # libgdal.so...
    find_program(GDAL_CONFIG gdal-config
        HINTS
          ENV GDAL_DIR
          ENV GDAL_ROOT
        PATH_SUFFIXES bin
        PATHS
            /sw # Fink
            /opt/local # DarwinPorts
            /opt/csw # Blastwave
            /opt
    )

    if(GDAL_CONFIG)
        exec_program(${GDAL_CONFIG} ARGS --libs OUTPUT_VARIABLE GDAL_CONFIG_LIBS)
        if(GDAL_CONFIG_LIBS)
            string(REGEX MATCHALL "-l[^ ]+" _gdal_dashl ${GDAL_CONFIG_LIBS})
            string(REGEX REPLACE "-l" "" _gdal_lib "${_gdal_dashl}")
            string(REGEX MATCHALL "-L[^ ]+" _gdal_dashL ${GDAL_CONFIG_LIBS})
            string(REGEX REPLACE "-L" "" _gdal_libpath "${_gdal_dashL}")
        endif()
    endif()


    find_library(GDAL_LIBRARY
      NAMES ${_gdal_lib} gdal gdal_i gdal1.5.0 gdal1.4.0 gdal1.3.2 GDAL
      HINTS
         ENV GDAL_DIR
         ENV GDAL_ROOT
         ${_gdal_libpath}
      PATH_SUFFIXES lib
      PATHS
        /sw
        /opt/local
        /opt/csw
        /opt
        /usr/freeware
    )
elseif(WIN32)
  find_library(GDAL_LIBRARY_RELEASE 
               NAMES gdal_i gdal
               PATH_SUFFIXES lib)

  find_library(GDAL_LIBRARY_DEBUG
               NAMES gdal_id gdal_i_d gdald gdal_d
               PATH_SUFFIXES lib)

  if(GDAL_LIBRARY_RELEASE AND GDAL_LIBRARY_DEBUG)
    set(GDAL_LIBRARY optimized ${GDAL_LIBRARY_RELEASE} debug ${GDAL_LIBRARY_DEBUG})
  elseif(GDAL_LIBRARY_RELEASE)
    set(GDAL_LIBRARY optimized ${GDAL_LIBRARY_RELEASE} debug ${GDAL_LIBRARY_RELEASE})
  elseif(GDAL_LIBRARY_DEBUG)
    set(GDAL_LIBRARY optimized ${GDAL_LIBRARY_DEBUG} debug ${GDAL_LIBRARY_DEBUG})
  endif()
endif()

include(FindPackageHandleStandardArgs)

if (WIN32)
  FIND_PACKAGE_HANDLE_STANDARD_ARGS(GDAL DEFAULT_MSG GDAL_LIBRARY GDAL_INCLUDE_DIR GDAL_DATA_DIR)
else()
  FIND_PACKAGE_HANDLE_STANDARD_ARGS(GDAL DEFAULT_MSG GDAL_LIBRARY GDAL_INCLUDE_DIR)
endif()

set(GDAL_LIBRARIES ${GDAL_LIBRARY})
set(GDAL_INCLUDE_DIRS ${GDAL_INCLUDE_DIR})
