/*  Copyright (C) 2008-2014 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/classification/Config.h

  \brief Configuration flags for the Terrralib Classification module.
 */

#ifndef __TERRALIB_CLASSIFICATION_INTERNAL_CONFIG_H
#define __TERRALIB_CLASSIFICATION_INTERNAL_CONFIG_H

// TerraLib
#include "../common/Translator.h"
#include "../Config.h"

#define TE_CL_MODULE_NAME "te.classification"

/** @name ROI Defines
 *  Flags for ROI export and import funcions
 */
//@{

/*!
  \def TE_CL_ROI_GEOM_ID_NAME

  \brief This mark defines the geom id attribute name
 */
#define TE_CL_ROI_GEOM_ID_NAME "geom_id"

/*!
  \def TE_CL_ROI_LABEL_NAME

  \brief This mark defines the label attribute name
 */
#define TE_CL_ROI_LABEL_NAME "label"

/*!
  \def TE_CL_ROI_COLOR_NAME

  \brief This mark defines the color attribute name
 */
#define TE_CL_ROI_COLOR_NAME "color"

/*!
  \def TE_CL_ROI_GEOM_NAME

  \brief This mark defines the geom attribute name
 */
#define TE_CL_ROI_GEOM_NAME "geom"

/*!
  \def TE_CL_ROI_PK_NAME

  \brief This mark defines the primary key attribute name
 */
#define TE_CL_ROI_PK_NAME "pk_roi"

//@}

/** @name DLL/LIB Module
 *  Flags for building TerraLib as a DLL or as a Static Library
 */
//@{

/*!
  \def TECLEXPORT

  \brief You can use this macro in order to export/import classes and functions from this module.

  \note If you want to compile TerraLib as DLL in Windows, remember to insert TECLDLL into the project's list of defines.

  \note If you want to compile TerraLib as an Static Library under Windows, remember to insert the TECLSTATIC flag into the project list of defines.
 */
#ifdef WIN32

  #ifdef _MSC_VER
    #pragma warning( disable : 4251 )
    //#pragma warning( disable : 4275 )
  #endif

  #ifdef TECLSTATIC
    #define TECLEXPORT                           // Don't need to export/import... it is a static library
  #elif TECLDLL
    #define TECLEXPORT  __declspec(dllexport)    // export DLL information
  #else
    #define TECLEXPORT  __declspec(dllimport)    // import DLL information
  #endif 
#else
  #define TECLEXPORT
#endif

//@}

#endif  // __TERRALIB_CLASSIFICATION_INTERNAL_CONFIG_H

