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
  \file GPMGraphBuilder.cpp

  \brief This class defines the GPM strategy to build a graph,

        This is builder uses diferent strategies to build a graph based
        on a generalized proximity matrix.

*/

// TerraLib
#include "../../common/progress/TaskProgress.h"
#include "../../common/StringUtils.h"
#include "../../common/STLUtils.h"
#include "../../common/Translator.h"
#include "../../dataaccess/datasource/DataSource.h"
#include "../../dataaccess/utils/Utils.h"
#include "../../datatype/AbstractData.h"
#include "../../datatype/SimpleData.h"
#include "../../datatype/SimpleProperty.h"
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
#include "GPMGraphBuilder.h"


te::graph::GPMGraphBuilder::GPMGraphBuilder() : AbstractGraphBuilder()
{
  m_edgeId = 0;
}

te::graph::GPMGraphBuilder::~GPMGraphBuilder()
{
}

bool te::graph::GPMGraphBuilder::setGraphInfo(const std::map<std::string, std::string>& dsInfo, const std::string& graphType, const std::map<std::string, std::string>& gInfo)
{
  //create output graph
  m_graph.reset(te::graph::AbstractGraphFactory::make(graphType, dsInfo, gInfo));

  assert(m_graph);

  return true;
}

bool te::graph::GPMGraphBuilder::buildAdjacency(std::auto_ptr<te::da::DataSource> ds, std::string dataSetName, std::string columnId, bool calcDist)
{
  std::auto_ptr<te::da::DataSet> dataSet = ds->getDataSet(dataSetName);

  std::auto_ptr<te::da::DataSetType> dataSetType = ds->getDataSetType(dataSetName);

  te::gm::GeometryProperty* gp = te::da::GetFirstGeomProperty(dataSetType.get());

  createVertexObjects(dataSet.get(), columnId, gp->getSRID());

  createAdjacencyEdges(dataSet.get(), columnId, calcDist);

  return true;
}

bool te::graph::GPMGraphBuilder::buildDistance(std::auto_ptr<te::da::DataSource> ds, std::string dataSetName, std::string columnId, double dist)
{
  std::auto_ptr<te::da::DataSet> dataSet = ds->getDataSet(dataSetName);

  std::auto_ptr<te::da::DataSetType> dataSetType = ds->getDataSetType(dataSetName);

  te::gm::GeometryProperty* gp = te::da::GetFirstGeomProperty(dataSetType.get());

  createVertexObjects(dataSet.get(), columnId, gp->getSRID());

  createDistanceEdges(dataSet.get(), columnId, dist);

  return true;
}

void te::graph::GPMGraphBuilder::createVertexObjects(te::da::DataSet* dataSet, std::string columnId, int srid)
{
  std::size_t geomPos = te::da::GetFirstSpatialPropertyPos(dataSet);

  //create graph vertex attrs
  te::gm::GeometryProperty* gProp = new te::gm::GeometryProperty("coords");
  gProp->setId(0);
  gProp->setGeometryType(te::gm::PointType);
  gProp->setSRID(srid);

  m_graph->addVertexProperty(gProp);

  //create graph vertex
  dataSet->moveBeforeFirst();

  while(dataSet->moveNext())
  {
    int id = dataSet->getInt32(columnId);

    Vertex* v = new Vertex(id);
    
    v->setAttributeVecSize(1);

    std::auto_ptr<te::gm::Geometry> g = dataSet->getGeometry(geomPos);

    te::dt::AbstractData* ad = 0;

    if(g->getGeomTypeId() == te::gm::PointType)
    {
      g->setSRID(srid);

      ad = g->clone();
    }
    else if(g->getGeomTypeId() == te::gm::PolygonType)
    {
      te::gm::Point* p = ((te::gm::Polygon*)g.get())->getCentroid();
      p->setSRID(srid);

      ad = p;
    }
    else if(g->getGeomTypeId() == te::gm::MultiPolygonType)
    {
      te::gm::Polygon* poly = (te::gm::Polygon*)((te::gm::MultiPolygon*)g.get())->getGeometryN(0);

      te::gm::Point* p = poly->getCentroid();
      p->setSRID(srid);

      ad = p;
    }

    v->addAttribute(0, ad);

    m_graph->add(v);
  }
}

void te::graph::GPMGraphBuilder::createAdjacencyEdges(te::da::DataSet* dataSet, std::string columnId, bool calcDist)
{
  std::size_t geomPos = te::da::GetFirstSpatialPropertyPos(dataSet);

  //create distance attribute
  if(calcDist)
  {
    te::dt::SimpleProperty* p = new te::dt::SimpleProperty("dist", te::dt::DOUBLE_TYPE);
    p->setParent(0);
    p->setId(0);

    m_graph->addEdgeProperty(p);
  }

  //create tree
  te::sam::rtree::Index<int> rtree;
  std::map<int, te::gm::Geometry*> geomMap;

  dataSet->moveBeforeFirst();

  while(dataSet->moveNext())
  {
    int id = dataSet->getInt32(columnId);
    te::gm::Geometry* g = dataSet->getGeometry(geomPos).release();
    const te::gm::Envelope* box = g->getMBR();

    rtree.insert(*box, id);

    geomMap.insert(std::map<int, te::gm::Geometry*>::value_type(id, g));
  }

  te::common::TaskProgress task;
  
  task.setTotalSteps(dataSet->size());
  task.setMessage("GPM Builder - Extracting Edges");

  //create vertex objects
  dataSet->moveBeforeFirst();

  while(dataSet->moveNext())
  {
    int vFromId = dataSet->getInt32(columnId);

    std::auto_ptr<te::gm::Geometry> g = dataSet->getGeometry(geomPos);

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

          if(calcDist)
          {
            Vertex* vFrom = m_graph->getVertex(vFromId);
            te::gm::Point* pFrom = dynamic_cast<te::gm::Point*>(vFrom->getAttributes()[0]);

            Vertex* vTo = m_graph->getVertex(vToId);
            te::gm::Point* pTo = dynamic_cast<te::gm::Point*>(vTo->getAttributes()[0]);

            double dist = pFrom->distance(pTo);

            te::dt::SimpleData<double, te::dt::DOUBLE_TYPE>* sd = new te::dt::SimpleData<double, te::dt::DOUBLE_TYPE>(dist);
            
            e->setAttributeVecSize(1);
            e->addAttribute(0, sd);
          }

          m_graph->add(e);
        }
      }
    }

    task.pulse();
  }

  te::common::FreeContents(geomMap);
  geomMap.clear();
}

void te::graph::GPMGraphBuilder::createDistanceEdges(te::da::DataSet* dataSet, std::string columnId, double distance)
{
  std::size_t geomPos = te::da::GetFirstSpatialPropertyPos(dataSet);

  //create distance attribute
  te::dt::SimpleProperty* p = new te::dt::SimpleProperty("dist", te::dt::DOUBLE_TYPE);
  p->setParent(0);
  p->setId(0);

  m_graph->addEdgeProperty(p);

  //create tree
  te::sam::rtree::Index<int> rtree;
  std::map<int, te::gm::Geometry*> geomMap;

  dataSet->moveBeforeFirst();

  while(dataSet->moveNext())
  {
    int id = dataSet->getInt32(columnId);
    te::gm::Geometry* g = dataSet->getGeometry(geomPos).release();
    const te::gm::Envelope* box = g->getMBR();

    rtree.insert(*box, id);

    geomMap.insert(std::map<int, te::gm::Geometry*>::value_type(id, g));
  }

  te::common::TaskProgress task;
  
  task.setTotalSteps(dataSet->size());
  task.setMessage("GPM Builder - Extracting Edges");

  //create vertex objects
  dataSet->moveBeforeFirst();

  while(dataSet->moveNext())
  {
    int vFromId = dataSet->getInt32(columnId);

    std::auto_ptr<te::gm::Geometry> g = dataSet->getGeometry(geomPos);

    std::vector<int> results;

    te::gm::Envelope ext(*g->getMBR());

    ext.m_llx -= distance;
    ext.m_lly -= distance;
    ext.m_urx += distance;
    ext.m_ury += distance;

    rtree.search(ext, results);

    for(size_t t = 0; t < results.size(); ++t)
    {
      std::map<int, te::gm::Geometry*>::iterator it = geomMap.find(results[t]);

      if(it != geomMap.end())
      {
        int vToId = results[t];

        Vertex* vFrom = m_graph->getVertex(vFromId);
        te::gm::Point* pFrom = dynamic_cast<te::gm::Point*>(vFrom->getAttributes()[0]);

        Vertex* vTo = m_graph->getVertex(vToId);
        te::gm::Point* pTo = dynamic_cast<te::gm::Point*>(vTo->getAttributes()[0]);

        double dist = pFrom->distance(pTo);

        if(dist <= distance)
        {
          int edgeId = getEdgeId();

          Edge* e = new Edge(edgeId, vFromId, vToId);

          te::dt::SimpleData<double, te::dt::DOUBLE_TYPE>* sd = new te::dt::SimpleData<double, te::dt::DOUBLE_TYPE>(dist);
            
          e->setAttributeVecSize(1);
          e->addAttribute(0, sd);

          m_graph->add(e);
        }
      }
    }

    task.pulse();
  }

  te::common::FreeContents(geomMap);
  geomMap.clear();
}

int  te::graph::GPMGraphBuilder::getEdgeId()
{
  int id = m_edgeId;

  m_edgeId++;

  return id;
}
