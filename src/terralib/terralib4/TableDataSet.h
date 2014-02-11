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
  \file terralib/terralib4/TableDataSet.h

  \brief ????
*/

#ifndef __TERRALIB_TERRALIB4_INTERNAL_TABLEDATASET_H
#define __TERRALIB_TERRALIB4_INTERNAL_TABLEDATASET_H

// TerraLib
#include "../dataaccess/dataset/DataSet.h"
#include "Config.h"

// Terralib 4.x
#include <terralib/kernel/TeDatabase.h>

// STL
#include <memory>
#include <vector>

namespace te
{
// Forward declarations
  namespace da
  {
    class GeometryProperty;
  }
}

class TeLayer;
class TeDatabasePortal;

namespace terralib4
{
  /*!
    \class TableDataSet

    \brief Implementation of a dataset for the TerraLib 4 driver.

    \sa te::da::DataSet
  */
  class TETERRALIB4EXPORT TableDataSet : public te::da::DataSet
  {
    public:

      TableDataSet(TeDatabase* db, TeTable table);

      ~TableDataSet();

      te::common::TraverseType getTraverseType() const;

      te::common::AccessPolicy getAccessPolicy() const;

      std::size_t getNumProperties() const;

      int getPropertyDataType(std::size_t i) const;

      std::string getPropertyName(std::size_t i) const;

      std::string getDatasetNameOfProperty(std::size_t i) const;

      bool isEmpty() const;

      bool isConnected() const;

      std::size_t size() const;

      std::auto_ptr<te::gm::Envelope> getExtent(std::size_t i);

      bool moveNext();

      bool movePrevious();

      bool moveBeforeFirst();

      bool moveFirst();

      bool moveLast();

      bool move(std::size_t i);

      bool isAtBegin() const;

      bool isBeforeBegin() const;

      bool isAtEnd() const;

      bool isAfterEnd() const;

      char getChar(std::size_t i) const;

      unsigned char getUChar(std::size_t i) const;

      boost::int16_t getInt16(std::size_t i) const;

      boost::int32_t getInt32(std::size_t i) const;

      boost::int64_t getInt64(std::size_t i) const;

      bool getBool(std::size_t i) const;

      float getFloat(std::size_t i) const;

      double getDouble(std::size_t i) const;

      std::string getNumeric(std::size_t i) const;

      std::string getString(std::size_t i) const;

      std::auto_ptr<te::dt::ByteArray> getByteArray(std::size_t i) const;

      std::auto_ptr<te::gm::Geometry> getGeometry(std::size_t i) const;

      std::auto_ptr<te::rst::Raster> getRaster(std::size_t i) const;

      std::auto_ptr<te::dt::DateTime> getDateTime(std::size_t i) const;

      std::auto_ptr<te::dt::Array> getArray(std::size_t i) const;

      bool isNull(std::size_t i) const;

    protected:

      te::da::DataSetType* m_dt;
      TeTable m_table;
      int m_i;
      int m_size;
      int m_nCols;
      te::da::DataSet* m_data;
      TeDatabase* m_db;
      TeDatabasePortal* m_portal;
  };

}   // end namespace te


#endif  // __TERRALIB_TERRALIB4_INTERNAL_TABLEDATASET_H



