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
  \file terralib/module/Config.h

  \brief TerraLib Module Manager configuration flags.
*/

#ifndef __TERRALIB_MODULE_CONFIG_H__
#define __TERRALIB_MODULE_CONFIG_H__

#if defined(WIN32) && defined(terralib_mod_manager_EXPORTS)
    #define TEMODULEEXPORT  __declspec(dllexport)
#elif defined(WIN32)
    #define TEMODULEEXPORT  __declspec(dllimport)
#else
  #define TEMODULEEXPORT
#endif

#endif  // __TERRALIB_MODULE_CONFIG_H__

