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
  \file terralib/gdal/Transactor.cpp
*/

// TerraLib
#include "../common/StringUtils.h"
#include "../common/Translator.h"
#include "../dataaccess2/dataset/DataSetType.h"
#include "../geometry/Envelope.h"
#include "../raster/Grid.h"
#include "../raster/Raster.h"
#include "../raster/RasterProperty.h"
#include "DataSet.h"
#include "DataSource.h"
#include "Exception.h"
#include "Transactor.h"
#include "Utils.h"

// GDAL
#include <gdal_priv.h>

// Boost
#include <boost/filesystem.hpp>
#include <boost/format.hpp>

void te::gdal::Transactor::getMetadata(const std::string& dsfullname, const std::string& dsname)
{
  assert(!dsfullname.empty());
  assert(!dsname.empty());
  
  GDALDataset* ds = static_cast<GDALDataset*>(GDALOpen(dsfullname.c_str(), GA_ReadOnly));
  if (!ds)
    return;
  
  std::pair<std::string, std::string> p1 = std::make_pair(dsname,dsfullname);
  m_locations.insert(p1);
  
  te::da::DataSetTypePtr ptr(new te::da::DataSetType(dsname, m_lastId));
  ++m_lastId;
  ptr->setTitle("raster");
  
  te::rst::Grid* grid = GetGrid(ds);
  
  std::vector<te::rst::BandProperty*> bprops;
  
  GetBandProperties(ds, bprops);
  
  te::rst::RasterProperty* rp = new te::rst::RasterProperty("raster");
  rp->set(grid);
  for (size_t i=0; i<bprops.size(); ++i)
    rp->add(bprops[i]);

  ptr->add(rp);     
  
  GDALClose(ds);
  
  std::pair<std::string, te::da::DataSetTypePtr> p2 = std::make_pair(dsname,ptr);
  m_catalog.insert(p2);
}

void te::gdal::Transactor::init(const boost::filesystem::path& path)
{
  if (boost::filesystem::is_regular_file(path))
  {
    GDALDataset* gds = static_cast<GDALDataset*>(GDALOpen(path.c_str(), GA_ReadOnly));
    if (!gds)
      return;
    
    char** subdatasets = gds->GetMetadata("SUBDATASETS");
    if(subdatasets == 0)
    {
      GDALClose(gds);
      getMetadata(path.string(),path.leaf().string());
      return;
    }
    
    for(char** i = subdatasets; *i != 0; ++i)
    {
      std::map<std::string, std::string> sdsmap;
      
      te::common::ExtractKVP(std::string(*i), sdsmap);
      
      if(sdsmap.begin()->first.find("_NAME") != std::string::npos)
      {
        std::string fullName = sdsmap.begin()->second;
        std::string subdsname = GetSubDataSetName(fullName, te::gdal::GetDriverName(path.string()));
        getMetadata(fullName,subdsname);
      }
    }
    GDALClose(gds);
  }
  else 
  {
    for (boost::filesystem::directory_iterator it(path), itEnd; it != itEnd; ++it)
      init(*it);
  }
}

te::gdal::Transactor::Transactor(const std::string& accessInfo) :
  m_lastId(0)
{
  m_path = boost::filesystem::path(accessInfo);
  init(m_path);
}

te::gdal::Transactor::~Transactor()
{
  m_locations.clear();
  m_catalog.clear();
}

te::da::DataSource* te::gdal::Transactor::getDataSource() const
{
  return 0;
}

// TODO: review
bool te::gdal::Transactor::isDataSetNameValid(const std::string& datasetName)
{
  return true;
}

// TODO: review
bool te::gdal::Transactor::isPropertyNameValid(const std::string& propertyName)
{
  return true;
}

std::vector<std::string> te::gdal::Transactor::getDataSetNames()
{
  std::vector<std::string> dsnames;
  
  std::map<std::string, std::string>::const_iterator it = m_locations.begin();
  while (it!=m_locations.end())
  {
    dsnames.push_back(it->first);
    ++it;
  }
  
  return dsnames;
}

bool te::gdal::Transactor::hasDataSets()
{
  return !m_locations.empty();
}

std::size_t te::gdal::Transactor::getNumberOfDataSets()
{
  return m_locations.size();
}

te::da::DataSetTypePtr te::gdal::Transactor::getDataSetType(const std::string& name)
{
  assert(!name.empty());
  
  std::map<std::string, te::da::DataSetTypePtr>::const_iterator it = m_catalog.find(name);
  if(it != m_catalog.end())
    return it->second;
  
  return boost::shared_ptr<te::da::DataSetType>();
}


std::auto_ptr<te::da::DataSet> te::gdal::Transactor::getDataSet(const std::string& name, 
                                                                te::common::TraverseType travType, 
                                                                bool /*connected*/) 
{
  assert(!name.empty());
  
  std::map<std::string, te::da::DataSetTypePtr>::const_iterator it = m_catalog.find(name);
  if(it != m_catalog.end())
    return std::auto_ptr<te::da::DataSet>(new DataSet(static_cast<te::da::DataSetType*>(it->second->clone()),m_locations[name])); 
  
  return std::auto_ptr<te::da::DataSet>();
}

std::auto_ptr<te::da::DataSet>  te::gdal::Transactor::getDataSet(const std::string& name,
                                          const std::string& /*propertyName*/,
                                          const te::gm::Envelope* /*e*/,
                                          te::gm::SpatialRelation /*r*/,
                                          te::common::TraverseType travType, 
                                          bool /*connected*/)
{
  return getDataSet(name,travType);
}

std::auto_ptr<te::da::DataSet> te::gdal::Transactor::getDataSet(const std::string& name,
                                                                const std::string& /*propertyName*/,
                                                                const te::gm::Geometry* /*g*/,
                                                                te::gm::SpatialRelation /*r*/,
                                                                te::common::TraverseType travType, 
                                                                bool /*connected*/)
{
  return getDataSet(name,travType);
}

std::auto_ptr<te::da::DataSet> te::gdal::Transactor::getDataSet(const std::string& name,
                                                                const te::da::ObjectIdSet* /*oids*/, 
                                                                te::common::TraverseType travType, 
                                                                bool /*connected*/)
{
  return getDataSet(name,travType);
}


boost::ptr_vector<te::dt::Property> te::gdal::Transactor::getProperties(const std::string& datasetName)
{
  assert(!datasetName.empty());
  
  boost::ptr_vector<te::dt::Property> properties;
  
  std::map<std::string, te::da::DataSetTypePtr>::const_iterator it = m_catalog.find(datasetName);
  if(it != m_catalog.end())
  {
    std::vector<te::dt::Property*>& dtProperties = it->second->getProperties();

    for(std::size_t i = 0; i < dtProperties.size(); ++i)
      properties.push_back(dtProperties[i]->clone());
  }
  
  return properties;
}

std::auto_ptr<te::dt::Property> te::gdal::Transactor::getProperty(const std::string& datasetName, const std::string& name)
{
  assert(!datasetName.empty());
  
  std::map<std::string, te::da::DataSetTypePtr>::const_iterator it = m_catalog.find(datasetName);
  if(it != m_catalog.end())
    return std::auto_ptr<te::dt::Property>((it->second->getProperty(name))->clone());

  return std::auto_ptr<te::dt::Property>();
}

std::auto_ptr<te::dt::Property> te::gdal::Transactor::getProperty(const std::string& datasetName, std::size_t propertyPos)
{
  assert(!datasetName.empty());
  
  std::map<std::string, te::da::DataSetTypePtr>::const_iterator it = m_catalog.find(datasetName);
  if(it != m_catalog.end())
    return std::auto_ptr<te::dt::Property>((it->second->getProperty(propertyPos))->clone());
  
  return std::auto_ptr<te::dt::Property>();
}

std::vector<std::string> te::gdal::Transactor::getPropertyNames(const std::string& datasetName)
{
  assert(!datasetName.empty());
  
  std::vector<std::string> pNames;
  
  std::map<std::string, te::da::DataSetTypePtr>::const_iterator it = m_catalog.find(datasetName);
  if(it != m_catalog.end())
  {
    for(std::size_t i = 0; i < it->second->size(); ++i)
      pNames.push_back(it->second->getProperty(i)->getName());
  }
  
  return pNames;
}

std::size_t te::gdal::Transactor::getNumberOfProperties(const std::string& datasetName)
{
  assert(!datasetName.empty());
  
  std::map<std::string, te::da::DataSetTypePtr>::const_iterator it = m_catalog.find(datasetName);
  if(it != m_catalog.end())
    return it->second->size();
  
  return 0;
}

bool te::gdal::Transactor::propertyExists(const std::string& datasetName, const std::string& name)
{
  assert(!datasetName.empty());

  std::map<std::string, te::da::DataSetTypePtr>::const_iterator it = m_catalog.find(datasetName);
  if(it != m_catalog.end())
  {
    for(std::size_t i = 0; i < it->second->size(); ++i)
      if (it->second->getProperty(i)->getName() == name)
        return true;
  }
  
  return false;
}

bool te::gdal::Transactor::propertyExists(const std::string& datasetName, size_t propertyPos)
{
  assert(!datasetName.empty());
  assert(propertyPos >=0 );
  
  std::map<std::string, te::da::DataSetTypePtr>::const_iterator it = m_catalog.find(datasetName);
  return (it != m_catalog.end() && propertyPos < it->second->size());
}

std::auto_ptr<te::da::DataSet> te::gdal::Transactor::query(const te::da::Select& ,
                                     te::common::TraverseType , 
                                     bool)
{
  return std::auto_ptr<te::da::DataSet>();
}

std::auto_ptr<te::da::DataSet> te::gdal::Transactor::query(const std::string& ,
                             te::common::TraverseType, 
                             bool)
{
  return std::auto_ptr<te::da::DataSet>();
}

bool te::gdal::Transactor::dataSetExists(const std::string& name)
{
  assert(!name.empty());
  
  std::map<std::string, std::string>::const_iterator it = m_locations.find(name);
  if(it == m_locations.end())
    return false;
  
  GDALDataset* dataset = static_cast<GDALDataset*>(GDALOpen(it->second.c_str(), GA_ReadOnly));
  
  if(dataset == 0)
    return false;
  
  GDALClose(dataset);
  return true;
}

std::auto_ptr<te::gm::Envelope> te::gdal::Transactor::getExtent(const std::string& datasetName, std::size_t propertyPos)
{
  assert(!datasetName.empty());
  
  std::auto_ptr<te::dt::Property> pp = getProperty(datasetName,propertyPos);
  
  if (pp.get())
  {
    te::rst::RasterProperty* rp = static_cast<te::rst::RasterProperty*>(pp.get());
    te::gm::Envelope* env = rp->getGrid()->getExtent();
    return std::auto_ptr<te::gm::Envelope>(new te::gm::Envelope(env->getLowerLeftX(), env->getLowerLeftY(),
                                                                env->getUpperRightX(), env->getUpperRightY()));
  }
  
  return std::auto_ptr<te::gm::Envelope>();
}

//TODO review
std::auto_ptr<te::gm::Envelope> te::gdal::Transactor::getExtent(const std::string& datasetName, const std::string& propertyName)
{
  std::auto_ptr<te::dt::Property> pp = getProperty(datasetName,propertyName);
  
  if (pp.get())
  {
    te::rst::RasterProperty* rp = static_cast<te::rst::RasterProperty*>(pp.get());
    te::gm::Envelope* env = rp->getGrid()->getExtent();
    return std::auto_ptr<te::gm::Envelope>(new te::gm::Envelope(env->getLowerLeftX(), env->getLowerLeftY(),
                                                                env->getUpperRightX(), env->getUpperRightY()));
  }
  
  return std::auto_ptr<te::gm::Envelope>();
}

void te::gdal::Transactor::createDataSet(te::da::DataSetType* dt,
                                         const std::map<std::string, std::string>& options) 
{ 
  if (!boost::filesystem::is_directory(m_path))
    throw Exception(TR_GDAL("Create operation supported just on directory data sources."));
      
  te::rst::RasterProperty* rstp = static_cast<te::rst::RasterProperty*>(dt->getProperty(0));
  
  boost::filesystem::path mpath(m_path);
  mpath /= dt->getName();
  
  GDALDataset* gds = te::gdal::CreateRaster(mpath.string(), rstp->getGrid(), rstp->getBandProperties(),options);
  
  if (!gds)
    throw Exception(TR_GDAL("GDAL driver couldn't persist the raster file."));
  
  GDALClose(gds);
  
  dt->setId(m_locations.size());
  
  std::pair<std::string, te::da::DataSetTypePtr> p1 = std::make_pair(dt->getName(),dt);
  m_catalog.insert(p1);
  
  std::pair<std::string, std::string> p2 = std::make_pair(dt->getName(),mpath.string());
  m_locations.insert(p2);
}

void te::gdal::Transactor::cloneDataSet(const std::string& name,
                                        const std::string& cloneName,
                                        const std::map<std::string, std::string>& options)
{
  assert(!name.empty());
  assert(!cloneName.empty());
  
  std::map<std::string, std::string>::const_iterator it = m_locations.find(name);
  if(it == m_locations.end())
    return;
  
  std::map<std::string, te::da::DataSetTypePtr>::const_iterator it2 = m_catalog.find(name);
  if(it2 == m_catalog.end())
    return;
  
  boost::filesystem::path mpath(it->second);
  if (!boost::filesystem::is_regular_file(m_path))
    throw Exception(TR_GDAL("Can not clone a dataset that it is not a raster file."));
  
  boost::filesystem::path newpath(m_path.parent_path() /= cloneName);
  boost::filesystem::copy_file(mpath, newpath);
  
  te::da::DataSetType* newdst = new te::da::DataSetType(cloneName,m_lastId);
  ++m_lastId;
  newdst->setTitle(cloneName);
  
  std::pair<std::string, te::da::DataSetTypePtr> p1 = std::make_pair(cloneName,te::da::DataSetTypePtr(newdst));
  m_catalog.insert(p1);
  
  std::pair<std::string, std::string> p2 = std::make_pair(cloneName,newpath.string());
  m_locations.insert(p2);  
}

void te::gdal::Transactor::dropDataSet(const std::string& name)
{
  assert(!name.empty());
  
  std::map<std::string, std::string>::iterator it = m_locations.find(name);
  if(it == m_locations.end())
    return;
  
  boost::filesystem::path mpath(it->second);
  if (!boost::filesystem::is_regular_file(m_path))
    throw Exception(TR_GDAL("Can not clone a dataset that it is not a raster file."));

  boost::filesystem::remove(mpath.string());
  m_locations.erase(it,it);
  
  std::map<std::string, te::da::DataSetTypePtr>::iterator it2 = m_catalog.find(name);
  m_catalog.erase(it2,it2);  
}

void te::gdal::Transactor::renameDataSet(const std::string& name, const std::string& newName)
{
  std::map<std::string, std::string>::iterator it = m_locations.find(name);
  if(it == m_locations.end())
    return;
  
  std::map<std::string, te::da::DataSetTypePtr>::iterator it2 = m_catalog.find(name);
  if(it2 == m_catalog.end())
    return;
  
  boost::filesystem::path mpath(it->second);
  if (!boost::filesystem::is_regular_file(m_path))
    throw Exception(TR_GDAL("Can not rename a dataset that it is not a raster file."));
  
  boost::filesystem::path newpath(m_path.parent_path() /= newName);
  boost::filesystem::rename(mpath, newpath);
  
  m_locations.erase(it,it);
  std::pair<std::string, std::string> p1 = std::make_pair(newName,newpath.string());
  m_locations.insert(p1);
  
  te::da::DataSetType* newdst = new te::da::DataSetType(newName,it2->second->getId());
  newdst->setTitle(newName);
  
  m_catalog.erase(it2,it2);
  std::pair<std::string, te::da::DataSetTypePtr> p2 = std::make_pair(newName,te::da::DataSetTypePtr(newdst));
  m_catalog.insert(p2);
}

