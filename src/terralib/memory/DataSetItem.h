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
  \file DataSetItem.h

  \brief An implementation of the DatasetItem class for the TerraLib In-Memory Data Access driver.
*/

#ifndef __TERRALIB_MEMORY_INTERNAL_DATASETITEM_H
#define __TERRALIB_MEMORY_INTERNAL_DATASETITEM_H

// TerraLib
#include "../common/Holder.h"
#include "../dataaccess/dataset/DataSetItem.h"
#include "Config.h"

// Boost
#include <boost/ptr_container/ptr_vector.hpp>

namespace te
{  
  namespace mem
  {
// Forward declarations
    class DataSet;

    /*!
      \class DataSetItem

      \brief An implementation of the DatasetItem class for the TerraLib In-Memory Data Access driver.

      \sa te::da::DataSetItem
    */
    class TEMEMORYEXPORT DataSetItem : public te::da::DataSetItem
    {
      public:

        /*!
          \brief It creates a new item having the same schema as the parent dataset.

          \param parent The dataset to which this item is associated.

          \note The dataset item will share the parent's dataset type.
        */
        explicit DataSetItem(const te::da::DataSet* parent);

        /*!
          \brief It creates a new item having the informed schema.

          \param dt The associated schema. The item will take its ownership.
        */
        explicit DataSetItem(te::da::DataSetType* dt);

        /*!
          \brief Constructor.

          It creates a new empty item associated to the informed dataset having a defined schema.

          \param parent      A pointer to the dataset associated to this item.
          \param dt          The item schema. (Can not be NULL)
          \param dtOwnership If true the item will take the schema ownership.

          \note The item will share the dataset type information.
        */
        explicit DataSetItem(const te::da::DataSet* parent, te::da::DataSetType* dt, const bool dtOwnership = false);

        /*!
          \brief It creates a new item by cloning the values in the source item (rhs).

          \param rhs The right-hand-side object to copy its values.
          \param dt  The associated schema. The item will take its ownership.
        */
        explicit DataSetItem(const te::da::DataSetItem& rhs, te::da::DataSetType* dt); 

        /*!
          \brief It creates a new item by cloning the values in the source item (rhs).

          \param rhs         The right-hand-side object to copy its values.
          \param parent      A pointer to the dataset associated to this item. (Can not be NULL)
          \param dt          The item schema. (Can not be NULL)
          \param dtOwnership If true the item will take the schema ownership.
        */
        explicit DataSetItem(const te::da::DataSetItem& rhs, DataSet* parent, te::da::DataSetType* dt, const bool dtOwnership = false); 

        /*!
          \brief Copy constructor.

          \param rhs The riht-hand-side item.
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
        DataSetItem& operator=(const te::da::DataSetItem& rhs);

        /*!
          \brief Assignment operator.

          It copies the values from the rhs item.

          \param rhs The right-hand-side ibject to copy its values.

          \return A reference to this item.

          \pre The rhs item must be of a compatible type with the item being assigned.
        */
        DataSetItem& operator=(const DataSetItem& rhs);

        DataSetItem* clone() const;

        te::da::DataSetType* getType();

        const te::da::DataSetType* getType() const;

        te::da::DataSet* getParent() const;

        char getChar(int i) const;

        char getChar(const std::string& name) const;

        void setChar(int i, char value);

        void setChar(const std::string& name, char value);

        unsigned char getUChar(int i) const;

        unsigned char getUChar(const std::string& name) const;

        void setUChar(int i, unsigned char value);

        void setUChar(const std::string& name, unsigned char value);

        boost::int16_t getInt16(int i) const;

        boost::int16_t getInt16(const std::string& name) const;

        void setInt16(int i, boost::int16_t value);

        void setInt16(const std::string& name, boost::int16_t value);

        boost::int32_t getInt32(int i) const;

        boost::int32_t getInt32(const std::string& name) const;

        void setInt32(int i, boost::int32_t value);

        void setInt32(const std::string& name, boost::int32_t value);

        boost::int64_t getInt64(int i) const;

        boost::int64_t getInt64(const std::string& name) const;

        void setInt64(int i, boost::int64_t value);

        void setInt64(const std::string& name, boost::int64_t value);

        bool getBool(int i) const;

        bool getBool(const std::string& name) const;

        void setBool(int i, bool value);

        void setBool(const std::string& name, bool value);

        float getFloat(int i) const;

        float getFloat(const std::string& name) const;

        void setFloat(int i, float value);

        void setFloat(const std::string& name, float value);

        double getDouble(int i) const;

        double getDouble(const std::string& name) const;

        void setDouble(int i, double value);

        void setDouble(const std::string& name, double value);

        std::string getNumeric(int i) const;

        std::string getNumeric(const std::string& name) const;

        void setNumeric(int i, const std::string& value);

        void setNumeric(const std::string& name, const std::string& value);

        std::string getString(int i) const;

        std::string getString(const std::string& name) const;

        void setString(int i, const std::string& value);

        void setString(const std::string& name, const std::string& value);

        te::dt::ByteArray* getByteArray(int i) const;

        te::dt::ByteArray* getByteArray(const std::string& name) const;

        void setByteArray(int i, const te::dt::ByteArray& value);

        void setByteArray(const std::string& name, const te::dt::ByteArray& value);

        te::gm::Geometry* getGeometry(int i) const;

        te::gm::Geometry* getGeometry(const std::string& name) const;

        void setGeometry(int i, const te::gm::Geometry& value);

        void setGeometry(const std::string& name, const te::gm::Geometry& value);

        te::rst::Raster* getRaster(int i) const;

        te::rst::Raster* getRaster(const std::string& name) const;

        void setRaster(int i, const te::rst::Raster& value);

        void setRaster(const std::string& name, const  te::rst::Raster& value);

        te::dt::DateTime* getDateTime(int i) const;

        te::dt::DateTime* getDateTime(const std::string& name) const;

        void setDateTime(int i, const te::dt::DateTime& value);

        void setDateTime(const std::string& name, const te::dt::DateTime& value);

        void getArray(int i, std::vector<boost::int16_t>& values) const;

        void getArray(const std::string& name, std::vector<boost::int16_t>& values) const;

        const unsigned char* getWKB(int i) const;

        const unsigned char* getWKB(const std::string& name) const;

        te::da::DataSet* getDataSet(int i);

        te::da::DataSet* getDataSet(const std::string& name);

        void setDataSet(int i, const te::da::DataSet& value);

        void setDataSet(const std::string& name, const te::da::DataSet& value);

        void setValue(int i, te::dt::AbstractData* value);

        void setValue(const std::string& name, te::dt::AbstractData* value);

        bool isNull(int i) const;

        bool isNull(const std::string& name) const;

      protected:

        const te::da::DataSet* m_parent;                                    //!< The parent dataset if the item is associate to one.
        boost::ptr_vector<boost::nullable<te::dt::AbstractData> > m_data;   //!< The data values of the dataset item.
        te::common::Holder<te::da::DataSetType> m_dt;                       //!< The item schema.

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
