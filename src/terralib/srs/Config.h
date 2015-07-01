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
 
 \brief Configuration flags for the TerraLib SRS module.
 */

#ifndef __TERRALIB_SRS_INTERNAL_CONFIG_H
#define __TERRALIB_SRS_INTERNAL_CONFIG_H

// TerraLib
#include "../Config.h"

/*!
 \def TE_SRS_MODULE_NAME
 
 \brief It defines the module name.
 */
#define TE_SRS_MODULE_NAME "te.srs"

/*!
 \def TE_UNKNOWN_SRS
 
 \brief A numeric value to represent a unknown SRS identification in TerraLib.
 */
#define TE_UNKNOWN_SRS 0

/** @name EPSG codes
 *  Some commonly used numeric codes for Spatial Reference Systems as defined in the EPSG database.
 *
 */
//@{

/*!\def TE_SRS_SAD69
 Geographic coordinate reference system based on SAD69 Datum.
 */
#define TE_SRS_SAD69 4618

/*!\def TE_SRS_CORREGO_ALEGRE
 Geographic coordinate reference system based on Brazilian Corrego Alegre Datum.
 */
#define TE_SRS_CORREGO_ALEGRE 4225

/*!
 \def TE_SRS_WGS84
 Geographic coordinate reference system based on WGS84 Datum.
 */
#define TE_SRS_WGS84 4326

/*!\def TE_SRS_SIRGAS2000
 Geographic coordinate reference system based on SIRGAS 2000 Datum.
 */
#define TE_SRS_SIRGAS2000 4674

/*!\def TE_SRS_CORREGO_ALEGRE_UTM_ZONE_21S
 Projected coordinate reference system: UTM projection, Datum Corrego Alegre Datum, Zone 21 South.
 */
#define TE_SRS_CORREGO_ALEGRE_UTM_ZONE_21S 22521

/*!\def TE_SRS_CORREGO_ALEGRE_UTM_ZONE_22S
 Projected coordinate reference system: UTM projection, Datum Corrego Alegre Datum, Zone 22 South.
 */
#define TE_SRS_CORREGO_ALEGRE_UTM_ZONE_22S 22522

/*!\def TE_SRS_CORREGO_ALEGRE_UTM_ZONE_23S
 Projected coordinate reference system: UTM projection, Datum Corrego Alegre Datum, Zone 23 South.
 */
#define TE_SRS_CORREGO_ALEGRE_UTM_ZONE_23S 22523

/*!\def TE_SRS_CORREGO_ALEGRE_UTM_ZONE_24S
 Projected coordinate reference system: UTM projection, Datum Corrego Alegre Datum, Zone 24 South.
 */
#define TE_SRS_CORREGO_ALEGRE_UTM_ZONE_24S 22524

/*!\def TE_SRS_CORREGO_ALEGRE_UTM_ZONE_25S
 Projected coordinate reference system: UTM projection, Datum Corrego Alegre Datum, Zone 25 South.
 */
#define TE_SRS_CORREGO_ALEGRE_UTM_ZONE_25S 22525

/*!\def TE_SRS_SAD69_POLYCONIC
 Projected coordinate reference system: Brazil Polyconic, Datum SAD69.
 */
#define TE_SRS_SAD69_POLYCONIC 29101

/*!\def TE_SRS_SAD69_UTM_ZONE_18N
 Projected coordinate reference system: UTM projection, Datum SAD69 Datum, Zone 18 North.
 */
#define TE_SRS_SAD69_UTM_ZONE_18N 29168

/*!\def TE_SRS_SAD69_UTM_ZONE_19N
 Projected coordinate reference system: UTM projection, Datum SAD69 Datum, Zone 19 North.
 */
#define TE_SRS_SAD69_UTM_ZONE_19N 29169

/*!\def TE_SRS_SAD69_UTM_ZONE_20N
 Projected coordinate reference system: UTM projection, Datum SAD69 Datum, Zone 20 North.
 */
#define TE_SRS_SAD69_UTM_ZONE_20N 29170

/*!\def TE_SRS_SAD69_UTM_ZONE_21N
 Projected coordinate reference system: UTM projection, Datum SAD69 Datum, Zone 21 North.
 */
#define TE_SRS_SAD69_UTM_ZONE_21N 29171

/*!\def TE_SRS_SAD69_UTM_ZONE_22N
 Projected coordinate reference system: UTM projection, Datum SAD69 Datum, Zone 22 North.
 */
#define TE_SRS_SAD69_UTM_ZONE_22N 29172


/*!\def TE_SRS_SAD69_UTM_ZONE_17S
 Projected coordinate reference system: UTM projection, Datum SAD69 Datum, Zone 17 South.
 */
#define TE_SRS_SAD69_UTM_ZONE_17S 29187

/*!\def TE_SRS_SAD69_UTM_ZONE_18S
 Projected coordinate reference system: UTM projection, Datum SAD69 Datum, Zone 18 South.
 */
#define TE_SRS_SAD69_UTM_ZONE_18S 29188

/*!\def TE_SRS_SAD69_UTM_ZONE_19S
 Projected coordinate reference system: UTM projection, Datum SAD69 Datum, Zone 19 South.
 */
#define TE_SRS_SAD69_UTM_ZONE_19S 29189

/*!\def TE_SRS_SAD69_UTM_ZONE_20S
 Projected coordinate reference system: UTM projection, Datum SAD69 Datum, Zone 20 South.
 */
#define TE_SRS_SAD69_UTM_ZONE_20S 29190

/*!\def TE_SRS_SAD69_UTM_ZONE_21S
 Projected coordinate reference system: UTM projection, Datum SAD69 Datum, Zone 21 South.
 */
#define TE_SRS_SAD69_UTM_ZONE_21S 29191

/*!\def TE_SRS_SAD69_UTM_ZONE_22S
 Projected coordinate reference system: UTM projection, Datum SAD69 Datum, Zone 22 South.
 */
#define TE_SRS_SAD69_UTM_ZONE_22S 29192

/*!\def TE_SRS_SAD69_UTM_ZONE_23S
 Projected coordinate reference system: UTM projection, Datum SAD69 Datum, Zone 23 South.
 */
#define TE_SRS_SAD69_UTM_ZONE_23S 29193

/*!\def TE_SRS_SAD69_UTM_ZONE_24S
 Projected coordinate reference system: UTM projection, Datum SAD69 Datum, Zone 24 South.
 */
#define TE_SRS_SAD69_UTM_ZONE_24S 29194

/*!\def TE_SRS_SAD69_UTM_ZONE_25S
 Projected coordinate reference system: UTM projection, Datum SAD69 Datum, Zone 25 South.
 */
#define TE_SRS_SAD69_UTM_ZONE_25S 29195

/*!\def TE_SRS_SIRGAS2000_UTM_ZONE_17N
 Projected coordinate reference system: UTM projection, Datum SIRGAS2000 Datum, Zone 17 North.
 */
#define TE_SRS_SIRGAS2000_UTM_ZONE_17N 31971

/*!\def TE_SRS_SIRGAS2000_UTM_ZONE_18N
 Projected coordinate reference system: UTM projection, Datum SIRGAS2000 Datum, Zone 18 North.
 */
#define TE_SRS_SIRGAS2000_UTM_ZONE_18N 31972

/*!\def TE_SRS_SIRGAS2000_UTM_ZONE_19N
 Projected coordinate reference system: UTM projection, Datum SIRGAS2000 Datum, Zone 19 North.
 */
#define TE_SRS_SIRGAS2000_UTM_ZONE_19N 31973

/*!\def TE_SRS_SIRGAS2000_UTM_ZONE_20N
 Projected coordinate reference system: UTM projection, Datum SIRGAS2000 Datum, Zone 20 North.
 */
#define TE_SRS_SIRGAS2000_UTM_ZONE_20N 31974

/*!\def TE_SRS_SIRGAS2000_UTM_ZONE_21N
 Projected coordinate reference system: UTM projection, Datum SIRGAS2000 Datum, Zone 21 North.
 */
#define TE_SRS_SIRGAS2000_UTM_ZONE_21N 31975

/*!\def TE_SRS_SIRGAS2000_UTM_ZONE_22N
 Projected coordinate reference system: UTM projection, Datum SIRGAS2000 Datum, Zone 22 North.
 */
#define TE_SRS_SIRGAS2000_UTM_ZONE_22N 31976


/*!\def TE_SRS_SIRGAS2000_UTM_ZONE_17S
 Projected coordinate reference system: UTM projection, Datum SIRGAS2000 Datum, Zone 17 South.
 */
#define TE_SRS_SIRGAS2000_UTM_ZONE_17S 31977

/*!\def TE_SRS_SIRGAS2000_UTM_ZONE_18S
 Projected coordinate reference system: UTM projection, Datum SIRGAS2000 Datum, Zone 18 South.
 */
#define TE_SRS_SIRGAS2000_UTM_ZONE_18S 31978

/*!\def TE_SRS_SIRGAS2000_UTM_ZONE_19S
 Projected coordinate reference system: UTM projection, Datum SIRGAS2000 Datum, Zone 19 South.
 */
#define TE_SRS_SIRGAS2000_UTM_ZONE_19S 31979

/*!\def TE_SRS_SIRGAS2000_UTM_ZONE_20S
 Projected coordinate reference system: UTM projection, Datum SIRGAS2000 Datum, Zone 20 South.
 */
#define TE_SRS_SIRGAS2000_UTM_ZONE_20S 31980

/*!\def TE_SRS_SIRGAS2000_UTM_ZONE_21S
 Projected coordinate reference system: UTM projection, Datum SIRGAS2000 Datum, Zone 21 South.
 */
#define TE_SRS_SIRGAS2000_UTM_ZONE_21S 31981

/*!\def TE_SRS_SIRGAS2000_UTM_ZONE_22S
 Projected coordinate reference system: UTM projection, Datum SIRGAS2000 Datum, Zone 22 South.
 */
#define TE_SRS_SIRGAS2000_UTM_ZONE_22S 31982

/*!\def TE_SRS_SIRGAS2000_UTM_ZONE_23S
 Projected coordinate reference system: UTM projection, Datum SIRGAS2000 Datum, Zone 23 South.
 */
#define TE_SRS_SIRGAS2000_UTM_ZONE_23S 31983

/*!\def TE_SRS_SIRGAS2000_UTM_ZONE_24S
 Projected coordinate reference system: UTM projection, Datum SIRGAS2000 Datum, Zone 24 South.
 */
#define TE_SRS_SIRGAS2000_UTM_ZONE_24S 31984

/*!\def TE_SRS_SIRGAS2000_UTM_ZONE_25S
 Projected coordinate reference system: UTM projection, Datum SIRGAS2000 Datum, Zone 25 South.
 */
#define TE_SRS_SIRGAS2000_UTM_ZONE_25S 31985


/*!\def TE_SRS_WGS84_UTM_ZONE_18N
 Projected coordinate reference system: UTM projection, Datum WGS84 Datum, Zone 18 North.
 */
#define TE_SRS_WGS84_UTM_ZONE_18N 32618

/*!\def TE_SRS_WGS84_UTM_ZONE_19N
 Projected coordinate reference system: UTM projection, Datum WGS84 Datum, Zone 19 North.
 */
#define TE_SRS_WGS84_UTM_ZONE_19N 32619

/*!\def TE_SRS_WGS84_UTM_ZONE_20N
 Projected coordinate reference system: UTM projection, Datum WGS84 Datum, Zone 20 North.
 */
#define TE_SRS_WGS84_UTM_ZONE_20N 32620

/*!\def TE_SRS_WGS84_UTM_ZONE_21N
 Projected coordinate reference system: UTM projection, Datum WGS84 Datum, Zone 21 North.
 */
#define TE_SRS_WGS84_UTM_ZONE_21N 32621

/*!\def TE_SRS_WGS84_UTM_ZONE_22N
 Projected coordinate reference system: UTM projection, Datum WGS84 Datum, Zone 22 North.
 */
#define TE_SRS_WGS84_UTM_ZONE_22N 32622


/*!\def TE_SRS_WGS84_UTM_ZONE_17S
 Projected coordinate reference system: UTM projection, Datum WGS84 Datum, Zone 17 South.
 */
#define TE_SRS_WGS84_UTM_ZONE_17S 32717

/*!\def TE_SRS_WGS84_UTM_ZONE_18S
 Projected coordinate reference system: UTM projection, Datum WGS84 Datum, Zone 18 South.
 */
#define TE_SRS_WGS84_UTM_ZONE_18S 32718

/*!\def TE_SRS_WGS84_UTM_ZONE_19S
 Projected coordinate reference system: UTM projection, Datum WGS84 Datum, Zone 19 South.
 */
#define TE_SRS_WGS84_UTM_ZONE_19S 32719

/*!\def TE_SRS_WGS84_UTM_ZONE_20S
 Projected coordinate reference system: UTM projection, Datum WGS84 Datum, Zone 20 South.
 */
#define TE_SRS_WGS84_UTM_ZONE_20S 32720

/*!\def TE_SRS_WGS84_UTM_ZONE_21S
 Projected coordinate reference system: UTM projection, Datum WGS84 Datum, Zone 21 South.
 */
#define TE_SRS_WGS84_UTM_ZONE_21S 32721

/*!\def TE_SRS_WGS84_UTM_ZONE_22S
 Projected coordinate reference system: UTM projection, Datum WGS84 Datum, Zone 22 South.
 */
#define TE_SRS_WGS84_UTM_ZONE_22S 32722

/*!\def TE_SRS_WGS84_UTM_ZONE_23S
 Projected coordinate reference system: UTM projection, Datum WGS84 Datum, Zone 23 South.
 */
#define TE_SRS_WGS84_UTM_ZONE_23S 32723

/*!\def TE_SRS_WGS84_UTM_ZONE_24S
 Projected coordinate reference system: UTM projection, Datum WGS84 Datum, Zone 24 South.
 */
#define TE_SRS_WGS84_UTM_ZONE_24S 32724

/*!\def TE_SRS_WGS84_UTM_ZONE_25S
 Projected coordinate reference system: UTM projection, Datum WGS84 Datum, Zone 25 South.
 */
#define TE_SRS_WGS84_UTM_ZONE_25S 32725


/*!\def TE_SRS_WGS84_ANTARTIC_POLAR_STEREOGRAPHIC
 Projected coordinate reference system: Antartic Polar Stereographic, Datum WGS84 Datum.
 */
#define TE_SRS_WGS84_ANTARTIC_POLAR_STEREOGRAPHIC 3031
//@}

/*!
 \def TE_SRS_TEXT_DOMAIN
 
 \brief It contains the name of the text domain used in the translation of messages in the TerraLib srs module.
 */
#define TE_SRS_TEXT_DOMAIN "tesrs"

/** @name DLL/LIB Module
 *  Flags for building TerraLib as a DLL or as a Static Library
 */
//@{

//@}

/*!
 \def TESRSEXPORT
 
 \brief You can use this macro in order to export/import classes and functions from this module.
 
 \note If you want to compile TerraLib as DLL in Windows, remember to insert TESRSDLL into the project's list of defines.
 
 \note If you want to compile TerraLib as an Static Library under Windows, remember to insert the TESRSSTATIC flag into the project list of defines.
 */

#ifdef WIN32

#ifdef _MSC_VER
#pragma warning( disable : 4251 )
#endif

#ifdef TESRSSTATIC
#define TESRSEXPORT                          // Don't need to export/import... it is a static library
#elif TESRSDLL
#define TESRSEXPORT  __declspec(dllexport)   // export DLL information
#else
#define TESRSEXPORT  __declspec(dllimport)   // import DLL information
#endif
#else
#define TESRSEXPORT
#endif

//@}

#endif  // __TERRALIB_SRS_INTERNAL_CONFIG_H

