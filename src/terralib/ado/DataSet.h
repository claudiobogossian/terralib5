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
  namespace ado
  {

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
        char getChar(const std::string& name) const;

        void setChar(int i, char value);
        void setChar(const std::string& name, char value);

        unsigned char getUChar(int i) const;
        unsigned char getUChar(const std::string& name) const;

        void setUChar(int i, unsigned char value);
        void setUChar(const std::string& name, unsigned char value);

        boost::int16_t getInt16(int i) const;
        boost::int16_t getInt16(const std::string& name) const;

        void setInt16(int i, boost::int16_t value);
        void setInt16(const std::string& name, boost::int16_t value);

        boost::int32_t getInt32(int i) const;
        boost::int32_t getInt32(const std::string& name) const;

        void setInt32(int i, boost::int32_t value);
        void setInt32(const std::string& name, boost::int32_t value);

        boost::int64_t getInt64(int i) const;
        boost::int64_t getInt64(const std::string& name) const;

        void setInt64(int i, boost::int64_t value);
        void setInt64(const std::string& name, boost::int64_t value);

        bool getBool(int i) const;
        bool getBool(const std::string& name) const;

        void setBool(int i, bool value);
        void setBool(const std::string& name, bool value);

        float getFloat(int i) const;
        float getFloat(const std::string& name) const;

        void setFloat(int i, float value);
        void setFloat(const std::string& name, float value);

        double getDouble(int i) const;
        double getDouble(const std::string& name) const;

        void setDouble(int i, double value);
        void setDouble(const std::string& name, double value);

        std::string getNumeric(int i) const;
        std::string getNumeric(const std::string& name) const;

        void setNumeric(int i, const std::string& value);
        void setNumeric(const std::string& name, const std::string& value);

        std::string getString(int i) const;
        std::string getString(const std::string& name) const;

        void setString(int i, const std::string& value);
        void setString(const std::string& name, const std::string& value);

        te::dt::ByteArray* getByteArray(int i) const;
        te::dt::ByteArray* getByteArray(const std::string& name) const;

        void setByteArray(int i, const te::dt::ByteArray& value);
        void setByteArray(const std::string& name, const te::dt::ByteArray& value);

        te::gm::Geometry* getGeometry(int i) const;
        te::gm::Geometry* getGeometry(const std::string& name) const;

        void setGeometry(int i, const te::gm::Geometry& value);
        void setGeometry(const std::string& name, const te::gm::Geometry& value);

        te::rst::Raster* getRaster(int i) const;
        te::rst::Raster* getRaster(const std::string& name) const;

        void setRaster(int i, const te::rst::Raster& value);
        void setRaster(const std::string& name, const te::rst::Raster& value);

        te::dt::DateTime* getDateTime(int i) const;
        te::dt::DateTime* getDateTime(const std::string& name) const;

        void setDateTime(int i, const te::dt::DateTime& value);
        void setDateTime(const std::string& name, const te::dt::DateTime& value);
        
        void getArray(int i, std::vector<boost::int16_t>& a) const;
        void getArray(const std::string& name, std::vector<boost::int16_t>& a) const;

        const unsigned char* getWKB(int i) const;
        const unsigned char* getWKB(const std::string& name) const;

        te::da::DataSet* getDataSet(int i);

        void setDataSet(int i, const te::da::DataSet& value);
        
        void setValue(int i, te::dt::AbstractData* ad);

        bool isNull(int i) const;

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


