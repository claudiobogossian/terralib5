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
  \file terralib/postgis/Globals.h
   
  \brief An static class with global definitions.
*/

#ifndef __TERRALIB_POSTGIS_INTERNAL_GLOBALS_H
#define __TERRALIB_POSTGIS_INTERNAL_GLOBALS_H

// TerraLib
#include "../common/Static.h"
#include "Config.h"

// STL
#include <string>

namespace te
{
  // Forward declaration
  namespace da
  { 
    class DataSourceCapabilities;
    class SQLDialect;
  }

  namespace pgis
  {
    /*!
      \class Globals

      \brief An static class with global definitions.
     */
    class TEPGISEXPORT Globals : public te::common::Static
    {
      public:

// PostgreSQL type names
        static const std::string sm_unknownTypeName;              //!< The string literal representation for the unknown type.
        static const std::string sm_charTypeName;                 //!< The string literal representation for the 1-byte character type.
        static const std::string sm_int2TypeName;                 //!< The string literal representation for the int2 type
        static const std::string sm_intTypeName;                  //!< The string literal representation for the int type
        static const std::string sm_int8TypeName;                 //!< The string literal representation for the int8 type
        static const std::string sm_numericTypeName;              //!< The string literal representation for the numeric type
        static const std::string sm_dateTypeName;                 //!< The string literal representation for the date type
        static const std::string sm_timeTypeName;                 //!< The string literal representation for the time type
        static const std::string sm_timeTZTypeName;               //!< The string literal representation for the time type
        static const std::string sm_timeStampTypeName;            //!< The string literal representation for the time stamp type
        static const std::string sm_timeStampTZTypeName;          //!< The string literal representation for the time stamp with time zone type
        static const std::string sm_intervalTypeName;             //!< The string literal representation for the interval       
        static const std::string sm_floatTypeName;                //!< The string literal representation for the float type
        static const std::string sm_doubleTypeName;               //!< The string literal representation for the double type
        static const std::string sm_booleanTypeName;              //!< The string literal representation for the boolean type
        static const std::string sm_fixedcharTypeName;            //!< The string literal representation for the char type.
        static const std::string sm_varcharTypeName;              //!< The string literal representation for the varchar type
        static const std::string sm_stringTypeName;               //!< The string literal representation for the string type
        static const std::string sm_byteArrayTypeName;            //!< The string literal representation for the byte array type
        static const std::string sm_pointTypeName;                //!< The string literal representation for PostGIS point geometry type
        static const std::string sm_pointMTypeName;               //!< The string literal representation for PostGIS point geometry type 
        static const std::string sm_lineStringTypeName;           //!< The string literal representation for PostGIS line string geometry type
        static const std::string sm_lineStringMTypeName;          //!< The string literal representation for PostGIS line string with m geometry type
        static const std::string sm_polygonTypeName;              //!< The string literal representation for PostGIS polygon geometry type
        static const std::string sm_polygonMTypeName;             //!< The string literal representation for PostGIS polygon with m geometry type
        static const std::string sm_geometryCollectionTypeName;   //!< The string literal representation for PostGIS geometry collection geometry type
        static const std::string sm_geometryCollectionMTypeName;  //!< The string literal representation for PostGIS geometry collection with m geometry type
        static const std::string sm_multiPointTypeName;           //!< The string literal representation for PostGIS multi-point geometry type
        static const std::string sm_multiPointMTypeName;          //!< The string literal representation for PostGIS multi-point with m geometry type
        static const std::string sm_multiLineStringTypeName;      //!< The string literal representation for PostGIS multi-linestring geometry type
        static const std::string sm_multiLineStringMTypeName;     //!< The string literal representation for PostGIS multi-linestring with m geometry type
        static const std::string sm_multiPolygonTypeName;         //!< The string literal representation for PostGIS multi-polygon geometry type
        static const std::string sm_multiPolygonMTypeName;        //!< The string literal representation for PostGIS multi-polygon with m geometry type
        static const std::string sm_geometryTypeName;             //!< The string literal representation for PostGIS base geometry type

        static const std::string sm_driverIdentifier;  //!< The PostGIS driver identifier.

        static te::da::SQLDialect* sm_queryDialect;                //!< The query dialect supported by PostGIS driver.
        static te::da::DataSourceCapabilities* sm_capabilities;    //!< The query dialect supported by PostGIS driver.
    };

  } // end namespace pgis
}   // end namespace te

#endif  // __TERRALIB_POSTGIS_INTERNAL_GLOBALS_H

