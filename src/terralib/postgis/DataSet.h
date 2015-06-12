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
  \file terralib/postgis/DataSet.h

  \brief Implementation of a dataset for the PostGIS driver.
*/

#ifndef __TERRALIB_POSTGIS_INTERNAL_DATASET_H
#define __TERRALIB_POSTGIS_INTERNAL_DATASET_H

// TerraLib
#include "../dataaccess/dataset/DataSet.h"
#include "Config.h"

// Forward declaration for libpq
//extern "C"
//{
//  struct pg_conn;
//  typedef struct pg_conn PGconn;
//  struct pg_result;
//  typedef struct pg_result PGresult;
//}

namespace te
{
// Forward declarations
  namespace da
  {
    class DataSource;
    class GeometryProperty;
  }

  namespace pgis
  {
    /*!
      \class DataSet

      \brief Implementation of a dataset for the PostGIS driver.

      \sa te::da::DataSet
    */
    class TEPGISEXPORT DataSet : public te::da::DataSet
    {
      public:

        /*!
          \brief Constructor.

          \param result        The internal PGresult.
          \param ds            The data source.
          \param timeIsInteger A flag that indicates if the postgis stores, internally, the time and timestamp as an integer.
          \param ce            The connection character encoding.
        */
        DataSet(PGresult* result,
                const std::vector<int>& ptypes,
                bool timeIsInteger = true,
                const te::common::CharEncoding& ce = te::common::UNKNOWN_CHAR_ENCODING);

        /*! \brief The destructor will clear the internal PGresult. */
        ~DataSet();

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

        /** @name PostGIS Extended Methods
         *  Methods that exists only in the PostGIS DataSet implementation.
         */
        //@{

        /*!
          \brief It returns the internal pg result.

          \return The internal pg result.
        */
        PGresult* getPGResult() const { return m_result; }

        //@}

      protected:

        int m_i;                            //!< The index of the current row.
        int m_size;                         //!< The number of datasets in the collection.
        PGresult* m_result;                 //!< The internal buffer with the result query.
        std::vector<int> m_ptypes;          //!< The list of property types.
        te::gm::Envelope* m_mbr;            //!< The dataset extent.
        bool m_timeIsInteger;               //!< It indicates if the postgis stores, internally, the time and timestamp as an integer. 
        te::common::CharEncoding m_ce;      //!< The connection character encoding.
    };

  } // end namespace pgis
}   // end namespace te


#endif  // __TERRALIB_POSTGIS_INTERNAL_DATASET_H
