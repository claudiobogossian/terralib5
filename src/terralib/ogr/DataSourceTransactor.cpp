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
  \file terralib/ogr/Transactor.cpp

  \brief Transactor class for OGR data provider.  
 */

// TerraLib
#include "../common/Exception.h"
#include "../common/Translator.h"
#include "../common/StringUtils.h"
#include "../dataaccess/dataset/DataSetType.h"
#include "../dataaccess/dataset/CheckConstraint.h"
#include "../dataaccess/dataset/ForeignKey.h"
#include "../dataaccess/dataset/Index.h"
#include "../dataaccess/dataset/PrimaryKey.h"
#include "../dataaccess/dataset/Sequence.h"
#include "../dataaccess/dataset/UniqueKey.h"
#include "../dataaccess/datasource/DataSourceCatalog.h"
#include "../datatype/ArrayProperty.h"
#include "../geometry/Envelope.h"
#include "../geometry/Geometry.h"
#include "CatalogLoader.h"
#include "DataSet.h"
#include "DataSource.h"
#include "DataSourceTransactor.h"
#include "DataSetPersistence.h"
#include "Globals.h"
#include "Utils.h"

// OGR
#include <ogrsf_frmts.h>

// STL
#include <cassert>

te::ogr::DataSourceTransactor::DataSourceTransactor(DataSource* ds, OGRDataSource* ogrDS)
  : m_ds(ds),
    m_ogrDS(ogrDS),
    m_isInTransaction(false)
{
}

te::ogr::DataSourceTransactor::~DataSourceTransactor()
{
}

void te::ogr::DataSourceTransactor::begin()
{
  OGRLayer* result = m_ogrDS->ExecuteSQL("START TRANSACTION", 0, 0);

  if(result)
    m_ogrDS->ReleaseResultSet(result);

  m_isInTransaction = true;
}

void te::ogr::DataSourceTransactor::commit()
{
  m_isInTransaction = false;

  OGRLayer* result = m_ogrDS->ExecuteSQL("COMMIT", 0, 0);

  if(result)
    m_ogrDS->ReleaseResultSet(result);
}

void te::ogr::DataSourceTransactor::rollBack()
{
  m_isInTransaction = false;

  OGRLayer* result = m_ogrDS->ExecuteSQL("ROLLBACK", 0, 0);

  if(result)
    m_ogrDS->ReleaseResultSet(result);
}

bool te::ogr::DataSourceTransactor::isInTransaction() const
{
  return m_isInTransaction;
}

te::da::DataSet* te::ogr::DataSourceTransactor::getDataSet(const std::string& name, 
                                                           te::common::TraverseType /*travType*/,
                                                           te::common::AccessPolicy /*rwRole*/)
{
  std::string sql = "SELECT * FROM " + name;
  OGRLayer* layer = m_ogrDS->ExecuteSQL(sql.c_str(), 0, 0);

  if(layer == 0)
    throw(te::common::Exception(TR_OGR("The informed data set could not be found in the data source.")));

  return new DataSet(this, layer, true);
}

te::da::DataSet* te::ogr::DataSourceTransactor::getDataSet(const std::string& name,
                                                           const te::dt::Property* /*p*/,
                                                           const te::gm::Envelope* e,
                                                           te::gm::SpatialRelation /*r*/,
                                                           te::common::TraverseType /*travType*/, 
                                                           te::common::AccessPolicy /*rwRole*/)
{
  std::string sql = "SELECT * FROM " + name;
  OGRLayer* layer = m_ogrDS->ExecuteSQL(sql.c_str(), 0, 0);

  if(layer == 0)
    throw(te::common::Exception(TR_OGR("The informed data set could not be found in the data source.")));

  layer->SetSpatialFilterRect(e->m_llx, e->m_lly, e->m_urx, e->m_ury);
  
  return new DataSet(this, layer, true);
}

te::da::DataSet* te::ogr::DataSourceTransactor::getDataSet(const std::string& name,
                                                           const te::dt::Property* /*p*/,
                                                           const te::gm::Geometry* g,
                                                           te::gm::SpatialRelation /*r*/,
                                                           te::common::TraverseType /*travType*/, 
                                                           te::common::AccessPolicy /*rwRole*/)
{ 
  std::string sql = "SELECT * FROM " + name;
  OGRLayer* layer = m_ogrDS->ExecuteSQL(sql.c_str(), 0, 0);

  if(layer == 0)
    throw(te::common::Exception(TR_OGR("The informed data set could not be found in the data source.")));
  
  OGRGeometry* ogrg = Convert2OGR(g);

  layer->SetSpatialFilter(ogrg);
  
  OGRGeometryFactory::destroyGeometry(ogrg);
  
  return new DataSet(this, layer, true);
}

te::da::DataSet* te::ogr::DataSourceTransactor::query(const te::da::Select& /*q*/, 
                                                      te::common::TraverseType /*travType*/,
                                                      te::common::AccessPolicy /*rwRole*/)
{
  return 0; // OGR Library not supports sql dialect
}

te::da::DataSet* te::ogr::DataSourceTransactor::query(const std::string& query, 
                                                      te::common::TraverseType /*travType*/,
                                                      te::common::AccessPolicy /*rwRole*/)
{
  OGRLayer* layer = m_ogrDS->ExecuteSQL(query.c_str(), 0, "");
  if(layer == 0)
    throw(te::common::Exception(TR_OGR("Could not retrieve the DataSet from data source.")));

  return new DataSet(this, layer, true);
}

void te::ogr::DataSourceTransactor::execute(const te::da::Query& /*command*/)
{
  // OGR Library not supports sql dialect
}

void te::ogr::DataSourceTransactor::execute(const std::string& command)
{
  OGRLayer* layer = m_ogrDS->ExecuteSQL(command.c_str(), 0, "");

  if(layer)
     m_ogrDS->ReleaseResultSet(layer);
}

OGRDataSource* te::ogr::DataSourceTransactor::getOGRDataSource() const
{
  return m_ogrDS;
}

te::da::PreparedQuery* te::ogr::DataSourceTransactor::getPrepared(const std::string& /*qName*/)
{
  return 0;
}

te::da::BatchExecutor* te::ogr::DataSourceTransactor::getBatchExecutor()
{
  return 0;
}

te::da::DataSourceCatalogLoader* te::ogr::DataSourceTransactor::getCatalogLoader()
{
  return new CatalogLoader(this);
}

te::da::DataSetTypePersistence* te::ogr::DataSourceTransactor::getDataSetTypePersistence()
{
  return 0;
}

te::da::DataSetPersistence* te::ogr::DataSourceTransactor::getDataSetPersistence()
{
  return new te::ogr::DataSetPersistence(this);
}

void te::ogr::DataSourceTransactor::cancel()
{
  return;
}

boost::int64_t te::ogr::DataSourceTransactor::getLastInsertId()
{
  throw te::common::Exception(TR_OGR("Not implemented yet!"));
}

te::da::DataSource* te::ogr::DataSourceTransactor::getDataSource() const
{
  return m_ds;
}
