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
  \file terralib/memory/DataSetPersistence.cpp

  \brief DataSetPersistence implementation for the In-Memory Data Access driver.
*/

// TerraLib
#include "../common/Translator.h"
#include "../dataaccess/dataset/DataSetType.h"
#include "../dataaccess/dataset/PrimaryKey.h"
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

void te::mem::DataSetPersistence::remove(const std::string& datasetName, const te::da::ObjectIdSet* /*oids*/)
{
  DataSource* ds = m_t->getMemDataSource();

  DataSource::LockWrite l(ds);

  DataSet* dataset = ds->getDataSetRef(datasetName);

  if(dataset == 0)
    throw Exception((boost::format(TR_MEMORY("The dataset %1% doesn't exist!")) % datasetName).str());

  dataset->clear();
}

void te::mem::DataSetPersistence::add(const std::string& datasetName, te::da::DataSet* d, const std::map<std::string, std::string>& /*options*/, std::size_t limit)
{
  DataSource* ds = m_t->getMemDataSource();

  DataSource::LockWrite l(ds);

  te::mem::DataSet* idataset = ds->getDataSetRef(datasetName);

  if(idataset == 0)
    throw Exception((boost::format(TR_MEMORY("The dataset %1% doesn't exist!")) % datasetName).str());

  idataset->copy(*d, limit);
}

void te::mem::DataSetPersistence::update(const std::string& /*datasetName*/,
                                         te::da::DataSet* /*dataset*/,
                                         const std::vector<std::size_t>& /*properties*/,
                                         const te::da::ObjectIdSet* /*oids*/,
                                         const std::map<std::string, std::string>& /*options*/,
                                         std::size_t /*limit*/)
{
  throw Exception(TR_MEMORY("Not implemented yet!"));
}

