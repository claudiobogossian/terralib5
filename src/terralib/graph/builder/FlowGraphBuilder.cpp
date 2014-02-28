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
  \file terralib/graph/FlowGraphBuilder.cpp

  \brief This class defines the Flow strategy to build a graph.

  This function needs a vectorial data and table with flow information,
  this table must have the origin and destination info about each flow.

*/
 
// TerraLib Includes
#include "../../common/StringUtils.h"
#include "../../common/Translator.h"
#include "../../dataaccess/datasource/DataSource.h"
#include "../../dataaccess/datasource/DataSourceFactory.h"
#include "../../dataaccess/dataset/DataSet.h"
#include "../../datatype/AbstractData.h"
#include "../../datatype/SimpleData.h"
#include "../../datatype/SimpleProperty.h"
#include "../../geometry/GeometryProperty.h"
#include "../../geometry/MultiPolygon.h"
#include "../../geometry/Point.h"
#include "../../geometry/Polygon.h"
#include "../core/AbstractGraphFactory.h"
#include "../core/Edge.h"
#include "../core/Vertex.h"
#include "../core/VertexProperty.h"
#include "../graphs/Graph.h"
#include "../Config.h"
#include "../Exception.h"
#include "FlowGraphBuilder.h"

// STL Includes
#include <fstream>
#include <iosfwd>

// BOOST Includes
#include<boost/tokenizer.hpp>


te::graph::FlowGraphBuilder::FlowGraphBuilder() : AbstractGraphBuilder()
{
  m_edgeId = 0;
}

te::graph::FlowGraphBuilder::~FlowGraphBuilder()
{
}

bool te::graph::FlowGraphBuilder::build(const std::string& shapeFileName, const std::string& linkColumn, const int& srid, const std::string& csvFileName, const int& fromIdx, const int& toIdx, const int& weightIdx,
  const std::map<std::string, std::string>& dsInfo, const std::string& graphType, const std::map<std::string, std::string>& gInfo)
{
  //create output graph
  m_graph.reset(te::graph::AbstractGraphFactory::make(graphType, dsInfo, gInfo));

  assert(m_graph);

  if(createVertexObjects(shapeFileName, linkColumn, srid) == false)
  {
    return false;
  }

  if(createEdgeObjects(csvFileName, fromIdx, toIdx, weightIdx) == false)
  {
    return false;
  }

  return true;
}

int  te::graph::FlowGraphBuilder::getEdgeId()
{
  int id = m_edgeId;

  m_edgeId++;

  return id;
}

std::auto_ptr<te::da::DataSource> te::graph::FlowGraphBuilder::getDataSource(const std::string fileName)
{
  // Creates and connects data source
  std::map<std::string, std::string> connInfo;
  connInfo["path"] = fileName;
  std::auto_ptr<te::da::DataSource> ds = te::da::DataSourceFactory::make("OGR");
  ds->setConnectionInfo(connInfo);
  ds->open();

  return ds;
}

std::auto_ptr<te::da::DataSet> te::graph::FlowGraphBuilder::getDataSet(te::da::DataSource* ds)
{
  std::vector<std::string> names = ds->getDataSetNames();

  std::string dsName = names[0];

  std::auto_ptr<te::da::DataSet> dataset = ds->getDataSet(dsName);

  return dataset;
}

boost::ptr_vector<te::dt::Property> te::graph::FlowGraphBuilder::getProperties(te::da::DataSource* ds)
{
  std::vector<std::string> names = ds->getDataSetNames();

  std::string dsName = names[0];

  return ds->getProperties(dsName);
}

bool te::graph::FlowGraphBuilder::createVertexObjects(const std::string& shapeFileName, const std::string& linkColumn, const int& srid)
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

  //get properties
  boost::ptr_vector<te::dt::Property> properties = getProperties(ds.get());

  if(properties.empty())
  {
    return false;
  }

//create graph vertex attrs
  boost::ptr_vector<te::dt::Property>::iterator it = properties.begin();

  int count = 0;

  while(it != properties.end())
  {
    if(te::common::Convert2UCase(it->getName()) == te::common::Convert2UCase(linkColumn))
    {
      continue;
    }

    te::dt::Property* p;

    if(it->getType() == te::dt::GEOMETRY_TYPE)
    {
      //create graph attrs
      te::gm::GeometryProperty* gProp = new te::gm::GeometryProperty("coords");
      gProp->setId(0);
      gProp->setGeometryType(te::gm::PointType);
      gProp->setSRID(srid);

      p = gProp;
    }
    else
    {
      p = it->clone();
      p->setParent(0);
      p->setId(count);
    }

    m_graph->addVertexProperty(p);

    ++count;

    ++it;
  }

  //create vertex objects
  while(dataSet->moveNext())
  {
    int id = dataSet->getInt32(linkColumn);

    Vertex* v = new Vertex(id);
    
    v->setAttributeVecSize(properties.size() - 1);

    int shift = 0;

    it = properties.begin();

    count = 0;

    while(it != properties.end())
    {
      if(te::common::Convert2UCase(it->getName()) == te::common::Convert2UCase(linkColumn))
      {
        shift = 1;
        continue;
      }

      te::dt::AbstractData* ad = 0;

      if(it->getType() == te::dt::GEOMETRY_TYPE)
      {
        te::gm::Geometry* g = dataSet->getGeometry(it->getName()).release();
        g->setSRID(srid);

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
      }
      else
      {
        ad = dataSet->getValue(it->getName()).release();
      }
      
      v->addAttribute(count - shift, ad);

      ++ count;

      ++it;
    }

    m_graph->add(v);
  }

  return true;
}

bool te::graph::FlowGraphBuilder::createEdgeObjects(const std::string& csvFileName, const int& fromIdx, const int& toIdx, const int& weightIdx)
{
  //open file
  std::ifstream in(csvFileName.c_str());

  if(in.is_open() == false)
  {
    return false;
  }

  te::dt::SimpleProperty* p = new te::dt::SimpleProperty("weight", te::dt::INT32_TYPE);
  p->setParent(0);
  p->setId(0);

  m_graph->addEdgeProperty(p);

  //create boost tokenizer
  typedef boost::tokenizer< boost::escaped_list_separator<char> > Tokenizer;
  boost::escaped_list_separator<char> sep('\\', ';', '\"');
  
  std::vector<std::string> line;
  std::string buffer;

  //access each line of the csv file
  while(std::getline(in, buffer))
  {
    line.clear();

    Tokenizer tok(buffer, sep);

    line.assign(tok.begin(), tok.end());

    std::string fromStr, toStr, weightStr;
    try
    {
      fromStr = line[fromIdx];
      toStr = line[toIdx];
      weightStr = line[weightIdx];
    }
    catch(...)
    {
      return false;
    }

    //create edge
    int id = getEdgeId();
    int from = atoi(fromStr.c_str());
    int to = atoi(toStr.c_str());
    int weight = atoi(weightStr.c_str());

    Edge* e = new Edge(id, from, to);

    e->setAttributeVecSize(1); //weight attribute
    e->addAttribute(0, new te::dt::SimpleData<int, te::dt::INT32_TYPE>(weight));

    m_graph->add(e);
  }

  return true;
}
