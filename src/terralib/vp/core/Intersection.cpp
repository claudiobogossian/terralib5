/*  Copyright (C) 2011-2012 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/vp/core/IntersectionDialog.cpp

  \brief Group of functions to intersection operation
*/

// Terralib
#include "../../common/Translator.h"
#include "../../dataaccess/dataset/DataSet.h"
#include "../../dataaccess/dataset/DataSetType.h"
#include "../../datatype/Property.h"
#include "../../memory/DataSet.h"
#include "../../memory/DataSetItem.h"
#include "../../geometry/Geometry.h"
#include "../../geometry/GeometryProperty.h"
#include "Config.h"
#include "Exception.h"
#include "Intersection.h"

te::da::DataSetType* te::vp::CreateDataSetType(std::string newName, te::da::DataSetType* firstDt,
                                               std::vector<te::dt::Property*>& firstProps, te::da::DataSetType* secondDt,
                                               std::vector<te::dt::Property*>& secondProps)
{
  te::da::DataSetType* outputDt = new te::da::DataSetType(newName);

  size_t fiGeomPropPos = firstDt->getDefaultGeomPropertyPos();

  for(size_t i = 0; i < firstProps.size(); ++i)
  {
    te::dt::Property* prop = firstProps[i]->clone();
    if(!firstDt->getTitle().empty())
      prop->setName(firstDt->getTitle() + "_" + prop->getName());
    outputDt->add(prop);
  }

  for(size_t i = 0; i < secondProps.size(); ++i)
  {
    te::dt::Property* prop = secondProps[i]->clone();
    prop->setName(secondDt->getTitle() + "_" + prop->getName());
    outputDt->add(prop);
  }

  te::gm::GeometryProperty* fiGeomProp = firstDt->getDefaultGeomProperty();

  te::gm::GeometryProperty* newGeomProp = (te::gm::GeometryProperty*)fiGeomProp->clone();
  outputDt->add(newGeomProp);
  outputDt->setDefaultGeomProperty(newGeomProp);

  return outputDt;
}

te::sam::rtree::Index<size_t, 8>* te::vp::CreateRTree(te::da::DataSetType* dt, te::da::DataSet* ds)
{
  te::sam::rtree::Index<size_t, 8>* rtree = new te::sam::rtree::Index<size_t, 8>;

  size_t secGeomPropPos = dt->getDefaultGeomPropertyPos();

  size_t secondDsCount = 0;
  while(ds->moveNext())
  {
    te::gm::Geometry* g = ds->getGeometry(secGeomPropPos);

    rtree->insert(*g->getMBR(), secondDsCount);

    delete g;

    ++secondDsCount;
  }

  return rtree;
}

std::pair<te::da::DataSetType*, te::da::DataSet*> te::vp::Intersect(std::string newName,
                                                                    std::map<te::map::AbstractLayerPtr,
                                                                    std::vector<te::dt::Property*> > layers)
{
  if(layers.size() <= 1)
    throw te::common::Exception(TR_VP("At least two layers are necessary for an intersection!"));

  std::map<te::map::AbstractLayerPtr, std::vector<te::dt::Property*> >::iterator it = layers.begin();
  std::map<te::map::AbstractLayerPtr, std::vector<te::dt::Property*> >::iterator itAux;

  size_t countAux = 0;
  std::pair<te::da::DataSetType*, te::da::DataSet*> resultPair;
  while(it != layers.end())
  {
    if(it == layers.begin())
    {
      itAux = it;
      ++countAux;
      ++it;
      continue;
    }

    te::da::DataSetType* secondDt = (te::da::DataSetType*)it->first->getSchema();
    te::da::DataSet* secondDs = it->first->getData();
    std::vector<te::dt::Property*> secondProps = it->second;

    if(countAux == 1)
    {
      te::da::DataSetType* firstDt = (te::da::DataSetType*)itAux->first->getSchema();
      te::da::DataSet* fiDs = itAux->first->getData();
      std::vector<te::dt::Property*> firstProps = itAux->second;

      resultPair = PairwiseIntersection(newName, firstDt, fiDs, firstProps, secondDt, secondDs, secondProps);

    }
    else if(countAux > 1)
    {
      std::vector<te::dt::Property*> resultProps = resultPair.first->getProperties();
      std::vector<te::dt::Property*> auxProps;

      for(size_t i = 0; i < resultProps.size(); ++i)
      {
        if(resultProps[i] != resultPair.first->getDefaultGeomProperty())
          auxProps.push_back(resultProps[i]);
      }

      resultPair = PairwiseIntersection(newName, resultPair.first, resultPair.second, auxProps, secondDt, secondDs, secondProps);
    }

    ++countAux;
    ++it;
  }

  return resultPair;
}

std::pair<te::da::DataSetType*, te::da::DataSet*> te::vp::PairwiseIntersection(std::string newName, 
                                                                               te::da::DataSetType* firstDt, 
                                                                               te::da::DataSet* firstDs,
                                                                               std::vector<te::dt::Property*> firstProps, 
                                                                               te::da::DataSetType* secondDt, 
                                                                               te::da::DataSet* secondDs, 
                                                                               std::vector<te::dt::Property*> secondProps)
{

  //Creating the RTree with the secound layer geometries
  te::sam::rtree::Index<size_t, 8>* rtree = CreateRTree(secondDt, secondDs);

  firstDs->moveBeforeFirst();

  size_t fiGeomPropPos = firstDt->getDefaultGeomPropertyPos();
  te::gm::GeometryProperty* fiGeomProp = firstDt->getDefaultGeomProperty();

  size_t secGeomPropPos = secondDt->getDefaultGeomPropertyPos();

  // Create the DataSetType and DataSet
  te::da::DataSetType* outputDt = CreateDataSetType(newName, firstDt, firstProps, secondDt, secondProps);
  te::mem::DataSet* outputDs = new te::mem::DataSet(outputDt);

  while(firstDs->moveNext())
  {
    te::gm::Geometry* currGeom = firstDs->getGeometry(fiGeomPropPos);

    std::vector<size_t> report;
    rtree->search(*currGeom->getMBR(), report);

    for(size_t i = 0; i < report.size(); ++i)
    {
      secondDs->move(report[i]);
      te::gm::Geometry* secGeom = secondDs->getGeometry(secGeomPropPos);

      if(!currGeom->intersects(secGeom))
        continue;

      te::mem::DataSetItem* item = new te::mem::DataSetItem(outputDs);
      te::gm::Geometry* newGeom = currGeom->intersection(secGeom);

      item->setGeometry(fiGeomProp->getName(), *newGeom);

      for(size_t j = 0; j < firstProps.size(); ++j)
      {
        std::string name = "";

        name = firstProps[j]->getName();

        if(!firstDt->getTitle().empty())
          name = firstDt->getTitle() + "_" + firstProps[j]->getName();

        if(!firstDs->isNull(firstProps[j]->getName()))
        {
          te::dt::AbstractData* ad = firstDs->getValue(firstProps[j]->getName());

          item->setValue(name, ad);
        }
      }

      for(size_t j = 0; j < secondProps.size(); ++j)
      {
        std::string name = secondDt->getTitle() + "_" + secondProps[j]->getName();

        if(!secondDs->isNull(secondProps[j]->getName()))
        {
          te::dt::AbstractData* ad = secondDs->getValue(secondProps[j]->getName());

          item->setValue(name, ad);
        }
      }

      outputDs->moveNext();
      outputDs->add(item);

    }
  }

  std::pair<te::da::DataSetType*, te::da::DataSet*> resultPair;
  resultPair.first = outputDt;
  resultPair.second = outputDs;
  return resultPair;
}