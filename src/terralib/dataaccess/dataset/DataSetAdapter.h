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

        /*! \brief Destructor. */
        ~DataSetAdapter();

        //@}

        te::common::TraverseType getTraverseType() const;

        te::common::AccessPolicy getAccessPolicy() const;

        std::auto_ptr<te::gm::Envelope> getExtent(std::size_t i);

        std::size_t getNumProperties() const;

        int getPropertyDataType(std::size_t pos) const;

        std::string getPropertyName(std::size_t pos) const;

        te::common::CharEncoding getPropertyCharEncoding(std::size_t i) const;

        std::string getDatasetNameOfProperty(std::size_t pos) const;

        bool isEmpty() const;

        bool isConnected() const;

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

        std::auto_ptr<te::dt::ByteArray> getByteArray(std::size_t i) const;

        std::auto_ptr<te::gm::Geometry> getGeometry(std::size_t i) const;

        std::auto_ptr<te::rst::Raster> getRaster(std::size_t i) const;

        std::auto_ptr<te::dt::DateTime> getDateTime(std::size_t i) const;

        std::auto_ptr<te::dt::Array> getArray(std::size_t i) const;

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

        te::dt::AbstractData* getAdaptedValue(std::size_t i) const;

        //@}

      private:

        std::vector<int> m_datatypes;                             //!< The datatype for each property.
        std::vector<std::string> m_pnames;                        //!< The name for each property.
        te::common::Holder<DataSet> m_ds;                         //!< A pointer to the DataSet that will be handled by adapter
        std::vector<std::vector<std::size_t> > m_propertyIndexes; //!< A vector that stores the adapted property indexes.
        std::vector<AttributeConverter> m_converters;             //!< A vector that stores the attribute converters functions.
    };

  } // end namespace da
}   // end namespace te

#endif  // __TERRALIB_DATAACCESS_INTERNAL_DATASETADAPTER_H
