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
  \file RAGGraphBuilder.cpp

  \brief This class defines the RAG strategy to build a graph,

        This is strategy is based on adjacency of a region.

*/

// TerraLib
#include "../../common/progress/TaskProgress.h"
#include "../../common/StringUtils.h"
#include "../../common/STLUtils.h"
#include "../../common/Translator.h"
#include "../../dataaccess/datasource/DataSource.h"
#include "../../dataaccess/datasource/DataSourceFactory.h"
#include "../../datatype/AbstractData.h"
#include "../../geometry/GeometryProperty.h"
#include "../../geometry/MultiPolygon.h"
#include "../../geometry/Point.h"
#include "../../geometry/Polygon.h"
#include "../../sam/rtree.h"
#include "../core/AbstractGraphFactory.h"
#include "../core/Edge.h"
#include "../core/Vertex.h"
#include "../core/VertexProperty.h"
#include "../graphs/Graph.h"
#include "../Config.h"
#include "../Exception.h"
#include "RAGGraphBuilder.h"


te::graph::RAGGraphBuilder::RAGGraphBuilder() : AbstractGraphBuilder()
{
  m_edgeId = 0;
}

te::graph::RAGGraphBuilder::~RAGGraphBuilder()
{
}

bool te::graph::RAGGraphBuilder::build(const std::string& shapeFileName, const std::string& linkColumn, const int& srid, 
  const std::map<std::string, std::string>& dsInfo, const std::string& graphType, const std::map<std::string, std::string>& gInfo)
{
  //create output graph
  m_graph.reset(te::graph::AbstractGraphFactory::make(graphType, dsInfo, gInfo));

  assert(m_graph);

  if(createVertexObjects(shapeFileName, linkColumn, srid) == false)
  {
    return false;
  }

  if(createEdgeObjects(shapeFileName, linkColumn) == false)
  {
    return false;
  }


  return true;
}

int  te::graph::RAGGraphBuilder::getEdgeId()
{
  int id = m_edgeId;

  m_edgeId++;

  return id;
}


std::auto_ptr<te::da::DataSource> te::graph::RAGGraphBuilder::getDataSource(const std::string& fileName)
{
  // Creates and connects data source
  std::map<std::string, std::string> connInfo;
  connInfo["URI"] = fileName;
  std::auto_ptr<te::da::DataSource> ds = te::da::DataSourceFactory::make("OGR");
  ds->setConnectionInfo(connInfo);
  ds->open();

  return ds;
}

std::auto_ptr<te::da::DataSet> te::graph::RAGGraphBuilder::getDataSet(te::da::DataSource* ds)
{
  std::vector<std::string> names = ds->getDataSetNames();

  std::string dsName = names[0];

  std::auto_ptr<te::da::DataSet> dataset = ds->getDataSet(dsName);

  return dataset;
}

boost::ptr_vector<te::dt::Property> te::graph::RAGGraphBuilder::getProperties(te::da::DataSource* ds)
{
  std::vector<std::string> names = ds->getDataSetNames();

  std::string dsName = names[0];

  return ds->getProperties(dsName);
}

bool te::graph::RAGGraphBuilder::createVertexObjects(const std::string& shapeFileName, const std::string& linkColumn, const int& srid)
{
 //get data source
  std::auto_ptr<te::da::DataSource> ds = getDataSource(shapeFileName);

  if(ds.get() == 0)
  {
    return false;
  }

  //get data set
  std::auto_ptr<te::da::DataSet> dataSet = getDataSet(ds.get());

  if(dataSet.get() == 0)
  {
    return false;
  }

  //create graph vertex attrs
  te::gm::GeometryProperty* gProp = new te::gm::GeometryProperty("coords");
  gProp->setId(0);
  gProp->setGeometryType(te::gm::PointType);
  gProp->setSRID(srid);

  m_graph->addVertexProperty(gProp);

  //get geometry column
  std::string geomColumn = "";
  if(getGeometryColumn(shapeFileName, geomColumn) == false)
  {
    return false;
  }

  //create vertex objects
  while(dataSet->moveNext())
  {
    int id = dataSet->getInt32(linkColumn);

    Vertex* v = new Vertex(id);
    
    v->setAttributeVecSize(1);

    te::gm::Geometry* g = dataSet->getGeometry(geomColumn).release();
    g->setSRID(srid);

    te::dt::AbstractData* ad = 0;

    if(g->getGeomTypeId() == te::gm::PointType)
    {
      ad = g;
    }
    else if(g->getGeomTypeId() == te::gm::PolygonType)
    {
      te::gm::Point* p = ((te::gm::Polygon*)g)->getCentroid();
      p->setSRID(srid);

      ad = p;
    }
    else if(g->getGeomTypeId() == te::gm::MultiPolygonType)
    {
      te::gm::Polygon* poly = (te::gm::Polygon*)((te::gm::MultiPolygon*)g)->getGeometryN(0);

      te::gm::Point* p = poly->getCentroid();
      p->setSRID(srid);

      ad = p;
    }

    v->addAttribute(0, ad);

    m_graph->add(v);
  }

  return true;
}

bool te::graph::RAGGraphBuilder::createEdgeObjects(const std::string& shapeFileName, const std::string& linkColumn)
{
 //get data source
  std::auto_ptr<te::da::DataSource> ds = getDataSource(shapeFileName);

  if(ds.get() == 0)
  {
    return false;
  }

  //get data set
  std::auto_ptr<te::da::DataSet> dataSet = getDataSet(ds.get());

  if(dataSet.get() == 0)
  {
    return false;
  }

  //get geometry column
  std::string geomColumn = "";
  if(getGeometryColumn(shapeFileName, geomColumn) == false)
  {
    return false;
  }

  te::sam::rtree::Index<int> rtree;
  std::map<int, te::gm::Geometry*> geomMap;
  //create tree
  while(dataSet->moveNext())
  {
    int id = dataSet->getInt32(linkColumn);
    te::gm::Geometry* g = dataSet->getGeometry(geomColumn).release();
    const te::gm::Envelope* box = g->getMBR();

    rtree.insert(*box, id);

    geomMap.insert(std::map<int, te::gm::Geometry*>::value_type(id, g));
  }

  dataSet->moveBeforeFirst();

  te::common::TaskProgress task;
  
  task.setTotalSteps(dataSet->size());
  task.setMessage("RAG Builder - Extracting Edges");

  //create vertex objects
  while(dataSet->moveNext())
  {
    int vFromId = dataSet->getInt32(linkColumn);

    std::auto_ptr<te::gm::Geometry> g = dataSet->getGeometry(geomColumn);

    std::vector<int> results;

    rtree.search(*g->getMBR(), results);

    for(size_t t = 0; t < results.size(); ++t)
    {
      std::map<int, te::gm::Geometry*>::iterator it = geomMap.find(results[t]);

      if(it != geomMap.end())
      {
        if(g->touches(it->second))
        {
          int edgeId = getEdgeId();

          int vToId = results[t];

          Edge* e = new Edge(edgeId, vFromId, vToId);

          m_graph->add(e);
        }
      }
    }

    task.pulse();
  }

  te::common::FreeContents(geomMap);
  geomMap.clear();

  return true;
}

bool te::graph::RAGGraphBuilder::getGeometryColumn(const std::string& shapeFileName, std::string& columnName)
{
 //get data source
  std::auto_ptr<te::da::DataSource> ds = getDataSource(shapeFileName);

  if(ds.get() == 0)
  {
    return false;
  }

  //get properties
  boost::ptr_vector<te::dt::Property> properties = getProperties(ds.get());

  if(properties.empty())
  {
    return false;
  }

  boost::ptr_vector<te::dt::Property>::iterator it = properties.begin();
  while(it != properties.end())
  {
    if(it->getType() == te::dt::GEOMETRY_TYPE)
    {
      columnName = it->getName();
    }

    ++it;
  }

  if(columnName.empty())
  {
    return false;
  }

  return true;
}
