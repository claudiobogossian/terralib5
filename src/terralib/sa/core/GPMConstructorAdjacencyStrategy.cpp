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
  \file GPMConstructorAdjacencyStrategy.cpp

  \brief This class defines a an adjacency strategy class for a GPM constructor.
*/

// TerraLib Includes
#include "../../common/STLUtils.h"
#include "../../dataaccess/datasource/DataSource.h"
#include "../../dataaccess/utils/Utils.h"
#include "../../datatype/SimpleData.h"
#include "../../datatype/SimpleProperty.h"
#include "../../geometry/Point.h"
#include "../../graph/core/Edge.h"
#include "../../graph/core/Vertex.h"
#include "GeneralizedProximityMatrix.h"
#include "GPMConstructorAdjacencyStrategy.h"

te::sa::GPMConstructorAdjacencyStrategy::GPMConstructorAdjacencyStrategy() : te::sa::GPMConstructorAbstractStrategy()
{
  m_type = te::sa::AdjacencyStrategy;
  m_calcDistance = true;
}

te::sa::GPMConstructorAdjacencyStrategy::GPMConstructorAdjacencyStrategy(bool calcDistance) : te::sa::GPMConstructorAbstractStrategy() ,
  m_calcDistance(calcDistance)
{
  m_type = te::sa::AdjacencyStrategy;
}

te::sa::GPMConstructorAdjacencyStrategy::~GPMConstructorAdjacencyStrategy()
{
}

void te::sa::GPMConstructorAdjacencyStrategy::constructStrategy()
{
  //get input information
  std::auto_ptr<te::da::DataSet> dataSet = m_ds->getDataSet(m_gpm->getDataSetName());

  std::size_t geomPos = te::da::GetFirstSpatialPropertyPos(dataSet.get());

  //create distance attribute
  if(m_calcDistance)
  {
    te::dt::SimpleProperty* p = new te::dt::SimpleProperty(TE_SA_DISTANCE_ATTR_NAME, te::dt::DOUBLE_TYPE);
    p->setParent(0);
    p->setId(0);

    m_gpm->getGraph()->addEdgeProperty(p);
  }

  //create tree
  te::sam::rtree::Index<int> rtree;
  std::map<int, te::gm::Geometry*> geomMap;

  dataSet->moveBeforeFirst();

  while(dataSet->moveNext())
  {
    int id = dataSet->getInt32(m_gpm->getAttributeName());
    te::gm::Geometry* g = dataSet->getGeometry(geomPos).release();
    const te::gm::Envelope* box = g->getMBR();

    rtree.insert(*box, id);

    geomMap.insert(std::map<int, te::gm::Geometry*>::value_type(id, g));
  }

  //create edges objects
  dataSet->moveBeforeFirst();

  while(dataSet->moveNext())
  {
    int vFromId = dataSet->getInt32(m_gpm->getAttributeName());

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

          te::graph::Edge* e = new te::graph::Edge(edgeId, vFromId, vToId);

          if(m_calcDistance)
          {
            te::graph::Vertex* vFrom = m_gpm->getGraph()->getVertex(vFromId);
            te::gm::Point* pFrom = dynamic_cast<te::gm::Point*>(vFrom->getAttributes()[0]);

            te::graph::Vertex* vTo = m_gpm->getGraph()->getVertex(vToId);
            te::gm::Point* pTo = dynamic_cast<te::gm::Point*>(vTo->getAttributes()[0]);

            double dist = pFrom->distance(pTo);

            te::dt::SimpleData<double, te::dt::DOUBLE_TYPE>* sd = new te::dt::SimpleData<double, te::dt::DOUBLE_TYPE>(dist);
            
            e->setAttributeVecSize(1);
            e->addAttribute(0, sd);
          }

          m_gpm->getGraph()->add(e);
        }
      }
    }
  }

  te::common::FreeContents(geomMap);
  geomMap.clear();
}