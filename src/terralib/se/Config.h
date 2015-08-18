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
  \file terralib/se/Config.h

  \brief Configuration flags for the Symbology Encoding support of TerraLib.
*/

#ifndef __TERRALIB_SE_INTERNAL_CONFIG_H
#define __TERRALIB_SE_INTERNAL_CONFIG_H

#define TE_SE_MODULE_NAME "te.symbology_encoding"

/** @name General Defines
 *  General defines for Symbology Encoding
 */
//@{

/*!
  \def TE_SE_DEFAULT_UOM

  \brief It specifies the default unit-of-measurement used by OGC Symbology Encoding.
*/
#define TE_SE_DEFAULT_UOM 3

/*!
  \def TE_SE_DEFAULT_TEXT_COLOR

  \brief It specifies the default color used to show texts.
*/
#define TE_SE_DEFAULT_TEXT_COLOR 0, 0, 0

/*!
  \def TE_SE_DEFAULT_FONT_SIZE

  \brief It specifies the default font size.
*/
#define TE_SE_DEFAULT_FONT_SIZE 10

/*!
  \def TE_SE_DEFAULT_HALO_COLOR

  \brief It specifies the default color used to show text halo.
*/
#define TE_SE_DEFAULT_HALO_COLOR 255, 255, 255

/*!
  \def TE_SE_DEFAULT_HALO_RADIUS

  \brief It specifies the default halo radius (in pixels).
*/
#define TE_SE_DEFAULT_HALO_RADIUS 1

/*!
  \def TE_SE_DEFAULT_FILL_BASIC_COLOR

  \brief It specifies the default color used by basic fill (solid colors).
*/
#define TE_SE_DEFAULT_FILL_BASIC_COLOR 128, 128, 128

/*!
  \def TE_SE_DEFAULT_STROKE_BASIC_COLOR

  \brief It specifies the default color used by stroke basic (solid colors).
*/
#define TE_SE_DEFAULT_STROKE_BASIC_COLOR 0, 0, 0

/*!
  \def TE_SE_DEFAULT_STROKE_BASIC_WIDTH

  \brief It specifies the default width used by stroke basic (solid colors).
*/
#define TE_SE_DEFAULT_STROKE_BASIC_WIDTH 1

/*!
  \def TE_SE_DEFAULT_GRAPHIC_SIZE

  \brief It specifies the default graphic size.
*/
#define TE_SE_DEFAULT_GRAPHIC_SIZE 16

/*!
  \def TE_SE_DEFAULT_GAMMA_VALUE

  \brief It specifies the default gamma value.
*/
#define TE_SE_DEFAULT_GAMMA_VALUE 1.0

/*!
  \def TE_SE_DEFAULT_VERSION

  \brief It specifies the default version used by OGC Symbology Encoding.
*/
#define TE_SE_DEFAULT_VERSION "1.1.0"

/*!
  \def TE_SE_BUTT_CAP

  \brief It specifies the value "butt" for stroke-linecap parameter.
*/
#define TE_SE_BUTT_CAP "butt"

/*!
  \def TE_SE_ROUND_CAP

  \brief It specifies the value "round" for stroke-linecap parameter.
*/
#define TE_SE_ROUND_CAP "round"

/*!
  \def TE_SE_SQUARE_CAP

  \brief It specifies the value "square" for stroke-linecap parameter.
*/
#define TE_SE_SQUARE_CAP "square"

/*!
  \def TE_SE_MITRE_JOIN

  \brief It specifies the value "mitre" for stroke-linejoin parameter.
*/
#define TE_SE_MITRE_JOIN "mitre"

/*!
  \def TE_SE_ROUND_JOIN

  \brief It specifies the value "round" for stroke-linejoin parameter.
*/
#define TE_SE_ROUND_JOIN "round"

/*!
  \def TE_SE_BEVEL_JOIN

  \brief It specifies the value "bevel" for stroke-linejoin parameter.
*/
#define TE_SE_BEVEL_JOIN "bevel"

//@}


/** @name DLL/LIB Module
 *  Flags for building TerraLib as a DLL or as a Static Library
 */
//@{

/*!
  \def TESEEXPORT

  \brief You can use this macro in order to export/import classes and functions from this module.

  \note If you want to compile TerraLib as DLL in Windows, remember to insert TESEDLL into the project's list of defines.

  \note If you want to compile TerraLib as an Static Library under Windows, remember to insert the TESESTATIC flag into the project list of defines.
*/
#ifdef WIN32

  #ifdef _MSC_VER 
    #pragma warning( disable : 4251 )
    #pragma warning( disable : 4275 )
  #endif

  #ifdef TESESTATIC
    #define TESEEXPORT                          // Don't need to export/import... it is a static library
  #elif TESEDLL
    #define TESEEXPORT  __declspec(dllexport)   // export DLL information
  #else
    #define TESEEXPORT  __declspec(dllimport)   // import DLL information
  #endif 
#else
  #define TESEEXPORT
#endif

//@}

#endif  // __TERRALIB_SE_INTERNAL_CONFIG_H
