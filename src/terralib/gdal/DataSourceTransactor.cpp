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
  \file DataSourceTransactor.cpp

  \brief Transactor class for GDAL data provider.
*/

// TerraLib
#include "../common/Translator.h"
#include "../dataaccess/dataset/DataSetType.h"
#include "../dataaccess/datasource/DataSourceCatalog.h"
#include "../dataaccess/datasource/DataSourceCatalogLoader.h"
#include "../geometry/Envelope.h"
#include "../raster/Grid.h"
#include "../raster/RasterProperty.h"
#include "CatalogLoader.h"
#include "DataSet.h"
#include "DataSetPersistence.h"
#include "DataSetTypePersistence.h"
#include "DataSource.h"
#include "DataSourceTransactor.h"
#include "Exception.h"

// STL
#include <cassert>

te::gdal::DataSourceTransactor::DataSourceTransactor(te::gdal::DataSource* ds)
  : m_ds(ds)
{
  assert(m_ds);
}

te::gdal::DataSourceTransactor::~DataSourceTransactor()
{
}

void te::gdal::DataSourceTransactor::begin()
{
  throw Exception(TR_GDAL("GDAL Transactor doesn't support transactions to group operations."));
}

void te::gdal::DataSourceTransactor::commit()   
{
  throw Exception(TR_GDAL("GDAL Transactor doesn't support transactions to group operations."));
}

void te::gdal::DataSourceTransactor::rollBack() 
{
  throw Exception(TR_GDAL("GDAL Transactor doesn't support transactions to group operations."));
}

bool te::gdal::DataSourceTransactor::isInTransaction() const
{
  throw Exception(TR_GDAL("GDAL Transactor doesn't support transactions to group operations."));
}

te::da::DataSet* te::gdal::DataSourceTransactor::getDataSet(const std::string& name,
                                                            te::common::TraverseType /*travType*/, 
                                                            te::common::AccessPolicy rwRole) 
{
  te::da::DataSetType* dt = m_ds->getCatalog()->getDataSetType(name);

  if(dt)
    return new DataSet(this, static_cast<te::da::DataSetType*>(dt->clone()), rwRole);  // return a full loaded dataset type
  
// create a datset type with basic raster info: just type and name!
  dt = new te::da::DataSetType(name);

  te::rst::Grid* grid = new te::rst::Grid();

  std::vector<te::rst::BandProperty*> bprops;

  std::map<std::string, std::string> rinfo;

  te::rst::RasterProperty* rp = new te::rst::RasterProperty(grid, bprops, rinfo);
  
  dt->add(rp);

  return new DataSet(this, dt, rwRole);  // return a full loaded dataset type
}

te::da::DataSet* 
te::gdal::DataSourceTransactor::getDataSet(const std::string& name,
                                           const te::dt::Property* /*p*/,
                                           const te::gm::Envelope* /*e*/,
                                           te::gm::SpatialRelation /*r*/,
                                           te::common::TraverseType travType, 
                                           te::common::AccessPolicy rwRole)
{ 
  return getDataSet(name, travType, rwRole);
}

te::da::DataSet* 
te::gdal::DataSourceTransactor::getDataSet(const std::string& name,
                                           const te::dt::Property* /*p*/,
                                           const te::gm::Geometry* /*g*/,
                                           te::gm::SpatialRelation /*r*/,
                                           te::common::TraverseType travType, 
                                           te::common::AccessPolicy rwRole)
{ 
  return getDataSet(name, travType, rwRole);
}


te::da::DataSet* 
te::gdal::DataSourceTransactor::query(const te::da::Select& /*q*/, 
                                      te::common::TraverseType /*travType = te::common::FORWARDONLY*/, 
                                      te::common::AccessPolicy /*rwRole = te::common::RAccess*/) 
{ 
  throw(Exception(TR_GDAL("GDAL Transactor doesn't support queries.")));
}

te::da::DataSet* 
te::gdal::DataSourceTransactor::query(const std::string& /*query*/, 
                                      te::common::TraverseType /*travType = te::common::FORWARDONLY*/, 
                                      te::common::AccessPolicy /*rwRole = te::common::RAccess*/) 
{ 
  throw(Exception(TR_GDAL("GDAL Transactor doesn't support queries.")));
}

void te::gdal::DataSourceTransactor::execute(const te::da::Query& /*command*/)
{ 
  throw(Exception(TR_GDAL("GDAL Transactor doesn't support queries.")));
}

void te::gdal::DataSourceTransactor::execute(const std::string& /*command*/)
{ 
  throw(Exception(TR_GDAL("GDAL Transactor doesn't support queries.")));
}

void te::gdal::DataSourceTransactor::cancel()
{ 
  throw(Exception(TR_GDAL("GDAL Transactor doesn't support cancel commands.")));
}

te::da::DataSourceCatalogLoader* te::gdal::DataSourceTransactor::getCatalogLoader()
{
 return new te::gdal::CatalogLoader(this);
}

te::da::BatchExecutor* te::gdal::DataSourceTransactor::getBatchExecutor()
{ 
  throw Exception(TR_GDAL("GDAL Transactor doesn't support batch commands."));
}

te::da::PreparedQuery* te::gdal::DataSourceTransactor::getPrepared(const std::string& /*qName = std::string("")*/) 
{ 
  throw Exception(TR_GDAL("GDAL Transactor doesn't support batch commands."));
}

te::da::DataSetTypePersistence* te::gdal::DataSourceTransactor::getDataSetTypePersistence()
{
  return new te::gdal::DataSetTypePersistence(this);
}

te::da::DataSetPersistence* te::gdal::DataSourceTransactor::getDataSetPersistence()
{
  return new te::gdal::DataSetPersistence(this);;
}

te::da::DataSource* te::gdal::DataSourceTransactor::getDataSource() const
{
  return m_ds;
}

te::gdal::DataSource* te::gdal::DataSourceTransactor::getGDALDataSource() const
{
  return m_ds;
}
