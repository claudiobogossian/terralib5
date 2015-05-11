/*  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

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

  \brief Configuration flags for TerraLib's Google JavaScript V8 API module.
 */

#ifndef __TERRALIB_BINDING_V8_INTERNAL_CONFIG_H
#define __TERRALIB_BINDING_V8_INTERNAL_CONFIG_H

// TerraLib
#include "../../Config.h"

#define TE_JAVA_COMPILE_XML     1  
#define TE_JAVA_COMPILE_XERCES  1  // XML must be 1 if this is set to 1 
#define TE_JAVA_COMPILE_PLUGIN  1  // XML must be 1 if this is set to 1 
#define TE_JAVA_COMPILE_DT      1
#define TE_JAVA_COMPILE_GM      1  // DT must be 1 if this is set to 1
#define TE_JAVA_COMPILE_DA      1  // DT and GM must be set to 1 if this is set to 1
#define TE_JAVA_COMPILE_QT      1  // DT, GM and DA must be set to 1 if this is set to 1

#endif  // __TERRALIB_BINDING_V8_INTERNAL_CONFIG_H

