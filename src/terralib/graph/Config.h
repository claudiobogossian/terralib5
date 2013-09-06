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
  \file Config.h

  \brief Configuration flags for the Graph module of TerraLib.
 */

#ifndef __TERRALIB_GRAPH_INTERNAL_CONFIG_H
#define __TERRALIB_GRAPH_INTERNAL_CONFIG_H

// TerraLib
#include "../Config.h"

#define TE_GRAPH_MODULE_NAME "te.graph"

/** @name Internationalization Defines
 *  Flags for TerraLib code internationalization.
 */
//@{

/*!
  \def TE_GRAPH_TEXT_DOMAIN

  \brief It contains the name of the text domain used in the translation of messages in the TerraLib Graph module.
 */
#define TE_GRAPH_TEXT_DOMAIN "tegraph"

/*!
  \def TE_GRAPH_TEXT_DOMAIN_DIR

  \brief It contains the translation catalog directory.
 */
#define TE_GRAPH_TEXT_DOMAIN_DIR "locale"

/*!
  \def TR_GRAPH

  \brief It marks a string in order to get translated. This is a special mark used in the Graph module of TerraLib. 
 */
#define TR_GRAPH(message) TR(message, TE_GRAPH_TEXT_DOMAIN)

/*!
  \def TR_PLURAL_GRAPH

  \brief This mark can be used when you have a plural phrase to be translated. This is a special mark used in the Graph module of TerraLib. 
 */
#define TR_PLURAL_GRAPH(message1, message2, n) TR_PLURAL(TE_GRAPH_TEXT_DOMAIN, message1, message2, n)

//@}

/** @name Graph Module Main Defines
 *  Defines for graph module
 */
//@{

/*!
  \def TE_GRAPH_MODULE_IDENTIFIER

  \brief This definition is the graph module identifier.
 */
#define TE_GRAPH_MODULE_IDENTIFIER  "GRAPH"

/*!
  \def TE_DEFAULT_GRAPH_TYPE

  \brief This definition is used to set the default graph type.
 */
#define TE_DEFAULT_GRAPH_TYPE  TE_GRAPH_FACTORY_GRAPH_TYPE_GRAPH

/*!
  \def TE_DEFAULT_CACHE_POLICY_TYPE

  \brief This definition is used to set the default cache policy.
 */
#define TE_DEFAULT_CACHE_POLICY_TYPE TE_GRAPH_FACTORY_CACHEPOLICY_TYPE_FIFO

/*!
  \def TE_DEFAULT_GRAPH_LOADER_STRATEGY_TYPE

  \brief This definition is used to set the default graph loader strategy.
 */
#define TE_DEFAULT_GRAPH_LOADER_STRATEGY_TYPE TE_GRAPH_FACTORY_LOADERSTRATEGY_TYPE_BOX

/*!
  \def TE_GRAPH_DEFAULT_MAX_CACHE_SIZE

  \brief This definition is used to set the max graph cache size.
 */
#define TE_GRAPH_DEFAULT_MAX_CACHE_SIZE 100000  

/*!
  \def TE_GRAPH_DEFAULT_MAX_VEC_CACHE_SIZE

  \brief This definition is used to set the max graph cache vector size.
 */
#define TE_GRAPH_DEFAULT_MAX_VEC_CACHE_SIZE 5

/*!
  \def TE_GRAPH_DEFAULT_BOX_STRATEGY_LOADER_SIZE

  \brief This definition is used to set the default box strategy loader box size.
 */
#define TE_GRAPH_DEFAULT_BOX_STRATEGY_LOADER_SIZE 20

/*!
  \def TE_GRAPH_STORAGE_MODE_BY_VERTEX

  \brief This definition is used to set the vertex storage mode.
 */
#define TE_GRAPH_STORAGE_MODE_BY_VERTEX  "GRAPH_STORAGE_MODE_BY_VERTEX"

/*!
  \def TE_GRAPH_STORAGE_MODE_BY_EDGE

  \brief This definition is used to set the edge storage mode.
 */
#define TE_GRAPH_STORAGE_MODE_BY_EDGE  "GRAPH_STORAGE_MODE_BY_EDGE"
//@}

/** @name Graph Module Factories Names
 *  Defines the all graph module factories names
 */
//@{

#define TE_GRAPH_FACTORY_GRAPH_TYPE_GRAPH "GRAPH"
#define TE_GRAPH_FACTORY_GRAPH_TYPE_BIDIRECTIONALGRAPH "BIDIRECTIONALGRAPH"
#define TE_GRAPH_FACTORY_GRAPH_TYPE_DIRECTEDGRAPH "DIRECTEDGRAPH"
#define TE_GRAPH_FACTORY_GRAPH_TYPE_UNDIRECTEDGRAPH "UNDIRECTEDGRAPH"

#define TE_GRAPH_FACTORY_CACHEPOLICY_TYPE_FIFO "FIFO"
#define TE_GRAPH_FACTORY_CACHEPOLICY_TYPE_LFU "LFU"

#define TE_GRAPH_FACTORY_LOADERSTRATEGY_TYPE_BOX "BOX_LOADER_STRATEGY"
#define TE_GRAPH_FACTORY_LOADERSTRATEGY_TYPE_SEQUENCE "SEQUENCE_LOADER_STRATEGY"
//@}



/** @name Database Metadata Module
 *  Defines for graph database tables
 */
//@{
#define TE_GRAPH_GRAPH_TABLE_NAME "te_graph"
#define TE_GRAPH_GRAPH_TABLE_ATTR_ID "id"
#define TE_GRAPH_GRAPH_TABLE_ATTR_NAME "name"
#define TE_GRAPH_GRAPH_TABLE_ATTR_TYPE "type"
#define TE_GRAPH_GRAPH_TABLE_ATTR_TABLE_NAME "table_name"
#define TE_GRAPH_GRAPH_TABLE_ATTR_DESCRIPTION "description"

#define TE_GRAPH_GRAPH_ATTR_TABLE_NAME "te_graph_attr"
#define TE_GRAPH_GRAPH_ATTR_TABLE_ATTR_ID "id"
#define TE_GRAPH_GRAPH_ATTR_TABLE_ATTR_GRAPH_ID "graph_id"
#define TE_GRAPH_GRAPH_ATTR_TABLE_ATTR_TABLE "table_name"
#define TE_GRAPH_GRAPH_ATTR_TABLE_ATTR_COLUMN "column_name"
#define TE_GRAPH_GRAPH_ATTR_TABLE_ATTR_LINK "link_column"
#define TE_GRAPH_GRAPH_ATTR_TABLE_ATTR_TYPE "type"

#define TE_GRAPH_GRAPH_TABLE_ATTR_EDGE_SUFIX "_attr_model_edge"
#define TE_GRAPH_GRAPH_TABLE_EDGE_SUFIX "_model_edge"
#define TE_GRAPH_GRAPH_EDGE_MODEL_ID "edge_id"
#define TE_GRAPH_GRAPH_EDGE_MODEL_VFROM "vertex_from"
#define TE_GRAPH_GRAPH_EDGE_MODEL_VTO "vertex_to"

#define TE_GRAPH_GRAPH_TABLE_ATTR_VERTEX_SUFIX "_attr_model_vertex"
#define TE_GRAPH_GRAPH_TABLE_VERTEX_SUFIX "_model_vertex"
#define TE_GRAPH_GRAPH_VERTEX_MODEL_ID "vertex_id"
//@}

/** @name DLL/LIB Module
 *  Flags for building TerraLib as a DLL or as a Static Library
 */
//@{

/*!
  \def TEGRAPHEXPORT

  \brief You can use this macro in order to export/import classes and functions from this module.

  \note If you want to compile TerraLib as DLL in Windows, remember to insert TEGRAPHDLL into the project's list of defines.

  \note If you want to compile TerraLib as an Static Library under Windows, remember to insert the TEGRAPHSTATIC flag into the project list of defines.
 */
#ifdef WIN32

  #ifdef _MSC_VER 
    #pragma warning( disable : 4251 )
    #pragma warning( disable : 4275 )
  #endif

  #ifdef TEGRAPHSTATIC
    #define TEGRAPHEXPORT                          // Don't need to export/import... it is a static library
  #elif TEGRAPHDLL
    #define TEGRAPHEXPORT  __declspec(dllexport)   // export DLL information
  #else
    #define TEGRAPHEXPORT  __declspec(dllimport)   // import DLL information
  #endif 
#else
  #define TEGRAPHEXPORT
#endif

//@}        

#endif  // __TERRALIB_GRAPH_INTERNAL_CONFIG_H

