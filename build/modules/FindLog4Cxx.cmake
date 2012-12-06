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
# LOG4CXX_FOUND - system has liblog4cxx
# LOG4CXX_INCLUDE_DIR - the log4cxx include directory
# LOG4CXX_LIBRARY -log4cxx library
# LOG4CXX_LIBRARY_DEBUG - log4cxx debug library
# LOG4CXX_LIBRARY_RELEASE - log4cxx release library

find_path(
  LOG4CXX_INCLUDE_DIR NAMES log4cxx.h 
  PATHS /include/log4cxx /usr/include /usr/include/log4cxx /usr/local/include/log4cxx
  PATH_SUFFIXES log4cxx log4cxx/include/log4cxx log4cxx/include
)
                
find_library(
  LOG4CXX_LIBRARY_DEBUG
  NAMES log4cxx_d
  PATHS /usr/local/lib /lib /usr/lib
  PATH_SUFFIXES log4cxx/lib
)
             
find_library(
  LOG4CXX_LIBRARY_RELEASE
  NAMES log4cxx
  PATHS /usr/local/lib /lib /usr/lib
  PATH_SUFFIXES log4cxx/lib
)
               
checkLibNames("LOG4CXX")

set(Log4Cxx_FOUND FALSE)

if(LOG4CXX_INCLUDE_DIR AND LOG4CXX_LIBRARY)
  set(Log4Cxx_FOUND TRUE)
  message(STATUS "-- Found log4cxx library")
else()
  if(Log4Cxx_FIND_REQUIRED)
    message(FATAL_ERROR "-- Error: log4cxx required but NOT found.")
  else()
    message(STATUS "-- NOT Found log4cxx, disabling it")
  endif()
endif()

mark_as_advanced(LOG4CXX_INCLUDE_DIR LOG4CXX_LIBRARY)
