/*
  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/core/Config.h

  \brief Configuration flags for TerraLib Core.
*/

#ifndef __TERRALIB_CORE_CONFIG_H__
#define __TERRALIB_CORE_CONFIG_H__

// TerraLib
#include "../Defines.h"

#ifdef WIN32
  #ifdef TECOREDLL
    #define TECOREEXPORT TE_DLL_EXPORT
  #else
    #define TECOREEXPORT TE_DLL_IMPORT
  #endif
#else
  #define TECOREEXPORT
#endif

#endif  // __TERRALIB_CORE_CONFIG_H__

