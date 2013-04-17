/*  Copyright (C) 2011-2012 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/mysql/DataSet.h

  \brief Implementation of the DataSet class to MySQL data access driver.
*/

#ifndef __TERRALIB_MYSQL_INTERNAL_DATASET_H
#define __TERRALIB_MYSQL_INTERNAL_DATASET_H

// TerraLib
#include "../dataaccess/dataset/DataSet.h"
#include "Config.h"

// Forward declaration
namespace sql { class ResultSet; }

namespace te
{
// Forward declaration
  namespace da
  {
    class DataSetType;
  }

  namespace mysql
  {
// Forward declaration
    class DataSourceTransactor;

    /*!
      \class DataSet

      \brief Implementation of the DataSet class to MySQL data access driver.

      \sa te::da::DataSet
    */
    class TEMYSQLEXPORT DataSet : public te::da::DataSet
    {
      public:

        DataSet(DataSourceTransactor* t, sql::ResultSet* result, std::string* sql);

        ~DataSet();

        te::common::TraverseType getTraverseType() const;

        te::common::AccessPolicy getAccessPolicy() const;
        
        te::da::DataSourceTransactor* getTransactor() const;

        te::gm::Envelope* getExtent(std::size_t i);

        std::size_t getNumProperties() const;

        int getPropertyDataType(std::size_t pos) const;

        std::string getPropertyName(std::size_t pos) const;

        std::string getDatasetNameOfProperty(std::size_t pos) const;

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

      private:

        DataSourceTransactor* m_transactor;   //!< The associated transactor.
        sql::ResultSet* m_result;             //!< The interanl resultset.
        mutable te::da::DataSetType* m_dt;    //!< The dataset schema.
        std::string* m_originalsql;           //!< The original SQL.
    };

  } // end namespace mysql
}   // end namespace te


#endif  // __TERRALIB_MYSQL_INTERNAL_DATASET_H


