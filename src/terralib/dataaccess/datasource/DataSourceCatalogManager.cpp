/*  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/dataaccess/datasource/DataSourceCatalogManager.cpp

  \brief This is a singleton for managing all data source catalog instances available in the system.
*/

// TerraLib
#include "../../common/Globals.h"
#include "../../common/STLUtils.h"
#include "../../common/StringUtils.h"
#include "../../common/Translator.h"
#include "../Exception.h"
#include "DataSource.h"
#include "DataSourceCatalogManager.h"

// STL
#include <algorithm>
#include <cassert>
#include <memory>

te::da::DataSourceCatalogPtr te::da::DataSourceCatalogManager::create(DataSourcePtr ds)
{
  if(find(ds))
    throw Exception(TE_TR("There is already a catalog for the data source given!"));

  DataSourceCatalogPtr catalog(new te::da::DataSourceCatalog());

  catalog->setDataSource(ds.get());

  // Get the informations associated to the datasets of the data source and insert them into the catalog
  if(!ds->isOpened())
    ds->open();

  std::vector<std::string> datasetNames = ds->getDataSetNames();

  for(std::size_t i = 0; i < datasetNames.size(); ++i)
  {
    std::auto_ptr<te::da::DataSetType> dt = ds->getDataSetType(datasetNames[i]);
    catalog->add(DataSetTypePtr(dt));
  }

  // Get the sequences of the data source
  std::vector<std::string> seqNames = ds->getSequenceNames();

  for(std::size_t i = 0; i < seqNames.size(); ++i)
  {
    std::auto_ptr<te::da::Sequence> seq = ds->getSequence(seqNames[i]);
    catalog->add(seq.release());
  }

  // Insert the catalog into the manager
  insert(catalog);

  return catalog;
}


te::da::DataSourceCatalogPtr te::da::DataSourceCatalogManager::get(DataSourcePtr ds)
{
  LockRead l(this);

  const_iterator it = m_catalogs.find(ds);

  if(it != m_catalogs.end())
    return it->second;

  return create(ds);
}

bool te::da::DataSourceCatalogManager::find(const DataSourcePtr& ds) const
{
  LockRead l(this);

  const_iterator it = m_catalogs.find(ds);

  if(it != m_catalogs.end())
    return true;

  return false;
}

void te::da::DataSourceCatalogManager::insert(const DataSourceCatalogPtr& catalog)
{
  if(catalog.get() == 0)
    throw Exception(TE_TR("Please, specifify a non-null data source to be managed!"));

  LockWrite l(this);

  DataSourcePtr ds(catalog->getDataSource());

  if(find(ds))
    throw Exception(TE_TR("There is already a catalog for the data source of the given catalog!"));

  m_catalogs[ds] = catalog;
}

void te::da::DataSourceCatalogManager::detach(DataSourceCatalogPtr catalog)
{
  te::da::DataSourcePtr ds(catalog->getDataSource());

  if(ds.get() == 0)
    return;

  detach(ds);
}

te::da::DataSourceCatalogPtr te::da::DataSourceCatalogManager::detach(const DataSourcePtr& ds)
{
  LockWrite l(this);

  if(!find(ds))
    throw Exception(TE_TR("This catalog is not valid to be detached from the manager!"));

  iterator it = m_catalogs.find(ds);

  m_catalogs.erase(it->first);

  return it->second;
}

void te::da::DataSourceCatalogManager::detachAll(const std::string& dsType)
{
  LockWrite l(this);

  iterator it;
  for(it = m_catalogs.begin(); it != m_catalogs.end(); ++it)
  {
    if(it->first->getType() == dsType)
      m_catalogs.erase(it++);
    else
      ++it;
  }
}

void te::da::DataSourceCatalogManager::detachAll()
{
  LockWrite l(this);

  m_catalogs.clear();
}

te::da::DataSourceCatalogManager::DataSourceCatalogManager()
{
}

te::da::DataSourceCatalogManager::~DataSourceCatalogManager()
{
}
