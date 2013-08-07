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
 \file terralib/gdal/DataSource.h
 
 \brief The implementation of a DataSource that consists of datasets that can be decoded by the GDAL Library.
 */

#ifndef __TERRALIB_GDAL_INTERNAL_DATASOURCE_H
#define __TERRALIB_GDAL_INTERNAL_DATASOURCE_H

// TerraLib
#include "../dataaccess2/datasource/DataSource.h"
#include "../dataaccess2/datasource/DataSourceCapabilities.h"
#include "../dataaccess2/dataset/DataSetType.h"
#include "Config.h"
#include "Exception.h"

// Boost
#include <boost/shared_ptr.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/cstdint.hpp>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/mem_fun.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/random_access_index.hpp>
#include <boost/noncopyable.hpp>

// STL 
#include <map>
#include <memory>
#include <string>
#include <vector>

class GDALDataset;

namespace te
{
  namespace dt
  {
    class Property;
  }
  
  namespace gm
  {
    class Envelope;
    class Geometry;
  }
  namespace da
  {
    class DataSet;
    class DataSource;
    class DataSourceCapabilities;
    class ObjectIdSet;
    typedef boost::shared_ptr<DataSetType> DataSetTypePtr;
  }
  
  namespace gdal
  {
    /*!
     \class DataSource
     
     \brief A driver to access raster data using the GDAL library.
     
     This class is an implementation of a DataSource that consists of raster that can be decoded 
     by the GDAL - Geospatial Data Abstraction Library (http://www.gdal.org/).
     */
    class TEGDALEXPORT DataSource : public te::da::DataSource
    {
    public:
      
      DataSource();
      
      ~DataSource();
      
      std::string getType() const;
      
      const std::map<std::string, std::string>& getConnectionInfo() const;
      
      void setConnectionInfo(const std::map<std::string, std::string>& connInfo);
      
      void open();
      
      void close();  
      
      bool isOpened() const;
      
      bool isValid() const;
      
      const te::da::DataSourceCapabilities& getCapabilities() const;
      
      std::auto_ptr<te::da::DataSet> getDataSet(const std::string& name, 
                                                te::common::TraverseType travType = te::common::FORWARDONLY);
      
      std::auto_ptr<te::da::DataSet> getDataSet(const std::string& name,
                                                const std::string& propertyName,
                                                const te::gm::Envelope* e,
                                                te::gm::SpatialRelation r,
                                                te::common::TraverseType travType = te::common::FORWARDONLY);
      
      std::auto_ptr<te::da::DataSet> getDataSet(const std::string& name,
                                                const std::string& propertyName,
                                                const te::gm::Geometry* g,
                                                te::gm::SpatialRelation r,
                                                te::common::TraverseType travType = te::common::FORWARDONLY);
      
      bool isDataSetNameValid(const std::string& datasetName);
      
      bool isPropertyNameValid(const std::string& propertyName);
      
      std::vector<std::string> getDataSetNames();
      
      const te::da::DataSetTypePtr& getDataSetType(const std::string& name);
      
      std::vector<std::string> getPropertyNames(const std::string& datasetName);

      std::size_t getNumberOfProperties(const std::string& datasetName);

      bool propertyExists(const std::string& datasetName, const std::string& name);

      boost::ptr_vector<te::dt::Property> getProperties(const std::string& datasetName);

      te::dt::Property* getProperty(const std::string& datasetName, const std::string& name);

      te::dt::Property* getProperty(const std::string& datasetName, std::size_t propertyPos);

      void addProperty(const std::string& datasetName, te::dt::Property* p);

      void dropProperty(const std::string& datasetName, const std::string& propertyName);

      void renameProperty(const std::string& datasetName, const std::string& propertyName, const std::string& newPropertyName);

      std::auto_ptr<te::gm::Envelope> getExtent(const std::string& datasetName,
                                                const std::string& propertyName);
      
      std::auto_ptr<te::gm::Envelope> getExtent(const std::string& datasetName,
                                                std::size_t propertyPos);
      
      std::size_t getNumberOfItems(const std::string& datasetName);
      
      bool hasDataSets();
      
      bool dataSetExists(const std::string& name);
      
      void createDataSet(te::da::DataSetType* dt,
                         const std::map<std::string, std::string>& options);
      
      void cloneDataSet(const std::string& name,
                        const std::string& cloneName,
                        const std::map<std::string, std::string>& options) ;
      
      void dropDataSet(const std::string& name);
      
      void renameDataSet(const std::string& name,
                         const std::string& newName);
      
      void add(const std::string& datasetName,
               te::da::DataSet* d,
               const std::map<std::string, std::string>& options,
               std::size_t limit = 0);
      
      void remove(const std::string& datasetName,
                  const te::da::ObjectIdSet* oids = 0);
      
      void update(const std::string& datasetName,
                  te::da::DataSet* dataset,
                  const std::vector<std::size_t>& properties,
                  const te::da::ObjectIdSet* oids,
                  const std::map<std::string, std::string>& options,
                  std::size_t limit = 0);

  
      static void setCapabilities(const te::da::DataSourceCapabilities& capabilities);

      void getProperties(te::da::DataSetTypePtr& dt);

    protected:
      
      void create(const std::map<std::string, std::string>& dsInfo);
      
      void drop(const std::map<std::string, std::string>& dsInfo);
      
      bool exists(const std::map<std::string, std::string>& dsInfo);
      
      std::vector<std::string> getDataSourceNames(const std::map<std::string, std::string>& dsInfo);

      std::vector<std::string> getEncodings(const std::map<std::string, std::string>& dsInfo);

    private:
      
      void getSubDatasets(GDALDataset* gds, const std::string& driver);
      
      std::string getDataSetFullName(const std::string& name);
      
      std::map<std::string, std::string> m_connectionInfo;  
      
      std::map<std::string, te::da::DataSetTypePtr> m_catalog; 
      
      std::vector<std::string> m_datasetNames;
      std::vector<std::string> m_datasetFullNames;
      
      bool m_isOpened;
      bool m_isDirectory; 
      
      static te::da::DataSourceCapabilities sm_capabilities; 

    };
  } // end namespace gdal
} // end namespace te

#endif // __TERRALIB_GDAL_INTERNAL_DATASOURCE_H

