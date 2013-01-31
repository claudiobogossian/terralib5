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
  \file terralib/ogr/DataSet.h

  \brief Implementation of a DataSet for OGR data provider.  
 */

#ifndef __TERRALIB_OGR_INTERNAL_DATASET_H
#define __TERRALIB_OGR_INTERNAL_DATASET_H

// TerraLib
#include "../dataaccess/dataset/DataSet.h"
#include "Config.h"

// Forward declarations
class OGRLayer;
class OGRFeature;

namespace te
{
// Forward declarations
  namespace da
  {
    class GeometryProperty;
  }

  namespace ogr
  {
// Forward declarations
    class DataSourceTransactor;

    /*!
      \class DataSet

      \brief Implementation of a DataSet for OGR data provider.

      \sa Transactor, te::da::DataSet
     */
    class TEOGREXPORT DataSet : public te::da::DataSet
    {
      public:

        /** @name Constructor/Destructor
         *  Initilizer methods.
         */
        //@{

        /*! \brief Constructor. */
        DataSet(DataSourceTransactor* trans, OGRLayer* layer, bool isOwner = false);

        /*! \brief Destructor. */
        ~DataSet();

        //@}

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

        te::gm::Geometry* getGeometry() const;

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

      private:

        DataSourceTransactor* m_trans;        //!< The OGR transactor associated to this dataset.

        mutable te::da::DataSetType* m_dt;    //!< DataSetType.

        OGRLayer*   m_ogrLayer;               //<! A pointer to OGR Layer.
        OGRFeature* m_currentFeature;         //<! A pointer to current OGR Feature of layer.
        int m_i;                              //<! The current dataset index.

        mutable unsigned char* m_wkbArray;    //<! The array that stores the wkb generated by OGR Library.
        mutable int m_wkbArraySize;           //<! The size of wkb array.

        bool m_isOwner;                       //!< It defines if this dataset has the ownership of OGR Layer.
        int m_srid;                           //!< The STR id associated to this dataset.
    };

  } // end namespace ogr
}   // end namespace te


#endif  // __TERRALIB_OGR_INTERNAL_DATASET_H



