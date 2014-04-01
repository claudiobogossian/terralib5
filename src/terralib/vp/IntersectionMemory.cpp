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
  \file AggregationMemory.h

  \brief Aggregation Vector Processing functions.
*/

//Terralib

#include "../common/progress/TaskProgress.h"
#include "../common/Logger.h"
#include "../common/Translator.h"

#include "../dataaccess/dataset/DataSet.h"
#include "../dataaccess/dataset/DataSetType.h"
#include "../dataaccess/utils/Utils.h"
#include "../datatype/Property.h"

#include "../datatype/SimpleProperty.h"
#include "../datatype/StringProperty.h"

#include "../geometry/Geometry.h"
#include "../geometry/GeometryCollection.h"
#include "../geometry/GeometryProperty.h"
#include "../geometry/MultiLineString.h"
#include "../geometry/MultiPoint.h"
#include "../geometry/MultiPolygon.h"
#include "../geometry/Utils.h"

#include "../memory/DataSet.h"
#include "../memory/DataSetItem.h"

#include "../sam.h"

#include "IntersectionMemory.h"
#include "Config.h"
#include "Exception.h"
#include "Utils.h"

// STL
#include <map>
#include <math.h>
#include <string>
#include <vector>

// BOOST
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>


te::vp::IntersectionMemory::IntersectionMemory()
{}

te::vp::IntersectionMemory::~IntersectionMemory()
{}


bool te::vp::IntersectionMemory::run()
{  
  if(m_SRID == 0)
  {
    te::gm::GeometryProperty* geom = te::da::GetFirstGeomProperty(m_inFirstDsetType.get());
    m_SRID = geom->getSRID();
  }

  std::vector<te::dt::Property*> firstProps = getTabularProps(m_inFirstDsetType.get());

  IntersectionMember firstMember;
  firstMember.dt = m_inFirstDsetType.release();
  firstMember.ds = m_inFirstDset.release();
  firstMember.props = firstProps;

  IntersectionMember secondMember;
  secondMember.dt = m_inSecondDsetType.release();
  secondMember.ds = m_inSecondDset.release();
  if(m_copyInputColumns)
    secondMember.props = getTabularProps(secondMember.dt);
  
  std::pair<te::da::DataSetType*, te::da::DataSet*> resultPair;
  resultPair = this->pairwiseIntersection(m_outDsetName, firstMember, secondMember, m_SRID);

  if(resultPair.second->size() < 1)
    throw te::common::Exception(TE_TR("The Layers do not intersect!"));

  std::auto_ptr<te::da::DataSet> outDataSet(resultPair.second);
  std::auto_ptr<te::da::DataSetType> outDataSetType(resultPair.first);

  return save(outDataSet, outDataSetType);
}

std::pair<te::da::DataSetType*, te::da::DataSet*> te::vp::IntersectionMemory::pairwiseIntersection(std::string newName, 
                                                                                                  IntersectionMember firstMember, 
                                                                                                  IntersectionMember secondMember,
                                                                                                  std::size_t outputSRID)
{

  //Creating the RTree with the secound layer geometries
  DataSetRTree rtree(new te::sam::rtree::Index<size_t, 8>);
  size_t secGeomPropPos = secondMember.dt->getPropertyPosition(secondMember.dt->findFirstPropertyOfType(te::dt::GEOMETRY_TYPE));

  size_t secondDsCount = 0;
  while(secondMember.ds->moveNext())
  {
    std::auto_ptr<te::gm::Geometry> g = secondMember.ds->getGeometry(secGeomPropPos);

    rtree->insert(*g->getMBR(), secondDsCount);

    ++secondDsCount;
  }

  firstMember.ds->moveBeforeFirst();

  std::auto_ptr<te::gm::GeometryProperty> fiGeomProp (te::da::GetFirstGeomProperty(firstMember.dt));
  size_t fiGeomPropPos = firstMember.dt->getPropertyPosition(fiGeomProp.get());

  // Create the DataSetType and DataSet
  te::da::DataSetType* outputDt = this->createDataSetType(newName, firstMember.dt, firstMember.props, secondMember.dt, secondMember.props);
  te::mem::DataSet* outputDs = new te::mem::DataSet(outputDt);

  std::pair<te::da::DataSetType*, te::da::DataSet*> resultPair;

  te::common::TaskProgress task("Processing intersection...");
  task.setTotalSteps(firstMember.ds->size());
  task.useTimer(true);

  std::size_t pk = 0;

  while(firstMember.ds->moveNext())
  {
    std::auto_ptr<te::gm::Geometry> currGeom = firstMember.ds->getGeometry(fiGeomPropPos);

    if(currGeom->getSRID() != outputSRID && outputSRID != 0)
      currGeom->transform(outputSRID);

    std::vector<size_t> report;
    rtree->search(*currGeom->getMBR(), report);

    for(size_t i = 0; i < report.size(); ++i)
    {
      secondMember.ds->move(report[i]);
      std::auto_ptr<te::gm::Geometry> secGeom = secondMember.ds->getGeometry(secGeomPropPos);

      if(secGeom->getSRID() != outputSRID && outputSRID != 0)
        secGeom->transform(outputSRID);

      if(!currGeom->intersects(secGeom.get()))
        continue;

      te::mem::DataSetItem* item = new te::mem::DataSetItem(outputDs);
      std::auto_ptr<te::gm::Geometry> resultGeom; 

      if(currGeom->isValid() && secGeom->isValid())
        resultGeom.reset(currGeom->intersection(secGeom.get()));
      
      if(resultGeom.get()!=0 && resultGeom->isValid())
      {
        te::gm::GeometryProperty* fiGeomProp = (te::gm::GeometryProperty*)outputDt->findFirstPropertyOfType(te::dt::GEOMETRY_TYPE);

        if(fiGeomProp->getGeometryType() == te::gm::MultiPolygonType)
        {
          if(resultGeom->getGeomTypeId() == te::gm::MultiPolygonType)
          {
            item->setGeometry("geom", resultGeom.release());
          }
          else if(resultGeom->getGeomTypeId() == te::gm::PolygonType)
          {
            te::gm::MultiPolygon* newGeom = new te::gm::MultiPolygon(0, te::gm::MultiPolygonType, resultGeom->getSRID());
            newGeom->add(resultGeom.release());
            item->setGeometry("geom", newGeom);
          }
        }
        else if(fiGeomProp->getGeometryType() == te::gm::MultiLineStringType)
        {
          if(resultGeom->getGeomTypeId() == te::gm::MultiLineStringType)
          {
            item->setGeometry("geom", resultGeom.release());
          }
          else if(resultGeom->getGeomTypeId() == te::gm::LineStringType)
          {
            te::gm::MultiLineString* newGeom = new te::gm::MultiLineString(0, te::gm::MultiLineStringType, resultGeom->getSRID());
            newGeom->add(resultGeom.release());
            item->setGeometry("geom", newGeom);
          }
        }
        else if(fiGeomProp->getGeometryType() == te::gm::MultiPointType)
        {
          if(resultGeom->getGeomTypeId() == te::gm::MultiPointType)
          {
            item->setGeometry("geom", resultGeom.release());
          }
          else if(resultGeom->getGeomTypeId() == te::gm::PointType)
          {
            te::gm::MultiPoint* newGeom = new te::gm::MultiPoint(0, te::gm::MultiPointType, resultGeom->getSRID());
            newGeom->add(resultGeom.release());
            item->setGeometry("geom", newGeom);
          }
        }
      }
      else
      {
        te::common::Logger::logDebug("vp", "Intersection - Invalid geometry found");
        continue;
      }

      for(size_t j = 0; j < firstMember.props.size(); ++j)
      {
        std::string name = firstMember.props[j]->getName();

        if(!firstMember.dt->getTitle().empty())
          name = te::vp::GetSimpleTableName(firstMember.dt->getTitle()) + "_" + name;

        te::dt::AbstractData* ad = firstMember.ds->getValue(firstMember.props[j]->getName()).release();

        item->setValue(name, ad);
      }

      for(size_t j = 0; j < secondMember.props.size(); ++j)
      {
        std::string name = secondMember.props[j]->getName();
        
        if (!secondMember.dt->getTitle().empty())
          name = te::vp::GetSimpleTableName(secondMember.dt->getTitle()) + "_" + name;

        te::dt::AbstractData* ad = secondMember.ds->getValue(secondMember.props[j]->getName()).release();

        item->setValue(name, ad);
      }

      item->setInt32(newName + "_id", pk);
      ++pk;

      outputDs->moveNext();

      int aux = te::da::GetFirstSpatialPropertyPos(outputDs);

      if(!item->isNull(aux))
        outputDs->add(item);
    }

    if(task.isActive() == false)
    {
      delete outputDt;
      delete outputDs;

      throw te::common::Exception(TE_TR("Operation canceled!"));
    }

    task.pulse();
  }

  outputDs->moveBeforeFirst();

  resultPair.first = outputDt;
  resultPair.second = outputDs;
  return resultPair;
}

te::da::DataSetType* te::vp::IntersectionMemory::createDataSetType(std::string newName, 
                                                                  te::da::DataSetType* firstDt,
                                                                  std::vector<te::dt::Property*> firstProps, 
                                                                  te::da::DataSetType* secondDt,
                                                                  std::vector<te::dt::Property*> secondProps)
{
  te::da::DataSetType* outputDt = new te::da::DataSetType(newName);
  
  

  te::dt::SimpleProperty* pkProperty = new te::dt::SimpleProperty(newName + "_id", te::dt::INT32_TYPE);
  pkProperty->setAutoNumber(true);
  outputDt->add(pkProperty);
  
  te::da::PrimaryKey* pk = new te::da::PrimaryKey(newName + "_pk", outputDt);
  pk->add(pkProperty);
  outputDt->setPrimaryKey(pk);

  for(size_t i = 0; i < firstProps.size(); ++i)
  {
    te::dt::Property* prop = firstProps[i]->clone();
    if(!firstDt->getTitle().empty())
      prop->setName(te::vp::GetSimpleTableName(firstDt->getTitle()) + "_" + prop->getName());
    outputDt->add(prop);
  }

  for(size_t i = 0; i < secondProps.size(); ++i)
  {
    te::dt::Property* prop = secondProps[i]->clone();
    prop->setName(te::vp::GetSimpleTableName(secondDt->getTitle()) + "_" + prop->getName());
    outputDt->add(prop);
  }

  te::gm::GeomType newType = te::vp::GeomOpResultType(te::da::GetFirstGeomProperty(firstDt)->getGeometryType(), te::da::GetFirstGeomProperty(secondDt)->getGeometryType());

  te::gm::GeometryProperty* newGeomProp = new te::gm::GeometryProperty("geom");
  newGeomProp->setGeometryType(newType);
  newGeomProp->setSRID(te::da::GetFirstGeomProperty(firstDt)->getSRID());
  
  outputDt->add(newGeomProp);

  return outputDt;
}