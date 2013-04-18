/*  Copyright (C) 2001-2009 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/dataaccess/dataset/DataSetAdapter.h

  \brief An adapter for DataSet.
*/

#ifndef __TERRALIB_DATAACCESS_INTERNAL_DATASETADAPTER_H
#define __TERRALIB_DATAACCESS_INTERNAL_DATASETADAPTER_H

// TerraLib
#include "../../common/Holder.h"
#include "../Config.h"
#include "AttributeConverters.h"
#include "DataSet.h"

// STL
#include <vector>

namespace te
{
  namespace da
  {
// Forward declarations
    class DataSourceCapabilities;

    /*!
      \class DataSetAdapter

      \brief An adapter for DataSet.

      \sa DataSet
     */
    class TEDATAACCESSEXPORT DataSetAdapter : public DataSet
    {
      public:

        /** @name Constructor/Destructor
         *  Initilizer methods.
         */
        //@{

        /*!
          \brief Constructor.

          \param dataset A pointer to the DataSet that will be handled by adapter.
          \param isOwner If true the DataSetAdapter will have the ownership of the given data set pointer.

          \note Here no automatic property adaptation will be made. i.e. The adapater is invalid.
          \note The method "adapt" can be used to do manual adaptations.
        */
        DataSetAdapter(DataSet* dataset, bool isOwner = false);

        /*!
          \brief Constructor.

          \param dataset A pointer to the DataSet that will be handled by adapter.
          \param capabilities A data source capabilities of data source that the user intend use.
                 e.g. the data source where the given data set will be saved.
          \param isOwner If true the DataSetAdapter will have the ownership of the given data set pointer.

          \note This method will try create automatic property adaptations based on given capabilities.
                Here we will look up the data type support of DataSourceCapabilities.
                Case there is not support for a given data type, the method "getHint" will be used to try retrieves an appropriate property.

          \note Case the method "getHint" no informs any hint, the specific property will not have an adaptation. i.e. The adapater is invalid.
          \note The method "adapt" can be used to do manual adaptations.
          \note The method "getNonAdaptedProperties" can be used to retrieve that properties that no have defined adaptation.
          
          \sa DataSourceCapabilities DataTypeCapabilities
        */
        DataSetAdapter(DataSet* dataset, const DataSourceCapabilities& capabilities, bool isOwner = false);

        /*! \brief Virtual Destructor. */
        virtual ~DataSetAdapter();

        //@}

        te::common::TraverseType getTraverseType() const;

        te::common::AccessPolicy getAccessPolicy() const;

        te::gm::Envelope* getExtent(std::size_t i);

        std::size_t getNumProperties() const;

        int getPropertyDataType(std::size_t pos) const;

        std::string getPropertyName(std::size_t pos) const;

        /*!
          \brief It returns the underlying dataset name of the property at position pos.

          \param pos The property position of interest.

          \return The underlying dataset name of the property at position pos.

          \note This method will always return an empty string for the adapter.
        */
        std::string getDatasetNameOfProperty(std::size_t pos) const;

        bool isEmpty() const;

        std::size_t size() const;

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

        te::dt::ByteArray* getByteArray(std::size_t i) const;

        te::gm::Geometry* getGeometry(std::size_t i) const;

        te::rst::Raster* getRaster(std::size_t i) const;

        te::dt::DateTime* getDateTime(std::size_t i) const;

        te::dt::Array* getArray(std::size_t i) const;

        bool isNull(std::size_t i) const;

        /** @name DataSet Adapter Extended Methods
         *  Methods that exists only in the DataSet Adapter implementation.
         */
        //@{

        /*!
          \brief This method returns the pointer to the DataSet that is handled by adapter.

          \return The pointer to the DataSet that is handled by adapter.

          \note The caller will NOT take the ownership of the returned pointer.
        */
        te::da::DataSet* getAdaptee() const;

        /*!
          \brief This method returns the name of the properties that have not yet been adapted.

          \param propertyNames A vector that will be filled with the name of the properties that have not yet been adapted.
        */
        void getNonAdaptedProperties(std::vector<std::string>& propertyNames) const;

        /*!
          \brief This method returns the index of the properties that have not yet been adapted.

          \param propertyPos A vector that will be filled with the index of the properties that have not yet been adapted.
        */
        void getNonAdaptedProperties(std::vector<std::size_t>& propertyPos) const;

         /*!
          \brief This method tells which properties of the handled data set that have been adapted based on the given property name.

          \param propertyName A property name of the DataSetAdapter.
          \param adaptedPropertyPos  A vector that will be filled with the property positions of the handled data set have been adapted.
        */
        void getAdaptedProperties(const std::string& propertyName, std::vector<std::size_t>& adaptedPropertyPos);

        /*!
          \brief This method tells which properties of the handled data set that have been adapted based on the given property name.

          \param propertyPos A property position of the DataSetAdapter.
          \param adaptedPropertyPos  A vector that will be filled with the property positions of the handled data set have been adapted.
        */
        void getAdaptedProperties(std::size_t propertyPos, std::vector<std::size_t>& adaptedPropertyPos);

        /*!
          \brief This method removes a property of DataSetAdapter.

          \param propertyName The property name of the DataSetAdapter that will be removed.
        */
        void remove(const std::string& propertyName);

        /*!
          \brief This method removes a property of DataSetAdapter.

          \param i The property index of the DataSetAdapter that will be removed.
        */
        void remove(std::size_t i);

        void add(const std::string& newPropertyName,
                 int newPropertyType,
                 const std::string& adaptedPropertyName,
                 AttributeConverter conv = GenericAttributeConverter);

        void add(const std::string& newPropertyName,
                 int newPropertyType,
                 std::size_t adaptedPropertyPos,
                 AttributeConverter conv = GenericAttributeConverter);

        void add(const std::string& newPropertyName,
                 int newPropertyType,
                 const std::vector<std::string>& adaptedPropertyNames,
                 AttributeConverter conv);

        void add(const std::string& newPropertyName,
                 int newPropertyType,
                 const std::vector<std::size_t>& adaptedPropertyPos,
                 AttributeConverter conv);

        //@}

      private:

        /** @name DataSet Adapter Internal Methods
         *  DataSet Adapter Internal Methods implementation.
         */
        //@{

        bool isAdapted(std::size_t i) const;

        te::dt::AbstractData* getAdaptedValue(std::size_t i) const;

        //@}

      private:

        std::vector<int> m_datatypes;                             //!< The datatype for each property.
        std::vector<std::string> m_pnames;                        //!< The name for each property.
        te::common::Holder<DataSet> m_ds;                         //!< A pointer to the DataSet that will be handled by adapter
        std::vector<std::vector<std::size_t> > m_propertyIndexes; //!< A vector that stores the adapted property indexes.
        std::vector<AttributeConverter> m_converters;             //!< A vector that stores the attribute converters functions.
        std::vector<std::size_t> m_adaptedProperties;             //!< Internal vector to count the references to adapted properties.
    };

  } // end namespace da
}   // end namespace te

#endif  // __TERRALIB_DATAACCESS_INTERNAL_DATASETADAPTER_H
