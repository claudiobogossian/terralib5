/*  Copyright (C) 2008-2013 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/sa/core/SpatialStatisticsFunctions.cpp

  \brief Functions used to calculate spatial statistics operations.

  \reference Methods adapted from TerraLib4.
*/

// TerraLib
#include "../../datatype/SimpleData.h"
#include "../../graph/core/Edge.h"
#include "../../graph/core/GraphMetadata.h"
#include "../../graph/core/Vertex.h"
#include "../../graph/iterator/MemoryIterator.h"
#include "GeneralizedProximityMatrix.h"
#include "SpatialStatisticsFunctions.h"
#include "StatisticsFunctions.h"
#include "Utils.h"

// STL
#include <cassert>

//BOOST
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>


void te::sa::GStatistics(te::sa::GeneralizedProximityMatrix* gpm, int attrIdx)
{
  assert(gpm);

  //calculate the sum of the selected attribute
  double totalSum = te::sa::Sum(gpm, attrIdx);

  te::graph::AbstractGraph* graph = gpm->getGraph();

  //add G and G* attributes into gpm
  int gAttrIdx = te::sa::AddGraphVertexAttribute(graph, TE_SA_G_ATTR_NAME, te::dt::DOUBLE_TYPE);
  int gStarAttrIdx = te::sa::AddGraphVertexAttribute(graph, TE_SA_GSTAR_ATTR_NAME, te::dt::DOUBLE_TYPE);
  int nAttrs = graph->getMetadata()->getVertexPropertySize();

  //create graph vertex iterator
  std::auto_ptr<te::graph::MemoryIterator> it(new te::graph::MemoryIterator(graph));

  te::graph::Vertex* v = it->getFirstVertex();

  while(!it->isVertexIteratorAfterEnd())
  {
    double attrValue = te::sa::GetDataValue(v->getAttributes()[attrIdx]);
    double excludeSum = totalSum - attrValue;
    double G = 0.;
    double GStar = attrValue;

    //get neighbours information
    int id = v->getId();
    std::set<int> neighbours = v->getSuccessors();
    std::set<int>::iterator itNeighbours = neighbours.begin();
    int nNeighbours = (int)neighbours.size();

    while(itNeighbours != neighbours.end())
    {
      te::graph::Edge* e = graph->getEdge(*itNeighbours);
      te::graph::Vertex* vTo = 0;

      if(e)
      {
        if(e->getIdFrom() == id)
          vTo = graph->getVertex(e->getIdTo());
        else
          vTo = graph->getVertex(e->getIdFrom());
      }

      if(vTo)
      {
        double attrValueTo = te::sa::GetDataValue(vTo->getAttributes()[attrIdx]);

        G +=  attrValueTo;
        GStar +=  attrValueTo;
      }

      ++itNeighbours;
    }

    G /= nNeighbours;
    GStar /= (nNeighbours+1);

    G /= excludeSum; 
    GStar /= totalSum;

    v->setAttributeVecSize(nAttrs);
    v->addAttribute(gAttrIdx, new te::dt::SimpleData<double, te::dt::DOUBLE_TYPE>(G));
    v->addAttribute(gStarAttrIdx, new te::dt::SimpleData<double, te::dt::DOUBLE_TYPE>(GStar));

    v = it->getNextVertex();
  }
}

void te::sa::LocalMean(te::sa::GeneralizedProximityMatrix* gpm, int attrIdx)
{
  assert(gpm);

  te::graph::AbstractGraph* graph = gpm->getGraph();

  //check if the graph has the weight attribute
  int weightAttrIdx;
  if(!te::sa::GetGraphEdgeAttrIndex(graph, TE_SA_WEIGHT_ATTR_NAME, weightAttrIdx))
    throw;

  bool normalized = true;

  //add local mean and number of neighbours attributes into gpm
  int localMeanAttrIdx = te::sa::AddGraphVertexAttribute(graph, TE_SA_LOCALMEAN_ATTR_NAME, te::dt::DOUBLE_TYPE);
  int nNeighboursAttrIdx = te::sa::AddGraphVertexAttribute(graph, TE_SA_NUMNEIGHBORS_ATTR_NAME, te::dt::INT32_TYPE);
  int nAttrs = graph->getMetadata()->getVertexPropertySize();

  //create graph vertex iterator
  std::auto_ptr<te::graph::MemoryIterator> it(new te::graph::MemoryIterator(graph));

  te::graph::Vertex* v = it->getFirstVertex();

  while(!it->isVertexIteratorAfterEnd())
  {
    double sum = 0.;

    //get neighbours information
    int id = v->getId();
    std::set<int> neighbours = v->getSuccessors();
    std::set<int>::iterator itNeighbours = neighbours.begin();
    int nNeighbours = (int)neighbours.size();

    while(itNeighbours != neighbours.end())
    {
      te::graph::Edge* e = graph->getEdge(*itNeighbours);
      te::graph::Vertex* vTo = 0;

      if(e)
      {
        if(e->getIdFrom() == id)
          vTo = graph->getVertex(e->getIdTo());
        else
          vTo = graph->getVertex(e->getIdFrom());
      }

      if(vTo)
      {
        //retrieve the neighbor attribute value
        double attrValue = te::sa::GetDataValue(vTo->getAttributes()[attrIdx]);

        //retrieve the weight associated with the neighbor
        double attrWeight = te::sa::GetDataValue(e->getAttributes()[weightAttrIdx]);

        //verify if the weight is normalized by number of neighbours
        if(!normalized)
          attrWeight = attrWeight/nNeighbours;

        sum +=  attrWeight * attrValue;
      }

      ++itNeighbours;
    }

    v->setAttributeVecSize(nAttrs);
    v->addAttribute(localMeanAttrIdx, new te::dt::SimpleData<double, te::dt::DOUBLE_TYPE>(sum));
    v->addAttribute(nNeighboursAttrIdx, new te::dt::SimpleData<double, te::dt::INT32_TYPE>(nNeighbours));

    v = it->getNextVertex();
  }
}

void te::sa::ZAndWZ(te::sa::GeneralizedProximityMatrix* gpm, int attrIdx)
{
  assert(gpm);

  te::graph::AbstractGraph* graph = gpm->getGraph();

  //check if the graph has the weight attribute
  int weightAttrIdx;
  if(!te::sa::GetGraphEdgeAttrIndex(graph, TE_SA_WEIGHT_ATTR_NAME, weightAttrIdx))
    throw;

  // calculate the standard deviation Z
  double mean = te::sa::FirstMoment(gpm, attrIdx);

  //add Z attribute into gpm
  int zAttrIdx = te::sa::AddGraphVertexAttribute(graph, TE_SA_STDDEVZ_ATTR_NAME, te::dt::DOUBLE_TYPE);
  int nAttrs = graph->getMetadata()->getVertexPropertySize();

 //create graph vertex iterator
  std::auto_ptr<te::graph::MemoryIterator> it(new te::graph::MemoryIterator(graph));

  te::graph::Vertex* v = it->getFirstVertex();

  while(!it->isVertexIteratorAfterEnd())
  {
    double attrValue = te::sa::GetDataValue(v->getAttributes()[attrIdx]);

    double stdDezZ = attrValue - mean;

    v->setAttributeVecSize(nAttrs);
    v->addAttribute(zAttrIdx, new te::dt::SimpleData<double, te::dt::DOUBLE_TYPE>(stdDezZ));

    v = it->getNextVertex();
  }

  //calculate the local mean of Z(WZ)
  int wzAttrIdx = te::sa::AddGraphVertexAttribute(graph, TE_SA_LOCALMEANWZ_ATTR_NAME, te::dt::DOUBLE_TYPE);
  nAttrs = graph->getMetadata()->getVertexPropertySize();

  bool normalized = true;

  //vertex iterator
  v = it->getFirstVertex();

  while(!it->isVertexIteratorAfterEnd())
  {
    double sum = 0.;

    //get neighbours information
    int id = v->getId();
    std::set<int> neighbours = v->getSuccessors();
    std::set<int>::iterator itNeighbours = neighbours.begin();
    int nNeighbours = (int)neighbours.size();

    while(itNeighbours != neighbours.end())
    {
      te::graph::Edge* e = graph->getEdge(*itNeighbours);
      te::graph::Vertex* vTo = 0;

      if(e)
      {
        if(e->getIdFrom() == id)
          vTo = graph->getVertex(e->getIdTo());
        else
          vTo = graph->getVertex(e->getIdFrom());
      }

      if(vTo)
      {
        //retrieve the neighbor attribute value
        double attrValue = te::sa::GetDataValue(vTo->getAttributes()[zAttrIdx]);

        //retrieve the weight associated with the neighbor
        double attrWeight = te::sa::GetDataValue(e->getAttributes()[weightAttrIdx]);

        //verify if the weight is normalized by number of neighbours
        if(!normalized)
          attrWeight = attrWeight/nNeighbours;

        sum +=  attrWeight * attrValue;
      }

      ++itNeighbours;
    }

    v->setAttributeVecSize(nAttrs);
    v->addAttribute(wzAttrIdx, new te::dt::SimpleData<double, te::dt::DOUBLE_TYPE>(sum));

    v = it->getNextVertex();
  }
}

double te::sa::MoranIndex(te::sa::GeneralizedProximityMatrix* gpm)
{
  assert(gpm);

  te::graph::AbstractGraph* graph = gpm->getGraph();

  //check if the graph has the Z attribute
  int zAttrIdx;
  if(!te::sa::GetGraphEdgeAttrIndex(graph, TE_SA_STDDEVZ_ATTR_NAME, zAttrIdx))
    throw;

  //check if the graph has the WZ attribute
  int wzAttrIdx;
  if(!te::sa::GetGraphEdgeAttrIndex(graph, TE_SA_LOCALMEANWZ_ATTR_NAME, wzAttrIdx))
    throw;

  double variance = te::sa::SecondMoment(gpm, zAttrIdx, 0); // MEAN is 0 ??
  double sum = 0.;
  int count = 0;

  //add moran index attribute into gpm
  int moranAttrIdx = te::sa::AddGraphVertexAttribute(graph, TE_SA_MORANINDEX_ATTR_NAME, te::dt::DOUBLE_TYPE);
  int nAttrs = graph->getMetadata()->getVertexPropertySize();

  //create graph vertex iterator
  std::auto_ptr<te::graph::MemoryIterator> it(new te::graph::MemoryIterator(graph));

  te::graph::Vertex* v = it->getFirstVertex();

  while(!it->isVertexIteratorAfterEnd())
  {
    double zValue  = te::sa::GetDataValue(v->getAttributes()[zAttrIdx]);
    double wzValue = te::sa::GetDataValue(v->getAttributes()[wzAttrIdx]);
    double ZxWZ = 0.;

    if(variance != 0.)
      ZxWZ = (zValue*wzValue)/variance; 

    v->setAttributeVecSize(nAttrs);
    v->addAttribute(moranAttrIdx, new te::dt::SimpleData<double, te::dt::DOUBLE_TYPE>(ZxWZ));

    sum += ZxWZ;
    ++count;

    v = it->getNextVertex();
  }

  return sum /= count;
}

double te::sa::MoranIndex(te::sa::GeneralizedProximityMatrix* gpm, double mean, double variance, int attrIdx)
{
  assert(gpm);

  double moran = 0.;

  te::graph::AbstractGraph* graph = gpm->getGraph();

  //check if the graph has the weight attribute
  int weightAttrIdx;
  if(!te::sa::GetGraphEdgeAttrIndex(graph, TE_SA_WEIGHT_ATTR_NAME, weightAttrIdx))
    throw;

   bool normalized = true;

  //create graph vertex iterator
  std::auto_ptr<te::graph::MemoryIterator> it(new te::graph::MemoryIterator(graph));

  te::graph::Vertex* v = it->getFirstVertex();

  std::size_t numberObjs = it->getVertexInteratorCount();

  while(!it->isVertexIteratorAfterEnd())
  {
    double normObjVal = te::sa::GetDataValue(v->getAttributes()[attrIdx]) - mean;
    double li = 0.;
    double weightSum = 0.;

    //get neighbours information
    int id = v->getId();
    std::set<int> neighbours = v->getSuccessors();
    std::set<int>::iterator itNeighbours = neighbours.begin();
    int nNeighbours = (int)neighbours.size();

    while(itNeighbours != neighbours.end())
    {
      te::graph::Edge* e = graph->getEdge(*itNeighbours);
      te::graph::Vertex* vTo = 0;

      if(e)
      {
        if(e->getIdFrom() == id)
          vTo = graph->getVertex(e->getIdTo());
        else
          vTo = graph->getVertex(e->getIdFrom());
      }

      if(vTo)
      {
        double normNeighVal = te::sa::GetDataValue(vTo->getAttributes()[attrIdx]) - mean;

        //retrieve the weight associated with the neighbor
        double attrWeight = te::sa::GetDataValue(e->getAttributes()[weightAttrIdx]);

        //verify if the weight is normalized by number of neighbours
        if(!normalized)
          attrWeight = attrWeight/nNeighbours;

        li +=  attrWeight*(normNeighVal)*(normObjVal);

        weightSum += attrWeight;
      }

      ++itNeighbours;
    }

    if (weightSum != 0.)
      li /= weightSum;

    moran += li;

    v = it->getNextVertex();
  }

  if(numberObjs > 1)
    return moran/(variance*(numberObjs - 1));
  else 
    return moran/variance;
}

double te::sa::GlobalMoranSignificance(te::sa::GeneralizedProximityMatrix* gpm, int attrIdx, int permutationsNumber, double moranIndex)
{
  assert(gpm);

  te::graph::AbstractGraph* graph = gpm->getGraph();

  //calculate statistics information
  double mean = te::sa::FirstMoment(gpm, attrIdx);
  double variance = te::sa::SecondMoment(gpm, attrIdx, mean);

  //create deviation vector
  std::vector<double> deviations;

  std::auto_ptr<te::graph::MemoryIterator> it(new te::graph::MemoryIterator(graph));
  te::graph::Vertex* v = it->getFirstVertex();

  while(!it->isVertexIteratorAfterEnd())
  {
    te::dt::AbstractData* ad = v->getAttributes()[attrIdx];

    if(ad)
      deviations.push_back(te::sa::GetDataValue(ad));

    v = it->getNextVertex();
  }

  //create permutation vector
  std::vector<double> permutationsResults(permutationsNumber);

  boost::random::mt19937 gen;
  boost::random::uniform_int_distribution<> dist(0, deviations.size() - 1);

  for(int i = 0; i < permutationsNumber; ++i)
  {
    //shuffle the attribute value
    te::graph::Vertex* v = it->getFirstVertex();

    while(!it->isVertexIteratorAfterEnd())
    {
      double value = deviations[dist(gen)];

      v->addAttribute(attrIdx, new te::dt::SimpleData<double, te::dt::DOUBLE_TYPE>(value));

      v = it->getNextVertex();
    }

    permutationsResults[i] = MoranIndex(gpm, mean, variance, attrIdx);
  }

  //fix attribute values
  int count = 0;
  v = it->getFirstVertex();

  while(!it->isVertexIteratorAfterEnd())
  {
    double value = deviations[count];

    v->addAttribute(attrIdx, new te::dt::SimpleData<double, te::dt::DOUBLE_TYPE>(value));

    ++count;

    v = it->getNextVertex();
  }
  
  // verify the significance
  int position = 0;
  double significance = 0.;

  for(int i = 0; i < permutationsNumber; i++)
  {
    if(moranIndex < permutationsResults[i])
      position++;
  }

  if(moranIndex >= 0)
    significance = (double)(position+1)/(permutationsNumber+1);
  else
    significance = (double)(permutationsNumber-position)/(permutationsNumber+1);

  return significance;
}

void te::sa::LisaStatisticalSignificance(te::sa::GeneralizedProximityMatrix* gpm, int permutationsNumber)
{
  assert(gpm);

  te::graph::AbstractGraph* graph = gpm->getGraph();

  //check if the graph has the Z attribute
  int zAttrIdx;
  if(!te::sa::GetGraphEdgeAttrIndex(graph, TE_SA_STDDEVZ_ATTR_NAME, zAttrIdx))
    throw;

  //check if the graph has the local moran attribute
  int lisaAttrIdx;
  if(!te::sa::GetGraphEdgeAttrIndex(graph, TE_SA_MORANINDEX_ATTR_NAME, lisaAttrIdx))
    throw;

  //check if the graph has the number of neighbours attribute
  int nNeighboursAttrIdx;
  if(!te::sa::GetGraphEdgeAttrIndex(graph, TE_SA_NUMNEIGHBORS_ATTR_NAME, nNeighboursAttrIdx))
    throw;

  //calculate variance
  double variance = te::sa::SecondMoment(gpm, zAttrIdx, 0); // MEAN = 0 ??

  //create deviation vector
  std::vector<double> deviations;

  std::auto_ptr<te::graph::MemoryIterator> it(new te::graph::MemoryIterator(graph));
  te::graph::Vertex* v = it->getFirstVertex();

  while(!it->isVertexIteratorAfterEnd())
  {
    te::dt::AbstractData* ad = v->getAttributes()[zAttrIdx];

    if(ad)
      deviations.push_back(te::sa::GetDataValue(ad));

    v = it->getNextVertex();
  }

  //add lisa significance attribute into gpm
  int lisaSigAttrIdx = te::sa::AddGraphVertexAttribute(graph, TE_SA_LISASIGNIFICANCE_ATTR_NAME, te::dt::DOUBLE_TYPE);
  int nAttrs = graph->getMetadata()->getVertexPropertySize();

  //calculate LISA Significance value
  boost::random::mt19937 gen;
  boost::random::uniform_int_distribution<> dist(0, deviations.size() - 1);

  int index = 0;

  v = it->getFirstVertex();

  while(!it->isVertexIteratorAfterEnd())
  {
    int nNeighbours = (int)te::sa::GetDataValue(v->getAttributes()[nNeighboursAttrIdx]);

    double lisaValue = te::sa::GetDataValue(v->getAttributes()[lisaAttrIdx]);

    //create pertumation vector
    std::vector<double> permut(permutationsNumber);

    for(int i = 0; i < permutationsNumber; ++i)
    {
      double sum = 0.;

      std::set<int> neighSet;

      for(int j = 0; j < nNeighbours; ++j)
      {
        int pos = dist(gen);

        if(index == pos || neighSet.find(pos) != neighSet.end())
        {
          j--;
        }
        else
        {
          neighSet.insert(pos);
          sum += deviations[pos];
        }
      }

      double WZperm = 0.;

      if(nNeighbours != 0)
        WZperm = sum/nNeighbours;

      if(variance == 0)
        permut[i] = 0.;
      else
        permut[i] = deviations[index] * WZperm / variance;
    }

    int position = 0;

    for(int k = 0; k < permutationsNumber; k++)
    {
      if(lisaValue > permut[k])
        position++;
    }

    //calculate significance
    double significance = 0.;

    if(lisaValue >= 0)
      significance = (double) (permutationsNumber-position)/(permutationsNumber+1);
    else
      significance = (double) position/( permutationsNumber + 1 );

    v->setAttributeVecSize(nAttrs);
    v->addAttribute(lisaSigAttrIdx, new te::dt::SimpleData<double, te::dt::DOUBLE_TYPE>(significance));

    ++index;

    v = it->getNextVertex();
  }
}

void te::sa::BoxMap(te::sa::GeneralizedProximityMatrix* gpm, double mean)
{
  assert(gpm);

  te::graph::AbstractGraph* graph = gpm->getGraph();

  //check if the graph has the Z attribute
  int zAttrIdx;
  if(!te::sa::GetGraphEdgeAttrIndex(graph, TE_SA_STDDEVZ_ATTR_NAME, zAttrIdx))
    throw;

  //check if the graph has the WZ attribute
  int wzAttrIdx;
  if(!te::sa::GetGraphEdgeAttrIndex(graph, TE_SA_LOCALMEANWZ_ATTR_NAME, wzAttrIdx))
    throw;

  //add boxmap attribute into gpm
  int boxMapAttrIdx = te::sa::AddGraphVertexAttribute(graph, TE_SA_BOXMAP_ATTR_NAME, te::dt::INT32_TYPE);
  int nAttrs = graph->getMetadata()->getVertexPropertySize();

 //create graph vertex iterator
  std::auto_ptr<te::graph::MemoryIterator> it(new te::graph::MemoryIterator(graph));

  te::graph::Vertex* v = it->getFirstVertex();

  while(!it->isVertexIteratorAfterEnd())
  {
    double zValue  = te::sa::GetDataValue(v->getAttributes()[zAttrIdx]);
    double wzValue = te::sa::GetDataValue(v->getAttributes()[wzAttrIdx]);

    int result = 0;

    if(zValue >= mean && wzValue >= mean)
      result = 1;
    else if(zValue < mean && wzValue >= mean)
      result = 4;
    else if(zValue < mean && wzValue < mean)
      result = 2;
    else if(zValue >= mean && wzValue < mean)
      result = 3;

    v->setAttributeVecSize(nAttrs);
    v->addAttribute(boxMapAttrIdx, new te::dt::SimpleData<int, te::dt::INT32_TYPE>(result));

    v = it->getNextVertex();
  }
}

void te::sa::LISAMap(te::sa::GeneralizedProximityMatrix* gpm, int permutationsNumber)
{
  assert(gpm);

  te::graph::AbstractGraph* graph = gpm->getGraph();

  //check if the graph has the LISASig attribute
  int lisaSigAttrIdx;
  if(!te::sa::GetGraphEdgeAttrIndex(graph, TE_SA_LISASIGNIFICANCE_ATTR_NAME, lisaSigAttrIdx))
    throw;

  //add boxmap attribute into gpm
  int lisaMapAttrIdx = te::sa::AddGraphVertexAttribute(graph, TE_SA_LISAMAP_ATTR_NAME, te::dt::INT32_TYPE);
  int nAttrs = graph->getMetadata()->getVertexPropertySize();

 //create graph vertex iterator
  std::auto_ptr<te::graph::MemoryIterator> it(new te::graph::MemoryIterator(graph));

  te::graph::Vertex* v = it->getFirstVertex();

  while(!it->isVertexIteratorAfterEnd())
  {
    double lisaSigValue  = te::sa::GetDataValue(v->getAttributes()[lisaSigAttrIdx]);

    int significanceClass = 0;

    if(lisaSigValue <= 0.001) //permutationNumber >= 999 
      significanceClass = 3;
    else if(lisaSigValue <= 0.01 && lisaSigValue > 0.001) //permutationNumber >= 99 
      significanceClass = 2;
    else if(lisaSigValue <= 0.05 && lisaSigValue > 0.01)
      significanceClass = 1;

    v->setAttributeVecSize(nAttrs);
    v->addAttribute(lisaMapAttrIdx, new te::dt::SimpleData<int, te::dt::INT32_TYPE>(significanceClass));

    v = it->getNextVertex();
  }
}

void te::sa::MoranMap(te::sa::GeneralizedProximityMatrix* gpm)
{
  assert(gpm);

  te::graph::AbstractGraph* graph = gpm->getGraph();

  //check if the graph has the LISAMap attribute
  int lisaMapAttrIdx;
  if(!te::sa::GetGraphEdgeAttrIndex(graph, TE_SA_LISAMAP_ATTR_NAME, lisaMapAttrIdx))
    throw;

  //check if the graph has the BoxMap attribute
  int boxMapAttrIdx;
  if(!te::sa::GetGraphEdgeAttrIndex(graph, TE_SA_BOXMAP_ATTR_NAME, boxMapAttrIdx))
    throw;

  //add boxmap attribute into gpm
  int moranMapAttrIdx = te::sa::AddGraphVertexAttribute(graph, TE_SA_MORANMAP_ATTR_NAME, te::dt::INT32_TYPE);
  int nAttrs = graph->getMetadata()->getVertexPropertySize();

  //create graph vertex iterator
  std::auto_ptr<te::graph::MemoryIterator> it(new te::graph::MemoryIterator(graph));

  te::graph::Vertex* v = it->getFirstVertex();

  while(!it->isVertexIteratorAfterEnd())
  {
    int lisaMapValue = (int) te::sa::GetDataValue(v->getAttributes()[lisaMapAttrIdx]);
    int boxMapValue = (int) te::sa::GetDataValue(v->getAttributes()[boxMapAttrIdx]);

    int result = 0;

    if(lisaMapValue != 0)
      result = boxMapValue;

    v->setAttributeVecSize(nAttrs);
    v->addAttribute(moranMapAttrIdx, new te::dt::SimpleData<int, te::dt::INT32_TYPE>(result));

    v = it->getNextVertex();
  }
}
