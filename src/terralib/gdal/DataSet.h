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
  \file terralib/gdal/DataSet.h

  \brief Implementation of a DataSet for GDAL data provider.
*/

#ifndef __TERRALIB_GDAL_INTERNAL_DATASET_H
#define __TERRALIB_GDAL_INTERNAL_DATASET_H

// TerraLib
#include "../dataaccess/dataset/DataSet.h"
#include "Config.h"

namespace te
{
  namespace gdal
  {
// Forward declaration
    class DataSourceTransactor;

    /*!
      \class DataSet

      \brief A GDAL data set gives access to a raster file.
    */
    class TEGDALEXPORT DataSet : public te::da::DataSet
    {
      public:

        /*!
          \brief Constructor.

          \param t      The data source transactor associated to the dataset.
          \param dt     The dataset type. DataSet will take its ownership.
          \param rwRole The access policy of the dataset.
        */
        DataSet(DataSourceTransactor* t, te::da::DataSetType* dt, te::common::AccessPolicy rwRole = te::common::RAccess);

        /*! \brief Destructor. */
        ~DataSet() {}

        te::common::TraverseType getTraverseType() const { return te::common::FORWARDONLY; }

        te::common::AccessPolicy getAccessPolicy() const { return m_rwRole; }

        te::da::DataSetType* getType() { return m_dsType; }

        const te::da::DataSetType* getType() const { return m_dsType; }

        te::da::DataSourceTransactor* getTransactor() const { return 0; }
        
        void loadTypeInfo();

        te::da::DataSet* getParent() const { return 0; }

        te::gm::Envelope* getExtent(const te::dt::Property* p);

        void setFilter(te::dt::Property* /*p*/, const te::gm::Geometry* /*g*/, te::gm::SpatialRelation /*r = te::gm::INTERSECTS*/) {}
        void setFilter(te::dt::Property* /*p*/, const te::gm::Envelope* /*e*/, te::gm::SpatialRelation /*r = te::gm::INTERSECTS*/) {}

        te::da::DataSetItem* getItem() const { return 0; }
        void add(te::da::DataSetItem* /*item*/) {}

        bool isEmpty() const { return false; }
        std::size_t size() const { return 1; }

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

        char getChar(int /*i*/) const { return '\0'; }
        char getChar(const std::string& /*name*/) const { return '\0'; }
        void setChar(int /*i*/, char /*value*/) {}
        void setChar(const std::string& /*name*/, char /*value*/){}

        unsigned char getUChar(int /*i*/) const { return 0; }
        unsigned char getUChar(const std::string& /*name*/) const { return 0; }
        void setUChar(int /*i*/, unsigned char /*value*/){}
        void setUChar(const std::string& /*name*/, unsigned char /*value*/){}

        boost::int16_t getInt16(int /*i*/) const { return 0; }
        boost::int16_t getInt16(const std::string& /*name*/) const { return 0; }
        void setInt16(int /*i*/, boost::int16_t /*value*/) {}
        void setInt16(const std::string& /*name*/, boost::int16_t /*value*/){}

        boost::int32_t getInt32(int /*i*/) const { return 0; }
        boost::int32_t getInt32(const std::string& /*name*/) const { return 0; }
        void setInt32(int /*i*/, boost::int32_t /*value*/) {}
        void setInt32(const std::string& /*name*/, boost::int32_t /*value*/) {}

        boost::int64_t getInt64(int /*i*/) const { return 0; }
        boost::int64_t getInt64(const std::string& /*name*/) const { return 0; }
        void setInt64(int /*i*/, boost::int64_t /*value*/) {}
        void setInt64(const std::string& /*name*/, boost::int64_t /*value*/) {}

        bool getBool(int /*i*/) const { return false; }
        bool getBool(const std::string& /*name*/) const { return false; }
        void setBool(int /*i*/, bool /*value*/){}
        void setBool(const std::string& /*name*/, bool /*value*/){}

        float getFloat(int /*i*/) const { return 0.0; }
        float getFloat(const std::string& /*name*/) const {return 0.0; }
        void setFloat(int /*i*/, float /*value*/) {}
        void setFloat(const std::string& /*name*/, float /*value*/){}
        
        double getDouble(int /*i*/) const { return 0.0; }
        double getDouble(const std::string& /*name*/) const { return 0.0; }
        void setDouble(int /*i*/, double /*value*/){}
        void setDouble(const std::string& /*name*/, double /*value*/){}
        
        std::string getNumeric(int /*i*/) const { return ""; }
        std::string getNumeric(const std::string& /*name*/) const { return ""; }
        void setNumeric(int /*i*/, const std::string& /*value*/) {}
        void setNumeric(const std::string& /*name*/, const std::string& /*value*/){}
        
        std::string getString(int /*i*/) const { return ""; }
        std::string getString(const std::string& /*name*/) const { return ""; }
        void setString(int /*i*/, const std::string& /*value*/) {}
        void setString(const std::string& /*name*/, const std::string& /*value*/) {}

        te::dt::ByteArray* getByteArray(int /*i*/) const { return 0; }
        te::dt::ByteArray* getByteArray(const std::string& /*name*/) const { return 0; }
        void setByteArray(int /*i*/, const te::dt::ByteArray& /*value*/) {}
        void setByteArray(const std::string& /*name*/, const te::dt::ByteArray& /*value*/) {}

        te::gm::Geometry* getGeometry(int /*i*/) const { return 0; }
        te::gm::Geometry* getGeometry(const std::string& /*name*/) const { return 0; }
        te::gm::Geometry* getGeometry() const { return 0; }
        void setGeometry(int /*i*/, const te::gm::Geometry& /*value*/) {}
        void setGeometry(const std::string& /*name*/, const te::gm::Geometry& /*value*/) {}

        te::rst::Raster* getRaster(int i) const;
        te::rst::Raster* getRaster(const std::string& name) const; 
        void setRaster(int /*i*/, const te::rst::Raster& /*value*/) {}
        void setRaster(const std::string& /*name*/, const te::rst::Raster& /*value*/) {}

        te::dt::DateTime* getDateTime(int /*i*/) const { return 0; }
        te::dt::DateTime* getDateTime(const std::string& /*name*/) const { return 0; }
        void setDateTime(int /*i*/, const te::dt::DateTime& /*value*/) {}
        void setDateTime(const std::string& /*name*/, const te::dt::DateTime& /*value*/) {}

        void getArray(int /*i*/, std::vector<boost::int16_t>& /*a*/) const {}
        void getArray(const std::string& /*name*/, std::vector<boost::int16_t>& /*a*/) const {}

        const unsigned char* getWKB(int /*i*/) const { return 0; }
        const unsigned char* getWKB(const std::string& /*name*/) const { return 0; }

        DataSet* getDataSet(int /*i*/) { return 0; }
        DataSet* getDataSet(const std::string& /*name*/) { return 0; }
        void setDataSet(int /*i*/, const te::da::DataSet& /*value*/) {}
        void setDataSet(const std::string& /*name*/, const te::da::DataSet& /*value*/) {}

        te::dt::AbstractData* getValue(int /*i*/) const { return 0; }
        te::dt::AbstractData* getValue(const std::string& /*name*/) const {return 0; }
        void setValue(int /*i*/, te::dt::AbstractData* /*ad*/) {}
        void setValue(const std::string& /*name*/, te::dt::AbstractData* /*ad*/) {}

        bool isNull(int i) const { return i != 0; }
        bool isNull(const std::string& /*name*/) const { return true; }

    private:
      
      DataSourceTransactor* m_transactor;   //!< The associated data source transactor.
      te::da::DataSetType* m_dsType;        //!< It describes the dataset.
      te::common::AccessPolicy m_rwRole;    //!< Access role.
      int m_size;                           //!< For GDAL driver this will be constant: 1.
      int m_i;                              //!< Just to indicate the internal pointer movement.
   };

  } // end namespace gdal
}   // end namespace te


#endif  // __TERRALIB_GDAL_INTERNAL_DATASET_H


