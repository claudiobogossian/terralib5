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
  \file terralib/dataaccess/dataset/FilteredDataSet.h

  \brief This class represents a filtered data set.
*/

#ifndef __TERRALIB_DATAACCESS_INTERNAL_FILTEREDDATASET_H
#define __TERRALIB_DATAACCESS_INTERNAL_FILTEREDDATASET_H

// TerraLib
#include "../../common/Holder.h"
#include "../Config.h"
#include "DataSet.h"
#include "DataSetCapabilities.h"

// STL
#include <vector>

namespace te
{
  namespace da
  {
    /*!
      \class FilteredDataSet

      \brief This class represents a filtered data set.

      \sa DataSet
     */
    class TEDATAACCESSEXPORT FilteredDataSet : public DataSet
    {
      public:

        /** @name Constructor/Destructor
         *  Initilizer methods.
         */
        //@{

        /*!
          \brief Constructor.

          \param dataset    A pointer to the DataSet that will be filtered.
          \param positions  The data set positions that actually will be accessed.
          \param isOwner    If true the FilteredDataSet will have the ownership of the given data set pointer.
        */
        FilteredDataSet(DataSet* dataset, const std::vector<std::size_t>& positions, bool isOwner = false);

        /*!
          \brief Constructor.

          \param dataset      A pointer to the DataSet that will be filtered.
          \param capabilities The capabilities of the DataSet that will be filtered.
          \param positions    The data set positions that actually will be accessed.
          \param isOwner      If true the FilteredDataSet will have the ownership of the given data set pointer.
        */
        FilteredDataSet(DataSet* dataset, const DataSetCapabilities& capabilities,
                        const std::vector<std::size_t>& positions, bool isOwner = false);

        /*! \brief Destructor. */
        ~FilteredDataSet();

        //@}

        te::common::TraverseType getTraverseType() const;

        te::common::AccessPolicy getAccessPolicy() const;

        std::auto_ptr<te::gm::Envelope> getExtent(std::size_t i);

        std::size_t getNumProperties() const;

        int getPropertyDataType(std::size_t i) const;

        std::string getPropertyName(std::size_t i) const;

        te::common::CharEncoding getPropertyCharEncoding(std::size_t i) const;

        std::string getDatasetNameOfProperty(std::size_t i) const;

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

      private:

        te::common::Holder<DataSet> m_ds;     //!< A pointer to the DataSet that will be filtered.
        DataSetCapabilities m_capabilites;    //<! The capabilities of the DataSet that will be filtered.
        std::vector<std::size_t> m_positions; //!< The data set positions that actually will be accessed.
        int m_i;                              //<! The current position.
    };

  } // end namespace da
}   // end namespace te

#endif  // __TERRALIB_DATAACCESS_INTERNAL_FILTEREDDATASET_H
