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
  \file terralib/memory/DataSet.h

  \brief Implementation of a random-access dataset class for the TerraLib In-Memory Data Access driver.
*/

#ifndef __TERRALIB_MEMORY_INTERNAL_DATASET_H
#define __TERRALIB_MEMORY_INTERNAL_DATASET_H

// TerraLib
#include "../dataaccess/dataset/DataSet.h"
#include "Config.h"

// STL
#include <map>
#include <memory>

// Boost
#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/shared_ptr.hpp>

namespace te
{
  namespace da
  {
    class DataSetType;
  }

  namespace dt
  {
    class Property;
  }

  namespace mem
  {
// Forward declarations
    class DataSetItem;

    /*!
      \class DataSet

      \brief Implementation of a random-access dataset class for the TerraLib In-Memory Data Access driver.

      \sa te::da::DataSet
    */
    class TEMEMORYEXPORT DataSet : public te::da::DataSet
    {
      public:

        /*!
          \brief It constructs an empty dataset having the schema dt.

          \param dt The DataSetType associated to the dataset.

          \note The dataset will NOT take the ownership of the given DataSetType.
        */
        explicit DataSet(const te::da::DataSetType* const dt);

        /*!
          \brief Regular copy constructor.

          It creates a new In-Memory dataset with the items from the rhs dataset. 

          \param rhs The dataset which will provide the items.

          \pre You must assure that rhs is in a valid item to be read.

          \note This constructor will use the method "moveNext()" of the source dataset (rhs)
                in order to read its dataset items. It will start reading the given 
                dataset in the current position. So, the caller is responsible for
                informing the dataset in the right position to start processing it.

          \note The new dataset will not have a transactor associated to it.
        */
        DataSet(te::da::DataSet& rhs);

        /*!
          \brief It creates a new In-Memory dataset with the items from the rhs dataset. 

          \param rhs      The dataset which will provide the items to copy from.
          \param deepCopy If true the new dataset instance will clone all dataset items from the source dataset (rhs), otherwise it will share all items.

          \note The new dataset will not have a transactor associated to it.
        */
        explicit DataSet(const DataSet& rhs, const bool deepCopy = false);

        /*!
          \brief Copy constructor with property restriction.

          It creates a new memory data set with items from a source dataset (rhs)
          considering just some properties.

          \param rhs        The data set whose data set items will be copied.
          \param properties The list of properties which will be considered in the copy.
          \param limit      The number of items to be copied. Use 0 to copy all items.

          \pre The properties in the property vector must come from the source dataset (rhs) schema. In other
                words, they must be valid pointers in the rhs dataset type.

          \note This constructor will use the method "moveNext()" of the given data set
                in order to read its dataset items. It will start reading the given 
                dataset in the current position. So, the caller is responsible for
                informing the dataset in the right position to start processing it.

          \note The dataset will NOT take the ownership of the given properties, internally it will clone them.
        */
        DataSet(te::da::DataSet& rhs, const std::vector<std::size_t>& properties, std::size_t limit = 0);

        /*! \brief Destructor. */
        ~DataSet();

        /*! \brief It clears all the dataset items. */
        void clear();

        /*!
          \brief It copies up to limit items from the source dataset.

          \param src   The source dataset with the items that will be copied.
          \param limit The number of items to be copied. Use 0 to copy all items.

          \note This method will call moveNext() for the source dataset
                in order to read its items. It will start reading the given 
                dataset in the current position. So, the caller is responsible for
                informing the dataset in the right position to start processing it.

          \note In-Memory driver extended method.
        */
        void copy(te::da::DataSet& src, std::size_t limit = 0);

        /*!
          \brief It copies up to limit items from the source dataset (src).

          \param src        The source dataset with the items that will be copied.
          \param properties The list of properties which will be considered in the copy.
          \param limit      The number of items to be copied. Use 0 to copy all items.

          \pre The properties in the property vector must come from the source dataset (rhs) schema. In other
                words, they must be valid pointers in the rhs dataset type.

          \note This method will call moveNext() for the source dataset
                in order to read its items. It will start reading the given 
                dataset in the current position. So, the caller is responsible for
                informing the dataset in the right position to start processing it.

          \note In-Memory driver extended method.
        */
        void copy(te::da::DataSet& src, const std::vector<std::size_t>& properties, std::size_t limit = 0);

        /*!
          \brief It adds a new item to the dataset and takes its ownership.

          \param item The item to be added to the dataset.

          \pre The item schema must be compatible with the dataset items.

          \note In-Memory driver extended method.
        */
        void add(DataSetItem* item);

        /*!
          \brief It removes the current dataset item.

          \note In-Memory driver extended method.
        */
        void remove();

        /*!
          \brief It removes a specific dataset item.

          \note In-Memory driver extended method.
        */
        void remove(DataSetItem* item);

        /*!
          \brief It adds a new property to the dataset schema and fills this new property with a default value.

          \param prop           The new property that will be created. The dataset will take its ownership.
          \param defaultValue   The default value that will be used to fill the new property values. May be a NULL value. 

          \note The DataSet will NOT take the ownership of the informed default value.

          \note In-Memory driver extended method.
        */
        void add(const std::string& propertyName, std::size_t propertyType, const te::dt::AbstractData* defaultValue = 0);

        /*!
          \brief It drops a property from the dataset.

          \param prop The property that will be dropped.

          \note The Property will be destroyed.

          \note In-Memory driver extended method.
        */
        void drop(std::size_t pos);

        /*!
          \brief It update a property from the dataset.

          \param prop The property that will be updated.

          \note For now, for all items, the value will be changed to 0.

          \todo Update the method to perform the necessary conversions
                when the type change

          \note In-Memory driver extended method.
        */
        void update(te::dt::Property* prop);

        /*! \name DataSet inherited methods */
        //@{
        te::common::TraverseType getTraverseType() const;

        te::common::AccessPolicy getAccessPolicy() const;

        std::size_t getNumProperties() const;

        int getPropertyDataType(std::size_t pos) const;

        void setPropertyDataType(int dt, std::size_t pos);

        std::string getPropertyName(std::size_t pos) const;

        void setPropertyName(const std::string& name, std::size_t pos);

        te::common::CharEncoding getPropertyCharEncoding(std::size_t i) const;

        std::string getDatasetNameOfProperty(std::size_t pos) const;

        DataSetItem* getItem() const;

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

        std::auto_ptr<te::dt::AbstractData> getValue(std::size_t i) const;

        bool isNull(std::size_t i) const;
        //@}

        /*! \name Methods to set values to the DataSet 

            \Note These methods will take the ownership of the given pointers.
        */
        //@{
        void setChar(std::size_t i, char value);

        void setChar(const std::string& name, char value);

        void setUChar(std::size_t i, unsigned char value);

        void setUChar(const std::string& name, unsigned char value);

        void setInt16(std::size_t i, boost::int16_t value);

        void setInt16(const std::string& name, boost::int16_t value);

        void setInt32(std::size_t i, boost::int32_t value);

        void setInt32(const std::string& name, boost::int32_t value);

        void setInt64(std::size_t i, boost::int64_t value);

        void setInt64(const std::string& name, boost::int64_t value);

        void setBool(std::size_t i, bool value);

        void setBool(const std::string& name, bool value);

        void setFloat(std::size_t i, float value);

        void setFloat(const std::string& name, float value);

        void setDouble(std::size_t i, double value);

        void setDouble(const std::string& name, double value);

        void setNumeric(std::size_t i, const std::string& value);

        void setNumeric(const std::string& name, const std::string& value);

        void setString(std::size_t i, const std::string& value);

        void setString(const std::string& name, const std::string& value);

        void setByteArray(std::size_t i, te::dt::ByteArray* value);

        void setByteArray(const std::string& name, te::dt::ByteArray* value);

        void setGeometry(std::size_t i, te::gm::Geometry* value);

        void setGeometry(const std::string& name, te::gm::Geometry* value);

        void setRaster(std::size_t i, te::rst::Raster* value);

        void setRaster(const std::string& name, te::rst::Raster* value);

        void setDateTime(std::size_t i, te::dt::DateTime* value);

        void setDateTime(const std::string& name, te::dt::DateTime* value);

        void setValue(std::size_t i, te::dt::AbstractData* value);

        void setValue(const std::string& name, te::dt::AbstractData* ad);

        //@}

      protected:

        boost::shared_ptr<boost::ptr_vector<DataSetItem> > m_items;   //!< The list of dataset items.
        std::vector<std::string> m_pnames;                            //!< The list of property names.
        std::vector<int> m_ptypes;                                    //!< The list of property types.
        std::map<int, te::common::CharEncoding> m_encodings;          //!< The list of string properties char-encoding.
        int m_i;                                                      //!< The index of the current item.
    };

    typedef boost::shared_ptr<DataSet> DataSetPtr;

  } // end namespace mem
}   // end namespace te

#endif  // __TERRALIB_MEMORY_INTERNAL_DATASET_H
