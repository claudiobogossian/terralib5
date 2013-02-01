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
  \file terralib/memory/DataSourceCatalogLoader.cpp

  \brief DataSourceCatalogLoader class implementation for the TerraLib In-Memory Driver.
*/

// TerraLib
#include "../common/STLUtils.h"
#include "../common/Translator.h"
#include "../dataaccess/dataset/DataSetType.h"
#include "../dataaccess/datasource/DataSourceCatalog.h"
#include "DataSet.h"
#include "DataSource.h"
#include "DataSourceCatalogLoader.h"
#include "DataSourceTransactor.h"
#include "Exception.h"

// STL
#include <memory>

te::mem::DataSourceCatalogLoader::DataSourceCatalogLoader(DataSourceTransactor* parent)
  : m_t(parent)
{
}

te::mem::DataSourceCatalogLoader::~DataSourceCatalogLoader()
{
}

void te::mem::DataSourceCatalogLoader::getDataSets(boost::ptr_vector<std::string>& datasets)
{
  m_t->getMemDataSource()->getDataSets(datasets);
}

te::da::DataSetType* te::mem::DataSourceCatalogLoader::getDataSetType(const std::string& datasetName,
                                                                      const bool /*full*/)
{
  return m_t->getMemDataSource()->getDataSetType(datasetName);
}

void te::mem::DataSourceCatalogLoader::getProperties(te::da::DataSetType* /*dt*/)
{
  throw Exception(TR_MEMORY("Not implemented yet!"));
}

te::dt::Property* te::mem::DataSourceCatalogLoader::getProperty(const std::string& /*datasetName*/, const std::string& /*propertyName*/)
{
  throw Exception(TR_MEMORY("Not implemented yet!"));
}

void te::mem::DataSourceCatalogLoader::getPrimaryKey(te::da::DataSetType* /*dt*/)
{
  throw Exception(TR_MEMORY("Not implemented yet!"));
}

void te::mem::DataSourceCatalogLoader::getUniqueKeys(te::da::DataSetType* /*dt*/)
{
  throw Exception(TR_MEMORY("Not implemented yet!"));
}

void te::mem::DataSourceCatalogLoader::getForeignKeys(te::da::DataSetType* /*dt*/, std::vector<std::string>& /*fkNames*/, std::vector<std::string>& /*rdts*/)
{
  throw Exception(TR_MEMORY("Not implemented yet!"));
}

te::da::ForeignKey* te::mem::DataSourceCatalogLoader::getForeignKey(const std::string& /*fkName*/, te::da::DataSetType* /*dt*/, te::da::DataSetType* /*rdt*/)
{
  throw Exception(TR_MEMORY("Not implemented yet!"));
}

void te::mem::DataSourceCatalogLoader::getIndexes(te::da::DataSetType* /*dt*/)
{
  throw Exception(TR_MEMORY("Not implemented yet!"));
}

void te::mem::DataSourceCatalogLoader::getCheckConstraints(te::da::DataSetType* /*dt*/)
{
  throw Exception(TR_MEMORY("Not implemented yet!"));
}

void te::mem::DataSourceCatalogLoader::getSequences(std::vector<std::string*>& /*sequences*/)
{
  throw Exception(TR_MEMORY("Not implemented yet!"));
}

te::da::Sequence* te::mem::DataSourceCatalogLoader::getSequence(const std::string& /*seqName*/)
{
  throw Exception(TR_MEMORY("Not implemented yet!"));
}

te::gm::Envelope* te::mem::DataSourceCatalogLoader::getExtent(const te::gm::GeometryProperty* /*gp*/)
{
  throw Exception(TR_MEMORY("Not implemented yet!"));
}

void te::mem::DataSourceCatalogLoader::loadCatalog(const bool /*full*/)
{
  te::da::DataSourceCatalog* catalog = m_t->getMemDataSource()->getCatalog();
  
  catalog->clear();

  boost::ptr_vector<std::string> datasets;
  
  m_t->getMemDataSource()->getDataSets(datasets);

  for(std::size_t i = 0; i < datasets.size(); ++i)
  {
    te::da::DataSetTypePtr dt(m_t->getMemDataSource()->getDataSetType(datasets[i]));

    dt->setId(static_cast<unsigned int>(i));

    catalog->add(dt);
  }
}

bool te::mem::DataSourceCatalogLoader::hasDataSets()
{
  throw Exception(TR_MEMORY("Not implemented yet!"));
}

bool te::mem::DataSourceCatalogLoader::datasetExists(const std::string& name)
{
  return m_t->getMemDataSource()->datasetExists(name);
}

bool te::mem::DataSourceCatalogLoader::primarykeyExists(const std::string& /*name*/)
{
  throw Exception(TR_MEMORY("Not implemented yet!"));
}

bool te::mem::DataSourceCatalogLoader::uniquekeyExists(const std::string& /*name*/)
{
  throw Exception(TR_MEMORY("Not implemented yet!"));
}

bool te::mem::DataSourceCatalogLoader::foreignkeyExists(const std::string& /*name*/)
{
  throw Exception(TR_MEMORY("Not implemented yet!"));
}

bool te::mem::DataSourceCatalogLoader::checkConstraintExists(const std::string& /*name*/)
{
  throw Exception(TR_MEMORY("Not implemented yet!"));
}

bool te::mem::DataSourceCatalogLoader::indexExists(const std::string& /*name*/)
{
  throw Exception(TR_MEMORY("Not implemented yet!"));
}

bool te::mem::DataSourceCatalogLoader::sequenceExists(const std::string& /*name*/)
{
  throw Exception(TR_MEMORY("Not implemented yet!"));
}

te::da::DataSourceTransactor* te::mem::DataSourceCatalogLoader::getTransactor() const
{
  return m_t;
}
