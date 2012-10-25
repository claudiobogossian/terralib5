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
        
        void getArray(int i, std::vector<boost::int16_t>& values) const;

        const unsigned char* getWKB(int i) const;

        te::da::DataSet* getDataSet(int i);

        void setDataSet(int i, const te::da::DataSet& value);
        
        void setValue(int i, te::dt::AbstractData* value);

        bool isNull(int i) const;

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

        te::gm::Geometry* m_geomFilter;     //!< The geometric filter.
        te::gm::Envelope* m_mbrFilter;      //!< The MBR filter.
        std::string* m_originalsql;         //!< The original SQL.
        sqlite3_stmt* m_stmt;               //!< A pointer to the internal prepared statement.
        DataSourceTransactor* m_t;          //!< A pointer to the associated transactor.
        mutable te::da::DataSetType* m_dt;  //!< DataSetType.
        bool m_releaseStmt;                 //!< If true it indicates that the statement must be released by dataset's destructor.
    };

  } // end namespace sqlite
}   // end namespace te


#endif  // __TERRALIB_SQLITE_INTERNAL_FWDATASET_H

