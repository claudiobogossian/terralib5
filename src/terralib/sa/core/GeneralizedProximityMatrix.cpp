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
  \file GeneralizedProximityMatrix.cpp

  \brief This class defines the GPM class.
*/

// Terralib Includes
#include "../../datatype/SimpleProperty.h"
#include "../../graph/core/AbstractGraph.h"
#include "../../graph/core/GraphMetadata.h"
#include "../../graph/core/Vertex.h"
#include "../../graph/iterator/MemoryIterator.h"
#include "../../memory/DataSet.h"
#include "../../memory/DataSetItem.h"
#include "GeneralizedProximityMatrix.h"

//STL Includes
#include <cassert>

te::sa::GeneralizedProximityMatrix::GeneralizedProximityMatrix() :
  m_dataSetName(""),
  m_attributeName("")
{
}

te::sa::GeneralizedProximityMatrix::~GeneralizedProximityMatrix()
{
}

void te::sa::GeneralizedProximityMatrix::setDataSetName(const std::string& dataSetName)
{
  m_dataSetName = dataSetName;
}

std::string te::sa::GeneralizedProximityMatrix::getDataSetName()
{
  return m_dataSetName;
}

void te::sa::GeneralizedProximityMatrix::setAttributeName( const std::string& attrName)
{
  m_attributeName = attrName;
}

std::string te::sa::GeneralizedProximityMatrix::getAttributeName()
{
  return m_attributeName;
}

void te::sa::GeneralizedProximityMatrix::setGraph(te::graph::AbstractGraph* graph)
{
  m_graph.reset(graph);
}

te::graph::AbstractGraph* te::sa::GeneralizedProximityMatrix::getGraph()
{
  return m_graph.get();
}

void te::sa::GeneralizedProximityMatrix::toDataSource(te::da::DataSourcePtr ds, std::string dataSetName)
{
  assert(m_graph.get());
  assert(ds.get());

  //create datasetype
  std::auto_ptr<te::da::DataSetType> dsType = createDataSetType(dataSetName);

  //create dataset
  std::auto_ptr<te::mem::DataSet> dataSet = createDataSet(dsType.get());

  //save gpm vertex into datasource
  if(dsType.get() && dataSet.get())
  {
    dataSet->moveBeforeFirst();

    std::map<std::string, std::string> options;

    ds->createDataSet(dsType.get(), options);

    ds->add(dataSetName, dataSet.get(), options);
  }
}

std::auto_ptr<te::da::DataSetType> te::sa::GeneralizedProximityMatrix::createDataSetType(std::string dataSetName)
{
  std::auto_ptr<te::da::DataSetType> dataSetType(new te::da::DataSetType(dataSetName));

  //create index property
  te::dt::SimpleProperty* idxProperty = new te::dt::SimpleProperty("index", te::dt::INT32_TYPE);
  dataSetType->add(idxProperty);

  //create primary key
  te::da::PrimaryKey* pk = new te::da::PrimaryKey("pk_index", dataSetType.get());
  pk->add(idxProperty);

  //create all gpm properties
  for(int i = 0; i < m_graph->getMetadata()->getVertexPropertySize(); ++i)
  {
    te::dt::Property* prop = m_graph->getMetadata()->getVertexProperty(i);

    if(prop->getName() == TE_SA_GEOMETRY_ATTR_NAME)
      continue;

    te::dt::Property* newProp = prop->clone();
    newProp->setId(0);
    newProp->setParent(0);

    dataSetType->add(newProp);
  }

  return dataSetType;
}

std::auto_ptr<te::mem::DataSet> te::sa::GeneralizedProximityMatrix::createDataSet(te::da::DataSetType* dsType)
{
  std::auto_ptr<te::mem::DataSet> outDataset(new te::mem::DataSet(dsType));

  //get property map
  std::map<int, std::string> propMap = getGPMPropertyMap();

  //create graph vertex iterator
  std::auto_ptr<te::graph::MemoryIterator> it(new te::graph::MemoryIterator(m_graph.get()));

  te::graph::Vertex* v = it->getFirstVertex();

  while(!it->isVertexIteratorAfterEnd())
  {
    //create dataset item
    te::mem::DataSetItem* outDSetItem = new te::mem::DataSetItem(outDataset.get());

    //get vertex info
    int idx = v->getId();

    //set index information
    outDSetItem->setInt32("index", idx);  

    //set the other attributes
    std::vector<te::dt::AbstractData*> adVec = v->getAttributes();

    std::map<int, std::string>::iterator itMap = propMap.begin();

    while(itMap != propMap.end())
    {
      te::dt::AbstractData* adClone = adVec[itMap->first]->clone();

      outDSetItem->setValue(itMap->second, adClone);

      ++itMap;
    }

    //add item into dataset
    outDataset->add(outDSetItem);

    v = it->getNextVertex();
  }

  return outDataset;
}

std::map<int, std::string> te::sa::GeneralizedProximityMatrix::getGPMPropertyMap()
{
  std::map<int, std::string> propMap;

  for(int i = 0; i < m_graph->getMetadata()->getVertexPropertySize(); ++i)
  {
    te::dt::Property* prop = m_graph->getMetadata()->getVertexProperty(i);

    if(prop->getName() == TE_SA_GEOMETRY_ATTR_NAME)
      continue;

    propMap.insert(std::map<int, std::string>::value_type(i, prop->getName()));
  }

  return propMap;
}
