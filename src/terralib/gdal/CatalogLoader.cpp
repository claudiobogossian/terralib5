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
  \file terralib/gdal/CatalogLoader.cpp

  \brief It allows to retrieve information about datasets in a GDAL data source.
*/

// TerraLib
#include "../common/StringUtils.h"
#include "../common/Translator.h"
#include "../dataaccess/dataset/DataSetType.h"
#include "../dataaccess/datasource/DataSourceCatalog.h"
#include "../datatype/Property.h"
#include "../datatype/SimpleData.h"
#include "../geometry/Envelope.h"
#include "../raster/RasterProperty.h"
#include "CatalogLoader.h"
#include "DataSourceTransactor.h"
#include "DataSource.h"
#include "Exception.h"
#include "Utils.h"

// GDAL
#include <gdal_priv.h>
#include <ogr_spatialref.h>

// Boost
#include <boost/filesystem.hpp>
#include <boost/format.hpp>

// STL
#include <cassert>

te::gdal::CatalogLoader::CatalogLoader(te::gdal::DataSourceTransactor* t)
  : m_t(t)
{
  assert(t);
}

te::gdal::CatalogLoader::~CatalogLoader()
{
}

void te::gdal::CatalogLoader::getDataSets(boost::ptr_vector<std::string>& datasets)
{
  te::da::DataSourceCatalog* catalog = initCatalog();
  
// retrieve dataset name list
  const std::size_t ndatasets = catalog->getNumberOfDataSets();

  for(std::size_t i = 0; i < ndatasets; ++i)
    datasets.push_back(new std::string(catalog->getDataSetType(i)->getName()));
}

te::da::DataSetType* te::gdal::CatalogLoader::getDataSetType(const std::string& datasetName, const bool /*full*/)
{
  te::da::DataSourceCatalog* catalog = initCatalog();

  const te::da::DataSetTypePtr& dt = catalog->getDataSetType(datasetName);

  if(dt.get() == 0)
    throw Exception((boost::format(TR_GDAL("Dataset %1% doesn't exist!")) % datasetName).str()); 

  return static_cast<te::da::DataSetType*>(dt->clone());
}

te::dt::Property* te::gdal::CatalogLoader::getProperty(const std::string& /*datasetName*/, const std::string& /*propertyName*/)
{
  throw Exception(TR_GDAL("Not implemented yet!"));
}

te::gm::Envelope* te::gdal::CatalogLoader::getExtent(const te::dt::Property* /*sp*/)
{
  return 0;
}

void te::gdal::CatalogLoader::loadCatalog(const bool /*full*/)
{
  DataSource* ds = m_t->getGDALDataSource();

  ds->getCatalog()->clear();

  std::vector<te::da::DataSetType*> dts;

  if(ds->isDirectory())
  {
    boost::filesystem::path path(GetGDALConnectionInfo(ds->getConnectionInfo()));

    for(boost::filesystem::directory_iterator it(path), itEnd; it != itEnd; ++it)
    {
      boost::filesystem::path foundFile = (*it);

      GDALDataset* dataset = static_cast<GDALDataset*>(GDALOpen(foundFile.string().c_str(), GA_ReadOnly));

      if(dataset == 0)
        continue;

      getDataSetTypeList(dataset, dts);

      GDALClose(dataset);
    }
  }
  else
  {
    GDALDataset* dataset = static_cast<GDALDataset*>(GDALOpen(GetGDALConnectionInfo(ds->getConnectionInfo()).c_str(), GA_ReadOnly));

    if(dataset == 0)
      return;

    getDataSetTypeList(dataset, dts);
    
    GDALClose(dataset);
  }

  for(std::size_t i = 0; i < dts.size(); ++i)
  {
    te::da::DataSetTypePtr dt(dts[i]);
    dt->setId(static_cast<unsigned int>(i));
    ds->getCatalog()->add(dt);
  }  
}

bool te::gdal::CatalogLoader::hasDataSets()
{
  DataSource* ds = m_t->getGDALDataSource();
  
  if (ds->isDirectory())
  {
    boost::filesystem::path path(GetGDALConnectionInfo(ds->getConnectionInfo()));
    
    for(boost::filesystem::directory_iterator it(path), itEnd; it != itEnd; ++it)
    {
      boost::filesystem::path foundFile = (*it);
      
      GDALDataset* dataset = static_cast<GDALDataset*>(GDALOpen(foundFile.string().c_str(), GA_ReadOnly));
      
      if(dataset == 0)
        continue;
      
      GDALClose(dataset);
      
      return true;
    }
    return false;
  }
  else
  {
    GDALDataset* dataset = static_cast<GDALDataset*>(GDALOpen(GetGDALConnectionInfo(ds->getConnectionInfo()).c_str(), GA_ReadOnly));
    
    if(dataset == 0)
      return false;
    
    GDALClose(dataset); 
    
    return true;
  }
}

te::da::DataSourceTransactor* te::gdal::CatalogLoader::getTransactor() const
{
  return m_t;
}

bool te::gdal::CatalogLoader::datasetExists(const std::string& name)
{
  GDALDataset* dataset;

  DataSource* ds = m_t->getGDALDataSource();

  if(ds->isDirectory())
  {
    boost::filesystem::path path(GetGDALConnectionInfo(ds->getConnectionInfo()));

    path /= name;

    dataset = static_cast<GDALDataset*>(GDALOpen(path.string().c_str(), GA_ReadOnly));
  }
  else
    dataset = static_cast<GDALDataset*>(GDALOpen(name.c_str(), GA_ReadOnly));

  if(dataset == 0)
    return false;

  GDALClose(dataset);

  return true;
}

void te::gdal::CatalogLoader::getPrimaryKey(te::da::DataSetType* /*dt*/)
{
  throw Exception(TR_GDAL("GDAL driver does not support the concept of primary key."));
}

void te::gdal::CatalogLoader::getUniqueKeys(te::da::DataSetType* /*dt*/)
{
  throw Exception(TR_GDAL("GDAL driver does not support the concept of unique key."));
}

void te::gdal::CatalogLoader::getForeignKeys(te::da::DataSetType* /*dt*/, std::vector<std::string>& /*fkNames*/, std::vector<std::string>& /*rdts*/)
{
  throw Exception(TR_GDAL("GDAL driver does not support the concept of foreign key."));
}

te::da::ForeignKey* te::gdal::CatalogLoader::getForeignKey(const std::string& /*fkName*/, te::da::DataSetType* /*dt*/, te::da::DataSetType* /*rdt*/)
{
  throw Exception(TR_GDAL("GDAL driver does not support the concept of foreign key."));
}

void te::gdal::CatalogLoader::getIndexes(te::da::DataSetType* /*dt*/)
{
  throw Exception(TR_GDAL("GDAL driver does not support the concept of index."));
}

void te::gdal::CatalogLoader::getCheckConstraints(te::da::DataSetType* /*dt*/)
{
  throw Exception(TR_GDAL("GDAL driver does not support the concept of check constraints."));
}

void te::gdal::CatalogLoader::getSequences(std::vector<std::string*>& /*sequences*/) 
{
  throw Exception(TR_GDAL("GDAL driver does not support the concept of sequence."));
}

te::da::Sequence* te::gdal::CatalogLoader::getSequence(const std::string& /*seqName*/) 
{
  throw Exception(TR_GDAL("GDAL driver does not support the concept of sequence."));
}

void te::gdal::CatalogLoader::getDataSetTypeList(GDALDataset* dataset, std::vector<te::da::DataSetType*>& datasets)
{
  assert(dataset);

  char** subdatasets = dataset->GetMetadata("SUBDATASETS");

  if(subdatasets == 0)
  {
    te::da::DataSetType* dt = getDataSetType(dataset);

    datasets.push_back(dt);

    return;
  }

  for(char** i = subdatasets; *i != 0; ++i)
  {
    std::map<std::string, std::string> sdsmap;

    te::common::ExtractKVP(std::string(*i), sdsmap);

    if(sdsmap.begin()->first.find("_NAME") != std::string::npos)
    {
      std::string fullName = sdsmap.begin()->second;

      GDALDataset* subdataset = static_cast<GDALDataset*>(GDALOpen(fullName.c_str(), GA_ReadOnly));

      te::da::DataSetType* dt = getDataSetType(subdataset);

      std::string sdsName = te::gdal::GetSubDataSetName(fullName, te::gdal::GetDriverName(fullName));

      dt->setName(sdsName + dt->getName());

      datasets.push_back(dt);

      GDALClose(subdataset);
    }
    else if(sdsmap.begin()->first.find("_DESC") != std::string::npos)
      (*datasets.rbegin())->setTitle(sdsmap.begin()->second);
  }
}

void te::gdal::CatalogLoader::getProperties(te::da::DataSetType* dt)
{
  assert(dt);

  GDALDataset* dataset = static_cast<GDALDataset*>(GDALOpen(dt->getName().c_str(), GA_ReadOnly));

  if (!dataset)
    throw Exception(TR_GDAL("GDAL couldn't retrieve the dataset properties."));

  te::rst::Grid* grid = GetGrid(dataset);

  std::vector<te::rst::BandProperty*> bprops;

  GetBandProperties(dataset, bprops);

  std::map<std::string, std::string> rinfo;

  rinfo["URI"] = dataset->GetDescription();
  
  te::rst::RasterProperty* rp = new te::rst::RasterProperty(grid, bprops, rinfo);

  dt->add(rp);

  dt->setDefaultRasterProperty(rp);

  dt->setFullLoaded(true);

  GDALClose(dataset);
}

te::da::DataSetType* te::gdal::CatalogLoader::getDataSetType(GDALDataset* dataset)
{
  te::rst::Grid* grid = GetGrid(dataset);

  std::vector<te::rst::BandProperty*> bprops;

  GetBandProperties(dataset, bprops);

  std::map<std::string, std::string> rinfo;

  rinfo["URI"] = dataset->GetDescription();

  te::rst::RasterProperty* rp = new te::rst::RasterProperty(grid, bprops, rinfo);

  boost::filesystem::path rpath (rinfo["URI"]);

  te::da::DataSetType* dt = new te::da::DataSetType(rpath.filename().string());

  dt->add(rp);

  dt->setDefaultRasterProperty(rp);

  dt->setFullLoaded(true);

  return dt;
}

te::da::DataSourceCatalog* te::gdal::CatalogLoader::initCatalog()
{
  te::da::DataSourceCatalog* catalog = m_t->getGDALDataSource()->getCatalog();

  if(catalog->getNumberOfDataSets() == 0)
    loadCatalog(true);

  return catalog;
}
