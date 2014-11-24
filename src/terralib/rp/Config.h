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
  \defgroup rp_optical Optical Raster Data Processing.
  \brief Optical Raster Data Processing
  \ingroup rp
*/

/*!
  \defgroup rp_radar Radar Raster Data Processing.
  \brief Radar Raster Data Processing
  \ingroup rp
*/

/*!
  \defgroup rp_class Classification
  \brief Algorithms to detect patterns in raster regions using different methods.
  \ingroup rp_optical
*/

/*!
  \defgroup rp_aux Auxiliary classes
  \brief Raster processing auxiliary classes.
  \ingroup rp_optical
*/

/*!
  \defgroup rp_func Auxiliary functions.
  \brief Raster processing auxiliary functions.
  \ingroup rp_optical
*/

/*!
  \defgroup rp_enh Enhancement
  \brief Raster enhancement algorithms.
  \ingroup rp_optical
*/

/*!
  \defgroup rp_mos Mosaic
  \brief Algorithms to merge two or more rasters into a single one.
  \ingroup rp_optical
*/

/*!
  \defgroup rp_fus Fusion
  \brief Algorithms to combine pixel information from different sensors to obtain better spatial or radiometric resolutions.
  \ingroup rp_optical
*/

/*!
  \defgroup rp_gen General purpose
  \brief General pupose raster processing algorithms.
  \ingroup rp_optical
*/

/*!
  \defgroup rp_seg Segmentation
  \brief Techniques for splitting one rasters into its components as homogeneous regions.
  \ingroup rp_optical
*/

/*!
  \defgroup rp_match Matching
  \brief Techniques to create tie-points between rasters by matching and correlating homologous regions.
  \ingroup rp_optical
*/

/*!
  \defgroup rp_radar_func Auxiliary functions.
  \brief Raster processing auxiliary functions.
  \ingroup rp_radar
*/

/*!
  \defgroup rp_radar_class Classification
  \brief Algorithms to detect patterns in raster regions using different methods.
  \ingroup rp_radar
*/

/*!
  \defgroup rp_radar_gen General purpose
  \brief General pupose raster processing algorithms.
  \ingroup rp_radar
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

