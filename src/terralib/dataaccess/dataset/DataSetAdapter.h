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
// Forward declarations
  namespace dt
  {
    class Property;
  }

  namespace da
  {
// Forward declarations
    class DataSourceCapabilities;

    /*!
      \class DataSetAdapter

      \brief An adapter for DataSet.

      \sa DataSet
     */
    class TEDATAACCESSEXPORT DataSetAdapter : public DataSet, public te::common::Holder<DataSet>
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

        te::da::DataSetType* getType();

        const te::da::DataSetType* getType() const;

        te::da::DataSourceTransactor* getTransactor() const;

        void loadTypeInfo();

        te::da::DataSet* getParent() const;

        te::gm::Envelope* getExtent(const te::dt::Property* p);

        void setFilter(te::dt::Property* p, const te::gm::Geometry* g, te::gm::SpatialRelation r = te::gm::INTERSECTS);

        void setFilter(te::dt::Property* p, const te::gm::Envelope* e, te::gm::SpatialRelation r = te::gm::INTERSECTS);
        
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

        void setChar(int i, char value);

        unsigned char getUChar(int i) const;

        void setUChar(int i, unsigned char value);

        boost::int16_t getInt16(int i) const;

        void setInt16(int i, boost::int16_t value);

        boost::int32_t getInt32(int i) const;

        void setInt32(int i, boost::int32_t value);

        boost::int64_t getInt64(int i) const;

        void setInt64(int i, boost::int64_t value);

        bool getBool(int i) const;

        void setBool(int i, bool value);

        float getFloat(int i) const;

        void setFloat(int i, float value);

        double getDouble(int i) const;

        void setDouble(int i, double value);

        std::string getNumeric(int i) const;

        void setNumeric(int i, const std::string& value);

        std::string getString(int i) const;

        void setString(int i, const std::string& value);

        te::dt::ByteArray* getByteArray(int i) const;

        void setByteArray(int i, const te::dt::ByteArray& value);

        te::gm::Geometry* getGeometry(int i) const;

        void setGeometry(int i, const te::gm::Geometry& value);

        te::rst::Raster* getRaster(int i) const;

        void setRaster(int i, const te::rst::Raster& value);

        te::dt::DateTime* getDateTime(int i) const;

        void setDateTime(int i, const te::dt::DateTime& value);

        void getArray(int i, std::vector<boost::int16_t>& a) const;

        const unsigned char* getWKB(int i) const;

        te::da::DataSet* getDataSet(int i);

        void setDataSet(int i, const te::da::DataSet& value);
        
        void setValue(int i, te::dt::AbstractData* ad);

        bool isNull(int i) const;

        /** @name DataSet Adapter Extended Methods
         *  Methods that exists only in the DataSet Adapter implementation.
         */
        //@{

        /*!
          \brief This method returns the name of the properties that have not yet been adapted.

          \param properties A vector that will be filled with the name of the properties that have not yet been adapted.
        */
        void getNonAdaptedProperties(std::vector<std::string>& properties);

        /*!
          \brief This method returns the index of the properties that have not yet been adapted.

          \param properties A vector that will be filled with the index of the properties that have not yet been adapted.
        */
        void getNonAdaptedProperties(std::vector<int>& properties);

        /*!
          \brief This method adapts a given property of the handled data set to another property.

          \param propertyName The name of the property that will be adapted.
          \param p The adapter property.
        */
        void adapt(const std::string& propertyName, te::dt::Property* p);

        /*!
          \brief This method adapts a given property of the handled data set to another property.

          \param i The index of the property that will be adapted.
          \param p The adapter property.
        */
        void adapt(int i, te::dt::Property* p);

        /*!
          \brief This method adapts a given property of the handled data set to another property.

          \param propertyName The name of the property that will be adapted.
          \param p The adapter property.
          \param conv The function that will be used to do the attribute values conversion.
        */
        void adapt(const std::string& propertyName, te::dt::Property* p, AttributeConverter conv);

        /*!
          \brief This method adapts a given property of the handled data set to another property.

          \param i The index of the property that will be adapted.
          \param p The adapter property.
          \param conv The function that will be used to do the attribute values conversion.
        */
        void adapt(int i, te::dt::Property* p, AttributeConverter conv);

        /*!
          \brief This method adapts the given properties of the handled data set to another property.

          \param propertyName The name of the property that will be adapted.
          \param p The adapter property.
          \param conv The function that will be used to do the attribute values conversion.
        */
        void adapt(const std::vector<std::string>& propertyNames, te::dt::Property* p, AttributeConverter conv);

        /*!
          \brief This method adapts the given properties of the handled data set to another property.

          \param propertyName The name of the property that will be adapted.
          \param p The adapter property.
          \param conv The function that will be used to do the attribute values conversion.
        */
        void adapt(const std::vector<int>& propertyIndexes, te::dt::Property* p, AttributeConverter conv);

        /*!
          \brief This method tells which properties of the handled data set that have been adapted based on the given property.

          \param p A property of the DataSetAdapter.

          \return The properties of the handled data set have been adapted.

          \note The caller will NOT take the ownership of the returned pointers.
        */
        std::vector<const te::dt::Property*> getAdaptedProperties(te::dt::Property* p);

        /*!
          \brief This method tells which properties of the handled data set that have been adapted based on the given property name.

          \param p A property name of the DataSetAdapter.

          \return The properties of the handled data set have been adapted.

          \note The caller will NOT take the ownership of the returned pointers.
        */
        std::vector<const te::dt::Property*> getAdaptedProperties(const std::string& name);

        /*!
          \brief This method tells which properties of the handled data set that have been adapted based on the given property index.

          \param p A property index of the DataSetAdapter.

          \return The properties of the handled data set have been adapted.

          \note The caller will NOT take the ownership of the returned pointers.
        */
        std::vector<const te::dt::Property*> getAdaptedProperties(int i);

        /*!
          \brief This method returns the pointer to the DataSet that is handled by adapter.

          \return The pointer to the DataSet that is handled by adapter.

          \note The caller will NOT take the ownership of the returned pointer.
        */
        te::da::DataSet* getInputDataSet() const;

        /*!
          \brief This method removes a property of DataSetAdapter.

          \param propertyName The property name of the DataSetAdapter that will be removed.
        */
        void remove(const std::string& propertyName);

        /*!
          \brief This method removes a property of DataSetAdapter.

          \param i The property index of the DataSetAdapter that will be removed.
        */
        void remove(int i);

        /*!
          \brief Static method that creates an adapter to the given data set.

          \param dataset The data set that will be adapted.
          \param isOwner If true the DataSetAdapter will have the ownership of the given data set pointer.

          \return Return an adapter to the given dataset.
        */
        static DataSetAdapter* adapt(DataSet* dataset, bool isOwner = false);

        /*!
          \brief Static method that creates an adapter to the given data set based on the given data source capabilities.

          \param dataset The data set that will be adapted.
          \param capabilities The data source capabilities that will be used.
          \param isOwner If true the DataSetAdapter will have the ownership of the given data set pointer.

          \return Return an adapter to the given dataset.
        */
        static DataSetAdapter* adapt(DataSet* dataset, const DataSourceCapabilities& capabilities, bool isOwner = false);

        /*!
          \brief Static method that verifies if the given data set need an adapter based on given data source capabilities.

          \param dataset The data set that will be verified.
          \param capabilities The data source capabilities that will be used.

          \return Return true if the given data set need an adapter. Otherwise, return false.
        */
        static bool needAdapter(DataSet* dataset, const DataSourceCapabilities& capabilities);

        //@}

      private:

        /** @name DataSet Adapter Internal Methods
         *  DataSet Adapter Internal Methods implementation.
         */
        //@{

        bool isAdapted(int i) const;

        te::dt::AbstractData* getAdaptedValue(int i) const;

        void setAdaptedValue(int i, te::dt::AbstractData* data);

        //@}

      private:

        DataSet* m_ds;                                       //!< A pointer to the DataSet that will be handled by adapter
        const DataSetType* m_inDataSetType;                  //!< A pointer to DataSetType of DataSet that will be handled by adapter.
        DataSetType* m_outDataSetType;                       //!< Adapter DataSetType.
        std::vector<std::vector<int> > m_propertyIndexes;    //!< A vector that stores the adapted property indexes.
        std::vector<AttributeConverter> m_converters;        //!< A vector that stores the attribute converters functions.
        std::vector<int> m_adaptedProperties;                //!< Internal vector to count the references to adapted properties.
    };

  } // end namespace da
}   // end namespace te

#endif  // __TERRALIB_DATAACCESS_INTERNAL_DATASETADAPTER_H
