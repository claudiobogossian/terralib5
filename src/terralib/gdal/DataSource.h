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
#include "../dataaccess/datasource/DataSource.h"
#include "../dataaccess/datasource/DataSourceCapabilities.h"
#include "../dataaccess/dataset/DataSetType.h"
#include "Config.h"
#include "Exception.h"
#include "Transactor.h"

// Boost
#include <boost/shared_ptr.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/filesystem.hpp>

// STL 
#include <map>
#include <memory>
#include <string>
#include <vector>

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
    class ObjectIdSet;
    class DataSourceTransactor;
  }
  
  namespace gdal
  {
    /*!
     \class DataSource
     
     \brief A driver to access raster data using the GDAL library.
     
     This class is an implementation of a DataSource that consists of raster that can be decoded 
     by the GDAL - Geospatial Data Abstraction Library (http://www.gdal.org/).
     
     \note Supported getCapabilities options: SUPPORTED_EXTENSIONS-The supported file extensions.
     */
    class TEGDALEXPORT DataSource : public te::da::DataSource
    {
    public:
      
      DataSource();
      
      ~DataSource();
      
      std::string getType() const;
      
      const std::map<std::string, std::string>& getConnectionInfo() const;
      
      void setConnectionInfo(const std::map<std::string, std::string>& connInfo);
      
      std::auto_ptr<te::da::DataSourceTransactor> getTransactor();
      
      void open();
      
      void close();
      
      bool isOpened() const;
       
      bool isValid() const;
      
      const te::da::DataSourceCapabilities& getCapabilities() const;
      
      std::size_t getNumberOfItems(const std::string& datasetName)
      { return 1; }
      
      static void setCapabilities(const te::da::DataSourceCapabilities& capabilities);

      void addProperty(const std::string& , te::dt::Property* ) {}
      
      void dropProperty(const std::string& , const std::string& ) {};
      
      void renameProperty(const std::string& , const std::string& , const std::string& ) {}
      
      
      void add(const std::string& ,
               te::da::DataSet* ,
               const std::map<std::string, std::string>& ,
               std::size_t) {}
      
      void remove(const std::string& ,
                  const te::da::ObjectIdSet* ) {}
      
      void update(const std::string& ,
                  te::da::DataSet* ,
                  const std::vector<std::size_t>&,
                  const te::da::ObjectIdSet* ,
                  const std::map<std::string, std::string>& ,
                  std::size_t ) {}
      
      const te::da::SQLDialect* getDialect() const 
      { return 0; }
      
      void execute(const te::da::Query&)
      {}
      
      void execute(const std::string&)
      {}
      
      std::string escape(const std::string& value)
      { return value; }
      
      bool primaryKeyExists(const std::string& , const std::string& )
      { return false; }
      
      void addPrimaryKey(const std::string& , te::da::PrimaryKey* )
      {}
      
      void dropPrimaryKey(const std::string& )
      {}
      
      std::auto_ptr<te::da::ForeignKey> getForeignKey(const std::string& , const std::string& )
      { return std::auto_ptr<te::da::ForeignKey>(); }
      
      std::vector<std::string> getForeignKeyNames(const std::string& ) 
      { return std::vector<std::string>(); }
      
      bool foreignKeyExists(const std::string& , const std::string& )
      { return false; }
      
      void addForeignKey(const std::string&, te::da::ForeignKey* )
      {}
      
      void dropForeignKey(const std::string& , const std::string& )
      {}
      
      std::auto_ptr<te::da::UniqueKey> getUniqueKey(const std::string& , const std::string& )
      { return std::auto_ptr<te::da::UniqueKey>(); }
      
      bool uniqueKeyExists(const std::string& , const std::string& )
      { return false; }
      
      void addUniqueKey(const std::string& , te::da::UniqueKey* )
      {}
      
      void dropUniqueKey(const std::string& , const std::string& )
      {}
      
      std::auto_ptr<te::da::CheckConstraint> getCheckConstraint(const std::string& , const std::string& )
      { return std::auto_ptr<te::da::CheckConstraint>(); }
      
      std::vector<std::string> getCheckConstraintNames(const std::string& )
      { return std::vector<std::string>(); }
      
      bool checkConstraintExists(const std::string& , const std::string& )
      { return false; }
      
      void addCheckConstraint(const std::string& , te::da::CheckConstraint* )
      {}
      
      void dropCheckConstraint(const std::string& , const std::string& )
      {}
      
      std::auto_ptr<te::da::Index> getIndex(const std::string& , const std::string& )
      { return std::auto_ptr<te::da::Index>(); }
      
      std::vector<std::string> getIndexNames(const std::string& )
      { return std::vector<std::string>(); }
      
      bool indexExists(const std::string& , const std::string& )
      { return false; }
      
      void addIndex(const std::string& , te::da::Index* ,
                    const std::map<std::string, std::string>& )
      {}
      
      void dropIndex(const std::string& , const std::string& )
      {}
      
      std::auto_ptr<te::da::Sequence> getSequence(const std::string& )
      { return std::auto_ptr<te::da::Sequence>(); }
      
      std::vector<std::string> getSequenceNames()
      { return std::vector<std::string>(); }
      
      bool sequenceExists(const std::string& )
      { return false; }
      
      void addSequence(te::da::Sequence* )
      {}
      
      void dropSequence(const std::string& )
      {}
      
    protected:
      
      void create(const std::map<std::string, std::string>& dsInfo);
      
      void drop(const std::map<std::string, std::string>& dsInfo);
      
      bool exists(const std::map<std::string, std::string>& dsInfo);
      
      std::vector<std::string> getDataSourceNames(const std::map<std::string, std::string>& dsInfo);

      std::vector<te::common::CharEncoding> getEncodings(const std::map<std::string, std::string>& dsInfo);

    private:
      
      std::map<std::string, std::string> m_connectionInfo; 
      
      std::string m_straccess;
      
      bool m_isOpened;
      
      static te::da::DataSourceCapabilities sm_capabilities; 

    };
  } // end namespace gdal
} // end namespace te

#endif // __TERRALIB_GDAL_INTERNAL_DATASOURCE_H

