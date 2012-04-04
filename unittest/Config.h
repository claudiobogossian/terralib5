/*  Copyright (C) 2001-2009 National Institute For Space Research (INPE) - Brazil.

    This file is part of the TerraLib - a Framework for building GIS enabled applications.

    TerraLib is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License,
    or (at your option) any later version.

    TerraLib is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with TerraLib. See COPYING. If not, write to
    TerraLib Team at <terralib-team@terralib.org>.
 */

/*!
  \file Config.h
  \brief Global configuration flags for the TerraLib Unit Tests.
 */

#ifndef __TERRALIB_UNITTEST_INTERNAL_CONFIG_H
#define __TERRALIB_UNITTEST_INTERNAL_CONFIG_H

// TerraLib
#include <terralib/Config.h>

#if TE_PLATFORM == TE_PLATFORMCODE_MSWINDOWS
  #include "windowsunittestplatformdefines.h"
#elif TE_PLATFORM == TE_PLATFORMCODE_APPLE
  #include "appleunittestplatformdefines.h"
#elif TE_PLATFORM == TE_PLATFORMCODE_LINUX
  #include "linuxunittestplatformdefines.h"
  
  #ifdef PACKAGE 
    #undef PACKAGE 
  #endif
  
  #ifdef PACKAGE_BUGREPORT 
    #undef PACKAGE_BUGREPORT 
  #endif
  
  #ifdef PACKAGE_NAME 
    #undef PACKAGE_NAME 
  #endif
  
  #ifdef PACKAGE_STRING 
    #undef PACKAGE_STRING 
  #endif
  
  #ifdef PACKAGE_TARNAME 
    #undef PACKAGE_TARNAME 
  #endif
  
  #ifdef PACKAGE_VERSION 
    #undef PACKAGE_VERSION 
  #endif
  
  #ifdef STDC_HEADERS 
    #undef STDC_HEADERS 
  #endif
  
  #ifdef VERSION 
    #undef VERSION 
  #endif  
#else
  #error "Platform not supported yet"
#endif

#endif  // __TERRALIB_UNITTEST_INTERNAL_CONFIG_H

