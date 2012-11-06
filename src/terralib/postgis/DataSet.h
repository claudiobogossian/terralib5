/*  Copyright (C) 2008-2011 National Institute For Space Research (INPE) - Brazil.

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

  \brief Implementation of a dataset for PostGIS driver.
*/

#ifndef __TERRALIB_POSTGIS_INTERNAL_DATASET_H
#define __TERRALIB_POSTGIS_INTERNAL_DATASET_H

// TerraLib
#include "../dataaccess/dataset/DataSet.h"
#include "Config.h"

// Boost
#include <boost/noncopyable.hpp>

// Forward declaration for libpq
extern "C"
{
  struct pg_conn;
  typedef struct pg_conn PGconn;
  struct pg_result;
  typedef struct pg_result PGresult;
}

namespace te
{
// Forward declarations
  namespace da
  {
    class GeometryProperty;
  }

  namespace pgis
  {
// Forward declarations
    class Transactor;

    /*!
      \class DataSet

      \brief Implementation of a dataset for PostGIS driver.

      \sa Transactor, te::da::DataSet
    */
    class TEPGISEXPORT DataSet : public te::da::DataSet, public boost::noncopyable
    {
      public:

        /*!
          \brief Constructor.

          \param result     The internal PGresult.
          \param transactor The transactor associated to this DataSet.
          \param sql        The sql command that generated the dataset.
        */
        DataSet(PGresult* result,
                Transactor* transactor,
                std::string* sql);

        /*!
          \brief Constructor.

          \param result     The internal PGresult.
          \param transactor The transactor associated to this DataSet.
          \param sql        The sql command that generated the dataset.
          \param name       The dataset name.
        */
        DataSet(PGresult* result,
                Transactor* transactor,
                std::string* sql,
                std::string* name);

        

        /*! \brief The destructor will clear the internal PGresult. */
        ~DataSet();

        te::common::TraverseType getTraverseType() const { return te::common::FORWARDONLY; }

        te::common::AccessPolicy getAccessPolicy() const { return te::common::RAccess; }

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

        unsigned char getUChar(int i) const;

        unsigned char getUChar(const std::string& name) const;

        void setUChar(int i, unsigned char value);

        boost::int16_t getInt16(int i) const;

        boost::int16_t getInt16(const std::string& name) const;

        void setInt16(int i, boost::int16_t value);

        boost::int32_t getInt32(int i) const;

        boost::int32_t getInt32(const std::string& name) const;

        void setInt32(int i, boost::int32_t value);

        boost::int64_t getInt64(int i) const;

        boost::int64_t getInt64(const std::string& name) const;

        void setInt64(int i, boost::int64_t value);

        bool getBool(int i) const;

        bool getBool(const std::string& name) const;

        void setBool(int i, bool value);

        float getFloat(int i) const;

        float getFloat(const std::string& name) const;

        void setFloat(int i, float value);

        double getDouble(int i) const;

        double getDouble(const std::string& name) const;

        void setDouble(int i, double value);

        std::string getNumeric(int i) const;

        std::string getNumeric(const std::string& name) const;

        void setNumeric(int i, const std::string& value);

        std::string getString(int i) const;

        std::string getString(const std::string& name) const;

        void setString(int i, const std::string& value);

        te::dt::ByteArray* getByteArray(int i) const;

        te::dt::ByteArray* getByteArray(const std::string& name) const; 

        void setByteArray(int i, const te::dt::ByteArray& value);

        te::gm::Geometry* getGeometry(int i) const;

        te::gm::Geometry* getGeometry(const std::string& name) const;

        void setGeometry(int i, const te::gm::Geometry& value);

        te::rst::Raster* getRaster(int i) const;

        te::rst::Raster* getRaster(const std::string& name) const;

        void setRaster(int i, const te::rst::Raster& value);

        te::dt::DateTime* getDateTime(int i) const; 

        te::dt::DateTime* getDateTime(const std::string& name) const;

        void setDateTime(int i, const te::dt::DateTime& value); 

        void getArray(int i, std::vector<boost::int16_t>& values) const;

        void getArray(const std::string& name, std::vector<boost::int16_t>& values) const;

        te::dt::Array* getArray(int i) const;

        te::dt::Array* getArray(const std::string& name) const;

        const unsigned char* getWKB(int i) const;

        const unsigned char* getWKB(const std::string& name) const;

        te::da::DataSet* getDataSet(int i);

        te::da::DataSet* getDataSet(const std::string& name);

        void setDataSet(int i, const te::da::DataSet& value);

        void setValue(int i, te::dt::AbstractData* value);

        bool isNull(int i) const;

        bool isNull(const std::string& name) const;

        //@}

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

        void getFilterSQL(const te::gm::GeometryProperty* gp, const te::gm::Geometry* g, te::gm::SpatialRelation r, std::string& sql);

        void getFilterSQL(const te::gm::GeometryProperty* gp, const te::gm::Envelope* e, te::gm::SpatialRelation r, std::string& sql);

      protected:

        int m_i;                            //!< The index of the current row.
        int m_size;                         //!< The number of datasets in the collection.
        int m_ncols;                        //!< The number of columns.
        PGresult* m_result;                 //!< The internal buffer with the result query.
        mutable te::da::DataSetType* m_dt;  //!< DataSetType.
        Transactor* m_t;                    //!< The PostGIS transactor associated to this dataset.
        std::string* m_name;                //!< The dataset name if one exists.
        std::string* m_sql;                 //!< The sql command that generated the dataset.
        te::gm::Geometry* m_geomFilter;     //!< The geometric filter.
        te::gm::Envelope* m_mbrFilter;      //!< The MBR filter.
        te::gm::SpatialRelation m_relation; //!< The spatial relation used by installed filters.

        friend class DataSetItem;
    };

  } // end namespace pgis
}   // end namespace te


#endif  // __TERRALIB_POSTGIS_INTERNAL_DATASET_H



