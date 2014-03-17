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
  \file terralib/ado/Utils.h
   
  \brief Utility functions for ADO.  
*/

#ifndef __TERRALIB_ADO_INTERNAL_UTILS_H
#define __TERRALIB_ADO_INTERNAL_UTILS_H

// TerraLib
#include "../common/StringUtils.h"
#include "../geometry/Geometry.h"
#include "Config.h"

// STL
#include <cstdlib>
#include <cstring>
#include <vector>

// ADO
#import "msado15.dll" \
  no_namespace rename("EOF", "EndOfFile")
#import "msadox.dll"

namespace te
{

  namespace da
  {
    class DataSet;
    class DataSetType;
    class PrimaryKey;
    class ForeignKey;
    class UniqueKey;
    class Constraint;
  }

  namespace dt
  {
    class Property;
  }

  namespace gm
  {
    class GeometryProperty;
  }

  namespace mem
  {
    class DataSetItem;
  }

  namespace ado
  {

    /*!
      \brief Convert a blob to a variant

      \param blob Blob
      \param size Blob size
      \param var Result variant
    */
    void Blob2Variant(const char* blob, int size, _variant_t & var);

    /*!
      \brief Create a connection string based on a map

      \param dsInfo Information Map.

      \return ADO connection string
    */
    std::string MakeConnectionStr(const std::map<std::string, std::string>& dsInfo);

    /*!
      \brief Convert a variant to a blob

      \param var Variant
      \param size Variant size
      \param blob Result variant
    */
    void Variant2Blob(const _variant_t var, int size, char* & blob);

    /*!
      \brief Bind TerraLib Type to ADO Type.

      \param terralib TerraLib Type.

      \return ADO Type
    */
    ADOX::DataTypeEnum Convert2Ado(int terralib);

    /*!
      \brief Bind TerraLib type to an ADO valid fiel type name.

      \terralib TerraLib data type.

      \return Valid ADO field type as string value.
    */
    std::string GetAdoStringType(const int& terralib);

    /*!
      \brief Bind TerraLib geometry to ADO variant.

      \param geo TerraLib Geometry.
      \param var Ado variant
    */
    void Convert2Ado(const te::gm::Geometry* geo, _variant_t & var);

    /*!
      \brief Bind ADOX Type to TerraLib Type.

      \param adoType ADOX Type.

      \return TerraLib Type
    */
    int Convert2Terralib(ADOX::DataTypeEnum adoType);

    /*!
      \brief Bind ADO Type to TerraLib Type.

      \param adoType Ado Type.

      \return TerraLib Type
    */
    int Convert2Terralib(::DataTypeEnum adoType);

    /*!
      \brief Bind ADO column to Terralib property

      \param column Ado Column.

      \return TerraLib Property
    */
    te::dt::Property* Convert2Terralib(ADOX::_ColumnPtr column);

    /*!
      \brief Bind ADO key to Terralib constraint

      \param key Ado key.

      \return TerraLib Constraint
    */
    te::da::Constraint* Convert2Terralib(ADOX::_KeyPtr key);

    /*!
      \brief Bind ADO columns to vector of Terralib properties 

      \param columns Ado columns.

      \return Vector of TerraLib properties
    */
    std::vector<te::dt::Property*> Convert2Terralib(ADOX::ColumnsPtr columns);

    /*!
      \brief It returns the geometry OGC names.

      \param t The TerraLib geometry type.

      \return The geometry OGC names.
    */
    const std::string& GetGeometryName(te::gm::GeomType t);

    /*!
      \brief It returns the geometry type concerning the OGC name

      \param t The OGC type name

      \return The Terralib geometry type
    */
    const te::gm::GeomType GetGeometryType(std::string t);

    /*!
      \brief Read the geometry_columns table end return a SRID

      \param adoConn Ado connection.
      \param tableName The table name.
      \param geomPropName The geometry property name.

      \return SRID of the geometry
    */
    int GetSRID(_ConnectionPtr adoConn, std::string tableName, std::string geomPropName);

    /*!
      \brief Read the geometry_columns table end return a geometry type

      \param adoConn Ado connection.
      \param tableName The table name.
      \param geomPropName The geometry property name.

      \return The geometry type
    */
    te::gm::GeomType GetType(_ConnectionPtr adoConn, std::string tableName, std::string geomPropName);

    /*!
      \brief Verifies whether is in the geometry_columns table

      \param adoConn Ado connection
      \param tableName Ado table name
      \param columnName Ado column name

      \return if is in geometry_columns table
    */
    bool IsGeomProperty(_ConnectionPtr adoConn, std::string tableName, std::string columnName);

    /*!
      \brief Verifies whether Z property

      \param type geometry type

      \return if is Z property
    */
    bool IsZProperty(te::gm::GeomType type);

    /*!
      \brief It gets the system Date and Time format.

      \param indAM  AM indicator string.
      \param indPM  PM indicator string.
      \param sepD   Date separator.
      \param sepT   Time separator.

      \return The full DateTime format.
    */
    std::string GetSystemDateTimeFormat(std::string& indAM, std::string& indPM, std::string& sepD, std::string& sepT);

    /*!
      \brief It gets the DateTime TerraLib 5 from string.

      \param value  String value to be read.
      \param mask   System DateTime mask;
      \param sepD   Date separator.
      \param sepT   Time separator.

      \return The TerraLib 5 DateTime.
    */
    std::auto_ptr<te::dt::DateTime> GetDateTime(std::string& value, std::string& mask, std::string& sepD, std::string& sepT);

    /*!
      \brief It gets the index of a month.

      \param month  Month name.

      \return The month index.
    */
    int GetMonth(const std::string& month);

    /*!
      \brief It gets a formatted DateTime string for ADO

      \param dateTime TerraLib 5 DateTime.

      \return A formatted DateTime string.
    */
    std::string GetFormattedDateTime(te::dt::DateTime* dateTime);

  } // end namespace ado
}   // end namespace te

#endif  // __TERRALIB_ADO_INTERNAL_UTILS_H
