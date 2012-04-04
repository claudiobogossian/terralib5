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
  \file terralib/memory/DataSetPersistence.cpp

  \brief DataSetPersistence implementation for the In-Memory Data Access driver.
*/

// TerraLib
#include "../common/Translator.h"
#include "../dataaccess/dataset/DataSetType.h"
#include "../dataaccess/dataset/PrimaryKey.h"
#include "../dataaccess/dataset/DataSetItem.h"
#include "../dataaccess/datasource/DataSourceCatalog.h"
#include "../raster/Grid.h"
#include "../raster/RasterProperty.h"
#include "DataSet.h"
#include "DataSetItem.h"
#include "DataSetPersistence.h"
#include "DataSetTypePersistence.h"
#include "DataSource.h"
#include "DataSourceTransactor.h"
#include "Exception.h"
#include "Raster.h"

// STL
#include <memory>

// Boost
#include <boost/format.hpp>

te::mem::DataSetPersistence::DataSetPersistence(DataSourceTransactor* parent)
  : m_t(parent)
{
}

te::mem::DataSetPersistence::~DataSetPersistence()
{
}

void te::mem::DataSetPersistence::create(te::da::DataSetType* dt, te::da::DataSet* d, const std::map<std::string, std::string>& options, std::size_t /*limit*/)
{
  DataSource* ds = m_t->getMemDataSource();

  DataSource::LockWrite l(ds);

  if(ds->datasetExists(dt->getName()))
    throw Exception((boost::format(TR_MEMORY("A dataset with the same name (%1%) already exists!")) % dt->getName()).str());

  std::auto_ptr<te::da::DataSetTypePersistence> dtp(m_t->getDataSetTypePersistence());

  dtp->create(dt, options);

  add(dt, d);
}

void te::mem::DataSetPersistence::remove(const te::da::DataSetType* dt)
{
  DataSource* ds = m_t->getMemDataSource();

  DataSource::LockWrite l(ds);

  if(!ds->datasetExists(dt->getName()))
    throw Exception((boost::format(TR_MEMORY("The dataset %1% not exists!")) % dt->getName()).str());

  ds->remove(dt->getName());
}

void te::mem::DataSetPersistence::remove(const te::da::DataSetType* /*dt*/, te::da::DataSet* /*d*/, std::size_t /*limit*/)
{
  throw Exception(TR_MEMORY("Not implemented yet!"));
}

void te::mem::DataSetPersistence::remove(const te::da::DataSetType* dt, te::da::DataSetItem* item)
{
  DataSource* ds = m_t->getMemDataSource();

  DataSource::LockWrite l(ds);

  if(!ds->datasetExists(dt->getName()))
    throw Exception((boost::format(TR_MEMORY("The dataset %1% not exists!")) % dt->getName()).str());

  ds->getDataSet(dt->getName())->remove(dynamic_cast<DataSetItem*>(item));
}

void te::mem::DataSetPersistence::add(const te::da::DataSetType* dt, te::da::DataSet* d, std::size_t /*limit*/)
{
  DataSource* ds = m_t->getMemDataSource();

  DataSource::LockWrite l(ds);

  te::mem::DataSet* idataset = ds->getDataSetRef(dt->getName());

  if(idataset == 0)
    throw Exception((boost::format(TR_MEMORY("The dataset %1% doesn't exist!")) % dt->getName()).str());

  idataset->copy(*d);
}

void te::mem::DataSetPersistence::add(const te::da::DataSetType* dt, te::da::DataSetItem* item)
{
  DataSource* ds = m_t->getMemDataSource();

  if(!ds->datasetExists(dt->getName()))
    throw Exception((boost::format(TR_MEMORY("The dataset %1% not exists!")) % dt->getName()).str());

  DataSource::LockWrite l(ds);

  ds->getDataSet(dt->getName())->add(item);
}

void te::mem::DataSetPersistence::update(const te::da::DataSetType* /*dt*/,
                                         te::da::DataSet* /*dataset*/,
                                         const std::vector<te::dt::Property*>& /*properties*/,
                                         std::size_t /*limit*/)
{
  throw Exception(TR_MEMORY("Not implemented yet!"));
}

void te::mem::DataSetPersistence::update(const te::da::DataSetType* /*dt*/,
                                         te::da::DataSet* /*oldD*/,
                                         te::da::DataSet* /*newD*/,
                                         const std::vector<te::dt::Property*>& /*properties*/,
                                         std::size_t /*limit*/)
{
  throw Exception(TR_MEMORY("Not implemented yet!"));
}

void te::mem::DataSetPersistence::update(const te::da::DataSetType* /*dt*/,
                                         te::da::DataSetItem* /*item*/,
                                         const std::vector<te::dt::Property*>& /*properties*/)
{
  throw Exception(TR_MEMORY("Not implemented yet!"));
}

void te::mem::DataSetPersistence::update(const te::da::DataSetType* /*dt*/,
                                         te::da::DataSetItem* /*oldItem*/,
                                         te::da::DataSetItem* /*newItem*/,
                                         const std::vector<te::dt::Property*>& /*properties*/)
{
  throw Exception(TR_MEMORY("Not implemented yet!"));
}

te::da::DataSourceTransactor* te::mem::DataSetPersistence::getTransactor() const
{
  return m_t;
}
                 
