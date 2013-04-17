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
  \file terralib/ado/DataSet.h

  \brief Implementation of the DataSet class to ADO.
*/

#ifndef __TERRALIB_ADO_INTERNAL_DATASET_H
#define __TERRALIB_ADO_INTERNAL_DATASET_H

// TerraLib
#include "../dataaccess/dataset/DataSet.h"
#include "Config.h"

// ADO
#import "msado15.dll" \
  no_namespace rename("EOF", "EndOfFile")
#import "msadox.dll"

namespace te
{
// Forward declaration
  namespace da
  {
    class DataSetType;
  }

  namespace ado
  {
// Forward declaration
    class DataSourceTransactor;

    /*!
      \class DataSet

      \brief Implementation of the DataSet class to ADO.

      \sa te::da::DataSet
    */
    class TEADOEXPORT DataSet : public te::da::DataSet
    {
      public:

        /*!
          \brief Constructor.

          \param dt         The Data set type
          \param result     The internal _RecordsetPtr.
          \param transactor The transactor associated to this DataSet.
        */
        DataSet(te::da::DataSetType* dt, _RecordsetPtr result, te::da::DataSourceTransactor* transactor);

        /*! \brief Destructor. */
        ~DataSet();

        te::common::TraverseType getTraverseType() const;

        te::common::AccessPolicy getAccessPolicy() const;

        te::da::DataSourceTransactor* getTransactor() const;

        te::da::DataSet* getParent() const;

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
        char getChar(const std::string& name) const;

        unsigned char getUChar(std::size_t i) const;
        unsigned char getUChar(const std::string& name) const;

        boost::int16_t getInt16(std::size_t i) const;
        boost::int16_t getInt16(const std::string& name) const;

        boost::int32_t getInt32(std::size_t i) const;
        boost::int32_t getInt32(const std::string& name) const;

        boost::int64_t getInt64(std::size_t i) const;
        boost::int64_t getInt64(const std::string& name) const;

        bool getBool(std::size_t i) const;
        bool getBool(const std::string& name) const;

        float getFloat(std::size_t i) const;
        float getFloat(const std::string& name) const;

        double getDouble(std::size_t i) const;
        double getDouble(const std::string& name) const;

        std::string getNumeric(std::size_t i) const;
        std::string getNumeric(const std::string& name) const;

        std::string getString(std::size_t i) const;
        std::string getString(const std::string& name) const;

        te::dt::ByteArray* getByteArray(std::size_t i) const;
        te::dt::ByteArray* getByteArray(const std::string& name) const;

        te::gm::Geometry* getGeometry(std::size_t i) const;
        te::gm::Geometry* getGeometry(const std::string& name) const;

        te::rst::Raster* getRaster(std::size_t i) const;
        te::rst::Raster* getRaster(const std::string& name) const;

        te::dt::DateTime* getDateTime(std::size_t i) const;
        te::dt::DateTime* getDateTime(const std::string& name) const;
        
        te::dt::Array* getArray(std::size_t i) const;
        te::dt::Array* getArray(const std::string& name) const;

        bool isNull(std::size_t i) const;

      protected:

        int m_i;                            //!< The index of the current row.
        long m_size;                        //!< The number of datasets in the collection.
        long m_ncols;                       //!< The number of columns.
        _RecordsetPtr m_result;             //!< The internal buffer with the result query.
        mutable te::da::DataSetType* m_dt;  //!< DataSetType.
        te::da::DataSourceTransactor* m_t;  //!< The ADO transactor associated to this dataset.
        std::string* m_name;                //!< The dataset name if one exists.
        te::gm::Geometry* m_geomFilter;     //!< The geometric filter.
        te::gm::Envelope* m_mbrFilter;      //!< The MBR filter.
        te::gm::SpatialRelation m_relation; //!< The spatial relation used by installed filters.
    };

  } // end namespace ado
}   // end namespace te


#endif  // __TERRALIB_ADO_INTERNAL_DATASET_H


