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
  \file SpatialWeightsExchanger.cpp

  \brief This class defines functions used to load and save graphs using GAL and GWT formats,
  both formats use a ' ' as separator.

  GAL FORMAT

  0  NUMBER_OBSERVATIONS  DATASET_NAME  ATTRIBUTE_ID_NAME  (HEADER LINE)
  OBSERVATION_ID  NUMBER_NEIGHBOURS
  NEIGHBOURS_1  NEIGHBOURS_2 ... NEIGHBOURS_N
  OBSERVATION_ID  NUMBER_NEIGHBOURS
  NEIGHBOURS_1  NEIGHBOURS_2 ... NEIGHBOURS_N
  ...


  GWT FORMAT
  
  0  NUMBER_OBSERVATIONS  DATASET_NAME  ATTRIBUTE_ID_NAME  (HEADER LINE)
  OBSERVATION_ID_FROM  OBSERVATION_ID_TO  DISTANCE
  OBSERVATION_ID_FROM  OBSERVATION_ID_TO  DISTANCE
  OBSERVATION_ID_FROM  OBSERVATION_ID_TO  DISTANCE
  ...
*/

//Terralib Includes
#include "../../dataaccess/datasource/DataSource.h"
#include "../../dataaccess/utils/Utils.h"
#include "../../datatype/SimpleData.h"
#include "../../datatype/SimpleProperty.h"
#include "../../geometry/GeometryProperty.h"
#include "../../geometry/MultiPolygon.h"
#include "../../geometry/Point.h"
#include "../../geometry/Polygon.h"
#include "../core/AbstractGraph.h"
#include "../core/AbstractGraphFactory.h"
#include "../core/GraphMetadata.h"
#include "../core/Edge.h"
#include "../core/Vertex.h"
#include "../iterator/AbstractIterator.h"
#include "../iterator/SequenceIterator.h"
#include "../iterator/MemoryIterator.h"
#include "SpatialWeightsExchanger.h"

//STL Includes
#include <cassert>
#include <fstream>
#include <iosfwd>
#include <stdio.h>

// BOOST Includes
#include<boost/tokenizer.hpp>

te::graph::SpatialWeightsExchanger::SpatialWeightsExchanger() : m_edgeId(0)
{
}

te::graph::SpatialWeightsExchanger::~SpatialWeightsExchanger()
{
}

void te::graph::SpatialWeightsExchanger::exportToGAL(te::graph::AbstractGraph* g, std::string pathFileName, std::string dataSetName, std::string propertyName)
{
  //get iterator
  std::auto_ptr<te::graph::AbstractIterator> it;

  if(g->getMetadata()->getDataSource())
  {
    it.reset(new te::graph::SequenceIterator(g));
  }
  else
  {
    it.reset(new te::graph::MemoryIterator(g));
  }

  assert(it.get());

  //create file
  FILE* fp = fopen(pathFileName.c_str(), "w");

  assert(fp);

  //write header info
  if(dataSetName.empty())
  {
    fprintf(fp, "%d\n", (int)it->getVertexInteratorCount());
  }
  else
  {
    fprintf(fp, "0 %d %s %s\n", (int)it->getVertexInteratorCount(), dataSetName.c_str(), propertyName.c_str());
  }

  //write body info
  te::graph::Vertex* v = it->getFirstVertex();

  while(it->isVertexIteratorAfterEnd() == false)
  {
    int id = v->getId();
    std::set<int> neighbours = v->getNeighborhood();
    std::set<int>::iterator itNeighbours = neighbours.begin();

    if(!neighbours.empty())
    {
      fprintf(fp, "%d %d\n", id, (int)neighbours.size());

      while(itNeighbours != neighbours.end())
      {
        te::graph::Edge* e = g->getEdge(*itNeighbours);

        if(e)
        {
          if(e->getIdFrom() == id)
            fprintf (fp, "%d ", e->getIdTo());
          else
            fprintf (fp, "%d ", e->getIdFrom());
        }

        ++itNeighbours;
      }

      fprintf (fp, "\n");
    }

    v = it->getNextVertex();
  }

  fclose(fp);
}

te::graph::AbstractGraph* te::graph::SpatialWeightsExchanger::importFromGAL(std::string pathFileName, std::map<std::string, std::string> dsInfo, 
  std::string graphType, std::map<std::string, std::string> gInfo, te::da::DataSource* ds)
{
  //open file
  std::ifstream file(pathFileName.c_str());

  if(file.is_open() == false)
  {
    return 0;
  }

  //create output graph
  te::graph::AbstractGraph* graph = te::graph::AbstractGraphFactory::make(graphType, dsInfo, gInfo);
  assert(graph);

  //create boost tokenizer
  typedef boost::tokenizer< boost::escaped_list_separator<char> > Tokenizer;
  boost::escaped_list_separator<char> sep('\\', ' ', '\"');
  
  std::vector<std::string> line;
  std::string buffer;

  //get header line
  std::getline(file, buffer);
  Tokenizer tok(buffer, sep);
  line.assign(tok.begin(), tok.end());

  std::string dataSetName = "";
  std::string attributeName = "";
  bool associateGeom = false;

  if(line.size() == 4 && ds) // has the number of observations and data set information
  {
    associateGeom = true;

    dataSetName = line[2];
    attributeName = line[3];
  }
  
  //access each line of the gat file
  m_edgeId = 0;

  while(std::getline(file, buffer))
  {
    line.clear();
    Tokenizer tok(buffer, sep);
    line.assign(tok.begin(), tok.end());

    std::string vertexIdStr, nNeighboursStr;

    try
    {
      vertexIdStr = line[0];
      nNeighboursStr = line[1];
    }
    catch(...)
    {
      delete graph;

      return 0;
    }

    //create vertex
    int vId = atoi(vertexIdStr.c_str());
    Vertex* v = graph->getVertex(vId);
    
    if(!v)
    {
      v = new Vertex(vId);
      graph->add(v);
    }

    //get new line in gal file
    std::getline(file, buffer);
    line.clear();
    Tokenizer tok2(buffer, sep);
    line.assign(tok2.begin(), tok2.end());

    //get neighbours
    int nNeighbours = atoi(nNeighboursStr.c_str());

    try
    {
      for(int i = 0; i < nNeighbours; ++i)
      {
        std::string vNeighbourStr = line[i];
        int  vNeighbourId = atoi(vNeighbourStr.c_str());

        //add vertex neighbour
        Vertex* vNeighbour = graph->getVertex(vNeighbourId);
    
        if(!vNeighbour)
        {
          vNeighbour = new Vertex(vNeighbourId);
          graph->add(vNeighbour);
        }

        //create edge
        int id = getEdgeId();

        Edge* e = new Edge(id, vId, vNeighbourId);
        graph->add(e);
      }
    }
    catch(...)
    {
      delete graph;

      return 0;
    }
  }

  file.close();

  if(associateGeom)
    associateGeometry(graph, ds, dataSetName, attributeName);

  return graph;
}

void te::graph::SpatialWeightsExchanger::exportToGWT(te::graph::AbstractGraph* g, std::string pathFileName, int distAttrIdx, std::string dataSetName, std::string propertyName)
{
  //get iterator
  std::auto_ptr<te::graph::AbstractIterator> it;

  if(g->getMetadata()->getDataSource())
  {
    it.reset(new te::graph::SequenceIterator(g));
  }
  else
  {
    it.reset(new te::graph::MemoryIterator(g));
  }

  assert(it.get());

  //create file
  FILE* fp = fopen(pathFileName.c_str(), "w");

  assert(fp);

  //write header info
  if(dataSetName.empty())
  {
    fprintf(fp, "%d\n", (int)it->getEdgeInteratorCount());
  }
  else
  {
    fprintf(fp, "0 %d %s %s\n", (int)it->getEdgeInteratorCount(), dataSetName.c_str(), propertyName.c_str());
  }

  //write body info
  te::graph::Edge* e = it->getFirstEdge();

  while(it->isEdgeIteratorAfterEnd() == false)
  {
    int idFrom = e->getIdFrom();
    int idTo = e->getIdTo();

    te::dt::SimpleData<double, te::dt::DOUBLE_TYPE>* sd = dynamic_cast< te::dt::SimpleData<double, te::dt::DOUBLE_TYPE>* >(e->getAttributes()[distAttrIdx]);
    
    double distance = sd->getValue();

    fprintf(fp, "%d %d %3.7f\n", idFrom, idTo, distance);

    e = it->getNextEdge();
  }

  fclose(fp);
}

te::graph::AbstractGraph* te::graph::SpatialWeightsExchanger::importFromGWT(std::string pathFileName, std::map<std::string, std::string> dsInfo, 
  std::string graphType, std::map<std::string, std::string> gInfo, te::da::DataSource* ds)
{
  //open file
  std::ifstream file(pathFileName.c_str());

  if(file.is_open() == false)
  {
    return 0;
  }

  //create output graph
  te::graph::AbstractGraph* graph = te::graph::AbstractGraphFactory::make(graphType, dsInfo, gInfo);
  assert(graph);

  //add edge property
  te::dt::SimpleProperty* p = new te::dt::SimpleProperty("dist", te::dt::DOUBLE_TYPE);
  p->setParent(0);
  p->setId(0);

  graph->addEdgeProperty(p);

  //create boost tokenizer
  typedef boost::tokenizer< boost::escaped_list_separator<char> > Tokenizer;
  boost::escaped_list_separator<char> sep('\\', ' ', '\"');
  
  std::vector<std::string> line;
  std::string buffer;

  //get header line
  std::getline(file, buffer);
  Tokenizer tok(buffer, sep);
  line.assign(tok.begin(), tok.end());

  std::string dataSetName = "";
  std::string attributeName = "";
  bool associateGeom = false;

  if(line.size() == 4 && ds) // has the number of observations and data set information
  {
    associateGeom = true;

    dataSetName = line[2];
    attributeName = line[3];
  }
  
  //access each line of the gwt file
  m_edgeId = 0;

  while(std::getline(file, buffer))
  {
    line.clear();

    Tokenizer tok(buffer, sep);

    line.assign(tok.begin(), tok.end());

    std::string fromStr, toStr, distanceStr;

    try
    {
      fromStr = line[0];
      toStr = line[1];
      distanceStr = line[2];
    }
    catch(...)
    {
      delete graph;

      return 0;
    }

    //create vertex from
    int from = atoi(fromStr.c_str());

    Vertex* vFrom = graph->getVertex(from);
    
    if(!vFrom)
    {
      vFrom = new Vertex(from);
      graph->add(vFrom);
    }

    //create vertex to
    int to = atoi(toStr.c_str());

    Vertex* vTo = graph->getVertex(to);

    if(!vTo)
    {
      vTo = new Vertex(to);
      graph->add(vTo);
    }

    //create edge
    int id = getEdgeId();
    double distance = atof(distanceStr.c_str());

    Edge* e = new Edge(id, from, to);
    e->setAttributeVecSize(1); //distance attribute
    e->addAttribute(0, new te::dt::SimpleData<double, te::dt::DOUBLE_TYPE>(distance));

    graph->add(e);
  }

  file.close();

  if(associateGeom)
    associateGeometry(graph, ds, dataSetName, attributeName);

  return graph;
}

int te::graph::SpatialWeightsExchanger::getEdgeId()
{
  int id = m_edgeId;

  m_edgeId++;

  return id;
}

void te::graph::SpatialWeightsExchanger::associateGeometry(te::graph::AbstractGraph* g, te::da::DataSource* ds, 
  std::string dataSetName, std::string propertyName)
{
  //get srid information
  std::auto_ptr<te::da::DataSetType> dataSetType = ds->getDataSetType(dataSetName);
  te::gm::GeometryProperty* gp = te::da::GetFirstGeomProperty(dataSetType.get());

  //create graph vertex attrs
  te::gm::GeometryProperty* gProp = new te::gm::GeometryProperty("coords");
  gProp->setId(0);
  gProp->setGeometryType(te::gm::PointType);
  gProp->setSRID(gp->getSRID());

  g->addVertexProperty(gProp);

  //get data set
  std::auto_ptr<te::da::DataSet> dataSet = ds->getDataSet(dataSetName);

  std::size_t geomPos = te::da::GetFirstSpatialPropertyPos(dataSet.get());

  dataSet->moveBeforeFirst();

  while(dataSet->moveNext())
  {
    int id = dataSet->getInt32(propertyName);

    Vertex* v = g->getVertex(id);

    if(v)
    {
      v->setAttributeVecSize(1);

      std::auto_ptr<te::gm::Geometry> g = dataSet->getGeometry(geomPos);

      te::dt::AbstractData* ad = 0;

      if(g->getGeomTypeId() == te::gm::PointType)
      {
        g->setSRID(gp->getSRID());

        ad = g->clone();
      }
      else if(g->getGeomTypeId() == te::gm::PolygonType)
      {
        te::gm::Point* p = ((te::gm::Polygon*)g.get())->getCentroid();
        p->setSRID(gp->getSRID());

        ad = p;
      }
      else if(g->getGeomTypeId() == te::gm::MultiPolygonType)
      {
        te::gm::Polygon* poly = (te::gm::Polygon*)((te::gm::MultiPolygon*)g.get())->getGeometryN(0);

        te::gm::Point* p = poly->getCentroid();
        p->setSRID(gp->getSRID());

        ad = p;
      }

      v->addAttribute(0, ad);

      v->setDirty(true);
    }
  }
}
