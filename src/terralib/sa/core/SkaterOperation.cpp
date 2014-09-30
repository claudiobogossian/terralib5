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
#include "../../dataaccess/datasource/DataSource.h"
#include "../../dataaccess/datasource/DataSourceManager.h"
#include "../../dataaccess/utils/Utils.h"
#include "../../datatype/SimpleProperty.h"
#include "../../datatype/SimpleData.h"
#include "../../graph/core/AbstractGraph.h"
#include "../../graph/core/AbstractGraphFactory.h"
#include "../../graph/core/Edge.h"
#include "../../graph/core/GraphMetadata.h"
#include "../../graph/core/Vertex.h"
#include "../../graph/iterator/MemoryIterator.h"
#include "../../graph/Globals.h"
#include "../../memory/DataSet.h"
#include "../../memory/DataSetItem.h"
#include "GeneralizedProximityMatrix.h"
#include "MinimumSpanningTree.h"
#include "SkaterOperation.h"
#include "SkaterPartition.h"
#include "Utils.h"

//STL
#include <cassert>
#include <list>
#include <queue>

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

  //check if use population information
  if(m_inputParams->m_attrPop != "")
  {
    std::size_t idx = m_inputParams->m_dsType->getPropertyPosition(m_inputParams->m_attrPop);
    int type = m_inputParams->m_ds->getPropertyDataType(idx);
    int gpmIdx = te::sa::AssociateGPMVertexAttribute(m_inputParams->m_gpm.get(), m_inputParams->m_ds.get(), m_inputParams->m_gpmAttrLink, m_inputParams->m_attrPop, type);
  }

  //calculate gpm edge attribute
  int edgeWeightIdx = te::sa::AddGraphEdgeAttribute(m_inputParams->m_gpm->getGraph(), TE_SA_SKATER_ATTR_WEIGHT_NAME, te::dt::DOUBLE_TYPE);

  createWeightAttribute(edgeWeightIdx, attrsIdx);

  //calculate the mst
  te::sa::MinimumSpanningTree mst(m_inputParams->m_gpm->getGraph());

  te::graph::AbstractGraph* graph = mst.kruskal(edgeWeightIdx);

  //partition the graph
  te::sa::SkaterPartition sp(graph, m_inputParams->m_attrs);

  std::vector<std::size_t> roots;

  if(m_inputParams->m_aggregType == te::sa::Both || m_inputParams->m_aggregType == te::sa::Clusters)
    roots = sp.execute(m_inputParams->m_nClusters, m_inputParams->m_attrPop, m_inputParams->m_minPop);
  else if(m_inputParams->m_aggregType == te::sa::Population)
    roots = sp.execute(m_inputParams->m_attrPop, m_inputParams->m_minPop);

  m_nClassGroups = (int)roots.size();

  std::map<int, int> skaterMap = createSkaterMap(graph, roots);

  //create output information information
  std::auto_ptr<te::da::DataSetType> outDsType = createDataSetType(m_inputParams->m_dsType.get());

  std::string gpmLink = m_inputParams->m_gpm->getAttributeName();

  std::auto_ptr<te::mem::DataSet> outDs = createDataSet(m_inputParams->m_ds.get(), outDsType.get(), skaterMap, gpmLink);

  //save data
  saveDataSet(outDs.get(), outDsType.get());

  roots.clear();
  skaterMap.clear();

  delete graph;
}

void te::sa::SkaterOperation::setParameters(te::sa::SkaterInputParams* inParams, te::sa::SkaterOutputParams* outParams)
{
  m_inputParams.reset(inParams);
  m_outputParams.reset(outParams);
}

int te::sa::SkaterOperation::getNumberOfClasses()
{
  return m_nClassGroups;
}

std::auto_ptr<te::da::DataSetType> te::sa::SkaterOperation::createDataSetType(te::da::DataSetType* dsType)
{
  std::auto_ptr<te::da::DataSetType> dataSetType(new te::da::DataSetType(m_outputParams->m_outputDataSetName));

  //create all input dataset properties
  std::vector<te::dt::Property*> propertyVec = dsType->getProperties();

  for(std::size_t t = 0; t < propertyVec.size(); ++t)
  {
    te::dt::Property* prop = propertyVec[t];

    te::dt::Property* newProp = prop->clone();
    newProp->setId(0);
    newProp->setParent(0);

    dataSetType->add(newProp);
  }

  //create skater class event property
  te::dt::SimpleProperty* skaterClass = new te::dt::SimpleProperty(TE_SA_SKATER_ATTR_CLASS_NAME, te::dt::INT32_TYPE);
  dataSetType->add(skaterClass);

  return dataSetType;
}

std::auto_ptr<te::mem::DataSet> te::sa::SkaterOperation::createDataSet(te::da::DataSet* inputDataSet, te::da::DataSetType* dsType, std::map<int, int>& skaterMap, std::string linkName)
{
  std::auto_ptr<te::mem::DataSet> outDataset(new te::mem::DataSet(dsType));

  std::size_t nProp = inputDataSet->getNumProperties();

  inputDataSet->moveBeforeFirst();

  while(inputDataSet->moveNext())
  {
    //create dataset item
    te::mem::DataSetItem* outDSetItem = new te::mem::DataSetItem(outDataset.get());

    std::string idStrValue = inputDataSet->getAsString(linkName);

    for(std::size_t t = 0; t < nProp; ++t)
    {
      te::dt::AbstractData* ad = inputDataSet->getValue(t).release();

      outDSetItem->setValue(t, ad);
    }

    //set kernel default value
    outDSetItem->setInt32(TE_SA_SKATER_ATTR_CLASS_NAME, skaterMap[atoi(idStrValue.c_str())]);

    //add item into dataset
    outDataset->add(outDSetItem);
  }

  return outDataset;
}

void te::sa::SkaterOperation::saveDataSet(te::da::DataSet* dataSet, te::da::DataSetType* dsType)
{
  //save dataset
  dataSet->moveBeforeFirst();

  std::map<std::string, std::string> options;

  m_outputParams->m_dataSource->createDataSet(dsType, options);

  m_outputParams->m_dataSource->add(m_outputParams->m_outputDataSetName, dataSet, options);
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

std::map<int, int> te::sa::SkaterOperation::createSkaterMap(te::graph::AbstractGraph* graph, std::vector<std::size_t>& roots)
{
  std::map<int, int> skaterMap;

  for(std::size_t t = 0; t < roots.size(); ++t)
  {
    std::size_t rootId = roots[t];

    //create list
    std::queue<int> queue;
    std::set<int> visited;

    queue.push(rootId);
    visited.insert(rootId);

    //bfs over the graph
    while(!queue.empty())
    {
      int currentId = queue.front();
      queue.pop();

      //get vertex from graph
      te::graph::Vertex* vertex = graph->getVertex(currentId);

      if(vertex)
      {
        //add to map
        skaterMap.insert(std::map<int, int>::value_type(currentId, (int)t));

        //get neighbours
        std::set<int> neighbours = vertex->getNeighborhood();
        std::set<int>::iterator itNeighbours = neighbours.begin();

        while(itNeighbours != neighbours.end())
        {
          te::graph::Edge* e = graph->getEdge(*itNeighbours);
          te::graph::Vertex* vTo = 0;

          if(e)
          {
            if(e->getIdFrom() == currentId)
              vTo = graph->getVertex(e->getIdTo());
            else
              vTo = graph->getVertex(e->getIdFrom());
          }

          if(vTo)
          {
            //check if already visted
            if(visited.find(vTo->getId()) == visited.end())
            {
              //add in queue
              queue.push(vTo->getId());
              visited.insert(vTo->getId());
            }
          }

          ++itNeighbours;
        }
      }
    }
  }

  return skaterMap;
}
