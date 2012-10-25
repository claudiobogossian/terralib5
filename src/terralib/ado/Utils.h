/*  Copyright (C) 2008-2011 National Institute For Space Research (INPE) - Brazil.

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
#include "../geometry/Geometry.h"

// ADO
#import "msado15.dll" \
    no_namespace rename("EOF", "EndOfFile")
#import "msadox.dll"

namespace te
{
  namespace da
  {
    class DataSetType;
    class DataSetItem;
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

  namespace ado
  {
    /*!
      \brief Add a ADO propert based on the TerraLib property

      \param table Table target
      \param prop Property to be added
    */
    void addAdoPropertyFromTerralib(ADOX::_TablePtr table, te::dt::Property* prop);

    /*!
      \brief Add a Terralib item in a Ado record set

      \param dt DataSetType
      \param recset The ADO record set where will be added
      \param props The properties that will be added
      \param item The TerraLib item referenced
    */
    void addItem(const te::da::DataSetType* dt, _RecordsetPtr recset, std::vector<te::dt::Property*> props, te::da::DataSetItem* item);
    
    /*!
      \brief It returns the geometry OGC names.

      \param t The TerraLib geometry type.

      \return The geometry OGC names.
    */
    const std::string& GetGeometryName(te::gm::GeomType t);

    /*!
      \brief Bind ADO Type to TerraLib Type.

      \param adoType Ado Type.

      \return TerraLib Type
    */
    int Convert2Terralib(ADOX::DataTypeEnum adoType);

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
      \brief Bind TerraLib Type to ADO Type.

      \param terralib TerraLib Type.

      \return ADO Type
    */
    ADOX::DataTypeEnum Convert2Ado(int terralib);

    /*!
      \brief Bind TerraLib geometry to ADO variant.

      \param geo TerraLib Geometry.
      \param var Ado variant
    */
    void Convert2Ado(const te::gm::Geometry* geo, _variant_t & var);
    
    /*!
      \brief Create TerraLib property from ado property (column)

      \param column ADO column pointer

      \return TerraLib Property
    */
    te::dt::Property* getPropertyFromADO(ADOX::_ColumnPtr column);
    
    /*!
      \brief Get the default geometry property from table "geometry_columns"
      created on Access database

      \param dt Data Set Type

      \return TerraLib geometry property
    */
    te::gm::GeometryProperty* getDefaultGeomProperty(te::da::DataSetType* dt, _ConnectionPtr adoConn);
    
    /*!
      \brief Verifies whether Z property

      \param type geometry type

      \return if is Z property
    */
    bool isZProperty(te::gm::GeomType type);

    /*!
      \brief Verifies whether is in the geometry_columns table

      \param adoConn Ado connection
      \param tableName Ado table name
      \param columnName Ado column name

      \return if is in geometry_columns table
    */
    bool isGeomProperty(_ConnectionPtr adoConn, std::string tableName, std::string columnName);

    /*!
      \brief Get the  geometry type in OGC standard

      \param type geometry type

      \return OGC standard type name
    */
    std::string getOGCType(te::gm::GeomType type);

    /*!
      \brief Convert a blob to a variant

      \param blob Blob
      \param size Blob size
      \param var Result variant
    */
    void Blob2Variant(const char* blob, int size, _variant_t & var);
    
    /*!
      \brief Convert a variant to a blob

      \param var Variant
      \param size Variant size
      \param blob Result variant
    */
    void Variant2Blob(const _variant_t var, int size, char* & blob);

    /*!
      \brief Update a ADO column based on informations from DataSetType

      \param dt DataSetType
      \param recset The ADO record to be updated
      \param prop The property that will be updated
      \param item The TerraLib item referenced
    */
    void updateAdoColumn(const te::da::DataSetType* dt, _RecordsetPtr recset, te::dt::Property* prop, te::da::DataSetItem* item);

    /*!
      \brief Insert DataSetType with geometry in the geometryColumns table

      \param adoConn ADO connection
      \param dt DataSetType to be inserted
    */
    void insertInGeometryColumns(_ConnectionPtr adoConn, const te::da::DataSetType* dt);
    
  } // end namespace ado
}   // end namespace te

#endif  // __TERRALIB_ADO_INTERNAL_UTILS_H
