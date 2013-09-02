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
// Forward declaration
  namespace da
  {
    class DataSetType;
  }

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
        ~DataSet();

        te::common::TraverseType getTraverseType() const { return te::common::FORWARDONLY; }

        te::common::AccessPolicy getAccessPolicy() const { return m_rwRole; }

        te::da::DataSourceTransactor* getTransactor() const { return 0; }

        te::gm::Envelope* getExtent(std::size_t i);

        std::size_t getNumProperties() const;

        int getPropertyDataType(std::size_t pos) const;

        std::string getPropertyName(std::size_t pos) const;

        std::string getDatasetNameOfProperty(std::size_t pos) const;

        bool isEmpty() const { return false; }

        std::size_t size() const { return 1; }

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

        char getChar(std::size_t /*i*/) const { return '\0'; }
        char getChar(const std::string& /*name*/) const { return '\0'; }

        unsigned char getUChar(std::size_t /*i*/) const { return 0; }
        unsigned char getUChar(const std::string& /*name*/) const { return 0; }

        boost::int16_t getInt16(std::size_t /*i*/) const { return 0; }
        boost::int16_t getInt16(const std::string& /*name*/) const { return 0; }

        boost::int32_t getInt32(std::size_t /*i*/) const { return 0; }
        boost::int32_t getInt32(const std::string& /*name*/) const { return 0; }

        boost::int64_t getInt64(std::size_t /*i*/) const { return 0; }
        boost::int64_t getInt64(const std::string& /*name*/) const { return 0; }

        bool getBool(std::size_t /*i*/) const { return false; }
        bool getBool(const std::string& /*name*/) const { return false; }

        float getFloat(std::size_t /*i*/) const { return 0.0; }
        float getFloat(const std::string& /*name*/) const {return 0.0; }
        
        double getDouble(std::size_t /*i*/) const { return 0.0; }
        double getDouble(const std::string& /*name*/) const { return 0.0; }
        
        std::string getNumeric(std::size_t /*i*/) const { return ""; }
        std::string getNumeric(const std::string& /*name*/) const { return ""; }
        
        std::string getString(std::size_t /*i*/) const { return ""; }
        std::string getString(const std::string& /*name*/) const { return ""; }

        te::dt::ByteArray* getByteArray(std::size_t /*i*/) const { return 0; }
        te::dt::ByteArray* getByteArray(const std::string& /*name*/) const { return 0; }

        te::gm::Geometry* getGeometry(std::size_t /*i*/) const { return 0; }
        te::gm::Geometry* getGeometry(const std::string& /*name*/) const { return 0; }

        te::rst::Raster* getRaster(std::size_t i) const;
        te::rst::Raster* getRaster(const std::string& name) const;

        te::dt::DateTime* getDateTime(std::size_t /*i*/) const { return 0; }
        te::dt::DateTime* getDateTime(const std::string& /*name*/) const { return 0; }

        te::dt::Array* getArray(std::size_t /*i*/) const { return 0; }
        te::dt::Array* getArray(const std::string& /*name*/) const { return 0; }

        te::dt::AbstractData* getValue(std::size_t /*i*/) const { return 0; }
        te::dt::AbstractData* getValue(const std::string& /*name*/) const {return 0; }

        bool isNull(std::size_t i) const { return i != 0; }
        bool isNull(const std::string& /*name*/) const { return true; }

      private:

        void loadTypeInfo();

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
