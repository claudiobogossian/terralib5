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
  \file terralib/gdal/DataSource.cpp

  \brief The implementation of a DataSource that consists of datasets that can be decoded by the GDAL Library.
*/

// TerraLib
#include "../common/Translator.h"
#include "../dataaccess2/dataset/DataSetType.h"
#include "../raster/Grid.h"
#include "../raster/Raster.h"
#include "../raster/RasterProperty.h"
#include "DataSet.h"
#include "DataSource.h"
#include "Exception.h"

// GDAL
#include <gdal_priv.h>

// Boost
#include <boost/filesystem.hpp>
#include <boost/format.hpp>

te::da::DataSourceCapabilities te::gdal::DataSource::sm_capabilities;

te::gdal::DataSource::DataSource():
    m_isOpened(false),
    m_isDirectory(false)
{
}

te::gdal::DataSource::~DataSource()
{
}

std::string te::gdal::DataSource::getType() const throw()
{
  return TE_GDAL_DRIVER_IDENTIFIER;
}

const std::map<std::string, std::string>& te::gdal::DataSource::getConnectionInfo() const throw()
{
  return m_connectionInfo;
}

void te::gdal::DataSource::setConnectionInfo(const std::map<std::string, std::string>& connInfo) throw()
{
  m_connectionInfo = connInfo;
}

bool te::gdal::DataSource::isOpened() const throw()
{
  return m_isOpened;
}

bool te::gdal::DataSource::isValid() const throw()
{
  if(m_connectionInfo.empty())
    return false;
  
  bool isValid = false;
  
  std::map<std::string, std::string>::const_iterator it = m_connectionInfo.find("URI");
  
  // if URI used, check if it is a valid directory or file name
  if(it != m_connectionInfo.end())
  {
    if(!boost::filesystem::exists(it->second))
      return false;
    
    if(boost::filesystem::is_directory(it->second))
      return true;    
  }
  else  // if it is another GDAL string let's check it
  {
    it = m_connectionInfo.find("SOURCE");
    
    if(it == m_connectionInfo.end())
      return false;
  }
  
  GDALDataset* gds = static_cast<GDALDataset*>(GDALOpen(it->second.c_str(), GA_ReadOnly));
  
  if(gds)
    isValid = true;
  
  GDALClose(gds);
  
  return isValid;
}

const te::da::DataSourceCapabilities& te::gdal::DataSource::getCapabilities() const  throw()
{
  return sm_capabilities;
}

std::auto_ptr<te::da::DataSet> te::gdal::DataSource::getDataSet(const std::string& name, 
                                                te::common::TraverseType /*travType*/)  throw(Exception)
{
  std::map<std::string, te::da::DataSetTypePtr>::const_iterator it = m_dstypes.find(name);
  if (it != m_dstypes.end())
  {
    return std::auto_ptr<te::da::DataSet>(new DataSet(static_cast<te::da::DataSetType*>(it->second->clone())));  // return a full loaded dataset type
  }
  
  // create a datset type with basic raster info: just type and name!
  te::da::DataSetType* ndt = new te::da::DataSetType(name);
  
  te::rst::Grid* grid = new te::rst::Grid();
  
  std::vector<te::rst::BandProperty*> bprops;
  
  std::map<std::string, std::string> rinfo;
  
  te::rst::RasterProperty* rp = new te::rst::RasterProperty(grid, bprops, rinfo);
  
  ndt->add(rp);
  
  return std::auto_ptr<te::da::DataSet>(new DataSet(ndt));  // return a full loaded dataset type
}


std::auto_ptr<te::da::DataSet> te::gdal::DataSource::getDataSet(const std::string& name,
                                                const std::string& propertyName,
                                                const te::gm::Envelope* /*e*/,
                                                te::gm::SpatialRelation /*r*/,
                                                te::common::TraverseType travType) throw(Exception)
{
  return getDataSet(name, travType);
}

std::auto_ptr<te::da::DataSet>  te::gdal::DataSource::getDataSet(const std::string& name,
                                                const std::string& propertyName,
                                                const te::gm::Geometry* /*g*/,
                                                te::gm::SpatialRelation /*r*/,
                                                te::common::TraverseType travType) throw(Exception)
{
  return getDataSet(name, travType);
}

std::auto_ptr<te::da::DataSet> te::gdal::DataSource::query(const te::da::Select& /*q*/,
                                           te::common::TraverseType /*travType*/) throw(Exception)
{
  throw(Exception(TR_GDAL("GDAL driver doesn't support queries.")));
}

std::auto_ptr<te::da::DataSet> te::gdal::DataSource::query(const std::string& /*query*/, 
                                           te::common::TraverseType /*travType*/) throw(Exception)
{
  throw(Exception(TR_GDAL("GDAL driver doesn't support queries.")));
}

void te::gdal::DataSource::execute(const te::da::Query& /*command*/) throw(Exception)
{
  throw(Exception(TR_GDAL("GDAL driver doesn't support queries.")));
}

void te::gdal::DataSource::execute(const std::string& /*command*/) throw(Exception)
{
  throw(Exception(TR_GDAL("GDAL driver doesn't support queries.")));
}

void te::gdal::DataSource::cancel() throw(Exception) 
{
  throw(Exception(TR_GDAL("GDAL driver doesn't support queries.")));
}

boost::int64_t te::gdal::DataSource::getLastGeneratedId() throw(Exception) 
{ 
  throw(Exception(TR_GDAL("GDAL driver doesn't support id generation.")));
}

std::string te::gdal::DataSource::escape(const std::string& value) throw(Exception) 
{ 
  throw(Exception(TR_GDAL("GDAL driver doesn't support string escaping.")));
}

bool te::gdal::DataSource::isDataSetNameValid(const std::string& datasetName) throw(Exception)
{
  throw(Exception(TR_GDAL("Not implemented yet.")));
}

bool te::gdal::DataSource::isPropertyNameValid(const std::string& propertyName) throw(Exception)
{
  throw(Exception(TR_GDAL("Not implemented yet.")));
}

std::vector<std::string> te::gdal::DataSource::getDataSetNames() throw(Exception)
{
  std::vector<std::string> dsnames;
  
  std::map<std::string, te::da::DataSetTypePtr>::const_iterator it =  m_dstypes.begin();
  while (it != m_dstypes.end())
  {
    dsnames.push_back(it->first);
    ++it;
  }
  
  return dsnames;
}

const te::da::DataSetTypePtr& te::gdal::DataSource::getDataSetType(const std::string& name) throw(Exception)
{
  std::map<std::string, te::da::DataSetTypePtr>::const_iterator it = m_dstypes.find(name);
  if (it != m_dstypes.end())
    //return std::auto_ptr<te::da::DataSetType>();
    return te::da::DataSetTypePtr();

  //return std::auto_ptr<te::da::DataSetType>(static_cast<te::da::DataSetType*>(it->second->clone()));
  return te::da::DataSetTypePtr(static_cast<te::da::DataSetType*>(it->second->clone()));
}

std::size_t te::gdal::DataSource::getNumberOfProperties(const std::string& datasetName) throw(Exception)
{
  std::map<std::string, te::da::DataSetTypePtr>::const_iterator it = m_dstypes.find(datasetName);
  if (it != m_dstypes.end())
    return 0;
  
  return it->second->size(); 
}

boost::ptr_vector<te::dt::Property> te::gdal::DataSource::getProperties(const std::string& datasetName) throw(Exception)
{
  boost::ptr_vector<te::dt::Property> properties;
  
  std::map<std::string, te::da::DataSetTypePtr>::const_iterator it = m_dstypes.find(datasetName);
  if (it != m_dstypes.end())
    return properties;

  const te::da::DataSetTypePtr& dt = it->second;
  
  const std::size_t np = dt->size();
  
  for(std::size_t i = 0; i != np; ++i)
  {
    properties.push_back(dt->getProperty(i)->clone());
  }
  
  return properties;
}

std::auto_ptr<te::dt::Property> te::gdal::DataSource::getProperty(const std::string& datasetName, const std::string& propertyName) throw(Exception)
{
  boost::ptr_vector<te::dt::Property> properties = getProperties(datasetName);
  
  if (!properties.empty())
  {
    for (size_t i=0; i<properties.size(); ++i)
      if (properties[i].getName() == propertyName)
        return std::auto_ptr<te::dt::Property>(properties[i].clone());
  }
  
  return std::auto_ptr<te::dt::Property>();
}

std::auto_ptr<te::dt::Property> te::gdal::DataSource::getProperty(const std::string& datasetName, std::size_t propertyPos) throw(Exception)
{
  boost::ptr_vector<te::dt::Property> properties = getProperties(datasetName);
  if (!properties.empty())
    std::auto_ptr<te::dt::Property>(properties[propertyPos].clone());
  
  return std::auto_ptr<te::dt::Property>();
}

std::auto_ptr<te::da::PrimaryKey> te::gdal::DataSource::getPrimaryKey(const std::string& /*datasetName*/) throw(Exception)
{
  throw(Exception(TR_GDAL("GDAL driver doesn't support primary keys.")));
}

std::vector<std::string> te::gdal::DataSource::getUniqueKeyNames(const std::string& datasetName) throw(Exception)
{
  throw(Exception(TR_GDAL("GDAL driver doesn't support unique keys.")));
}

boost::ptr_vector<te::da::UniqueKey> te::gdal::DataSource::getUniqueKeys(const std::string& /*datasetName*/) throw(Exception)
{
  throw(Exception(TR_GDAL("GDAL driver doesn't support unique keys.")));
}

std::auto_ptr<te::da::UniqueKey> te::gdal::DataSource::getUniqueKey(const std::string& /*datasetName*/,
                                                                    const std::string& /*name*/) throw (Exception)
{
  throw(Exception(TR_GDAL("GDAL driver doesn't support unique keys.")));
}

std::vector<std::string> te::gdal::DataSource::getForeignKeyNames(const std::string& /*datasetName*/) throw(Exception)
{
  throw(Exception(TR_GDAL("GDAL driver doesn't support foreign keys.")));
}

std::vector<std::string> te::gdal::DataSource::getIndexNames(const std::string& /*datasetName*/) throw(Exception)
{
  throw(Exception(TR_GDAL("GDAL driver doesn't support unique keys.")));
}

std::auto_ptr<te::da::Index> te::gdal::DataSource::getIndex(const std::string& datasetName,
                              const std::string& name) throw(Exception)
{
  throw(Exception(TR_GDAL("GDAL driver doesn't support indexes.")));
}

std::vector<std::string> te::gdal::DataSource::getCheckConstraintNames(const std::string& datasetName) throw(Exception)
{
  throw(Exception(TR_GDAL("GDAL driver doesn't support constraints.")));
}

std::auto_ptr<te::da::CheckConstraint> te::gdal::DataSource::getCheckConstraint(const std::string& datasetName,
                                                  const std::string& name) throw(Exception)
{
  throw(Exception(TR_GDAL("GDAL driver doesn't support constraints.")));
}

std::vector<std::string> te::gdal::DataSource::getSequenceNames() throw(Exception)
{
  throw(Exception(TR_GDAL("GDAL driver doesn't support sequences.")));
}

std::auto_ptr<te::da::Sequence> te::gdal::DataSource::getSequence(const std::string& name) throw(Exception)
{
  throw(Exception(TR_GDAL("GDAL driver doesn't support sequences.")));
}


std::auto_ptr<te::da::ForeignKey> te::gdal::DataSource::getForeignKey(const std::string& /*datasetName*/,
                                        const std::string& /*name*/) throw(Exception)
{
  throw(Exception(TR_GDAL("GDAL driver doesn't support foreigns keys.")));
}

std::auto_ptr<te::gm::Envelope> te::gdal::DataSource::getExtent(const std::string& datasetName,
                                          const std::string& propertyName) throw(Exception)
{
  return std::auto_ptr<te::gm::Envelope>();
}

std::auto_ptr<te::gm::Envelope> te::gdal::DataSource::getExtent(const std::string& datasetName,
                                          std::size_t propertyPos) throw(Exception)
{
  return std::auto_ptr<te::gm::Envelope>();
}
 
std::size_t te::gdal::DataSource::getNumberOfItems(const std::string& datasetName) throw(Exception)
{
  return 0; // TODO: review
}

bool te::gdal::DataSource::hasDataSets() throw(Exception)
{
  return !m_dstypes.empty();
}

bool te::gdal::DataSource::datasetExists(const std::string& name) throw(Exception)
{
  std::map<std::string, te::da::DataSetTypePtr>::const_iterator it = m_dstypes.find(name);
  return (it != m_dstypes.end());
}

void te::gdal::DataSource::createDataSet(te::da::DataSetType* dt,
                   const std::map<std::string, std::string>& options) throw (Exception)
{
  throw(Exception(TR_GDAL("To be implemented.")));
}

bool te::gdal::DataSource::primarykeyExists(const std::string& /*datasetName*/,
                      const std::string& /*name*/) throw(Exception) 
{ 
  throw(Exception(TR_GDAL("GDAL driver doesn't support primary keys.")));
}

bool te::gdal::DataSource::uniquekeyExists(const std::string& /*datasetName*/, const std::string& /*name*/) throw(Exception) 
{ 
  throw(Exception(TR_GDAL("GDAL driver doesn't support unique keys.")));
}

bool te::gdal::DataSource::foreignkeyExists(const std::string& /*datasetName*/,
                      const std::string& /*name*/) throw(Exception) 
{ 
  throw(Exception(TR_GDAL("GDAL driver doesn't support foreigns keys."))); 
}

bool te::gdal::DataSource::checkConstraintExists(const std::string& /*datasetName*/,
                           const std::string& /*name*/) throw(Exception) 
{ 
  throw(Exception(TR_GDAL("GDAL driver doesn't support check constraints."))); 
}

bool te::gdal::DataSource::indexExists(const std::string& /*datasetName*/,
                 const std::string& /*name*/) throw(Exception) 
{ 
  throw(Exception(TR_GDAL("GDAL driver doesn't support check indexes."))); 
}

bool te::gdal::DataSource::sequenceExists(const std::string& /*name*/) throw(Exception) 
{ 
  throw(Exception(TR_GDAL("GDAL driver doesn't support check sequences."))); 
}

void te::gdal::DataSource::cloneDataSet(const std::string& name,
                  const std::string& cloneName,
                  const std::map<std::string, std::string>& options) throw(Exception)
{
  throw(Exception(TR_GDAL("To be implemented.")));
}

void te::gdal::DataSource::dropDataSet(const std::string& name) throw(Exception)
{
  throw(Exception(TR_GDAL("To be implemented.")));
}

void te::gdal::DataSource::renameDataSet(const std::string& name,
                   const std::string& newName) throw(Exception)
{
  throw(Exception(TR_GDAL("To be implemented.")));
}

void te::gdal::DataSource::addProperty(const std::string& datasetName,
                 const te::dt::Property* p) throw(Exception)
{
    throw Exception(TR_GDAL("Not implemented yet!"));
}

void te::gdal::DataSource::dropProperty(const std::string& datasetName,
                  const std::string& propertyName) throw(Exception)
{
  throw(Exception(TR_GDAL("To be implemented.")));
}

void te::gdal::DataSource::renameProperty(const std::string& datasetName,
                    const std::string& propertyName,
                    const std::string& newPropertyName) throw(Exception)
{
  throw(Exception(TR_GDAL("To be implemented.")));
}

void te::gdal::DataSource::add(const std::string& datasetName,
         te::da::DataSet* d,
         const std::map<std::string, std::string>& options,
         std::size_t limit) throw(Exception)
{
  throw(Exception(TR_GDAL("To be implemented.")));
}

void te::gdal::DataSource::remove(const std::string& datasetName,
            const te::da::ObjectIdSet* oids) throw(Exception)
{
  throw(Exception(TR_GDAL("To be implemented.")));
}

void te::gdal::DataSource::update(const std::string& datasetName,
       te::da::DataSet* dataset,
       const std::vector<std::size_t>& properties,
       const te::da::ObjectIdSet* oids,
       const std::map<std::string, std::string>& options,
       std::size_t limit) throw(Exception)
{
  throw Exception(TR_GDAL("Not implemented yet!"));
}

void te::gdal::DataSource::optimize(const std::map<std::string, std::string>& /*opInfo*/)
{
  throw Exception(TR_GDAL("Not implemented yet!"));
}

void te::gdal::DataSource::open() throw(Exception)
{
  if(m_isOpened)
    return;

// by default assume datasource doesn't refer to a directory
  m_isDirectory = false;

// check if the datasource is a directory: GDAL doesn't give support on this
  std::map<std::string, std::string>::const_iterator it = m_connectionInfo.find("URI");

  if(it != m_connectionInfo.end())
  {
    if(!boost::filesystem::exists(it->second))
      throw Exception((boost::format(TR_GDAL("Invalid data source connection information: %1%.")) % it->second).str());

    if(boost::filesystem::is_directory(it->second))
      m_isDirectory = true;
  }
  else
  {
    it = m_connectionInfo.find("SOURCE");

    if(it == m_connectionInfo.end())
      throw Exception(TR_GDAL("Invalid data source connection information!"));
  }

  m_isOpened = true;
}

void te::gdal::DataSource::close() throw(Exception)
{
  m_dstypes.clear();

  m_isOpened = false;
}


void te::gdal::DataSource::setCapabilities(const te::da::DataSourceCapabilities& capabilities)
{
  sm_capabilities = capabilities;
}

void te::gdal::DataSource::create(const std::map<std::string, std::string>& dsInfo) throw(Exception)
{
  setConnectionInfo(dsInfo);
  
  std::map<std::string, std::string>::const_iterator it = m_connectionInfo.find("URI");

  if(it != m_connectionInfo.end())
  {
    try 
    {      
      if(!boost::filesystem::is_directory(it->second))
        boost::filesystem::create_directory(it->second);
    } 
    catch(const boost::filesystem::filesystem_error& e) 
    { 
      throw Exception((boost::format(TR_GDAL("Could not create the data source due to the following error: %1%.")) % e.what()).str());
    }
  }

  close();
}
 
void te::gdal::DataSource::drop(const std::map<std::string, std::string>& dsInfo) throw(Exception)
{
  std::map<std::string, std::string>::const_iterator it = dsInfo.find("URI");

  if(it != dsInfo.end())
  {
    try 
    {    
      boost::filesystem::remove(it->second);
    }
    catch(const boost::filesystem::filesystem_error& e) 
    { 
      throw Exception((boost::format(TR_GDAL("Could not drop the data source due to the following error: %1%.")) % e.what()).str());
    }
  }
}

std::vector<std::string> te::gdal::DataSource::getDataSourceNames(const std::map<std::string, std::string>& info) throw(Exception)
{
  return std::vector<std::string>();
}
