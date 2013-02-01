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
  \file terralib/ogr/CatalogLoader.cpp

  \brief It allows to retrieve information about datasets in a OGR data source.
 */

// TerraLib
#include "../common/Exception.h"
#include "../common/Translator.h"
#include "../dataaccess/dataset/DataSetType.h"
#include "../dataaccess/dataset/CheckConstraint.h"
#include "../dataaccess/dataset/ForeignKey.h"
#include "../dataaccess/dataset/PrimaryKey.h"
#include "../dataaccess/dataset/UniqueKey.h"
#include "../dataaccess/dataset/Sequence.h"
#include "../dataaccess/datasource/DataSourceCatalog.h"
#include "../geometry/GeometryProperty.h"
#include "CatalogLoader.h"
#include "DataSet.h"
#include "DataSource.h"
#include "DataSourceTransactor.h"
#include "Utils.h"

// OGR
#include <ogrsf_frmts.h>

// STL
#include <cassert>

te::ogr::CatalogLoader::CatalogLoader(DataSourceTransactor* t)
  : m_t(t)
{}

te::ogr::CatalogLoader::~CatalogLoader()
{}

void te::ogr::CatalogLoader::getDataSets(boost::ptr_vector<std::string>& datasets)
{
  OGRDataSource* ogrDS = m_t->getOGRDataSource();
  int nLayers = ogrDS->GetLayerCount(); 
  for(int i = 0; i < nLayers; i++) // for each layer
  {
    OGRLayer* layer = ogrDS->GetLayer(i);
    std::string* n = new std::string(layer->GetLayerDefn()->GetName());
    datasets.push_back(n);
  }
}

te::da::DataSetType* te::ogr::CatalogLoader::getDataSetType(const std::string& datasetName, const bool full)
{
  OGRDataSource* ogrDS = m_t->getOGRDataSource();
  OGRLayer* layer = ogrDS->GetLayerByName(datasetName.c_str());
 
  te::da::DataSetType* dt = Convert2TerraLib(layer->GetLayerDefn());
  if(dt->hasGeom())
  {
    OGRSpatialReference* osrs = layer->GetSpatialRef();
    if(osrs)
      dt->getDefaultGeomProperty()->setSRID(Convert2TerraLibProjection(osrs));
  }

  if(full)
  {
    const char* colIdName = layer->GetFIDColumn();
    if(colIdName && colIdName[0] != '\0')
    {
      int pos = layer->GetLayerDefn()->GetFieldIndex(colIdName);
      if(pos > 0)
      {
        te::da::PrimaryKey* pk = new te::da::PrimaryKey(colIdName, dt);
        pk->add(dt->getProperty(pos));
      }
    }

    if(dt->hasGeom())
    {
      te::gm::Envelope* env = getExtent(dt->getDefaultGeomProperty());
      dt->getDefaultGeomProperty()->setExtent(env);
    }
  }

  return dt;
}

void te::ogr::CatalogLoader::getProperties(te::da::DataSetType* dt)
{
  OGRDataSource* ogrDS = m_t->getOGRDataSource();
  OGRLayer* layer = ogrDS->GetLayerByName(dt->getName().c_str());
  
  Convert2TerraLib(layer->GetLayerDefn(),dt);
}

te::dt::Property* te::ogr::CatalogLoader::getProperty(const std::string& /*datasetName*/, const std::string& /*propertyName*/)
{
  throw te::common::Exception(TR_OGR("Not implemented yet!"));
}

void te::ogr::CatalogLoader::getPrimaryKey(te::da::DataSetType* dt)
{
  OGRDataSource* ogrDS = m_t->getOGRDataSource();
  OGRLayer* layer = ogrDS->GetLayerByName(dt->getName().c_str());
  const char* colIdName = layer->GetFIDColumn();
  if(colIdName && colIdName[0] != '\0')
  {
    int pos = layer->GetLayerDefn()->GetFieldIndex(colIdName);
    if(pos > 0)
    {
      te::da::PrimaryKey* pk = new te::da::PrimaryKey(colIdName, dt);
      pk->add(dt->getProperty(pos));
      return;
    }
  }
  // No primary key found
  dt->setPrimaryKey(0);
}

void te::ogr::CatalogLoader::getUniqueKeys(te::da::DataSetType* /*dt*/)
{}

void te::ogr::CatalogLoader::getForeignKeys(te::da::DataSetType* /*dt*/,
                                            std::vector<std::string>& /*fkNames*/,
                                            std::vector<std::string>& /*rdts*/)
{}

te::da::ForeignKey* te::ogr::CatalogLoader::getForeignKey(const std::string& /*fkName*/,
                                                          te::da::DataSetType* /*dt*/,
                                                          te::da::DataSetType* /*rdt*/)
{
  return 0;
}

void te::ogr::CatalogLoader::getIndexes(te::da::DataSetType* /*dt*/)
{}

void te::ogr::CatalogLoader::getCheckConstraints(te::da::DataSetType* /*dt*/)
{}

void te::ogr::CatalogLoader::getSequences(std::vector<std::string*>& /*sequences*/)
{}

te::da::Sequence* te::ogr::CatalogLoader::getSequence(const std::string& /*seqName*/)
{
  return 0;
}

te::gm::Envelope* te::ogr::CatalogLoader::getExtent(const te::gm::GeometryProperty* gp)
{
  assert(gp->getParent());

  OGRDataSource* ogrDS = m_t->getOGRDataSource();
  OGRLayer* layer = ogrDS->GetLayerByName(gp->getParent()->getName().c_str());

  OGREnvelope* envOGR = new OGREnvelope();

  if(!(layer->GetExtent(envOGR) == OGRERR_NONE))
  {
    delete envOGR;
    throw(te::common::Exception(TR_OGR("Error when attempting get extent.")));
  }

  te::gm::Envelope* envTL = Convert2TerraLib(envOGR);
  delete envOGR;

  return envTL;
}

void te::ogr::CatalogLoader::loadCatalog(const bool full)
{  
  te::da::DataSourceCatalog* catalog = m_t->getDataSource()->getCatalog();
  catalog->clear();
  
  OGRDataSource* ogrDS = m_t->getOGRDataSource();
  for(int i = 0; i < ogrDS->GetLayerCount(); i++) // for each layer
  {
    OGRLayer* layer = ogrDS->GetLayer(i);
    te::da::DataSetTypePtr dt(getDataSetType(layer->GetLayerDefn()->GetName(), full));
    dt->setId(i);
    catalog->add(dt);
  }
}

bool te::ogr::CatalogLoader::hasDataSets()
{
  OGRDataSource* ogrDS = m_t->getOGRDataSource();
  return (ogrDS->GetLayerCount() > 0); 
}

te::da::DataSourceTransactor* te::ogr::CatalogLoader::getTransactor() const
{
  return m_t;
}
