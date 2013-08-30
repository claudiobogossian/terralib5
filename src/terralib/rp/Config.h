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
 \defgroup RPModule Raster processing module.
 */

/*!
 \defgroup RPAlgorithms Raster processing algorithms.
 \ingroup RPModule
 */

/*!
 \defgroup SegmentationAlgorithms Raster segmentation algorithms.
 \ingroup RPAlgorithms
 */

/*!
 \defgroup MatchingAlgorithms Raster matching algorithms.
 \ingroup RPAlgorithms
 */

/*!
 \defgroup MosaicAlgorithms Raster mosaic algorithms.
 \ingroup RPAlgorithms
 */

/*!
 \defgroup FusionAlgorithms Raster fusion algorithms.
 \ingroup RPAlgorithms
 */

/*!
  \file terralib/rp/Config.h

  \brief Configuration flags for the Raster Processing module of TerraLib.
 */

#ifndef __TERRALIB_RP_INTERNAL_CONFIG_H
#define __TERRALIB_RP_INTERNAL_CONFIG_H

// TerraLib
#include "../Config.h"

#define TE_RP_MODULE_NAME "te.rasterprocessing"

/** @name Internationalization Defines
 *  Flags for TerraLib code internationalization.
 */
//@{

/*!
  \def TE_RP_TEXT_DOMAIN

  \brief It contains the name of the text domain used in the translation of messages in TerraLib Raster Processing module.
 */
#define TE_RP_TEXT_DOMAIN "terasterprocessing"

/*!
  \def TE_RP_TEXT_DOMAIN_DIR

  \brief It contains the translation catalog directory.
 */
#define TE_RP_TEXT_DOMAIN_DIR "locale"

/*!
  \def TR_RP

  \brief It marks a string in order to get translated. This is a special mark used in the Raster Processing module of TerraLib.
 */
#define TR_RP(message) TR(message, TE_RP_TEXT_DOMAIN)

//@}

/** @name DLL/LIB Module
 *  Flags for building TerraLib as a DLL or as a Static Library
 */
//@{

/*!
  \def TERPEXPORT

  \brief You can use this macro in order to export/import classes and functions from this module.

  \note If you want to compile TerraLib as DLL in Windows, remember to insert TERPEXPORT into the project's list of defines.

  \note If you want to compile TerraLib as an Static Library under Windows, remember to insert the TERPEXPORT flag into the project list of defines.
 */
#if TE_PLATFORM == TE_PLATFORMCODE_MSWINDOWS

  #if TE_COMPILER == TE_COMPILERCODE_VCPLUSPLUS
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

//@}        

#endif  // __TERRALIB_RP_INTERNAL_CONFIG_H

