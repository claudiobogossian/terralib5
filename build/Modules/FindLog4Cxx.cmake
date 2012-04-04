# Strigi
# This notice was added by Olivier Coupelon based on the project information from http://sourceforge.net/projects/strigi
# In case of error please contact me at olivier.coupelon@teardrop.fr
# Copyright (C) 2006 Strigi

# This library is free software; you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as
# published by the Free Software Foundation; either version 2.1 of the
# License, or (at your option) any later version.

# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Lesser General Public License for more details.

# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
# - Try to find the liblog4cxx libraries
# Once done this will define
#
# Log4cxx_FOUND - system has liblog4cxx
# LOG4CXX_INCLUDE_DIR - the log4cxx include directory
# LOG4CXX_LIBRARY -log4cxx library
# LOG4CXX_LIBRARY_DEBUG - log4cxx debug library
# LOG4CXX_LIBRARY_RELEASE - log4cxx release library

find_path(LOG4CXX_INCLUDE_DIR log4cxx/logger.h
          PATHS /include/log4cxx /usr/include /usr/include/log4cxx
      	        /usr/local/include/log4cxx
                "$ENV{TERRALIB_DEP_MSVC2010}/log4cxx/include")
                
if(WIN32)
  find_library(LOG4CXX_LIBRARY_DEBUG
               NAMES log4cxx_d
               PATHS "$ENV{TERRALIB_DEP_MSVC2010}/log4cxx/win32/debug")
               
  find_library(LOG4CXX_LIBRARY_RELEASE
               NAMES log4cxx
               PATHS "$ENV{TERRALIB_DEP_MSVC2010}/log4cxx/win32/release")
               
  if(LOG4CXX_LIBRARY_DEBUG AND LOG4CXX_LIBRARY_RELEASE)
    set(LOG4CXX_LIBRARY TRUE)
  endif(LOG4CXX_LIBRARY_DEBUG AND LOG4CXX_LIBRARY_RELEASE)
else(WIN32)
  find_library(LOG4CXX_LIBRARY
               NAMES log4cxx
               PATHS /usr/local/lib /lib /usr/lib)
endif(WIN32)                   

if(LOG4CXX_INCLUDE_DIR AND LOG4CXX_LIBRARY)
  set(Log4cxx_FOUND TRUE)
  #remove last /log4cxx string
  #STRING(REGEX REPLACE "/log4cxx" "" LOG4CXX_INCLUDE_DIR_SUP_LEVEL ${LOG4CXX_INCLUDE_DIR})
  #SET (LOG4CXX_INCLUDE_DIR ${LOG4CXX_INCLUDE_DIR_SUP_LEVEL} ${LOG4CXX_INCLUDE_DIR} )
  if(NOT Log4cxx_FIND_QUIETLY)
   message(STATUS "Found log4cxx: ${LOG4CXX_LIBRARY_DEBUG},${LOG4CXX_LIBRARY_RELEASE}")
  endif(NOT Log4cxx_FIND_QUIETLY)
else(LOG4CXX_INCLUDE_DIR AND LOG4CXX_LIBRARY)
  set(Log4cxx_FOUND FALSE CACHE BOOL "Not found log4cxx library")
  message(STATUS "NOT Found log4cxx, disabling it")
endif(LOG4CXX_INCLUDE_DIR AND LOG4CXX_LIBRARY)

mark_as_advanced(LOG4CXX_INCLUDE_DIR LOG4CXX_LIBRARY_DEBUG LOG4CXX_LIBRARY_RELEASE LOG4CXX_LIBRARY)
