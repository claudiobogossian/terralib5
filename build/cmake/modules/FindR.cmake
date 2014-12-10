#
# Note: This file is based on the original FindLua51 shipped with CMake.
#       We have added support for debug and release libraries for Windows
#       DLLs.
#
# Locate Lua library
# This module defines
#  LUA_FOUND, if false, do not try to link to Lua
#  LUA_LIBRARIES
#  LUA_INCLUDE_DIR, where to find lua.h
#  LUA_VERSION_STRING, the version of Lua found (since CMake 2.8.8)
#
# Note that the expected include convention is
#  #include "lua.h"
# and not
#  #include <lua/lua.h>
# This is because, the lua location is not standardized and may exist
# in locations other than lua/

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

find_path(R_INCLUDE_DIR R.h
  HINTS
    ENV LUA_DIR
  PATH_SUFFIXES include include/r
  PATHS
  ~/Library/Frameworks
  /Library/Frameworks
  /sw # Fink
  /opt/local # DarwinPorts
  /opt/csw # Blastwave
  /opt
)

if(R_INCLUDE_DIR AND EXISTS "${R_INCLUDE_DIR}/R.h")
  file(STRINGS "${R_INCLUDE_DIR}/R.h" r_version_str REGEX "^#define[ \t]+R_RELEASE[ \t]+\"R .+\"")

  string(REGEX REPLACE "^#define[ \t]+R_RELEASE[ \t]+\"R ([^\"]+)\".*" "\\1" R_VERSION_STRING "${r_version_str}")
  unset(r_version_str)
endif()

include(FindPackageHandleStandardArgs)

# handle the QUIETLY and REQUIRED arguments and set LUA_FOUND to TRUE if
# all listed variables are TRUE
FIND_PACKAGE_HANDLE_STANDARD_ARGS(R REQUIRED_VARS R_INCLUDE_DIR VERSION_VAR R_VERSION_STRING)

mark_as_advanced(R_INCLUDE_DIR)
