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
  \file terralib/rp/Config.h

  \brief Configuration flags for the Raster Processing module of TerraLib.
 */

#ifndef __TERRALIB_RP_INTERNAL_CONFIG_H
#define __TERRALIB_RP_INTERNAL_CONFIG_H

/*!
  \defgroup rp_class Classification
  \ingroup rp
*/

/*!
  \defgroup rp_aux Auxiliary classes
  \ingroup rp
*/

/*!
  \defgroup rp_func Auxiliary functions
  \ingroup rp
*/

/*!
  \defgroup rp_enh Enhancement
  \ingroup rp
*/

/*!
  \defgroup rp_mos Mosaic
  \ingroup rp
*/

/*!
  \defgroup rp_fus Fusion
  \ingroup rp
*/

/*!
  \defgroup rp_gen General purpose
  \ingroup rp
*/

/*!
  \defgroup rp_seg Segmentation
  \ingroup rp
*/

/*!
  \defgroup rp_match Matching
  \ingroup rp
*/

// TerraLib
#include "../Config.h"

#define TE_RP_MODULE_NAME "te.rasterprocessing"

/*!
  \def TERPEXPORT

  \brief You can use this macro in order to export/import classes and functions from this module.

  \note If you want to compile TerraLib as DLL in Windows, remember to insert TERPEXPORT into the project's list of defines.

  \note If you want to compile TerraLib as an Static Library under Windows, remember to insert the TERPEXPORT flag into the project list of defines.
 */
#ifdef WIN32

#ifdef _MSC_VER
#pragma warning( disable : 4251 )
#endif

#ifdef TERPSTATIC
#define TERPEXPORT                          // Don't need to export/import... it is a static library
#elif TERPDLL
#define TERPEXPORT  __declspec(dllexport)   // export DLL information
#else
#define TERPEXPORT  __declspec(dllimport)   // import DLL information
#endif
#else
#define TERPEXPORT
#endif

#endif  // __TERRALIB_RP_INTERNAL_CONFIG_H

