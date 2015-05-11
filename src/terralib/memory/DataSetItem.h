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
  namespace da
  {
// Forward declaration
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
          \brief It creates a new item having the given number of properties.

          \param nproperties The number of properties.
        */
        explicit DataSetItem(const std::size_t& nproperties);

        /*!
          \brief It creates a new item by cloning the values in the source item (rhs).

          \param rhs The right-hand-side object to copy its values.
        */
        explicit DataSetItem(const DataSetItem& rhs);

        /*! \brief Destructor. */
        virtual ~DataSetItem();

        /*!
          \brief Assignment operator.

          It copies the values from the rhs item.

          \param rhs The right-hand-side ibject to copy its values.

          \return A reference to this item.

          \pre The rhs item must be of a compatible type with the item being assigned.
        */
        DataSetItem& operator=(const DataSetItem& rhs);

        /*!
          \brief It returns a clone of the DataSetItem.
        */
        std::auto_ptr<DataSetItem> clone() const;

        /*!
          \brief It returns its parent.

          \note The caller does not have the ownership of the returned pointer.
        */
        te::da::DataSet* getParent() const;

        /*!
          \brief It returns the number of properties.
        */
        std::size_t getNumProperties() const;

        /*!
          \brief It returns the type of the pos-th property.
        */
        int getPropertyDataType(std::size_t pos) const;

        /*!
          \brief It returns the name of the pos-th property.
        */
        std::string getPropertyName(std::size_t pos) const;

        /*!
          \brief It returns the value of the i-th property.
        */
        char getChar(std::size_t i) const;

        /*!
          \brief It sets the value of the i-th property.
        */
        void setChar(std::size_t i, char value);

        /*!
          \brief It sets the value of the property, indicating its name. 
        */
        void setChar(const std::string& name, char value);

        /*!
          \brief It returns the value of the i-th property.
        */
        unsigned char getUChar(std::size_t i) const;

        /*!
          \brief It sets the value of the i-th property.
        */
        void setUChar(std::size_t i, unsigned char value);

        /*!
          \brief It sets the value of the property, indicating its name. 
        */
        void setUChar(const std::string& name, unsigned char value);

        /*!
          \brief It returns the value of the i-th property.
        */
        boost::int16_t getInt16(std::size_t i) const;

        /*!
          \brief It sets the value of the i-th property.
        */
        void setInt16(std::size_t i, boost::int16_t value);

        /*!
          \brief It sets the value of the property, indicating its name. 
        */
        void setInt16(const std::string& name, boost::int16_t value);

        /*!
          \brief It returns the value of the i-th property.
        */
        boost::int32_t getInt32(std::size_t i) const;

        /*!
          \brief It sets the value of the i-th property.
        */
        void setInt32(std::size_t i, boost::int32_t value);

        /*!
          \brief It sets the value of the property, indicating its name. 
        */
        void setInt32(const std::string& name, boost::int32_t value);

        /*!
          \brief It returns the value of the i-th property.
        */
        boost::int64_t getInt64(std::size_t i) const;

        /*!
          \brief It sets the value of the i-th property.
        */
        void setInt64(std::size_t i, boost::int64_t value);

        /*!
          \brief It sets the value of the property, indicating its name. 
        */
        void setInt64(const std::string& name, boost::int64_t value);

        /*!
          \brief It returns the value of the i-th property.
        */
        bool getBool(std::size_t i) const;

        /*!
          \brief It sets the value of the i-th property.
        */
        void setBool(std::size_t i, bool value);

        /*!
          \brief It sets the value of the property, indicating its name. 
        */
        void setBool(const std::string& name, bool value);

        /*!
          \brief It returns the value of the i-th property.
        */
        float getFloat(std::size_t i) const;

        /*!
          \brief It sets the value of the i-th property.
        */
        void setFloat(std::size_t i, float value);

        /*!
          \brief It sets the value of the property, indicating its name. 
        */
        void setFloat(const std::string& name, float value);

        /*!
          \brief It returns the value of the i-th property.
        */
        double getDouble(std::size_t i) const;

        /*!
          \brief It sets the value of the i-th property.
        */
        void setDouble(std::size_t i, double value);

        /*!
          \brief It sets the value of the property, indicating its name. 
        */
        void setDouble(const std::string& name, double value);

        /*!
          \brief It returns the value of the i-th property.
        */
        std::string getNumeric(std::size_t i) const;

        /*!
          \brief It sets the value of the i-th property.
        */
        void setNumeric(std::size_t i, const std::string& value);

        /*!
          \brief It sets the value of the property, indicating its name. 
        */
        void setNumeric(const std::string& name, const std::string& value);

        /*!
          \brief It returns the value of the i-th property.
        */
        std::string getString(std::size_t i) const;

        /*!
          \brief It sets the value of the i-th property.
        */
        void setString(std::size_t i, const std::string& value);

        /*!
          \brief It sets the value of the property, indicating its name. 
        */
        void setString(const std::string& name, const std::string& value);

        /*!
          \brief It returns the value of the i-th property.
        */
        std::auto_ptr<te::dt::ByteArray> getByteArray(std::size_t i) const;

        /*!
          \brief It sets the value of the i-th property.

          \note It will take the ownership of the given pointer.
        */
        void setByteArray(std::size_t i, te::dt::ByteArray* value);

        /*!
          \brief It sets the value of the property, indicating its name. 

          \note It will take the ownership of the given pointer.
        */
        void setByteArray(const std::string& name, te::dt::ByteArray* value);

        /*!
          \brief It returns the value of the i-th property.
        */
        std::auto_ptr<te::gm::Geometry> getGeometry(std::size_t i) const;

        /*!
          \brief It sets the value of the i-th property.

          \note It will take the ownership of the given pointer.
        */
        void setGeometry(std::size_t i, te::gm::Geometry* value);

        /*!
          \brief It sets the value of the property, indicating its name. 

          \note It will take the ownership of the given pointer.
        */
        void setGeometry(const std::string& name, te::gm::Geometry* value);

        /*!
          \brief It returns the value of the i-th property.
        */
        std::auto_ptr<te::rst::Raster> getRaster(std::size_t i) const;

        /*!
          \brief It sets the value of the i-th property.

          \note It will take the ownership of the given pointer.
        */
        void setRaster(std::size_t i, te::rst::Raster* value);

        /*!
          \brief It sets the value of the property, indicating its name. 
        */
        void setRaster(const std::string& name, te::rst::Raster* value);

        /*!
          \brief It returns the value of the i-th property.
        */
        std::auto_ptr<te::dt::DateTime> getDateTime(std::size_t i) const;

        /*!
          \brief It sets the value of the i-th property.

          \note It will take the ownership of the given pointer.
        */
        void setDateTime(std::size_t i, te::dt::DateTime* value);

        /*!
          \brief It sets the value of the property, indicating its name. 

          \note It will take the ownership of the given pointer.
        */
        void setDateTime(const std::string& name, te::dt::DateTime* value);

        /*!
          \brief It returns the value of the i-th property.
        */
        std::auto_ptr<te::dt::AbstractData> getValue(std::size_t i) const;

        /*!
          \brief It sets the value of the i-th property.

          \note It will take the ownership of the given pointer.
        */
        void setValue(std::size_t i, te::dt::AbstractData* value);

        /*!
          \brief It sets the value of the property, indicating its name. 

          \note It will take the ownership of the given pointer.
        */
        void setValue(const std::string& name, te::dt::AbstractData* value);

        bool isNull(std::size_t i) const;

      protected:

        const te::da::DataSet* m_parent;                                    //!< The parent dataset, if the item is associated to one.
        boost::ptr_vector<boost::nullable<te::dt::AbstractData> > m_data;   //!< The data values of the dataset item.

      friend class DataSet;
    };

    /*!
      \brief For use with boost containers.
    */
    inline DataSetItem* new_clone(const DataSetItem& a)
    {
      return a.clone().release();
    }

  } // end namespace mem
}   // end namespace te

#endif  // __TERRALIB_MEMORY_INTERNAL_DATASETITEM_H
