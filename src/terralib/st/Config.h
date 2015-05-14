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

  \brief Teste git! Configuration flags for the st (spatiotemporal) module of TerraLib.
*/

#ifndef __TERRALIB_ST_INTERNAL_CONFIG_H
#define __TERRALIB_ST_INTERNAL_CONFIG_H

#include "../Config.h"

/*!
  \def TE_ST_MODULE_NAME

  \brief The st module identifier string.
*/
#define TE_ST_MODULE_NAME "te.st"

/*!
  \def TE_ST_DATALOADER_FROM_DS

  \brief The st data loader from a general data source identifier string.
*/
#define TE_ST_DATALOADER_FROM_DS "STDATALOADERFROMDS"

/*!
  \def TE_ST_DATALOADER_FROM_MEMDS

  \brief The st data loader from a st in-memory data source identifier string.
*/
#define TE_ST_DATALOADER_FROM_MEMDS "STDATALOADERFROMMEMDS"

/*!
  \def TE_STMEMORY_DATASOURCE_ID

  \brief The global id of the st memory data source
*/
#define TE_STMEMORY_DATASOURCE_ID "STMEMORYDS"

/** @name DLL/LIB Module
 *  Flags for building TerraLib ST module as a DLL or as a Static Library
 */
//@{

/*!
  \def TESTEXPORT

  \brief You can use this macro in order to export/import classes and functions from this module.

  \note If you want to compile TerraLib ST module as DLL in Windows, remember to insert TESTDLL into the project's list of defines.

  \note If you want to compile TerraLib ST module as a Static Library under Windows, remember to insert the TESTSTATIC flag into the project list of defines.
 */
#ifdef WIN32

  #ifdef _MSC_VER 
    #pragma warning( disable : 4127 )
    #pragma warning( disable : 4251 )
    #pragma warning( disable : 4275 )

  #endif

  #ifdef TESTSTATIC
    #define TESTEXPORT                          // Don't need to export/import... it is a static library
  #elif TESTDLL
    #define TESTEXPORT  __declspec(dllexport)   // export DLL information
  #else
    #define TESTEXPORT  __declspec(dllimport)   // import DLL information
  #endif 
#else
  #define TESTEXPORT
#endif

//@}        

#endif  // __TERRALIB_ST_INTERNAL_CONFIG_H

