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
  \file AbstractGraphFactory.cpp

  \brief This is the abstract factory for Graphs.
*/

// TerraLib
#include "../../dataaccess/datasource/DataSource.h"
#include "../../common/StringUtils.h"
#include "../../common/Translator.h"
#include "../../dataaccess/datasource/DataSourceFactory.h"
#include "../cache/AbstractCachePolicyFactory.h"
#include "../drivers/database/DatabaseGraphMetadata.h"
#include "../loader/AbstractGraphLoaderStrategyFactory.h"
#include "../Config.h"
#include "../Exception.h"
#include "AbstractGraphFactory.h"
#include "GraphMetadata.h"

// STL
#include <memory>

te::graph::AbstractGraph* te::graph::AbstractGraphFactory::make()
{
  return te::common::AbstractFactory<AbstractGraph, std::string>::make(TE_DEFAULT_GRAPH_TYPE);
}

te::graph::AbstractGraph* te::graph::AbstractGraphFactory::make(const std::string& gType)
{
  std::string ucase = te::common::Convert2UCase(gType);

  return te::common::AbstractFactory<AbstractGraph, std::string>::make(ucase);
}

te::graph::AbstractGraph* te::graph::AbstractGraphFactory::make(const std::map<std::string, std::string>& dsInfo, const std::map<std::string, std::string>& gInfo)
{
  return make(TE_DEFAULT_GRAPH_TYPE, dsInfo, gInfo);
}

te::graph::AbstractGraph* te::graph::AbstractGraphFactory::make(const std::string& gType, const std::map<std::string, std::string>& dsInfo, const std::map<std::string, std::string>& gInfo)
{
  std::string ucase = te::common::Convert2UCase(gType);

  te::common::AbstractFactory<AbstractGraph, std::string>::dictionary_type& d = te::common::AbstractFactory<AbstractGraph, std::string>::getDictionary();

  AbstractGraphFactory* f = static_cast<AbstractGraphFactory*>(d.find(ucase));

  if(f == 0)
    throw Exception(TR_GRAPH("Could not find concrete factory! Check if it was initialized!"));

  AbstractGraph* g = f->create(dsInfo, gInfo);

  g->getMetadata()->setType(ucase);

  return g;
}

te::graph::AbstractGraph* te::graph::AbstractGraphFactory::open(const std::map<std::string, std::string>& dsInfo, const std::map<std::string, std::string>& gInfo)
{
  return open(TE_DEFAULT_GRAPH_TYPE, dsInfo, gInfo);
}

te::graph::AbstractGraph* te::graph::AbstractGraphFactory::open(const std::string& gType, const std::map<std::string, std::string>& dsInfo, const std::map<std::string, std::string>& gInfo)
{
  std::string ucase = te::common::Convert2UCase(gType);

  te::common::AbstractFactory<AbstractGraph, std::string>::dictionary_type& d = te::common::AbstractFactory<AbstractGraph, std::string>::getDictionary();

  AbstractGraphFactory* f = static_cast<AbstractGraphFactory*>(d.find(ucase));

  if(f == 0)
    throw Exception(TR_GRAPH("Could not find concrete factory! Check if it was initialized!"));

  AbstractGraph* g = f->iOpen(dsInfo, gInfo);

  g->getMetadata()->setType(ucase);

  return g;
}

te::graph::AbstractGraphFactory::AbstractGraphFactory(const std::string& factoryKey)
  : te::common::AbstractFactory<te::graph::AbstractGraph, std::string>(factoryKey)
{
}

te::graph::GraphMetadata* te::graph::AbstractGraphFactory::getMetadata(const std::map<std::string, std::string>& dsInfo, const std::map<std::string, std::string>& gInfo)
{
  te::graph::GraphMetadata* metadata;

  std::map<std::string, std::string>::const_iterator it;
  std::map<std::string, std::string>::const_iterator itend = gInfo.end();

  //create data source
  it = gInfo.find("GRAPH_DATA_SOURCE_TYPE");

  std::auto_ptr<te::da::DataSource> dsPtr;

  if(it != itend)
  {
    if(it->second == "MEM")
    {
      metadata = new te::graph::GraphMetadata(0);

      metadata->m_memoryGraph = true;
    }
    else
    {
      dsPtr = te::da::DataSourceFactory::make(it->second); //example: dsType = POSTGIS
      dsPtr->setConnectionInfo(dsInfo);
      dsPtr->open();

      te::da::DataSource* ds = dsPtr.release();
      
      metadata = new te::graph::DatabaseGraphMetadata(ds);

      metadata->m_memoryGraph = false;
    }
  }

  return metadata;
}

int te::graph::AbstractGraphFactory::getId(const std::map<std::string, std::string>& dsInfo, const std::map<std::string, std::string>& gInfo)
{
  std::map<std::string, std::string>::const_iterator it;
  std::map<std::string, std::string>::const_iterator itend = gInfo.end();

  it = gInfo.find("GRAPH_ID");

  if(it != itend)
    return atoi(it->second.c_str());

  return -1;
}

te::graph::AbstractCachePolicy* te::graph::AbstractGraphFactory::getCachePolicy(const std::map<std::string, std::string>& gInfo)
{
  std::map<std::string, std::string>::const_iterator it;
  std::map<std::string, std::string>::const_iterator itend = gInfo.end();

  te::graph::AbstractCachePolicy* cp = 0;

  it = gInfo.find("GRAPH_CACHE_POLICY");
  if(it != itend)
  {
    cp = te::graph::AbstractCachePolicyFactory::make(it->second);
  }

  return cp;
}

te::graph::AbstractGraphLoaderStrategy* te::graph::AbstractGraphFactory::getLoaderStrategy(const std::map<std::string, std::string>& gInfo, te::graph::GraphMetadata* metadata)
{
  std::map<std::string, std::string>::const_iterator it;
  std::map<std::string, std::string>::const_iterator itend = gInfo.end();

  te::graph::AbstractGraphLoaderStrategy* ls = 0;

  it = gInfo.find("GRAPH_STRATEGY_LOADER");
  if(it != itend)
  {
    ls = te::graph::AbstractGraphLoaderStrategyFactory::make(it->second, metadata);
  }

  return ls;
}

void te::graph::AbstractGraphFactory::setMetadataInformation(const std::map<std::string, std::string>& gInfo, te::graph::GraphMetadata* metadata)
{
  std::map<std::string, std::string>::const_iterator it;
  std::map<std::string, std::string>::const_iterator itend = gInfo.end();

  it = gInfo.find("GRAPH_NAME");
  if(it != itend)
  {
    metadata->setName(it->second);
  }

  it = gInfo.find("GRAPH_DESCRIPTION");
  if(it != itend)
  {
    metadata->setDescription(it->second);
  }

  it = gInfo.find("GRAPH_STORAGE_MODE");
  if(it != itend)
  {
    if(it->second == TE_GRAPH_STORAGE_MODE_BY_VERTEX)
    {
      metadata->setStorageMode(te::graph::Vertex_List);
    }
    else if(it->second == TE_GRAPH_STORAGE_MODE_BY_EDGE)
    {
      metadata->setStorageMode(te::graph::Edge_List);
    }
  }
}
