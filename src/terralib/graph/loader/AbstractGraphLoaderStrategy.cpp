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
  \file AbstractGraphLoaderStrategy.h

  \brief This class define the main functions necessary to
        save and load the graph data and metadata information
        using the Graph Data and Graph Cache conceptions.
*/


// Terralib Includes
#include "../../common/STLUtils.h"
#include "../../common/StringUtils.h"
#include "../../common/Translator.h"
#include "../../dataaccess/dataset/DataSet.h"
#include "../../dataaccess/dataset/DataSetType.h"
#include "../../dataaccess/datasource/DataSource.h"
#include "../../dataaccess/query_h.h"
#include "../../memory/DataSet.h"
#include "../../memory/DataSetItem.h"
#include "../core/AbstractGraph.h"
#include "../core/Edge.h"
#include "../core/GraphCache.h"
#include "../core/EdgeProperty.h"
#include "../core/GraphData.h"
#include "../core/GraphMetadata.h"
#include "../core/Vertex.h"
#include "../core/VertexProperty.h"
#include "../Config.h"
#include "../Globals.h"
#include "../Exception.h"
#include "AbstractGraphLoaderStrategy.h"


te::graph::AbstractGraphLoaderStrategy::AbstractGraphLoaderStrategy(te::graph::GraphMetadata* metadata) : m_graphMetadata(metadata)
{
}

te::graph::AbstractGraphLoaderStrategy::~AbstractGraphLoaderStrategy()
{
  delete m_graphMetadata;
}

te::graph::GraphMetadata* te::graph::AbstractGraphLoaderStrategy::getMetadata()
{
  return m_graphMetadata;
}

void te::graph::AbstractGraphLoaderStrategy::saveData(GraphData* data)
{
  if(m_graphMetadata == 0 || m_graphMetadata->getDataSource() == 0)
  {
    throw Exception(TE_TR(""));
  }

  //save graph topology
  if(m_graphMetadata->getStorageMode() == te::graph::Edge_List)
  {
    saveGraphEdgeList(data);
  }
}


void te::graph::AbstractGraphLoaderStrategy::removeEdge(int id)
{
  if(m_graphMetadata == 0 || m_graphMetadata->getDataSource() == 0)
  {
    throw Exception(TE_TR(""));
  }
}


void te::graph::AbstractGraphLoaderStrategy::removeVertex(int id)
{
  if(m_graphMetadata == 0 || m_graphMetadata->getDataSource() == 0)
  {
    throw Exception(TE_TR(""));
  }
}


void te::graph::AbstractGraphLoaderStrategy::saveGraphEdgeList(GraphData* data)
{
  if(data->isDirty() == false)
  {
    return;
  }

  std::string tableName = m_graphMetadata->getEdgeTableName();

  //get dataset type
  std::auto_ptr<te::da::DataSetType> dsType(m_graphMetadata->getDataSource()->getDataSetType(tableName));

  //create outputs data sets
  std::auto_ptr<te::mem::DataSet> outDataSet(new te::mem::DataSet(dsType.get()));
  std::auto_ptr<te::mem::DataSet> outDataSetUpdate(new te::mem::DataSet(dsType.get()));

  te::graph::GraphData::EdgeMap::iterator it = data->getEdgeMap().begin();

  bool hasNewObjects = false;
  bool hasUpdatedObjects = false;

  while(it != data->getEdgeMap().end())
  {
    if(it->second->isNew()) //new element
    {
      //create new item
      te::mem::DataSetItem* dsItem = new te::mem::DataSetItem(outDataSet.get());

      dsItem->setInt32(0, it->second->getId());
      dsItem->setInt32(1, it->second->getIdFrom());
      dsItem->setInt32(2, it->second->getIdTo());

      int pos = 3; // number of attributes already defined in dsItem.

      //check for edge properties
      if(m_graphMetadata->getEdgePropertySize() != 0)
      {
        for(size_t i = 0; i < it->second->getAttributes().size(); ++i)
        {
          dsItem->setValue(pos + i, it->second->getAttributes()[i]->clone());
        }
      }

      outDataSet->add(dsItem);

      hasNewObjects = true;
    }
    else if(it->second->isDirty()) //update element
    {
      //create new item
      te::mem::DataSetItem* dsItem = new te::mem::DataSetItem(outDataSetUpdate.get());

      dsItem->setInt32(0, it->second->getId());
      dsItem->setInt32(1, it->second->getIdFrom());
      dsItem->setInt32(2, it->second->getIdTo());

      int pos = 3; // number of attributes already defined in dsItem.

      //check for edge properties
      if(m_graphMetadata->getEdgePropertySize() != 0)
      {
        for(size_t i = 0; i < it->second->getAttributes().size(); ++i)
        {
          dsItem->setValue(pos + i, it->second->getAttributes()[i]->clone());
        }
      }

      outDataSetUpdate->add(dsItem);

      hasUpdatedObjects = true;
    }

    ++it;
  }

  std::map<std::string, std::string> options;

  if(hasNewObjects)
  {
    outDataSet->moveFirst();
    m_graphMetadata->getDataSource()->add(tableName, outDataSet.get(), options);
  }
  outDataSet->clear();

  if(hasUpdatedObjects)
  {
    outDataSetUpdate->moveFirst();
    std::map<std::string, std::string> options;
    std::vector<std::size_t> properties;

    for(size_t t = 0; t < dsType->getProperties().size(); ++t)
    {
      if(t != 0)
        properties.push_back(t);
    }

    m_graphMetadata->getDataSource()->update(tableName, outDataSetUpdate.get(), properties, 0, options);
  }
  outDataSetUpdate->clear();

  //must save the vertex attributes
  if(m_graphMetadata->getVertexPropertySize() != 0)
  {
    saveVertexAttributes(data);
  }
}

void te::graph::AbstractGraphLoaderStrategy::saveVertexAttributes(GraphData* data)
{
  if(data->isDirty() == false)
  {
    return;
  }

  std::string tableName = m_graphMetadata->getVertexTableName();

  //get dataset type
  std::auto_ptr<te::da::DataSetType> dsType(m_graphMetadata->getDataSource()->getDataSetType(tableName));

  //create outputs data sets
  std::auto_ptr<te::mem::DataSet> outDataSet(new te::mem::DataSet(dsType.get()));
  std::auto_ptr<te::mem::DataSet> outDataSetUpdate(new te::mem::DataSet(dsType.get()));

  te::graph::GraphData::VertexMap::iterator it = data->getVertexMap().begin();

  bool hasNewObjects = false;
  bool hasUpdatedObjects = false;

  while(it != data->getVertexMap().end())
  {
    if(it->second->isNew()) //new element
    {
      //create new item
      te::mem::DataSetItem* dsItem = new te::mem::DataSetItem(outDataSet.get());

      dsItem->setInt32(0, it->second->getId());

      int pos = 1; // number of attributes already defined in dsItem.

      //check for vertex properties
      if(m_graphMetadata->getVertexPropertySize() != 0)
      {
        for(size_t i = 0; i < it->second->getAttributes().size(); ++i)
        {
          dsItem->setValue(pos + i, it->second->getAttributes()[i]->clone());
        }
      }

      outDataSet->add(dsItem);

      hasNewObjects = true;
    }
    else if(it->second->isDirty()) //update element
    {
      //create new item
      te::mem::DataSetItem* dsItem = new te::mem::DataSetItem(outDataSetUpdate.get());

      dsItem->setInt32(0, it->second->getId());

      int pos = 1; // number of attributes already defined in dsItem.

      //check for vertex properties
      if(m_graphMetadata->getVertexPropertySize() != 0)
      {
        for(size_t i = 0; i < it->second->getAttributes().size(); ++i)
        {
          dsItem->setValue(pos + i, it->second->getAttributes()[i]->clone());
        }
      }

      outDataSetUpdate->add(dsItem);

      hasUpdatedObjects = true;
    }

    ++it;
  }

  std::map<std::string, std::string> options;

  if(hasNewObjects)
  {
    outDataSet->moveFirst();
    m_graphMetadata->getDataSource()->add(tableName, outDataSet.get(), options);
  }
  outDataSet->clear();

  if(hasUpdatedObjects)
  {
    outDataSetUpdate->moveFirst();
    std::map<std::string, std::string> options;
    std::vector<std::size_t> properties;

    for(size_t t = 0; t < dsType->getProperties().size(); ++t)
    {
      if(t != 0)
        properties.push_back(t);
    }

    m_graphMetadata->getDataSource()->update(tableName, outDataSetUpdate.get(), properties, 0, options);
  }
  outDataSetUpdate->clear();
}

void te::graph::AbstractGraphLoaderStrategy::saveGraphVertexList(GraphData* data)
{
  throw Exception(TE_TR("TO DO"));
}

void te::graph::AbstractGraphLoaderStrategy::saveEdgeAttributes(GraphData* data)
{
  throw Exception(TE_TR("TO DO"));
}

te::graph::Vertex* te::graph::AbstractGraphLoaderStrategy::loadVertex(int id)
{
  throw Exception(TE_TR("TO DO"));
}

te::graph::Vertex* te::graph::AbstractGraphLoaderStrategy::loadVertexAttrs(int id)
{
  //get dataset type
  std::string tableName = m_graphMetadata->getVertexTableName();
  std::auto_ptr<te::da::DataSetType> dsType(m_graphMetadata->getDataSource()->getDataSetType(tableName));

  //create query
  te::da::Field* f = new te::da::Field(new te::da::PropertyName("*"));
  te::da::Fields* fields = new te::da::Fields;
  fields->push_back(f);

  te::da::FromItem* t = new te::da::DataSetName(tableName);
  te::da::From* from = new te::da::From;
  from->push_back(t);

  te::da::LiteralString* l = new te::da::LiteralString(te::common::Convert2String(id));

  te::da::EqualTo* equal = new te::da::EqualTo(new te::da::PropertyName(Globals::sm_tableVertexModelAttrId), l);

  te::da::Where* filter = new te::da::Where(equal);

  te::da::Select select(fields, from, filter);

  std::auto_ptr<te::da::DataSet> dataset = m_graphMetadata->getDataSource()->query(select);

  //create vertex
  te::graph::Vertex* v = 0;

  if(dataset->moveNext())
  {
    //get fixed attribute
    int id = dataset->getInt32(Globals::sm_tableVertexModelAttrId);

    v = new te::graph::Vertex(id);

    v->setAttributeVecSize(dsType->getProperties().size() - 1);

    for(size_t i = 1; i < dsType->getProperties().size(); ++i)
    {
      v->addAttribute(i - 1, dataset->getValue(dsType->getProperty(i)->getName()).release());
    }
  }

  return v;
}

te::graph::Edge* te::graph::AbstractGraphLoaderStrategy::loadEdge(int id)
{
  //get dataset type
  std::string tableName = m_graphMetadata->getEdgeTableName();
  std::auto_ptr<te::da::DataSetType> dsType(m_graphMetadata->getDataSource()->getDataSetType(tableName));

  //create query
  te::da::Field* f = new te::da::Field(new te::da::PropertyName("*"));
  te::da::Fields* fields = new te::da::Fields;
  fields->push_back(f);

  te::da::FromItem* t = new te::da::DataSetName(tableName);
  te::da::From* from = new te::da::From;
  from->push_back(t);

  te::da::LiteralString* l = new te::da::LiteralString(te::common::Convert2String(id));

  te::da::EqualTo* equal = new te::da::EqualTo(new te::da::PropertyName(Globals::sm_tableEdgeModelAttrId), l);

  te::da::Where* filter = new te::da::Where(equal);

  te::da::Select select(fields, from, filter);

  std::auto_ptr<te::da::DataSet> dataset = m_graphMetadata->getDataSource()->query(select);

  //create vertex
  te::graph::Edge* e = 0;

  if(dataset->moveNext())
  {
    //get fixed attributes
    int id = dataset->getInt32(Globals::sm_tableEdgeModelAttrId);
    int vFrom = dataset->getInt32(Globals::sm_tableEdgeModelAttrVFrom);
    int vTo = dataset->getInt32(Globals::sm_tableEdgeModelAttrVTo);

    e = new te::graph::Edge(id, vFrom, vTo);

    e->setAttributeVecSize(dsType->getProperties().size() - 3);

    for(size_t i = 3; i < dsType->getProperties().size(); ++i)
    {
      e->addAttribute(i - 3, dataset->getValue(dsType->getProperty(i)->getName()).release());
    }
  }

  return e;
}

te::graph::Edge* te::graph::AbstractGraphLoaderStrategy::loadEdgeAttrs(int id)
{
  throw Exception(TE_TR("TO DO"));
}
