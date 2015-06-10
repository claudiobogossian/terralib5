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
  \file GPMConstructorAdjacencyStrategy.cpp

  \brief This class defines a an adjacency strategy class for a GPM constructor.
*/

// TerraLib Includes
#include "../../common/Exception.h"
#include "../../common/Translator.h"
#include "../../common/STLUtils.h"
#include "../../common/progress/TaskProgress.h"
#include "../../dataaccess/datasource/DataSource.h"
#include "../../dataaccess/utils/Utils.h"
#include "../../datatype/SimpleData.h"
#include "../../datatype/SimpleProperty.h"
#include "../../geometry/Point.h"
#include "../../graph/core/Edge.h"
#include "../../graph/core/Vertex.h"
#include "GeneralizedProximityMatrix.h"
#include "GPMConstructorDistanceStrategy.h"

te::sa::GPMConstructorDistanceStrategy::GPMConstructorDistanceStrategy() : te::sa::GPMConstructorAbstractStrategy()
{
  m_type = te::sa::DistanceStrategy;
  m_distance = 0.;
}

te::sa::GPMConstructorDistanceStrategy::GPMConstructorDistanceStrategy(double distance) : te::sa::GPMConstructorAbstractStrategy() ,
  m_distance(distance)
{
  m_type = te::sa::DistanceStrategy;
}

te::sa::GPMConstructorDistanceStrategy::~GPMConstructorDistanceStrategy()
{
}

void te::sa::GPMConstructorDistanceStrategy::constructStrategy()
{
  //get input information
  std::auto_ptr<te::da::DataSet> dataSet = m_ds->getDataSet(m_gpm->getDataSetName());

  std::size_t geomPos = te::da::GetFirstSpatialPropertyPos(dataSet.get());

  //create distance attribute
  createDistanceAttribute(m_gpm);

  //create tree
  te::sam::rtree::Index<int> rtree;
  std::map<int, te::gm::Geometry*> geomMap;

  dataSet->moveBeforeFirst();

  while(dataSet->moveNext())
  {
    std::string strId = dataSet->getAsString(m_gpm->getAttributeName());

    int id = atoi(strId.c_str());

    te::gm::Geometry* g = dataSet->getGeometry(geomPos).release();
    const te::gm::Envelope* box = g->getMBR();

    rtree.insert(*box, id);

    geomMap.insert(std::map<int, te::gm::Geometry*>::value_type(id, g));
  }

  //create task
  te::common::TaskProgress task;

  task.setTotalSteps(dataSet->size());
  task.setMessage(TE_TR("Creating Edge Objects."));

  //create edge objects
  dataSet->moveBeforeFirst();

  while(dataSet->moveNext())
  {
    std::string strIdFrom = dataSet->getAsString(m_gpm->getAttributeName());

    int vFromId = atoi(strIdFrom.c_str());

    std::auto_ptr<te::gm::Geometry> g = dataSet->getGeometry(geomPos);

    std::vector<int> results;

    te::gm::Envelope ext(*g->getMBR());

    ext.m_llx -= m_distance;
    ext.m_lly -= m_distance;
    ext.m_urx += m_distance;
    ext.m_ury += m_distance;

    rtree.search(ext, results);

    for(size_t t = 0; t < results.size(); ++t)
    {
      std::map<int, te::gm::Geometry*>::iterator it = geomMap.find(results[t]);

      if(it != geomMap.end())
      {
        int vToId = results[t];

        te::graph::Vertex* vFrom = m_gpm->getGraph()->getVertex(vFromId);
        te::gm::Point* pFrom = dynamic_cast<te::gm::Point*>(vFrom->getAttributes()[0]);

        te::graph::Vertex* vTo = m_gpm->getGraph()->getVertex(vToId);
        te::gm::Point* pTo = dynamic_cast<te::gm::Point*>(vTo->getAttributes()[0]);

        double dist = pFrom->distance(pTo);

        if(dist <= m_distance)
        {
          int edgeId = getEdgeId();

          te::graph::Edge* e = new te::graph::Edge(edgeId, vFromId, vToId);

          te::dt::SimpleData<double, te::dt::DOUBLE_TYPE>* sd = new te::dt::SimpleData<double, te::dt::DOUBLE_TYPE>(dist);
            
          e->setAttributeVecSize(1);
          e->addAttribute(0, sd);

          m_gpm->getGraph()->add(e);
        }
      }
    }

    if(!task.isActive())
    {
      te::common::FreeContents(geomMap);
      geomMap.clear();

      throw te::common::Exception(TE_TR("Operation canceled by the user."));
    }

    task.pulse();
  }

  te::common::FreeContents(geomMap);
  geomMap.clear();
}