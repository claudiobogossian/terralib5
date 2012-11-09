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
  \file terralib/mem/DataSet.h

  \brief Implementation of a random-access dataset class for the TerraLib In-Memory Data Access driver.
*/

#ifndef __TERRALIB_MEMORY_INTERNAL_DATASET_H
#define __TERRALIB_MEMORY_INTERNAL_DATASET_H

// TerraLib
#include "../dataaccess/dataset/DataSet.h"
#include "Config.h"

// STL
#include <memory>

// Boost
#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/shared_ptr.hpp>

namespace te
{
  namespace mem
  {
// Forward declarations
    class DataSetItem;
    class DataSourceTransactor;

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
          \param t  The associated transactor.
          
          \note DataSet will take the ownership of the given DataSetType.
        */
        explicit DataSet(te::da::DataSetType* dt, DataSourceTransactor* t = 0);

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
        DataSet(te::da::DataSet& rhs, const std::vector<te::dt::Property*>& properties, std::size_t limit = 0);

        /*! \brief Destructor. */
        ~DataSet();

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
        void copy(te::da::DataSet& src, const std::vector<te::dt::Property*>& properties, std::size_t limit = 0);

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
          \brief It removes the current dataset item.

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
        void add(te::dt::Property* prop, const te::dt::AbstractData* defaultValue = 0);

        /*!
          \brief It drops a property from the dataset.

          \param prop The property that will be dropped.          

          \note The Property will be destroyed.

          \note In-Memory driver extended method.
        */
        void drop(te::dt::Property* prop);

        /*!
          \brief It update a property from the dataset.

          \param prop The property that will be updated.

          \note For now, for all item the value will be changed to 0.

          \todo Update the method to perform the necessary conversions
                when the type change

          \note In-Memory driver extended method.
        */
        void update(te::dt::Property* prop);

        /*!          
          \note In-Memory driver extended method.
        */
        void setTransactor(DataSourceTransactor* t);

        te::common::TraverseType getTraverseType() const;

        te::common::AccessPolicy getAccessPolicy() const;

        te::da::DataSetType* getType();

        const te::da::DataSetType* getType() const;

        te::da::DataSourceTransactor* getTransactor() const;

        void loadTypeInfo();

        te::da::DataSet* getParent() const;

        te::gm::Envelope* getExtent(const te::dt::Property* p);

        void setFilter(te::dt::Property* p,
                       const te::gm::Geometry* g,
                       te::gm::SpatialRelation r = te::gm::INTERSECTS);

        void setFilter(te::dt::Property* p,
                       const te::gm::Envelope* e,
                       te::gm::SpatialRelation r = te::gm::INTERSECTS);

        te::da::DataSetItem* getItem() const;

        void add(te::da::DataSetItem* item);

        bool isEmpty() const;

        std::size_t size() const;

        bool moveNext();

        bool movePrevious();

        bool moveFirst();

        bool moveBeforeFirst();

        bool moveLast();

        bool moveAfterLast();

        bool move(std::size_t i);

        bool isAtBegin() const;

        bool isBeforeBegin() const;

        bool isAtEnd() const;

        bool isAfterEnd() const;

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

        te::dt::AbstractData* getValue(int i) const;

        te::dt::AbstractData* getValue(const std::string& name) const;

        void setValue(int i, te::dt::AbstractData* value);

        void setValue(const std::string& name, te::dt::AbstractData* ad);

        bool isNull(int i) const;

        bool isNull(const std::string& name) const;

        te::dt::AbstractData* clone() const;

      protected:

        boost::shared_ptr<boost::ptr_vector<te::mem::DataSetItem> > m_items;   //!< The list of dataset items.
        std::auto_ptr<te::da::DataSetType> m_dt;                               //!< The associated dataset type.
        DataSet* m_parent;                                                     //!< A parent dataset.
        DataSourceTransactor* m_t;                                             //!< The associated datasource transactor.
        int m_i;                                                               //!< The index of the current item.
    };

  } // end namespace mem
}   // end namespace te

#endif  // __TERRALIB_MEMORY_INTERNAL_DATASET_H
