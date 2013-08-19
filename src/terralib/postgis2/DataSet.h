/*  Copyright (C) 2008-2013 National Institute For Space Research (INPE) - Brazil.

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
#include "../dataaccess2/dataset/DataSet.h"
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

          \param result       The internal PGresult.
          \param ds           The data source.
          \param sql          The sql command that generated the dataset.
        */
        DataSet(PGresult* result,
                te::da::DataSource* ds,
                std::string* sql);

        /*! \brief The destructor will clear the internal PGresult. */
        ~DataSet();

        te::common::TraverseType getTraverseType() const;

        te::common::AccessPolicy getAccessPolicy() const;

        std::size_t getNumProperties() const;

        int getPropertyDataType(std::size_t i) const;

        std::string getPropertyName(std::size_t i) const;

        std::string getDatasetNameOfProperty(std::size_t i) const;

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
          \brief It returns the internal SQL used to generate the dataset.

          \return The internal SQL used to generate the dataset.
        */
        std::string* getSQL() const { return m_sql; }

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
        te::da::DataSource* m_ds;           //!< The data source.
        std::string* m_sql;                 //!< The sql command that generated the dataset.
        std::vector<int> m_ptypes;          //!< The list of property types.
    };

  } // end namespace pgis
}   // end namespace te


#endif  // __TERRALIB_POSTGIS_INTERNAL_DATASET_H



