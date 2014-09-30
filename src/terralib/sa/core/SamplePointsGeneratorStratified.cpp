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
  \file terralib/sa/core/SamplePointsGeneratorStratified.cpp

  \brief This file contains a class to generate samples points using stratified strategy.

  \reference Adapted from TerraLib4.
*/

// Terralib Includes
#include "../../common/Exception.h"
#include "../../common/Translator.h"
#include "../../common/STLUtils.h"
#include "../../common/progress/TaskProgress.h"
#include "../../dataaccess/utils/Utils.h"
#include "../../datatype/SimpleProperty.h"
#include "../../geometry/Geometry.h"
#include "../../geometry/GeometryProperty.h"
#include "../../geometry/Polygon.h"
#include "../../memory/DataSet.h"
#include "../../memory/DataSetItem.h"
#include "../core/Utils.h"
#include "../Enums.h"
#include "SamplePointsGeneratorStratified.h"

te::sa::SamplePointsGeneratorStratified::SamplePointsGeneratorStratified() : te::sa::SamplePointsGeneratorAbstract(), m_nPoints(0), m_attrName(""), m_propToArea(false)
{
  m_type = te::sa::Stratified;
}

te::sa::SamplePointsGeneratorStratified::~SamplePointsGeneratorStratified()
{
  std::map<std::string, std::vector<te::gm::Geometry*> >::iterator it = m_classMap.begin();

  while(it != m_classMap.end())
  {
    std::vector<te::gm::Geometry*> geomVec = it->second;

    te::common::FreeContents(geomVec);

    ++it;
  }

  m_classMap.clear();
}

void te::sa::SamplePointsGeneratorStratified::setNumberOfPoints(int nPoints)
{
  m_nPoints = nPoints;
}

void te::sa::SamplePointsGeneratorStratified::setInputDataSet(std::auto_ptr<te::da::DataSet> dataSet)
{
  m_dataSet = dataSet;
}

void te::sa::SamplePointsGeneratorStratified::setInputAttributeName(std::string attrName)
{
  m_attrName = attrName;
}

void te::sa::SamplePointsGeneratorStratified::isProportionalToArea(bool isProp)
{
  m_propToArea = isProp;
}

std::vector<std::string> te::sa::SamplePointsGeneratorStratified::getClassNames()
{
  std::vector<std::string> names;

  std::map<std::string, std::vector<te::gm::Geometry*> >::iterator it =  m_classMap.begin();

  while(it != m_classMap.end())
  {
    names.push_back(it->first);

    ++it;
  }

  return names;
}

std::auto_ptr<te::da::DataSetType> te::sa::SamplePointsGeneratorStratified::createDataSetType()
{
  std::auto_ptr<te::da::DataSetType> dsType(new te::da::DataSetType(m_outputDataSetName));

  //create id property
  te::dt::SimpleProperty* idProperty = new te::dt::SimpleProperty(TE_SA_SPG_ATTR_ID_NAME, te::dt::INT32_TYPE);
  dsType->add(idProperty);

  //create class property
  te::dt::SimpleProperty* classProperty = new te::dt::SimpleProperty(TE_SA_SPG_ATTR_CLASS_NAME, te::dt::STRING_TYPE);
  dsType->add(classProperty);

  //create geometry property
  te::gm::GeometryProperty* geomProperty = new te::gm::GeometryProperty(TE_SA_SPG_ATTR_GEOM_NAME, m_srid, te::gm::PointType);
  dsType->add(geomProperty);

  //create primary key
  std::string pkName = TE_SA_SPG_ATTR_PK_NAME;
              pkName+= "_" + m_outputDataSetName;
  te::da::PrimaryKey* pk = new te::da::PrimaryKey(pkName, dsType.get());
  pk->add(idProperty);

  return dsType;
}

std::auto_ptr<te::mem::DataSet> te::sa::SamplePointsGeneratorStratified::generateSamples(te::da::DataSetType* dsType)
{
  std::auto_ptr<te::mem::DataSet> ds(new te::mem::DataSet(dsType));

  //create the class map
  createClassMap();

  std::size_t nClass = m_classMap.size();

  std::map<std::string, int> nPointsPerClass;

  //define the number of samples for each class using the area of the polygons 
  if(m_propToArea)
  {
    std::map<std::string, double> areaPerClass;

    std::map<std::string, std::vector<te::gm::Geometry*> >::iterator it = m_classMap.begin();

    //calculate the total area of each class
    std::vector<double> areas;

    while(it != m_classMap.end())
    {
      std::vector<te::gm::Geometry*> geomVec = it->second;

      double totArea = 0.;

      for(std::size_t t = 0; t < geomVec.size(); ++t)
      {
        totArea += te::sa::GetArea(geomVec[t]);
      }

      areaPerClass.insert(std::map<std::string, double>::value_type(it->first, totArea));

      areas.push_back(totArea);

      ++it;
    }

    //calculate the proportional number of points for each class
    std::vector<double>::iterator minPos = std::min_element(areas.begin(), areas.end());
    double minArea = *minPos;

    it = m_classMap.begin();

    while(it != m_classMap.end())
    {
      int nPoints = (int)((areaPerClass[it->first] * m_nPoints) / minArea);

      nPointsPerClass.insert(std::map<std::string, int>::value_type(it->first, nPoints));

      ++it;
    }
  }
  else
  {
    std::map<std::string, std::vector<te::gm::Geometry*> >::iterator it = m_classMap.begin();

    //associate the default number of points for each class
    while(it != m_classMap.end())
    {
      nPointsPerClass.insert(std::map<std::string, int>::value_type(it->first, m_nPoints));

      ++it;
    }
  }

  //create task
  te::common::TaskProgress task;

  task.setTotalSteps(m_classMap.size());
  task.setMessage(TE_TR("Creating Points by Class."));

  //generate samples
  int idCount = 0;

  std::map<std::string, std::vector<te::gm::Geometry*> >::iterator it = m_classMap.begin();

  while(it != m_classMap.end())
  {
    std::string className = it->first;

    std::vector<te::gm::Geometry*> geomVec = it->second;

    m_distInt = boost::random::uniform_int_distribution<>(0, geomVec.size() - 1);

    int nPoints = nPointsPerClass[className];

    for(int i = 0; i < nPoints; ++i)
    {
       //create dataset item
      te::mem::DataSetItem* item = new te::mem::DataSetItem(ds.get());

      //set id
      item->setInt32(TE_SA_SPG_ATTR_ID_NAME, idCount);

      //set class name
      item->setString(TE_SA_SPG_ATTR_CLASS_NAME, className);

      //set geometry
      item->setGeometry(TE_SA_SPG_ATTR_GEOM_NAME, getPointInGeometry(geomVec[m_distInt(m_gen)]));

      ds->add(item);

      ++idCount;
    }

    if(!task.isActive())
    {
      throw te::common::Exception(TE_TR("Operation canceled by the user."));
    }

    task.pulse();

    ++it;
  }

  return ds;
}

void te::sa::SamplePointsGeneratorStratified::createClassMap()
{
  m_classMap.clear();

  m_dataSet->moveBeforeFirst();

  std::size_t geomIdx = te::da::GetFirstPropertyPos(m_dataSet.get(), te::dt::GEOMETRY_TYPE);

  while(m_dataSet->moveNext())
  {
    //class name
    std::string className = m_dataSet->getString(m_attrName);

    //geometry
    te::gm::Geometry* geom = m_dataSet->getGeometry(geomIdx).release();

    std::map<std::string, std::vector<te::gm::Geometry*> >::iterator it = m_classMap.find(className);

    //insert into map
    if(it == m_classMap.end())
    {
      std::vector<te::gm::Geometry*> geomVec;
      geomVec.push_back(geom);

      m_classMap.insert(std::map<std::string, std::vector<te::gm::Geometry*> >::value_type(className, geomVec));
    }
    else
    {
      it->second.push_back(geom);
    }
  }
}

te::gm::Point* te::sa::SamplePointsGeneratorStratified::getPointInGeometry(te::gm::Geometry* g)
{
  te::gm::Point* p = 0;

  bool within = false;

  while(within == false)
  {
    p = getPoint(g->getMBR());

    if(p->within(g))
      within = true;
    else
      delete p;
  }

  return p;
}
