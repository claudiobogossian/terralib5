#include "AbstractGraphLoaderStrategy.h"

#include "AbstractGraph.h"
#include "Edge.h"
#include "GraphCache.h"
#include "EdgeProperty.h"
#include "Globals.h"
#include "GraphData.h"
#include "GraphMetadata.h"
#include "Vertex.h"
#include "VertexProperty.h"

// Terralib Includes
#include "Config.h"
#include "Exception.h"
#include "../common/STLUtils.h"
#include "../common/StringUtils.h"
#include "../dataaccess.h"
#include "../datatype.h"
#include "../memory.h"
#include "../geometry.h"


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
    throw Exception(TR_GRAPH(""));
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
    throw Exception(TR_GRAPH(""));
  }
}


void te::graph::AbstractGraphLoaderStrategy::removeVertex(int id)
{
  if(m_graphMetadata == 0 || m_graphMetadata->getDataSource() == 0)
  {
    throw Exception(TR_GRAPH(""));
  }
}


void te::graph::AbstractGraphLoaderStrategy::saveGraphEdgeList(GraphData* data)
{
  if(data->isDirty() == false)
  {
    return;
  }

  // get a transactor to interact to the data source
  te::da::DataSourceTransactor* transactor = m_graphMetadata->getDataSource()->getTransactor();
  if(!transactor)
  {
    throw Exception(TR_GRAPH("Error getting Transactor."));
  }

  te::da::DataSourceCatalogLoader* catalog = transactor->getCatalogLoader();

  //get data set
  std::string tableName = m_graphMetadata->getEdgeTableName();

  te::da::DataSetType* ds = catalog->getDataSetType(tableName, true);

  //get persistence
  te::da::DataSetPersistence* dsP = transactor->getDataSetPersistence();

  te::mem::DataSet* outDataSet = new te::mem::DataSet((te::da::DataSetType*)ds->clone());
  te::mem::DataSet* outDataSetUpdate = new te::mem::DataSet((te::da::DataSetType*)ds->clone());

  te::graph::GraphData::EdgeMap::iterator it = data->getEdgeMap().begin();

  bool hasNewObjects = false;
  bool hasUpdatedObjects = false;

  while(it != data->getEdgeMap().end())
  {
    if(it->second->isNew()) //new element
    {
      //create new item
      te::da::DataSetItem* dsItem = new te::mem::DataSetItem((te::da::DataSetType*)ds->clone());

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
      te::da::DataSetItem* dsItem = new te::mem::DataSetItem((te::da::DataSetType*)ds->clone());

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

  if(hasNewObjects)
  {
    outDataSet->moveFirst();
    dsP->add(ds, outDataSet);
  }
  delete outDataSet;

  if(hasUpdatedObjects)
  {
    outDataSetUpdate->moveFirst();
    std::map<std::string, std::string> options;
    dsP->update(ds, outDataSetUpdate, ds->getProperties(), options);
  }
  delete outDataSetUpdate;

  //must save the vertex attributes
  if(m_graphMetadata->getVertexPropertySize() != 0)
  {
    saveVertexAttributes(data);
  }

  delete dsP;
  delete catalog;
  delete transactor;
}

void te::graph::AbstractGraphLoaderStrategy::saveVertexAttributes(GraphData* data)
{
  if(data->isDirty() == false)
  {
    return;
  }

  // get a transactor to interact to the data source
  te::da::DataSourceTransactor* transactor = m_graphMetadata->getDataSource()->getTransactor();
  if(!transactor)
  {
    throw Exception(TR_GRAPH("Error getting Transactor."));
  }

  te::da::DataSourceCatalogLoader* catalog = transactor->getCatalogLoader();

  //get data set

  std::string tableName = m_graphMetadata->getVertexTableName();

  te::da::DataSetType* ds = catalog->getDataSetType(tableName, true);

  //get persistence
  te::da::DataSetPersistence* dsP = transactor->getDataSetPersistence();

  te::mem::DataSet* outDataSet = new te::mem::DataSet((te::da::DataSetType*)ds->clone());
  te::mem::DataSet* outDataSetUpdate = new te::mem::DataSet((te::da::DataSetType*)ds->clone());

  te::graph::GraphData::VertexMap::iterator it = data->getVertexMap().begin();

  bool hasNewObjects = false;
  bool hasUpdatedObjects = false;

  while(it != data->getVertexMap().end())
  {
    if(it->second->isNew()) //new element
    {
      //create new item
      te::da::DataSetItem* dsItem = new te::mem::DataSetItem((te::da::DataSetType*)ds->clone());

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
      te::da::DataSetItem* dsItem = new te::mem::DataSetItem((te::da::DataSetType*)ds->clone());

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

  if(hasNewObjects)
  {
    outDataSet->moveFirst();
    dsP->add(ds, outDataSet);
  }
  delete outDataSet;

  if(hasUpdatedObjects)
  {
    outDataSetUpdate->moveFirst();
    std::map<std::string, std::string> options;
    dsP->update(ds, outDataSetUpdate, ds->getProperties(), options);
  }
  delete outDataSetUpdate;

  delete dsP;
  delete catalog;
  delete transactor;
}

void te::graph::AbstractGraphLoaderStrategy::saveGraphVertexList(GraphData* data)
{
  throw Exception(TR_GRAPH("TO DO"));
}

void te::graph::AbstractGraphLoaderStrategy::saveEdgeAttributes(GraphData* data)
{
  throw Exception(TR_GRAPH("TO DO"));
}

te::graph::Vertex* te::graph::AbstractGraphLoaderStrategy::loadVertex(int id)
{
  throw Exception(TR_GRAPH("TO DO"));
}

te::graph::Vertex* te::graph::AbstractGraphLoaderStrategy::loadVertexAttrs(int id)
{
  // get a transactor to interact to the data source
  te::da::DataSourceTransactor* transactor = m_graphMetadata->getDataSource()->getTransactor();

  if(!transactor)
  {
    throw Exception(TR_GRAPH("Error getting Transactor."));
  }

  //get data set
  std::string tableName = m_graphMetadata->getVertexTableName();

  te::da::DataSourceCatalogLoader* catalog = transactor->getCatalogLoader();

  te::da::DataSetType* ds = catalog->getDataSetType(tableName, true);

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

  te::da::DataSet* dataset = transactor->query(select);

  //create vertex
  te::graph::Vertex* v = 0;

  if(dataset->moveNext())
  {
    //get fixed attribute
    int id = dataset->getInt32(Globals::sm_tableVertexModelAttrId);

    v = new te::graph::Vertex(id);

    v->setAttributeVecSize(ds->getProperties().size() - 1);

    for(size_t i = 1; i < ds->getProperties().size(); ++i)
    {
      v->addAttribute(i - 1, dataset->getValue(ds->getProperty(i)->getName()));
    }
  }

  delete dataset;
  delete catalog;
  delete transactor;

  return v;
}

te::graph::Edge* te::graph::AbstractGraphLoaderStrategy::loadEdge(int id)
{
 // get a transactor to interact to the data source
  te::da::DataSourceTransactor* transactor = m_graphMetadata->getDataSource()->getTransactor();

  if(!transactor)
  {
    throw Exception(TR_GRAPH("Error getting Transactor."));
  }

  //get data set
  std::string tableName = m_graphMetadata->getEdgeTableName();

  te::da::DataSourceCatalogLoader* catalog = transactor->getCatalogLoader();

  te::da::DataSetType* ds = catalog->getDataSetType(tableName, true);

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

  te::da::DataSet* dataset = transactor->query(select);

  //create vertex
  te::graph::Edge* e = 0;

  if(dataset->moveNext())
  {
    //get fixed attributes
    int id = dataset->getInt32(Globals::sm_tableEdgeModelAttrId);
    int vFrom = dataset->getInt32(Globals::sm_tableEdgeModelAttrVFrom);
    int vTo = dataset->getInt32(Globals::sm_tableEdgeModelAttrVTo);

    e = new te::graph::Edge(id, vFrom, vTo);

    e->setAttributeVecSize(ds->getProperties().size() - 3);

    for(size_t i = 3; i < ds->getProperties().size(); ++i)
    {
      e->addAttribute(i - 3, dataset->getValue(ds->getProperty(i)->getName()));
    }
  }

  delete dataset;
  delete catalog;
  delete transactor;

  return e;
}

te::graph::Edge* te::graph::AbstractGraphLoaderStrategy::loadEdgeAttrs(int id)
{
  throw Exception(TR_GRAPH("TO DO"));
}
