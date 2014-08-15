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
  \file terralib/sa/Config.h

  \brief Configuration flags for the Terrralib Spatial Analysis module.
 */

#ifndef __TERRALIB_SA_CORE_INTERNAL_CONFIG_H
#define __TERRALIB_SA_CORE_INTERNAL_CONFIG_H

// TerraLib
#include "../Config.h"

#define TE_SA_MODULE_NAME "te.sa"

/*! Defines the attribute name used in gpm to identigy the distance information*/
#define TE_SA_DISTANCE_ATTR_NAME "distance"

/*! Defines the attribute name used in gpm to identigy the distance information*/
#define TE_SA_WEIGHT_ATTR_NAME "weight"

/*! Defines the attribute name used in gpm to identigy the distance information*/
#define TE_SA_GEOMETRY_ATTR_NAME "coord"

/*! Defines the attribute name G used in GStatistics operation*/
#define TE_SA_G_ATTR_NAME "G"

/*! Defines the attribute name G* used in GStatistics operation*/
#define TE_SA_GSTAR_ATTR_NAME "GStar"

/*! Defines the attribute name LocalMean used in Local Mean operation*/
#define TE_SA_LOCALMEAN_ATTR_NAME "LocalMean"

/*! Defines the attribute name NumNeighbors used in Local Mean operation*/
#define TE_SA_NUMNEIGHBORS_ATTR_NAME "NumNeighbors"

/*! Defines the attribute name Standard Deviation Z used in ZAndWZ operation*/
#define TE_SA_STDDEVZ_ATTR_NAME "Z"

/*! Defines the attribute name Local Mean of Z (WZ) used in ZAndWZ operation*/
#define TE_SA_LOCALMEANWZ_ATTR_NAME "WZ"

/*! Defines the attribute name Moran Index used in MoranIndex operation*/
#define TE_SA_MORANINDEX_ATTR_NAME "MoranIndex"

/*! Defines the attribute name LISA Significance used in LisaStatisticalSignificance operation*/
#define TE_SA_LISASIGNIFICANCE_ATTR_NAME "LISASig"

/*! Defines the attribute name Box Map used in BoxMap operation*/
#define TE_SA_BOXMAP_ATTR_NAME "BoxMap"

/*! Defines the attribute name Lisa Map used in LISAMap operation*/
#define TE_SA_LISAMAP_ATTR_NAME "LisaMap"

/*! Defines the attribute name Moran Map used in MoranMap operation*/
#define TE_SA_MORANMAP_ATTR_NAME "MoranMap"

/*! Defines the attribute name Bayes used in Bayes operation*/
#define TE_SA_BAYES_ATTR_NAME "Bayes"

/*! Defines the attribute name NeighEvent used in Local Bayes operation*/
#define TE_SA_BAYESEVENT_ATTR_NAME "Neigh_Event"

/*! Defines the attribute name NeighPop used in Local Bayes operation*/
#define TE_SA_BAYESPOP_ATTR_NAME "Neigh_Pop"

/** @name DLL/LIB Module
 *  Flags for building TerraLib as a DLL or as a Static Library
 */
//@{

/*!
  \def TESAEXPORT

  \brief You can use this macro in order to export/import classes and functions from this module.

  \note If you want to compile TerraLib as DLL in Windows, remember to insert TESADLL into the project's list of defines.

  \note If you want to compile TerraLib as an Static Library under Windows, remember to insert the TESASTATIC flag into the project list of defines.
 */
#ifdef WIN32

  #ifdef _MSC_VER
    #pragma warning( disable : 4251 )
    #pragma warning( disable : 4275 )
  #endif


  #ifdef TESASTATIC
    #define TESAEXPORT                          // Don't need to export/import... it is a static library
  #elif TESADLL
    #define TESAEXPORT  __declspec(dllexport)   // export DLL information
  #else
    #define TESAEXPORT  __declspec(dllimport)   // import DLL information
  #endif 
#else
  #define TESAEXPORT
#endif

//@}

#endif  // __TERRALIB_SA_CORE_INTERNAL_CONFIG_H

