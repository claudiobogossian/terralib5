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
  \file terralib/sa/core/SkaterOperation.cpp

  \brief This file contains a class that represents the skater operation.

  \reference Adapted from TerraLib4.
*/

//TerraLib
#include "../../datatype/Property.h"
#include "../../datatype/SimpleData.h"
#include "../../graph/core/AbstractGraph.h"
#include "../../graph/core/AbstractGraphFactory.h"
#include "../../graph/core/Edge.h"
#include "../../graph/core/GraphMetadata.h"
#include "../../graph/core/Vertex.h"
#include "../../graph/iterator/MemoryIterator.h"
#include "../../graph/Globals.h"
#include "GeneralizedProximityMatrix.h"
#include "Utils.h"
#include "SkaterOperation.h"
#include "SkaterPartition.h"

//STL
#include <cassert>
#include <list>

//Boost
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>

te::sa::SkaterOperation::SkaterOperation()
{
}

te::sa::SkaterOperation::~SkaterOperation()
{
}

void te::sa::SkaterOperation::execute()
{
  assert(m_inputParams->m_ds.get());
  assert(m_inputParams->m_dsType.get());

  //associate gpm vertex attributes
  std::vector<int> attrsIdx;

  for(std::size_t t = 0; t < m_inputParams->m_attrs.size(); ++t)
  {
    std::string attrName = m_inputParams->m_attrs[t];

    std::size_t idx = m_inputParams->m_dsType->getPropertyPosition(attrName);
    int type = m_inputParams->m_ds->getPropertyDataType(idx);
    int gpmIdx = te::sa::AssociateGPMVertexAttribute(m_inputParams->m_gpm.get(), m_inputParams->m_ds.get(), m_inputParams->m_gpmAttrLink, attrName, type);

    attrsIdx.push_back(gpmIdx);
  }

  //calculate gpm edge attribute
  int edgeWeightIdx = te::sa::AddGraphEdgeAttribute(m_inputParams->m_gpm->getGraph(), TE_SA_SKATER_ATTR_WEIGHT_NAME, te::dt::DOUBLE_TYPE);

  createWeightAttribute(edgeWeightIdx, attrsIdx);

  //calculate the mst
  te::graph::AbstractGraph* graph = minimumSpanningTree(edgeWeightIdx);

  //partition the graph
  te::sa::SkaterPartition sp(graph, m_inputParams->m_attrs);

  sp.execute();

  delete graph;
}

void te::sa::SkaterOperation::setParameters(te::sa::SkaterInputParams* inParams, te::sa::SkaterOutputParams* outParams)
{
  m_inputParams.reset(inParams);
  m_outputParams.reset(outParams);
}

void te::sa::SkaterOperation::createWeightAttribute(int weightAttrIdx, std::vector<int> attrsIdx)
{
  te::graph::AbstractGraph* graph = m_inputParams->m_gpm->getGraph();

  int size = graph->getMetadata()->getEdgePropertySize();

  te::graph::MemoryIterator* iterator = new te::graph::MemoryIterator(graph);

  te::graph::Edge* edge = iterator->getFirstEdge();

  while(edge)
  {
    te::graph::Vertex* vFrom = graph->getVertex(edge->getIdFrom());
    te::graph::Vertex* vTo = graph->getVertex(edge->getIdTo());

    if(vFrom && vTo)
    {
      double weight = calculateWeight(attrsIdx, vFrom, vTo);

      edge->setAttributeVecSize(size);
      edge->addAttribute(weightAttrIdx, new te::dt::SimpleData<double, te::dt::DOUBLE_TYPE>(weight));
    }

    edge = iterator->getNextEdge();
  }
}

double te::sa::SkaterOperation::calculateWeight(std::vector<int> attrsIdx, te::graph::Vertex* vFrom, te::graph::Vertex* vTo)
{
  double weight = 0.;

  for(std::size_t t = 0; t < attrsIdx.size(); ++t)
  {
    double valueFrom = te::sa::GetDataValue(vFrom->getAttributes()[attrsIdx[t]]);
    double valueTo = te::sa::GetDataValue(vTo->getAttributes()[attrsIdx[t]]);

    weight += (valueTo - valueFrom) * (valueTo - valueFrom);
  }

  return sqrt(weight);
}

te::graph::AbstractGraph* te::sa::SkaterOperation::minimumSpanningTree(int weightAttrIdx)
{
  //get gpm graph
  te::graph::AbstractGraph* gpmGraph = m_inputParams->m_gpm->getGraph();

  te::graph::MemoryIterator* iterator = new te::graph::MemoryIterator(gpmGraph);

  std::size_t nEdge = iterator->getEdgeInteratorCount();
  std::size_t nVertex = iterator->getVertexInteratorCount();

  //create boost graph
  typedef boost::adjacency_list < boost::vecS, boost::vecS, boost::undirectedS, boost::no_property, boost::property<boost::edge_weight_t, int > > boostGraph;

  typedef std::pair < int, int > boostEdge;

  std::vector<boostEdge> edge_vec;

  std::vector<double> weight_vec;

  te::graph::Edge* edge = iterator->getFirstEdge();

  while(edge)
  {
    edge_vec.push_back(boostEdge(edge->getIdFrom(), edge->getIdTo()));

    weight_vec.push_back(te::sa::GetDataValue(edge->getAttributes()[weightAttrIdx]));

    edge = iterator->getNextEdge();
  }

  boostGraph graph(nVertex);

  boost::property_map<boostGraph, boost::edge_weight_t>::type weightmap = boost::get(boost::edge_weight, graph);

  for(std::size_t t = 0; t < nEdge; ++t)
  {
    boost::graph_traits<boostGraph>::edge_descriptor e;

    bool inserted;

    boost::tie(e, inserted) = boost::add_edge(edge_vec[t].first, edge_vec[t].second, graph);

    weightmap[e] = (int)weight_vec[t];
  }

  //run kruskal algorithm
  typedef boost::graph_traits<boostGraph>::edge_descriptor Edge;
  typedef boost::graph_traits<boostGraph>::vertex_descriptor Vertex;

  boost::property_map<boostGraph, boost::edge_weight_t>::type weight = boost::get(boost::edge_weight, graph);

  std::vector<Edge> spanning_tree;

  kruskal_minimum_spanning_tree(graph, std::back_inserter(spanning_tree));

  //create minimum spanning tree graph
  te::graph::AbstractGraph* graphOut = createGraph();

  //create edge weight property
  int edgeWeightIdx = te::sa::AddGraphEdgeAttribute(graphOut, TE_SA_SKATER_ATTR_WEIGHT_NAME, te::dt::DOUBLE_TYPE);
  int size = graphOut->getMetadata()->getEdgePropertySize();

  for(std::size_t t = 0; t < spanning_tree.size(); ++t)
  {
    Edge eCur = spanning_tree[t];

    int vertexSourceId = boost::source(eCur, graph);
    int vertexTargetId = boost::target(eCur, graph);

    double weightEdge = weight[eCur];

    //check if output graph already has the input vertex
    te::graph::Vertex* vFrom = graphOut->getVertex(vertexSourceId);

    if(!vFrom)
    {
      vFrom = new te::graph::Vertex(gpmGraph->getVertex(vertexSourceId));

      vFrom->getNeighborhood().clear();
      vFrom->getPredecessors().clear();
      vFrom->getSuccessors().clear();
      
      graphOut->add(vFrom);
    }

    //check if output graph already has the output vertex
    te::graph::Vertex* vTo = graphOut->getVertex(vertexTargetId);

    if(!vTo)
    {
      vTo = new te::graph::Vertex(gpmGraph->getVertex(vertexTargetId));

      vTo->getNeighborhood().clear();
      vTo->getPredecessors().clear();
      vTo->getSuccessors().clear();

      graphOut->add(vTo);
    }

    //create edge
    te::graph::Edge* e = new te::graph::Edge((int)t, vertexSourceId, vertexTargetId);

    e->setAttributeVecSize(size);
    e->addAttribute(edgeWeightIdx, new te::dt::SimpleData<double, te::dt::DOUBLE_TYPE>(weightEdge));

    graphOut->add(e);
  }

  return graphOut;
}

te::graph::AbstractGraph* te::sa::SkaterOperation::createGraph()
{
// graph type
  std::string graphType = te::graph::Globals::sm_factoryGraphTypeUndirectedGraph;

// connection info
  std::map<std::string, std::string> connInfo;

// graph information
  std::map<std::string, std::string> graphInfo;
  graphInfo["GRAPH_DATA_SOURCE_TYPE"] = "MEM";
  graphInfo["GRAPH_NAME"] = "mst_graph";
  graphInfo["GRAPH_DESCRIPTION"] = "Generated by Skater.";

  //create output graph
  te::graph::AbstractGraph* graph = te::graph::AbstractGraphFactory::make(graphType, connInfo, graphInfo);

  //copy vertex properties from gpm graph
  te::graph::AbstractGraph* gpmGraph = m_inputParams->m_gpm->getGraph();

  for(int i = 0; i < gpmGraph->getMetadata()->getVertexPropertySize(); ++i)
  {
    te::dt::Property* p = gpmGraph->getMetadata()->getVertexProperty(i)->clone();
    
    p->setParent(0);

    graph->getMetadata()->addVertexProperty(p);
  }

  return graph;
}
