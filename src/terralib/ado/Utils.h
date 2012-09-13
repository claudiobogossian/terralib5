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
    int ado2Terralib(ADOX::DataTypeEnum adoType);

    /*!
      \brief Bind TerraLib Type to ADO Type.

      \param terralib TerraLib Type.

      \return ADO Type
    */
    ADOX::DataTypeEnum terralib2Ado(int terralib);
    
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
      \brief Add a ADO propert based on the TerraLib property

      \param table Table target
      \param prop Property to be added
    */
    void addAdoPropertyFromTerralib(ADOX::_TablePtr table, te::dt::Property* prop);

    
    te::dt::Property* Convert2Terralib(ADOX::_ColumnPtr column);
    te::da::Constraint* Convert2Terralib(ADOX::_KeyPtr key);
    std::vector<te::dt::Property*> Convert2Terralib(ADOX::ColumnsPtr columns);

    bool isZProperty(te::gm::GeomType type);
    bool isGeomProperty(_ConnectionPtr adoConn, std::string tableName, std::string columnName);

    std::string getOGCType(te::gm::GeomType type);

    void Blob2Variant(const char* blob, int size, _variant_t & var);
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
