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

#ifndef __TERRALIB_CORE_LIB_CONFIG_H__
#define __TERRALIB_CORE_LIB_CONFIG_H__

/*!
\def FUNCTIONSEXPORT
*/
#ifdef WIN32

#ifdef _MSC_VER 
#pragma warning( disable : 4251 )
#pragma warning( disable : 4275 )
#pragma warning( disable : 4290 )
#endif

#ifdef FUNCTIONSSTATIC
#define FUNCTIONSEXPORT                          // Don't need to export/import... it is a static library
#elif FUNCTIONSDLL
#define FUNCTIONSEXPORT  __declspec(dllexport)   // export DLL information
#else
#define FUNCTIONSEXPORT  __declspec(dllimport)   // import DLL information
#endif 
#else
#define FUNCTIONSEXPORT
#endif


#endif  // __TERRALIB_CORE_LIB_CONFIG_H__
