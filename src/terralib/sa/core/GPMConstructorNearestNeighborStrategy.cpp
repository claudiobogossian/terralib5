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
#include "../../common/Exception.h"
#include "../../common/Translator.h"
#include "../../common/STLUtils.h"
#include "../../common/progress/TaskProgress.h"
#include "../../dataaccess/datasource/DataSource.h"
#include "../../dataaccess/utils/Utils.h"
#include "../../datatype/SimpleData.h"
#include "../../datatype/SimpleProperty.h"
#include "../../geometry/GeometryProperty.h"
#include "../../geometry/Point.h"
#include "../../graph/core/Edge.h"
#include "../../graph/core/Vertex.h"
#include "../../sam/kdtree.h"
#include "GeneralizedProximityMatrix.h"
#include "GPMConstructorNearestNeighborStrategy.h"
#include "Utils.h"

te::sa::GPMConstructorNearestNeighborStrategy::GPMConstructorNearestNeighborStrategy() : te::sa::GPMConstructorAbstractStrategy()
{
  m_type = te::sa::NearestNeighbor;
  m_nNeighbors = 0;
}

te::sa::GPMConstructorNearestNeighborStrategy::GPMConstructorNearestNeighborStrategy(std::size_t nNeighbors) : te::sa::GPMConstructorAbstractStrategy() ,
  m_nNeighbors(nNeighbors)
{
  m_type = te::sa::NearestNeighbor;
}

te::sa::GPMConstructorNearestNeighborStrategy::~GPMConstructorNearestNeighborStrategy()
{
}

void te::sa::GPMConstructorNearestNeighborStrategy::constructStrategy()
{
  //get input information
  std::auto_ptr<te::da::DataSet> dataSet = m_ds->getDataSet(m_gpm->getDataSetName());

  std::size_t geomPos = te::da::GetFirstSpatialPropertyPos(dataSet.get());

  std::auto_ptr<te::da::DataSetType> dsType = m_ds->getDataSetType(m_gpm->getDataSetName());

  te::gm::GeometryProperty* gmProp = dynamic_cast<te::gm::GeometryProperty*>(dsType->getProperty(geomPos));

  //create distance attribute
  createDistanceAttribute(m_gpm);

  //create dataset of points
  te::gm::Envelope e;
  std::vector<std::pair<te::gm::Coord2D, te::gm::Point> > dataset;
  std::map<int, te::gm::Geometry*> geomMap;

  dataSet->moveBeforeFirst();

  while(dataSet->moveNext())
  {
    std::string strId = dataSet->getAsString(m_gpm->getAttributeName());

    int id = atoi(strId.c_str());

    te::gm::Geometry* g = dataSet->getGeometry(geomPos).release();

    te::gm::Coord2D coord = te::sa::GetCentroidCoord(g);
    te::gm::Point point = te::gm::Point(coord.x, coord.y, gmProp->getSRID());

    e.Union(*point.getMBR());

    dataset.push_back(std::pair<te::gm::Coord2D, te::gm::Point>(coord, point));

    geomMap.insert(std::map<int, te::gm::Geometry*>::value_type(id, g));
  }

  // K-d Tree
  typedef te::sam::kdtree::AdaptativeNode<te::gm::Coord2D, std::vector<te::gm::Point>, te::gm::Point> kdNode;
  typedef te::sam::kdtree::AdaptativeIndex<kdNode> kdTree;

  kdTree tree(e, m_nNeighbors);
  tree.build(dataset);

  //create task
  te::common::TaskProgress task;

  task.setTotalSteps(dataSet->size());
  task.setMessage(TE_TR("Creating Edge Objects."));

  //create edges objects
  dataSet->moveBeforeFirst();

  while(dataSet->moveNext())
  {
    std::string strIdFrom = dataSet->getAsString(m_gpm->getAttributeName());

    int vFromId = atoi(strIdFrom.c_str());

    std::auto_ptr<te::gm::Geometry> g = dataSet->getGeometry(geomPos);

    te::gm::Coord2D coord = te::sa::GetCentroidCoord(g.get());

    std::vector<double> distances;

    std::vector<te::gm::Point> points;

    for(std::size_t t = 0; t < m_nNeighbors; ++t)
      points.push_back(te::gm::Point(std::numeric_limits<double>::max(), std::numeric_limits<double>::max()));

    tree.nearestNeighborSearch(coord, points, distances, m_nNeighbors);

    for(size_t t = 0; t < points.size(); ++t)
    {
      int id;

      std::map<int, te::gm::Geometry*>::iterator it = geomMap.find(id);

      if(it != geomMap.end())
      {

        int edgeId = getEdgeId();

        int vToId = id;

        te::graph::Edge* e = new te::graph::Edge(edgeId, vFromId, vToId);

        //caculate distance
        te::graph::Vertex* vFrom = m_gpm->getGraph()->getVertex(vFromId);
        te::gm::Point* pFrom = dynamic_cast<te::gm::Point*>(vFrom->getAttributes()[0]);

        te::graph::Vertex* vTo = m_gpm->getGraph()->getVertex(vToId);
        te::gm::Point* pTo = dynamic_cast<te::gm::Point*>(vTo->getAttributes()[0]);

        double dist = pFrom->distance(pTo);

        te::dt::SimpleData<double, te::dt::DOUBLE_TYPE>* sd = new te::dt::SimpleData<double, te::dt::DOUBLE_TYPE>(dist);
            
        e->setAttributeVecSize(1);
        e->addAttribute(0, sd);

        m_gpm->getGraph()->add(e);
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