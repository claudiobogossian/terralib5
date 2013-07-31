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
#include "../common/StringUtils.h"
#include "../common/Translator.h"
#include "../dataaccess2/dataset/DataSetType.h"
#include "../raster/Grid.h"
#include "../raster/Raster.h"
#include "../raster/RasterProperty.h"
#include "DataSet.h"
#include "DataSource.h"
#include "Exception.h"
#include "Utils.h"

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
  sm_capabilities.setAccessPolicy(te::common::RWAccess);
}

te::gdal::DataSource::~DataSource()
{
}

std::string te::gdal::DataSource::getType() const
{
  return TE_GDAL_DRIVER_IDENTIFIER;
}

const std::map<std::string, std::string>& te::gdal::DataSource::getConnectionInfo() const
{
  return m_connectionInfo;
}

void te::gdal::DataSource::setConnectionInfo(const std::map<std::string, std::string>& connInfo)
{ 
  m_connectionInfo = connInfo;
}


void te::gdal::DataSource::getSubDatasets(GDALDataset* gds, const std::string& driver)
{
  assert(gds);
  assert(!driver.empty());
  
  char** subdatasets = gds->GetMetadata("SUBDATASETS");
  
  for(char** i = subdatasets; *i != 0; ++i)
  {
    std::map<std::string, std::string> sdsmap;
    
    te::common::ExtractKVP(std::string(*i), sdsmap);
    
    if(sdsmap.begin()->first.find("_NAME") != std::string::npos)
    {
      std::string fullName = sdsmap.begin()->second;
      
      // check if subdataset is valid
      GDALDataset* subdataset = static_cast<GDALDataset*>(GDALOpen(fullName.c_str(), GA_ReadOnly));
      if (!subdataset)
        continue;
      GDALClose(subdataset);
      
      std::string subdsname = GetSubDataSetName(fullName, driver);
      m_datasetNames.push_back(subdsname);
      
      m_datasetFullNames.push_back(fullName);
    }
  }
}

// open methods retrieves the names of the datasets in the data source
void te::gdal::DataSource::open()
{
  if (m_isOpened)
    return;

  if (m_connectionInfo.empty())
      throw Exception((boost::format(TR_GDAL("Empty data source connection information"))).str());
  
  m_isDirectory = false;
  
  std::map<std::string, std::string>::const_iterator it = m_connectionInfo.find("URI");
  if (it != m_connectionInfo.end())
  {
    if (boost::filesystem::is_regular_file(it->second))
    {
      GDALDataset* gds = static_cast<GDALDataset*>(GDALOpen(it->second.c_str(), GA_ReadOnly));
      if (gds)
      { 
        char** subdatasets = gds->GetMetadata("SUBDATASETS");
        
        if(subdatasets == 0)
        {
          GDALClose(gds);
          boost::filesystem::path path(it->second);
          m_datasetNames.push_back(path.leaf().string());
          m_datasetFullNames.push_back(path.string());
          m_isOpened = true;
          return;
        }
        getSubDatasets(gds,te::gdal::GetDriverName(it->second));
        GDALClose(gds);
      }
      else 
        throw Exception((boost::format(TR_GDAL("Invalid data source connection information"))).str());
    }
  }
  else
  {
    it = m_connectionInfo.find("SOURCE");
    if(it == m_connectionInfo.end())
      throw Exception((boost::format(TR_GDAL("Invalid data source connection information"))).str());
    
    m_isDirectory = true;
    
    boost::filesystem::path path(it->second);
    
    for(boost::filesystem::directory_iterator it(path), itEnd; it != itEnd; ++it)
    {
      boost::filesystem::path foundFile = (*it);
      
      GDALDataset* dataset = static_cast<GDALDataset*>(GDALOpen(foundFile.string().c_str(), GA_ReadOnly));
      
      if(dataset == 0)
        continue;
      
      char** subdatasets = dataset->GetMetadata("SUBDATASETS");
      if(subdatasets == 0)
      {
        m_datasetNames.push_back(foundFile.leaf().c_str());
        m_datasetFullNames.push_back(foundFile.c_str());
      }
      else 
        getSubDatasets(dataset,te::gdal::GetDriverName(foundFile.string()));
      GDALClose(dataset);
    }
  }  
  m_isOpened = true;
}

void te::gdal::DataSource::close()
{
  m_datasetNames.clear();
  m_datasetFullNames.clear();
  m_catalog.clear();
  m_isOpened = false;
}

bool te::gdal::DataSource::isOpened() const
{
  return m_isOpened;
}

bool te::gdal::DataSource::isValid() const
{
  if(m_connectionInfo.empty())
    return false;
  
  std::map<std::string, std::string>::const_iterator it = m_connectionInfo.find("SOURCE");
  
  // if URI used, check if it is a valid directory or file name
  if(it != m_connectionInfo.end())
  {
    if(boost::filesystem::is_directory(it->second))
      return true;    
  }
  else  // if it is another GDAL string let's check it
  {
    it = m_connectionInfo.find("URI");
    
    if(it == m_connectionInfo.end())
      return false;
    
    GDALDataset* gds = static_cast<GDALDataset*>(GDALOpen(it->second.c_str(), GA_ReadOnly));
    
    if(gds)
    {
      GDALClose(gds);
      return true;
    }
  }
  return false;
}

bool te::gdal::DataSource::hasDataSets()
{
  return !m_datasetNames.empty();
}

std::string te::gdal::DataSource::getDataSetFullName(const std::string& name)
{
  assert(!name.empty());
  
  std::vector<std::string>::const_iterator it = std::find(m_datasetNames.begin(), m_datasetNames.end(),name);
  if (it == m_datasetNames.end())
    return "";
  
  unsigned int pos = it-m_datasetNames.begin();
  if (pos < m_datasetFullNames.size())
    return m_datasetFullNames[pos];
  
  return "";
}

bool te::gdal::DataSource::dataSetExists(const std::string& name)
{
  std::string gdName = getDataSetFullName(name);
  if (gdName.empty())
    return false;
  
  GDALDataset* dataset = static_cast<GDALDataset*>(GDALOpen(gdName.c_str(), GA_ReadOnly));
  
  if(dataset == 0)
    return false;
  
  GDALClose(dataset);
  return true;
}

std::vector<std::string> te::gdal::DataSource::getDataSetNames()
{
  return m_datasetNames;
}

void te::gdal::DataSource::getProperties(te::da::DataSetTypePtr& dt)
{
  assert(dt.get());
  
  std::string gdName = getDataSetFullName(dt->getName());
  if (gdName.empty())
    throw Exception(TR_GDAL("GDAL couldn't retrieve the dataset properties."));

  GDALDataset* dataset = static_cast<GDALDataset*>(GDALOpen(gdName.c_str(), GA_ReadOnly));
  
  if (!dataset)
    throw Exception(TR_GDAL("GDAL couldn't retrieve the dataset properties."));
  
  te::rst::Grid* grid = GetGrid(dataset);
  
  std::vector<te::rst::BandProperty*> bprops;
  
  GetBandProperties(dataset, bprops);
  
  std::map<std::string, std::string> rinfo;
  
  rinfo["URI"] = dataset->GetDescription();
  
  te::rst::RasterProperty* rp = new te::rst::RasterProperty(grid, bprops, rinfo);
  
  dt->add(rp);
  
  GDALClose(dataset);
  
}

const te::da::DataSetTypePtr& te::gdal::DataSource::getDataSetType(const std::string& name)
{
  assert(!name.empty());
  
  // check if dataset name exists
  if(!dataSetExists(name))
    throw Exception((boost::format(TR_GDAL("The dataset \"%1%\" doesn't exist!")) % name).str());
  
  // check if it is in the catalog
  std::map<std::string, te::da::DataSetTypePtr>::const_iterator it = m_catalog.find(name);
  if(it != m_catalog.end())
    return it->second;
  
  // create the dataset type and insert into the catalog
  unsigned int dtid = (std::find(m_datasetNames.begin(), m_datasetNames.end(), name)-m_datasetNames.begin());
  te::da::DataSetTypePtr dt(new te::da::DataSetType(name, dtid));
  dt->setTitle(name);
  getProperties(dt);
  
  std::pair<std::string, te::da::DataSetTypePtr> mpair = std::make_pair(name,dt);
  m_catalog.insert(mpair);
  
  return getDataSetType(name);
}

std::auto_ptr<te::da::DataSet> te::gdal::DataSource::getDataSet(const std::string& name, 
                                                                te::common::TraverseType /*travType*/) 
{
  assert(!name.empty());
  
  std::map<std::string, te::da::DataSetTypePtr>::const_iterator it = m_catalog.find(name);
  if(it != m_catalog.end())
    return std::auto_ptr<te::da::DataSet>(new DataSet(static_cast<te::da::DataSetType*>(it->second->clone()))); 
  else 
  {
    unsigned int dtid = (std::find(m_datasetNames.begin(), m_datasetNames.end(), name)-m_datasetNames.begin());
    te::da::DataSetTypePtr dt(new te::da::DataSetType(name, dtid));
    dt->setTitle(name);
    getProperties(dt);
    
    return std::auto_ptr<te::da::DataSet>(new DataSet(static_cast<te::da::DataSetType*>(dt->clone())));
  }
}

std::size_t te::gdal::DataSource::getNumberOfProperties(const std::string& datasetName)
{
  const te::da::DataSetTypePtr& dt = getDataSetType(datasetName);
  
  return dt->size();
}

boost::ptr_vector<te::dt::Property> te::gdal::DataSource::getProperties(const std::string& datasetName)
{
  const te::da::DataSetTypePtr& dt = getDataSetType(datasetName);
  
  std::vector<te::dt::Property*>& dtProperties = dt->getProperties();
  
  boost::ptr_vector<te::dt::Property> properties;
  
  for(std::size_t i = 0; i < dtProperties.size(); ++i)
    properties.push_back(dtProperties[i]->clone());
  
  return properties;
}

bool te::gdal::DataSource::propertyExists(const std::string& datasetName, const std::string& name)
{  
  std::vector<std::string> pNames = getPropertyNames(datasetName);
  
  if(std::find(pNames.begin(), pNames.end(), name) != pNames.end())
    return true;
  
  return false;
}

te::dt::Property* te::gdal::DataSource::getProperty(const std::string& datasetName, const std::string& propertyName)
{
  if(!propertyExists(datasetName, propertyName))
    throw Exception((boost::format(TR_GDAL("The dataset \"%1%\" has no property with this name \"%2%\"!")) % datasetName % propertyName).str());
  
  const te::da::DataSetTypePtr& dt = getDataSetType(datasetName);
  
  return dt->getProperty(propertyName);
}


te::dt::Property* te::gdal::DataSource::getProperty(const std::string& datasetName, std::size_t propertyPos)
{
  const te::da::DataSetTypePtr& dt = getDataSetType(datasetName);
  
  assert(propertyPos < dt->size());
  
  return dt->getProperty(propertyPos);
}

std::auto_ptr<te::gm::Envelope> te::gdal::DataSource::getExtent(const std::string& datasetName, std::size_t propertyPos)
{
   throw(Exception(TR_GDAL("Not implemented yet.")));
}

const te::da::DataSourceCapabilities& te::gdal::DataSource::getCapabilities() const 
{
  return sm_capabilities;
}

void te::gdal::DataSource::begin()
{}

void te::gdal::DataSource::commit()
{}

void te::gdal::DataSource::rollBack()
{}


std::auto_ptr<te::da::DataSet> te::gdal::DataSource::getDataSet(const std::string& name,
                                                                const std::string& propertyName,
                                                                const te::gm::Envelope* /*e*/,
                                                                te::gm::SpatialRelation /*r*/,
                                                                te::common::TraverseType travType)
{
  return getDataSet(name, travType);
}

std::auto_ptr<te::da::DataSet>  te::gdal::DataSource::getDataSet(const std::string& name,
                                                                 const std::string& propertyName,
                                                                 const te::gm::Geometry* /*g*/,
                                                                 te::gm::SpatialRelation /*r*/,
                                                                 te::common::TraverseType travType)
{
  return getDataSet(name, travType);
}

std::auto_ptr<te::da::DataSet> te::gdal::DataSource::query(const te::da::Select& /*q*/,
                                                           te::common::TraverseType /*travType*/)
{
  throw(Exception(TR_GDAL("GDAL driver doesn't support queries.")));
}

std::auto_ptr<te::da::DataSet> te::gdal::DataSource::query(const std::string& /*query*/, 
                                                           te::common::TraverseType /*travType*/)
{
  throw(Exception(TR_GDAL("GDAL driver doesn't support queries.")));
}

void te::gdal::DataSource::execute(const te::da::Query& /*command*/)
{
  throw(Exception(TR_GDAL("GDAL driver doesn't support queries.")));
}

void te::gdal::DataSource::execute(const std::string& /*command*/)
{
  throw(Exception(TR_GDAL("GDAL driver doesn't support queries.")));
}

void te::gdal::DataSource::cancel() 
{
  throw(Exception(TR_GDAL("GDAL driver doesn't support queries.")));
}

boost::int64_t te::gdal::DataSource::getLastGeneratedId() 
{ 
  throw(Exception(TR_GDAL("GDAL driver doesn't support id generation.")));
}

std::string te::gdal::DataSource::escape(const std::string& value) 
{ 
  throw(Exception(TR_GDAL("GDAL driver doesn't support string escaping.")));
}

bool te::gdal::DataSource::isDataSetNameValid(const std::string& datasetName)
{
  throw(Exception(TR_GDAL("Not implemented yet.")));
}

bool te::gdal::DataSource::isPropertyNameValid(const std::string& propertyName)
{
  throw(Exception(TR_GDAL("Not implemented yet.")));
}

std::vector<std::string> te::gdal::DataSource::getPropertyNames(const std::string& datasetName)
{
  const te::da::DataSetTypePtr& dt = getDataSetType(datasetName);
  
  std::vector<std::string> pNames;
  
  std::size_t numProperties = dt->size();
  
  for(std::size_t i = 0; i < numProperties; ++i)
    pNames.push_back(dt->getProperty(i)->getName());
  
  return pNames;
}

void te::gdal::DataSource::addProperty(const std::string& datasetName, te::dt::Property* p)
{
    throw Exception(TR_GDAL("Not implemented yet!"));
}

void te::gdal::DataSource::dropProperty(const std::string& datasetName, const std::string& propertyName)
{
  throw(Exception(TR_GDAL("To be implemented.")));
}

void te::gdal::DataSource::renameProperty(const std::string& datasetName,
                                          const std::string& propertyName,
                                          const std::string& newPropertyName)
{
  throw(Exception(TR_GDAL("To be implemented.")));
}

te::da::PrimaryKey* te::gdal::DataSource::getPrimaryKey(const std::string& /*datasetName*/)
{
  throw(Exception(TR_GDAL("GDAL driver doesn't support primary keys.")));
}

bool te::gdal::DataSource::primaryKeyExists(const std::string& /*datasetName*/, const std::string& /*name*/) 
{ 
  throw(Exception(TR_GDAL("GDAL driver doesn't support primary keys.")));
}

void te::gdal::DataSource::addPrimaryKey(const std::string& datasetName, te::da::PrimaryKey* pk)
{
  throw(Exception(TR_GDAL("GDAL driver doesn't support primary keys.")));
}

void te::gdal::DataSource::dropPrimaryKey(const std::string& datasetName)
{
  throw(Exception(TR_GDAL("GDAL driver doesn't support primary keys.")));
}

std::vector<std::string> te::gdal::DataSource::getForeignKeyNames(const std::string& /*datasetName*/)
{
  throw(Exception(TR_GDAL("GDAL driver doesn't support foreign keys.")));
}

bool te::gdal::DataSource::foreignKeyExists(const std::string& /*datasetName*/, const std::string& /*name*/) 
{ 
  throw(Exception(TR_GDAL("GDAL driver doesn't support foreigns keys."))); 
}

te::da::ForeignKey* te::gdal::DataSource::getForeignKey(const std::string& /*datasetName*/, const std::string& /*name*/)
{
  throw(Exception(TR_GDAL("GDAL driver doesn't support foreigns keys.")));
}

void te::gdal::DataSource::addForeignKey(const std::string& datasetName, te::da::ForeignKey* fk)
{
  throw(Exception(TR_GDAL("GDAL driver doesn't support foreigns keys.")));
}

void te::gdal::DataSource::dropForeignKey(const std::string& datasetName, const std::string& fkName)
{
  throw(Exception(TR_GDAL("GDAL driver doesn't support foreigns keys.")));
}

std::vector<std::string> te::gdal::DataSource::getUniqueKeyNames(const std::string& datasetName)
{
  throw(Exception(TR_GDAL("GDAL driver doesn't support unique keys.")));
}

bool te::gdal::DataSource::uniqueKeyExists(const std::string& /*datasetName*/, const std::string& /*name*/) 
{ 
  throw(Exception(TR_GDAL("GDAL driver doesn't support unique keys.")));
}

te::da::UniqueKey* te::gdal::DataSource::getUniqueKey(const std::string& /*datasetName*/, const std::string& /*name*/)
{
  throw(Exception(TR_GDAL("GDAL driver doesn't support unique keys.")));
}

void te::gdal::DataSource::addUniqueKey(const std::string& datasetName, te::da::UniqueKey* uk)
{
  throw(Exception(TR_GDAL("GDAL driver doesn't support unique keys.")));
}

void te::gdal::DataSource::dropUniqueKey(const std::string& datasetName, const std::string& name)
{
  throw(Exception(TR_GDAL("GDAL driver doesn't support unique keys.")));
}

std::vector<std::string> te::gdal::DataSource::getCheckConstraintNames(const std::string& datasetName)
{
  throw(Exception(TR_GDAL("GDAL driver doesn't support constraints.")));
}

bool te::gdal::DataSource::checkConstraintExists(const std::string& /*datasetName*/, const std::string& /*name*/) 
{ 
  throw(Exception(TR_GDAL("GDAL driver doesn't support check constraints."))); 
}

te::da::CheckConstraint* te::gdal::DataSource::getCheckConstraint(const std::string& datasetName, const std::string& name)
{
  throw(Exception(TR_GDAL("GDAL driver doesn't support constraints.")));
}

void te::gdal::DataSource::addCheckConstraint(const std::string& datasetName, te::da::CheckConstraint* cc)
{
}

void te::gdal::DataSource::dropCheckConstraint(const std::string& datasetName, const std::string& name)
{
}

std::vector<std::string> te::gdal::DataSource::getIndexNames(const std::string& /*datasetName*/)
{
  throw(Exception(TR_GDAL("GDAL driver doesn't support unique keys.")));
}

bool te::gdal::DataSource::indexExists(const std::string& /*datasetName*/, const std::string& /*name*/) 
{ 
  throw(Exception(TR_GDAL("GDAL driver doesn't support check indexes."))); 
}

te::da::Index* te::gdal::DataSource::getIndex(const std::string& datasetName, const std::string& name)
{
  throw(Exception(TR_GDAL("GDAL driver doesn't support indexes.")));
}

void te::gdal::DataSource::addIndex(const std::string& datasetName, te::da::Index* idx, const std::map<std::string, std::string>& options)
{
  throw(Exception(TR_GDAL("GDAL driver doesn't support indexes.")));
}

void te::gdal::DataSource::dropIndex(const std::string& datasetName, const std::string& idxName)
{
  throw(Exception(TR_GDAL("GDAL driver doesn't support indexes.")));
}

std::vector<std::string> te::gdal::DataSource::getSequenceNames()
{
  throw(Exception(TR_GDAL("GDAL driver doesn't support sequences.")));
}

bool te::gdal::DataSource::sequenceExists(const std::string& /*name*/) 
{ 
  throw(Exception(TR_GDAL("GDAL driver doesn't support check sequences."))); 
}

te::da::Sequence* te::gdal::DataSource::getSequence(const std::string& name)
{
  throw(Exception(TR_GDAL("GDAL driver doesn't support sequences.")));
}

void te::gdal::DataSource::addSequence(te::da::Sequence* sequence)
{
  throw(Exception(TR_GDAL("GDAL driver doesn't support sequences.")));
}

void te::gdal::DataSource::dropSequence(const std::string& name)
{
  throw(Exception(TR_GDAL("GDAL driver doesn't support sequences.")));
}

std::auto_ptr<te::gm::Envelope> te::gdal::DataSource::getExtent(const std::string& datasetName, const std::string& propertyName)
{
  return std::auto_ptr<te::gm::Envelope>();
}

std::size_t te::gdal::DataSource::getNumberOfItems(const std::string& datasetName)
{
  return 0; // TODO: review
}

void te::gdal::DataSource::createDataSet(te::da::DataSetType* dt,
                                         const std::map<std::string, std::string>& options) 
{
  throw(Exception(TR_GDAL("To be implemented.")));
}

void te::gdal::DataSource::cloneDataSet(const std::string& name,
                                        const std::string& cloneName,
                                        const std::map<std::string, std::string>& options)
{
  throw(Exception(TR_GDAL("To be implemented.")));
}

void te::gdal::DataSource::dropDataSet(const std::string& name)
{
  throw(Exception(TR_GDAL("To be implemented.")));
}

void te::gdal::DataSource::renameDataSet(const std::string& name, const std::string& newName)
{
  throw(Exception(TR_GDAL("To be implemented.")));
}

void te::gdal::DataSource::add(const std::string& datasetName,
                               te::da::DataSet* d,
                               const std::map<std::string, std::string>& options,
                               std::size_t limit)
{
  throw(Exception(TR_GDAL("To be implemented.")));
}

void te::gdal::DataSource::remove(const std::string& datasetName,
                                  const te::da::ObjectIdSet* oids)
{
  throw(Exception(TR_GDAL("To be implemented.")));
}

void te::gdal::DataSource::update(const std::string& datasetName,
                                  te::da::DataSet* dataset,
                                  const std::vector<std::size_t>& properties,
                                  const te::da::ObjectIdSet* oids,
                                  const std::map<std::string, std::string>& options,
                                  std::size_t limit)
{
  throw Exception(TR_GDAL("Not implemented yet!"));
}

void te::gdal::DataSource::optimize(const std::map<std::string, std::string>& /*opInfo*/)
{
  throw Exception(TR_GDAL("Not implemented yet!"));
}

void te::gdal::DataSource::setCapabilities(const te::da::DataSourceCapabilities& capabilities)
{
  sm_capabilities = capabilities;
}

void te::gdal::DataSource::create(const std::map<std::string, std::string>& dsInfo)
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

bool te::gdal::DataSource::exists(const std::map<std::string, std::string>& dsInfo)
{
  std::map<std::string, std::string>::const_iterator it = m_connectionInfo.find("URI");
  
  if(it == m_connectionInfo.end())  // wrong datasource info
    return false;
  
  if (!boost::filesystem::exists(it->second)) // it doesn't exist
      return false;
    
  if (boost::filesystem::is_directory(it->second)) // it is a directory
      return true;
  
  GDALDataset* gds = static_cast<GDALDataset*>(GDALOpen(it->second.c_str(), GA_ReadOnly));
  
  if (gds)
  {
    GDALClose(gds);
    return true;
  }
  
  return false;
}
 
void te::gdal::DataSource::drop(const std::map<std::string, std::string>& dsInfo)
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

std::vector<std::string> te::gdal::DataSource::getDataSourceNames(const std::map<std::string, std::string>& dsInfo)
{
  return std::vector<std::string>();
}

std::vector<std::string> te::gdal::DataSource::getEncodings(const std::map<std::string, std::string>& dsInfo)
{
  return std::vector<std::string>();
}
