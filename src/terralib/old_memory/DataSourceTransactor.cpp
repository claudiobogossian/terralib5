/*  Copyright (C) 2011-2011 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/memory/DataSourceTransactor.cpp

  \brief An implementation of DataSourceTransactor class for the In-Memory Data Access driver.
*/

// TerraLib
#include "../common/Translator.h"
#include "../dataaccess/dataset/DataSetType.h"
#include "DataSet.h"
#include "DataSource.h"
#include "DataSourceCatalogLoader.h"
#include "DataSourceTransactor.h"
#include "DataSetTypePersistence.h"
#include "DataSetPersistence.h"
#include "Exception.h"

te::mem::DataSourceTransactor::DataSourceTransactor(DataSource* parent)
  : m_ds(parent)
{
}

te::mem::DataSourceTransactor::~DataSourceTransactor()
{
}

void te::mem::DataSourceTransactor::begin()
{
// Not supported by In-Memory driver!
}

void te::mem::DataSourceTransactor::commit()
{
// Not supported by In-Memory driver!
}

void te::mem::DataSourceTransactor::rollBack()
{
// Not supported by In-Memory driver!
}

bool te::mem::DataSourceTransactor::isInTransaction() const
{
  return false;
}

te::da::DataSet* te::mem::DataSourceTransactor::getDataSet(const std::string& name, 
                                                           te::common::TraverseType /*travType*/, 
                                                           te::common::AccessPolicy /*rwRole*/)
{
  DataSet* d = m_ds->getDataSet(name);

  d->setTransactor(this);

  return d;
}

te::da::DataSet* te::mem::DataSourceTransactor::getDataSet(const std::string& /*name*/,
                                                           const te::dt::Property* /*p*/,
                                                           const te::gm::Envelope* /*e*/,
                                                           te::gm::SpatialRelation /*r*/,
                                                           te::common::TraverseType /*travType*/, 
                                                           te::common::AccessPolicy /*rwRole*/)
{
  throw Exception(TR_MEMORY("Not implemented yet!"));
}

te::da::DataSet* te::mem::DataSourceTransactor::getDataSet(const std::string& /*name*/,
                                                           const te::dt::Property* /*p*/,
                                                           const te::gm::Geometry* /*g*/,
                                                           te::gm::SpatialRelation /*r*/,
                                                           te::common::TraverseType /*travType*/, 
                                                           te::common::AccessPolicy /*rwRole*/)
{
  throw Exception(TR_MEMORY("Not implemented yet!"));
}

te::da::DataSet* te::mem::DataSourceTransactor::query(const te::da::Select& /*q*/, 
                                                      te::common::TraverseType /*travType*/, 
                                                      te::common::AccessPolicy /*rwRole*/)
{
  throw Exception(TR_MEMORY("Not implemented yet!"));
}

te::da::DataSet* te::mem::DataSourceTransactor::query(const std::string& /*query*/, 
                                                      te::common::TraverseType /*travType*/, 
                                                      te::common::AccessPolicy /*rwRole*/)
{
  throw Exception(TR_MEMORY("Not implemented yet!"));
}

void te::mem::DataSourceTransactor::execute(const te::da::Query& /*command*/)
{
  throw Exception(TR_MEMORY("Not implemented yet!"));
}

void te::mem::DataSourceTransactor::execute(const std::string& /*command*/)
{
  throw Exception(TR_MEMORY("Not implemented yet!"));
}

te::da::PreparedQuery* te::mem::DataSourceTransactor::getPrepared(const std::string& /*qName*/)
{
  throw Exception(TR_MEMORY("Not implemented yet!"));
}

te::da::BatchExecutor* te::mem::DataSourceTransactor::getBatchExecutor()
{
  throw Exception(TR_MEMORY("Not implemented yet!"));
}

te::da::DataSourceCatalogLoader* te::mem::DataSourceTransactor::getCatalogLoader()
{
  return new DataSourceCatalogLoader(this);
}

te::da::DataSetTypePersistence* te::mem::DataSourceTransactor::getDataSetTypePersistence()
{
  return new DataSetTypePersistence(this);
}

te::da::DataSetPersistence* te::mem::DataSourceTransactor::getDataSetPersistence()
{
  return new DataSetPersistence(this);
}

void te::mem::DataSourceTransactor::cancel()
{
  throw Exception(TR_MEMORY("Not implemented yet!"));
}

boost::int64_t te::mem::DataSourceTransactor::getLastInsertId()
{
  throw Exception(TR_MEMORY("Not implemented yet!"));
}

te::da::DataSource* te::mem::DataSourceTransactor::getDataSource() const
{
  return m_ds;
}

te::mem::DataSource* te::mem::DataSourceTransactor::getMemDataSource() const
{
  return m_ds;
}

