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
  \file terralib/terralib4/VectorDataSet.h

  \brief ????
*/

#ifndef __TERRALIB_TERRALIB4_INTERNAL_VECTORDATASET_H
#define __TERRALIB_TERRALIB4_INTERNAL_VECTORDATASET_H

// TerraLib
#include "../dataaccess/dataset/DataSet.h"
#include "Config.h"

// Terralib 4.x
#include <terralib4/kernel/TeQuerier.h>

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
    \class VectorDataSet

    \brief Implementation of a dataset for the TerraLib 4 driver.

    \sa te::da::DataSet
  */
  class TETERRALIB4EXPORT VectorDataSet : public te::da::DataSet
  {
    public:

      VectorDataSet(TeLayer* layer);

      ~VectorDataSet();

      te::common::TraverseType getTraverseType() const;

      te::common::AccessPolicy getAccessPolicy() const;

      std::size_t getNumProperties() const;

      int getPropertyDataType(std::size_t i) const;

      std::string getPropertyName(std::size_t i) const;

      te::common::CharEncoding getPropertyCharEncoding(std::size_t i) const;

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
      TeQuerier* m_querier;
      mutable TeSTInstance m_instance;
      int m_i;
      int m_size;
      int m_nCols;
      int m_geomCol;
      bool m_hasGeometry;
      TeLayer* m_layer;
  };

}   // end namespace te


#endif  // __TERRALIB_TERRALIB4_INTERNAL_VECTORDATASET_H



