/*  Copyright (C) 2008-2013 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/postgis/Config.h

  \brief Configuration flags for the PostGIS Driver Implementation of TerraLib.
*/

#ifndef __TERRALIB_POSTGIS_INTERNAL_CONFIG_H
#define __TERRALIB_POSTGIS_INTERNAL_CONFIG_H

#include "../Config.h"

/** @name General Defines
 *  Configuring PostGIS support.
 */
//@{

/*!
  \def PGIS_DEFAULT_INITIAL_POOL_SIZE
  
  \brief This sets the default initial number of connections opened by a connection pool.
 */
#define PGIS_DEFAULT_INITIAL_POOL_SIZE   4

/*!
  \def PGIS_DEFAULT_MIN_POOL_SIZE
  
  \brief This sets the default minimum number of connections to be kept in the pool.
 */
#define PGIS_DEFAULT_MIN_POOL_SIZE       2

/*!
  \def PGIS_DEFAULT_MAX_POOL_SIZE
  
  \brief This sets the default maximum number of connections in the pool.
 */
#define PGIS_DEFAULT_MAX_POOL_SIZE       4

/*!
  \def PGIS_DEFAULT_MAX_IDLE_TIME
  
  \brief This sets the default maximum time that a connection can be in the pool without being used.
 */
#define PGIS_DEFAULT_MAX_IDLE_TIME       10

/*!
  \def PGIS_DEFAULT_PORT

  \brief The default port used to connect to a PostgreSQL database server.
 */
#define PGIS_DEFAULT_PORT                "5432"

/*!
  \def PGIS_DEFAULT_CONNECT_TIMEOUT
  
  \brief The default maximum wait for connection, in seconds.
 */
#define PGIS_DEFAULT_CONNECT_TIMEOUT     "4"

/*!
  \def PGIS_DEFAULT_CLIENT_ENCODING
  
  \brief The default client encoding when talking with the database.
 */
#define PGIS_DEFAULT_CLIENT_ENCODING     "UTF8"

/*!
  \def PGIS_DRIVER_IDENTIFIER

  \brief The PostGIS driver identifier string.
 */
#define PGIS_DRIVER_IDENTIFIER "POSTGIS"

/*!
  \def TERRALIB_RASTER_DRIVER_IDENTIFIER

  \brief The TerraLib Raster driver identifier string.
 */
#define TERRALIB_RASTER_DRIVER_IDENTIFIER "TEPG"

#define PG_UNKNOW_TYPE                    -1
#define PG_BOOL_TYPE                      16
#define PG_BYTEA_TYPE                     17
#define PG_CHAR_TYPE                      18
#define PG_NAME_TYPE                      19
#define PG_INT8_TYPE                      20
#define PG_INT2_TYPE                      21
#define PG_INT2_VECTOR_TYPE               22
#define PG_INT4_TYPE                      23
#define PG_TEXT_TYPE                      25
#define PG_OID_TYPE                       26
#define PG_FLOAT4_TYPE                   700
#define PG_FLOAT8_TYPE                   701
#define PG_CHARACTER_TYPE               1042
#define PG_VARCHAR_TYPE                 1043
#define PG_DATE_TYPE                    1082
#define PG_TIME_TYPE                    1083
#define PG_TIMETZ_TYPE                  1266
#define PG_TIMESTAMP_TYPE               1114
#define PG_TIMESTAMPTZ_TYPE             1184
#define PG_NUMERIC_TYPE                 1700
#define PG_BOOL_ARRAY_TYPE              1000
#define PG_BYTEA_ARRAY_TYPE             1001
#define PG_CHAR_ARRAY_TYPE              1002
#define PG_NAME_ARRAY_TYPE              1003
#define PG_INT8_ARRAY_TYPE              1016
#define PG_INT2_ARRAY_TYPE              1005
#define PG__INT2_VECTOR_TYPE            1006
#define PG_INT4_ARRAY_TYPE              1007
#define PG_TEXT_ARRAY_TYPE              1009
#define PG_OID_ARRAY_TYPE               1028
#define PG_OID_VECTOR_TYPE              1013
#define PG_FLOAT4_ARRAY_TYPE            1021
#define PG_FLOAT8_ARRAY_TYPE            1022
#define PG_CHARACTER_ARRAY_TYPE         1014
#define PG_VARCHAR_ARRAY_TYPE           1015
#define PG_DATE_ARRAY_TYPE              1182
#define PG_TIME_ARRAY_TYPE              1183
#define PG_TIMETZ_ARRAY_TYPE            1270
#define PG_TIMESTAMP_ARRAY_TYPE         1115
#define PG_TIMESTAMPTZ_ARRAY_TYPE       1185
#define PG_NUMERIC_ARRAY_TYPE           1231

//@}        

/** @name PostGIS Compatibility Flags
 *  Flags for constructing compatible WKB format with PostGIS
 */
//@{

#define TE_EWKB_ZM_OFFSET    0xC0000000
#define TE_EWKB_Z_OFFSET     0x80000000
#define TE_EWKB_M_OFFSET     0x40000000
#define TE_EWKB_SRID_FLAG    0x20000000
#define TE_EWKB_BBOX_FLAG    0x10000000

//@}

/** @name DLL/LIB Module
 *  Flags for building TerraLib as a DLL or as a Static Library
 */
//@{

/*!
  \def TEPGISEXPORT

  \brief You can use this macro in order to export/import classes and functions from this module.

  \note If you want to compile TerraLib as DLL in Windows, remember to insert TEPGISDLL into the project's list of defines.

  \note If you want to compile TerraLib as an Static Library under Windows, remember to insert the TEPGISSTATIC flag into the project list of defines.
 */
#ifdef WIN32

  #ifdef _MSC_VER 
    #pragma warning( disable : 4251 )
    #pragma warning( disable : 4275 )
  #endif

  #ifdef TEPGISSTATIC
    #define TEPGISEXPORT                          // Don't need to export/import... it is a static library
  #elif TEPGISDLL
    #define TEPGISEXPORT  __declspec(dllexport)   // export DLL information
  #else
    #define TEPGISEXPORT  __declspec(dllimport)   // import DLL information
  #endif 
#else
  #define TEPGISEXPORT
#endif

//@}        

#endif  // __TERRALIB_POSTGIS_INTERNAL_CONFIG_H

