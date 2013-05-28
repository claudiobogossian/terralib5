/*  Copyright (C) 2011-2011 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/sqlite/FwDataSet.h

  \brief Implementation of a forward-only dataset for the TerraLib SQLite Data Access driver.
*/

#ifndef __TERRALIB_SQLITE_INTERNAL_FWDATASET_H
#define __TERRALIB_SQLITE_INTERNAL_FWDATASET_H

// TerraLib
#include "../dataaccess/dataset/DataSet.h"
#include "Config.h"

// Forward declarations
extern "C"
{
  struct sqlite3_stmt;
  typedef struct sqlite3_stmt sqlite3_stmt;
}

namespace te
{
  namespace sqlite
  {
// Forward declaration
    class DataSourceTransactor;

    /*!
      \class DataSet

      \brief Implementation of a forward-only dataset for the TerraLib SQLite Data Access driver.

      \sa te::da::DataSet
    */
    class TESQLITEEXPORT FwDataSet : public te::da::DataSet
    {
      public:

        /*!
          \brief Constructor.

          \param stmt  The underlying prepared statement of this dataset. The dataset will take its ownership.
          \param t     The transactor associated to this DataSet.
        */
        FwDataSet(sqlite3_stmt* stmt, DataSourceTransactor* t, const bool releaseStmt = true);

        /*! \brief Destructor. */
        ~FwDataSet();

        te::common::TraverseType getTraverseType() const;

        te::common::AccessPolicy getAccessPolicy() const;

        te::gm::Envelope* getExtent(std::size_t i);

        std::size_t getNumProperties() const;

        int getPropertyDataType(std::size_t i) const;

        std::string getPropertyName(std::size_t i) const;

        std::string getDatasetNameOfProperty(std::size_t i) const;

        bool isEmpty() const;

        std::size_t size() const;

        bool moveNext();

        bool movePrevious();
        
        bool moveFirst();

        bool moveBeforeFirst();

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

        /*!
          \brief Returns the blob size in bytes.

          \param i    The column where the blob is.

          \return The blob size in bytes.

          \note SQLite driver extended method.
        */
        int getBlobSize(int i) const;

        /*!
          \brief Retrieves the data stored in a blob column.

          \param i    The column where the blob is.
          \param buff A buffer having at least the blob size.

          \note SQLite driver extended method.

          \todo We need a general method with byte offset and number of byte sto be copied!
        */
        void getBlob(int i, void* buff) const;

      private:

        std::string* m_originalsql;         //!< The original SQL.
        sqlite3_stmt* m_stmt;               //!< A pointer to the internal prepared statement.
        DataSourceTransactor* m_t;          //!< A pointer to the associated transactor.
        mutable te::da::DataSetType* m_dt;  //!< DataSetType.
        bool m_releaseStmt;                 //!< If true it indicates that the statement must be released by dataset's destructor.
    };

  } // end namespace sqlite
}   // end namespace te


#endif  // __TERRALIB_SQLITE_INTERNAL_FWDATASET_H


