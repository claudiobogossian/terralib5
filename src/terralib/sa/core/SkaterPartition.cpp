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
  \file terralib/sa/core/SkaterPartition.cpp

  \brief This file contains a class that represents the skater partition operation.

  \reference Adapted from TerraLib4.
*/

//TerraLib
#include "../../graph/core/Edge.h"
#include "../../graph/core/Vertex.h"
#include "../../graph/iterator/MemoryIterator.h"
#include "SkaterPartition.h"
#include "Utils.h"

//STL
#include <cassert>
#include <queue>

te::sa::SkaterPartition::SkaterPartition(te::graph::AbstractGraph* graph, std::vector<std::string> attrs)
{
  m_graph = graph;

  m_attrs = attrs;
}

te::sa::SkaterPartition::~SkaterPartition()
{

}

std::vector<std::size_t> te::sa::SkaterPartition::execute(std::size_t nGroups, std::string popAttr, std::size_t minPop)
{
  assert(m_graph);

  m_popAttr = popAttr;

  m_popMin = minPop;

  //get first vertex id from graph
  std::auto_ptr<te::graph::MemoryIterator> memIt(new te::graph::MemoryIterator(m_graph));
  int firstVertex = memIt->getFirstVertex()->getId();
  
  //create vector with root information
  std::vector< std::pair<int, double> > rootSet;
  rootSet.push_back(std::vector< std::pair<int, double> >::value_type(firstVertex, 0.));

  std::vector<std::size_t> rootsVertexId;

  //bfs over the graph
  while(!rootSet.empty() && (rootSet.size() + rootsVertexId.size() < nGroups))
  {
    //get max diff value index
    std::vector< std::pair<int, double> >::iterator rootIt;
    std::vector< std::pair<int, double> >::iterator rootItMax;
    double maxDiff = -std::numeric_limits<double>::max();

    for(rootIt = rootSet.begin(); rootIt != rootSet.end(); ++rootIt)
    {
      if(rootIt->second > maxDiff)
      {
        maxDiff = rootIt->second;
        rootItMax = rootIt;
      }
    }

    std::size_t currentId = rootItMax->first;

    rootSet.erase(rootItMax);

    //get vertex from graph
    te::graph::Vertex* vertex = m_graph->getVertex(currentId);

    if(vertex)
    {
      double diffA = 0.;
      double diffB = 0.;

      std::size_t edgeId;

      bool remove = edgeToRemove(currentId, diffA, diffB, edgeId);

      if(remove)
      {
        te::graph::Edge* edge = m_graph->getEdge(edgeId);

        int vertexFrom = edge->getIdFrom();
        int vertexTo = edge->getIdTo();

        //remove edge
        m_graph->removeEdge(edgeId);

        rootSet.push_back(std::vector< std::pair<int, double> >::value_type(vertexFrom, diffA));
        rootSet.push_back(std::vector< std::pair<int, double> >::value_type(vertexTo, diffB));
      }
      else
      {
        rootsVertexId.push_back(currentId);
      }
    }
  }

  for(std::size_t t = 0; t < rootSet.size(); ++t)
  {
    rootsVertexId.push_back(rootSet[t].first);
  }

  return rootsVertexId;
}

std::vector<std::size_t> te::sa::SkaterPartition::execute(std::string popAttr, std::size_t minPop)
{
  assert(m_graph);

  m_popAttr = popAttr;

  m_popMin = minPop;

  //get first vertex id from graph
  std::auto_ptr<te::graph::MemoryIterator> memIt(new te::graph::MemoryIterator(m_graph));
  int firstVertex = memIt->getFirstVertex()->getId();
  
  //create vector with root information
  std::vector< std::pair<int, double> > rootSet;
  rootSet.push_back(std::vector< std::pair<int, double> >::value_type(firstVertex, 0.));

  std::vector<std::size_t> rootsVertexId;

  //bfs over the graph
  while(!rootSet.empty())
  {
    //get max diff value index
    std::vector< std::pair<int, double> >::iterator rootIt;
    std::vector< std::pair<int, double> >::iterator rootItMax;
    double maxDiff = -std::numeric_limits<double>::max();

    for(rootIt = rootSet.begin(); rootIt != rootSet.end(); ++rootIt)
    {
      if(rootIt->second > maxDiff)
      {
        maxDiff = rootIt->second;
        rootItMax = rootIt;
      }
    }

    std::size_t currentId = rootItMax->first;

    rootSet.erase(rootItMax);

    //get vertex from graph
    te::graph::Vertex* vertex = m_graph->getVertex(currentId);

    if(vertex)
    {
      double diffA = 0.;
      double diffB = 0.;

      std::size_t edgeId;

      bool remove = edgeToRemove(currentId, diffA, diffB, edgeId);

      if(remove)
      {
        te::graph::Edge* edge = m_graph->getEdge(edgeId);

        int vertexFrom = edge->getIdFrom();
        int vertexTo = edge->getIdTo();

        //remove edge
        m_graph->removeEdge(edgeId);

        rootSet.push_back(std::vector< std::pair<int, double> >::value_type(vertexFrom, diffA));
        rootSet.push_back(std::vector< std::pair<int, double> >::value_type(vertexTo, diffB));
      }
      else
      {
        rootsVertexId.push_back(currentId);
      }
    }
  }

  for(std::size_t t = 0; t < rootSet.size(); ++t)
  {
    rootsVertexId.push_back(rootSet[t].first);
  }

  return rootsVertexId;
}

bool te::sa::SkaterPartition::edgeToRemove(int startVertex, double& diffA, double& diffB, std::size_t& edgeToRemoveId)
{
  //create map of differences
  std::vector<EdgeRemovalInfo> edgeRemovalVec;

  //calculate SSDTO
  std::size_t totalPop = 0;
  std::vector<double> meanVecStart = calculateRootMean(startVertex, -1, totalPop);
  double deviationStart = calculateRootDeviation(startVertex, -1, meanVecStart);

  //create list
  std::queue<int> queue;
  std::set<int> visited;

  queue.push(startVertex);
  visited.insert(startVertex);

  //bfs over the graph
  while(!queue.empty())
  {
    int currentId = queue.front();
    queue.pop();

    //get vertex from graph
    te::graph::Vertex* vertex = m_graph->getVertex(currentId);

    if(vertex)
    {
      //get neighbours
      std::set<int> neighbours = vertex->getNeighborhood();
      std::set<int>::iterator itNeighbours = neighbours.begin();

      while(itNeighbours != neighbours.end())
      {
        te::graph::Edge* e = m_graph->getEdge(*itNeighbours);
        te::graph::Vertex* vTo = 0;

        if(e)
        {
          if(e->getIdFrom() == currentId)
            vTo = m_graph->getVertex(e->getIdTo());
          else
            vTo = m_graph->getVertex(e->getIdFrom());
        }

        if(vTo)
        {
          //check if already visted
          if(visited.find(vTo->getId()) == visited.end())
          {
            //add in queue
            queue.push(vTo->getId());
            visited.insert(vTo->getId());

            //calculate SSDi
            double diffVFrom = 0.;
            double diffVTo = 0.;
            std::size_t popA = 0;
            std::size_t popB = 0;

            double ssdi = calculateEdgeDifference(vertex->getId(), vTo->getId(), diffVFrom, diffVTo, popA, popB);

            double l = deviationStart - ssdi;

            EdgeRemovalInfo eri;
            eri.m_edgeId = e->getId();
            eri.m_SSDT = deviationStart;
            eri.m_SSDi = ssdi;
            eri.m_l = l;

            if(e->getIdFrom() == vertex->getId())
            {
              eri.m_SSDTa = diffVFrom;
              eri.m_SSDTb = diffVTo;
            }
            else
            {
              eri.m_SSDTa = diffVTo;
              eri.m_SSDTb = diffVFrom;
            }

            eri.m_popa = popA;
            eri.m_popb = popB;

            edgeRemovalVec.push_back(eri);
          }
        }

        ++itNeighbours;
      }
    }
  }

  //get the edge with maximum SSDi
  bool found = false;

  double maxDiff = -std::numeric_limits<double>::max();

  for(std::size_t t = 0; t < edgeRemovalVec.size(); ++t)
  {
    if(edgeRemovalVec[t].m_l > maxDiff && edgeRemovalVec[t].m_popa >= m_popMin && edgeRemovalVec[t].m_popb >= m_popMin)
    {
      maxDiff = edgeRemovalVec[t].m_l;

      edgeToRemoveId = edgeRemovalVec[t].m_edgeId;

      diffA =  edgeRemovalVec[t].m_SSDTa;
      diffB =  edgeRemovalVec[t].m_SSDTb;

      found = true;
    }
  }

  if(found)
    m_SSDiValues.push_back(maxDiff);

  edgeRemovalVec.clear();

  return found;
}

double te::sa::SkaterPartition::calculateEdgeDifference(int vertexFrom, int vertexTo, double& diffA, double& diffB, std::size_t& popA, std::size_t& popB)
{
  //calculate the deviation for the tree that begins with the vertex from SQDTA
  std::vector<double> meanVecFrom = calculateRootMean(vertexFrom, vertexTo, popA);
  diffA = calculateRootDeviation(vertexFrom, vertexTo, meanVecFrom);

  //calculate the deviation for the tree that begins with the vertex to SQDTB
  std::vector<double> meanVecTo = calculateRootMean(vertexTo, vertexFrom, popB);
  diffB = calculateRootDeviation(vertexTo, vertexFrom, meanVecTo);

  //return the edge cost
  return diffA + diffB;
}

std::vector<double> te::sa::SkaterPartition::calculateRootMean(int startVertex, int vertexToIgnore, std::size_t& pop)
{
  std::vector<double> meanAttrs(m_attrs.size(), 0.);

  std::queue<int> queue;
  std::set<int> visited;

  queue.push(startVertex);
  visited.insert(startVertex);

  std::size_t count = 0;

  while(!queue.empty())
  {
    int currentId = queue.front();
    queue.pop();

    te::graph::Vertex* vertex = m_graph->getVertex(currentId);

    if(vertex)
    {
      for(std::size_t t = 0; t < m_attrs.size(); ++t)
      {
        std::string attrName = m_attrs[t];

        int attrIdx;

        if(te::sa::GetGraphVertexAttrIndex(m_graph, attrName, attrIdx))
        {
          meanAttrs[t] += te::sa::GetDataValue(vertex->getAttributes()[attrIdx]);
        }
      }

      //get population information
      int popIdx;
      if(te::sa::GetGraphVertexAttrIndex(m_graph, m_popAttr, popIdx))
      {
        pop += (std::size_t)te::sa::GetDataValue(vertex->getAttributes()[popIdx]);
      }

      //get neighbours
      std::set<int> neighbours = vertex->getNeighborhood();
      std::set<int>::iterator itNeighbours = neighbours.begin();

      while(itNeighbours != neighbours.end())
      {
        te::graph::Edge* e = m_graph->getEdge(*itNeighbours);
        te::graph::Vertex* vTo = 0;

        if(e)
        {
          if(e->getIdFrom() == currentId)
            vTo = m_graph->getVertex(e->getIdTo());
          else
            vTo = m_graph->getVertex(e->getIdFrom());
        }

        if(vTo && vTo->getId() != vertexToIgnore)
        {
          if(visited.find(vTo->getId()) == visited.end())
          {
            queue.push(vTo->getId());
            visited.insert(vTo->getId());
          }
        }

        ++itNeighbours;
      }
    }

    ++count;
  }

  for(std::size_t t = 0; t < m_attrs.size(); ++t)
  {
    meanAttrs[t] /= count;
  }

  return meanAttrs;
}

double te::sa::SkaterPartition::calculateRootDeviation(int startVertex, int vertexToIgnore, std::vector<double>& meanVec)
{
  double deviation = 0.;

  std::queue<int> queue;
  std::set<int> visited;

  queue.push(startVertex);
  visited.insert(startVertex);

  while(!queue.empty())
  {
    int currentId = queue.front();
    queue.pop();

    te::graph::Vertex* vertex = m_graph->getVertex(currentId);

    if(vertex)
    {
      deviation += calculateDistance(vertex, meanVec);

      //get neighbours
      std::set<int> neighbours = vertex->getNeighborhood();
      std::set<int>::iterator itNeighbours = neighbours.begin();

      while(itNeighbours != neighbours.end())
      {
        te::graph::Edge* e = m_graph->getEdge(*itNeighbours);
        te::graph::Vertex* vTo = 0;

        if(e)
        {
          if(e->getIdFrom() == currentId)
            vTo = m_graph->getVertex(e->getIdTo());
          else
            vTo = m_graph->getVertex(e->getIdFrom());
        }

        if(vTo && vTo->getId() != vertexToIgnore)
        {
          if(visited.find(vTo->getId()) == visited.end())
          {
            queue.push(vTo->getId());
            visited.insert(vTo->getId());
          }
        }

        ++itNeighbours;
      }
    }
  }

  return deviation;
}

double te::sa::SkaterPartition::calculateDistance(te::graph::Vertex* vertex, std::vector<double>& meanVec)
{
  double distance = 0.;

  for(std::size_t t = 0; t < m_attrs.size(); ++t)
  {
    std::string attrName = m_attrs[t];

    int attrIdx;

    if(te::sa::GetGraphVertexAttrIndex(m_graph, attrName, attrIdx))
    {
      distance += (te::sa::GetDataValue(vertex->getAttributes()[attrIdx]) - meanVec[t]) * (te::sa::GetDataValue(vertex->getAttributes()[attrIdx]) - meanVec[t]);
    }
  }

  return distance;
}
