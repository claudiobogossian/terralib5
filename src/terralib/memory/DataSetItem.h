/*  Copyright (C) 2010-2011 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/memory/DataSetItem.h

  \brief An implementation of the DatasetItem class for the TerraLib In-Memory Data Access driver.
*/

#ifndef __TERRALIB_MEMORY_INTERNAL_DATASETITEM_H
#define __TERRALIB_MEMORY_INTERNAL_DATASETITEM_H

// TerraLib
#include "Config.h"

// Boost
#include <boost/cstdint.hpp>
#include <boost/ptr_container/ptr_vector.hpp>

// Forward declarations
namespace te { namespace dt { class AbstractData; class Array; class ByteArray; class DateTime; } }
namespace te { namespace gm { class Geometry; } }
namespace te { namespace rst { class Raster; } }

namespace te
{

// Forward declaration
  namespace da
  {
    class DataSet;
  }

  namespace mem
  {
    /*!
      \class DataSetItem

      \brief An implementation of the DatasetItem class for the TerraLib In-Memory Data Access driver.
    */
    class TEMEMORYEXPORT DataSetItem
    {
      public:

        /*!
          \brief It creates a new item having the same schema as the parent dataset.

          \param parent The dataset to which this item is associated.

          \note The dataset item will NOT take the ownership of the given pointer.
        */
        explicit DataSetItem(const te::da::DataSet* parent);

        /*!
          \brief It creates a new item by cloning the values in the source item (rhs).

          \param rhs The right-hand-side object to copy its values.
        */
        explicit DataSetItem(const DataSetItem& rhs);

        /*! \brief Destructor. */
        ~DataSetItem();

        /*!
          \brief Assignment operator.

          It copies the values from the rhs item.

          \param rhs The right-hand-side ibject to copy its values.

          \return A reference to this item.

          \pre The rhs item must be of a compatible type with the item being assigned.
        */
        DataSetItem& operator=(const DataSetItem& rhs);

        DataSetItem* clone() const;

        te::da::DataSet* getParent() const;

        std::size_t getNumProperties() const;

        int getPropertyDataType(std::size_t pos) const;

        std::string getPropertyName(std::size_t pos) const;

        char getChar(std::size_t i) const;

        void setChar(std::size_t i, char value);

        void setChar(const std::string& name, char value);

        unsigned char getUChar(std::size_t i) const;

        void setUChar(std::size_t i, unsigned char value);

        void setUChar(const std::string& name, unsigned char value);

        boost::int16_t getInt16(std::size_t i) const;

        void setInt16(std::size_t i, boost::int16_t value);

        void setInt16(const std::string& name, boost::int16_t value);

        boost::int32_t getInt32(std::size_t i) const;

        void setInt32(std::size_t i, boost::int32_t value);

        void setInt32(const std::string& name, boost::int32_t value);

        boost::int64_t getInt64(std::size_t i) const;

        void setInt64(std::size_t i, boost::int64_t value);

        void setInt64(const std::string& name, boost::int64_t value);

        bool getBool(std::size_t i) const;

        void setBool(std::size_t i, bool value);

        void setBool(const std::string& name, bool value);

        float getFloat(std::size_t i) const;

        void setFloat(std::size_t i, float value);

        void setFloat(const std::string& name, float value);

        double getDouble(std::size_t i) const;

        void setDouble(std::size_t i, double value);

        void setDouble(const std::string& name, double value);

        std::string getNumeric(std::size_t i) const;

        void setNumeric(std::size_t i, const std::string& value);

        void setNumeric(const std::string& name, const std::string& value);

        std::string getString(std::size_t i) const;

        void setString(std::size_t i, const std::string& value);

        void setString(const std::string& name, const std::string& value);

        te::dt::ByteArray* getByteArray(std::size_t i) const;

        void setByteArray(std::size_t i, const te::dt::ByteArray& value);

        void setByteArray(const std::string& name, const te::dt::ByteArray& value);

        te::gm::Geometry* getGeometry(std::size_t i) const;

        void setGeometry(std::size_t i, const te::gm::Geometry& value);

        void setGeometry(const std::string& name, const te::gm::Geometry& value);

        te::rst::Raster* getRaster(std::size_t i) const;

        void setRaster(std::size_t i, const te::rst::Raster& value);

        void setRaster(const std::string& name, const  te::rst::Raster& value);

        te::dt::DateTime* getDateTime(std::size_t i) const;

        void setDateTime(std::size_t i, const te::dt::DateTime& value);

        void setDateTime(const std::string& name, const te::dt::DateTime& value);

        te::dt::Array* getArray(std::size_t i) const;

        /*!
          \brief Method for retrieving any other type of data value stored in the data source.

          This method can be use for extensible datatypes.

          \param i The attribute index.

          \return A pointer to the data value.

          \note The caller of this method will take the ownership of the returned pointer.
        */
        te::dt::AbstractData* getValue(std::size_t i) const;

        void setValue(std::size_t i, te::dt::AbstractData* value);

        void setValue(const std::string& name, te::dt::AbstractData* value);

        bool isNull(std::size_t i) const;

      protected:

        const te::da::DataSet* m_parent;                                    //!< The parent dataset if the item is associate to one.
        boost::ptr_vector<boost::nullable<te::dt::AbstractData> > m_data;   //!< The data values of the dataset item.

      friend class DataSet;
    };

    /*!
      \brief For use with boost conteiners.
    */
    inline DataSetItem* new_clone(const DataSetItem& a)
    {
      return static_cast<DataSetItem*>(a.clone());
    }

  } // end namespace mem
}   // end namespace te

#endif  // __TERRALIB_MEMORY_INTERNAL_DATASETITEM_H
