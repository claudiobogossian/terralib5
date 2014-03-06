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
#include "../dataaccess/utils/Utils.h"
#include "../datatype/Property.h"
#include "../datatype/SimpleProperty.h"
#include "../datatype/StringProperty.h"

#include "../geometry/Geometry.h"
#include "../geometry/GeometryCollection.h"
#include "../geometry/GeometryProperty.h"
#include "../geometry/Utils.h"

#include "../memory/DataSet.h"
#include "../memory/DataSetItem.h"

#include "../sam/rtree.h"

#include "BufferMemory.h"
#include "Config.h"
#include "Exception.h"
#include "Utils.h"

// STL
#include <map>
#include <string>
#include <vector>

// BOOST
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>

te::vp::BufferMemory::BufferMemory()
{}

te::vp::BufferMemory::~BufferMemory()
{}

bool te::vp::BufferMemory::run()
{
  std::auto_ptr<te::da::DataSetType> outDSType(GetDataSetType());
  std::auto_ptr<te::mem::DataSet> outDSet(new te::mem::DataSet(outDSType.get()));

  int type;
  int pk = 0;

  m_inDset->moveBeforeFirst();
  while(m_inDset->moveNext())
  {
    te::gm::Geometry* auxGeom = 0;

    for(int i = 1; i <= m_levels; ++i)
    {
      te::mem::DataSetItem* dataSetItem = new te::mem::DataSetItem(outDSet.get());

      for(std::size_t j = 0; j < m_inDset->getNumProperties(); ++j)
      {
        type = m_inDset->getPropertyDataType(j);
        if(m_copyInputColumns)
        {
          switch (type)
          {
            case te::dt::INT32_TYPE:
              if(m_inDset->getPropertyName(j) != "FID")
                dataSetItem->setInt32(j+2, m_inDset->getInt32(j));
              break;
            case te::dt::INT64_TYPE:
              dataSetItem->setInt64(j+2, m_inDset->getInt64(j));
              break;
            case te::dt::DOUBLE_TYPE:
              dataSetItem->setDouble(j+2, m_inDset->getDouble(j));
              break;
            case te::dt::STRING_TYPE:
              dataSetItem->setString(j+2, m_inDset->getString(j));
              break;
            case te::dt::GEOMETRY_TYPE:
              dataSetItem->setInt32(0, pk); //pk
              dataSetItem->setInt32(1, i); //level
              dataSetItem->setDouble(2, m_distance*(i)); //distance

              std::auto_ptr<te::gm::Geometry> currentGeom = m_inDset->getGeometry(j);
              std::auto_ptr<te::gm::Geometry> outGeom;

              if(currentGeom->isValid())
                outGeom.reset(setBuffer(currentGeom.get(), m_distance, i, auxGeom));
              else
                te::common::Logger::logDebug("vp", "Buffer - Invalid geometry found");

              if(outGeom.get() && outGeom->isValid())
              {
                if(outGeom->getGeomTypeId() == te::gm::MultiPolygonType)
                {
                  dataSetItem->setGeometry(j+2, outGeom.release());
                }
                else
                {
                  std::auto_ptr<te::gm::GeometryCollection> mPolygon(new te::gm::GeometryCollection(0, te::gm::MultiPolygonType, outGeom->getSRID()));
                  mPolygon->add(outGeom.release());
                  dataSetItem->setGeometry(j+2, mPolygon.release());
                }

                outDSet->add(dataSetItem);
                ++pk;
              }
          }
        }
        else
        {
          if(type == te::dt::GEOMETRY_TYPE)
          {
            dataSetItem->setInt32(0, pk); //pk
            dataSetItem->setInt32(1, i); //level
            dataSetItem->setDouble(2, m_distance*(i)); //distance

            std::auto_ptr<te::gm::Geometry> currentGeom = m_inDset->getGeometry(j);
            std::auto_ptr<te::gm::Geometry> outGeom;

            if(currentGeom->isValid())
              outGeom.reset(setBuffer(currentGeom.get(), m_distance, i, auxGeom));
            else
              te::common::Logger::logDebug("vp", "Buffer - Invalid geometry found");

            if(outGeom.get() && outGeom->isValid())
            {
              if(outGeom->getGeomTypeId() == te::gm::MultiPolygonType)
              {
                dataSetItem->setGeometry(3, outGeom.release());
              }
              else
              {
                std::auto_ptr<te::gm::GeometryCollection> mPolygon(new te::gm::GeometryCollection(0, te::gm::MultiPolygonType, outGeom->getSRID()));
                mPolygon->add(outGeom.release());
                dataSetItem->setGeometry(3, mPolygon.release());
              }

              outDSet->add(dataSetItem);
              ++pk;
            }
          }
        }
      }
    }
  }
  
  if(m_bufferBoundariesRule == te::vp::DISSOLVE)
  {
    dissolveMemory(outDSet.get(), m_levels);
  }

  return save(outDSet,outDSType);
}


te::gm::Geometry* te::vp::BufferMemory::setBuffer(te::gm::Geometry* geom,  
                                                  const double& distance, 
                                                  const int& level,
                                                  te::gm::Geometry*& auxGeom)
{
  te::gm::Geometry* geomResult = 0;
  te::gm::Geometry* geomTemp = 0;
  std::auto_ptr<te::gm::Geometry> outGeom;
  std::auto_ptr<te::gm::Geometry> inGeom;
  switch(m_bufferPolygonRule)
  {
    case (te::vp::INSIDE_OUTSIDE):
      outGeom.reset(geom->buffer(distance * level, 16, te::gm::CapButtType));
      inGeom.reset(geom->buffer(-distance * level, 16, te::gm::CapButtType));
      geomResult = outGeom->difference(inGeom.get());
      
      geomTemp = (te::gm::Geometry*)geomResult->clone();
      if(auxGeom && auxGeom->isValid())
        geomResult = geomResult->difference(auxGeom);

      delete auxGeom;
      auxGeom = geomTemp;

      break;

    case (te::vp::ONLY_OUTSIDE):
      outGeom.reset(geom->buffer(distance * level, 16, te::gm::CapButtType));
      geomResult = outGeom->difference(geom);

      geomTemp = (te::gm::Geometry*)geomResult->clone();
      if(auxGeom && auxGeom->isValid())
        geomResult = geomResult->difference(auxGeom);

      delete auxGeom;
      auxGeom = geomTemp;

      break;

    case (te::vp::ONLY_INSIDE):
      inGeom.reset(geom->buffer(-distance * level, 16, te::gm::CapButtType));
      geomResult = geom->difference(inGeom.get());

      geomTemp = (te::gm::Geometry*)geomResult->clone();
      if(auxGeom && auxGeom->isValid())
        geomResult = geomResult->difference(auxGeom);

      delete auxGeom;
      auxGeom = geomTemp;

      break;
  }
  return geomResult;
}

void te::vp::BufferMemory::dissolveMemory(te::mem::DataSet* outDSet,
                                          const int& levels)
{
  std::vector<std::vector<te::gm::Geometry*> > vecGeom;
    
  int levelPos = te::da::GetPropertyPos(outDSet, "level");
  int geomPos  = te::da::GetPropertyPos(outDSet, "geom");
  int level;

  //te::common::TaskProgress task1("Dissolving boundaries...");
  //task1.setTotalSteps(levels*outDSet->size());
  //task1.setCurrentStep(1);
  for(int i = 1; i <= levels; ++i)
  {
    te::sam::rtree::Index<te::gm::Geometry*, 4> rtree;
      
    outDSet->moveBeforeFirst();
    while(outDSet->moveNext())
    {
      level = outDSet->getInt32(levelPos);
      if(level == i)
      {
        te::gm::Geometry* geom = outDSet->getGeometry(geomPos).release();
          
        std::vector<te::gm::Geometry*> vec;
          
        rtree.search(*(geom->getMBR()), vec);
          
        if(!vec.empty())
        {
          for(std::size_t t = 0; t < vec.size(); ++t)
          {
            if(geom->intersects(vec[t]))
            {
              geom = geom->Union(vec[t]);
              rtree.remove(*(vec[t]->getMBR()), vec[t]);
            }
          }
        }
        rtree.insert(*(geom->getMBR()), geom);
      }
      //task1.pulse();
    }
      
    std::vector<te::gm::Geometry*> geomVec;
    std::auto_ptr<te::gm::Envelope> e = outDSet->getExtent(geomPos);
    rtree.search(*(e.get()), geomVec);

    vecGeom.push_back(geomVec);

    rtree.clear();
  }
    
  outDSet->clear();
  outDSet->moveBeforeFirst();

  
  int pk = 0;
  std::size_t vecSize = vecGeom.size();
  
  //task1.setTotalSteps(vecSize);
  //task1.setCurrentStep(1);
  if(levels > 1)
  {
    for(std::size_t i = vecSize - 1; i > 0; --i)
    {
      std::vector<te::gm::Geometry*> currentVec = vecGeom[i];
      std::size_t c_vecSize = currentVec.size();

      for(std::size_t j = 0; j < i; ++j)
      {
        std::vector<te::gm::Geometry*> innerVec = vecGeom[j];
        std::size_t i_vecSize = innerVec.size();

        for(std::size_t k = 0; k < c_vecSize; ++k)
        {
          for(std::size_t l = 0; l < i_vecSize; ++l)
          {
            te::gm::Geometry* k_geom = currentVec[k];
            te::gm::Geometry* l_geom = innerVec[l];

            if(k_geom->intersects(l_geom))
            {
              te::gm::Geometry* tGeom = k_geom->difference(l_geom);
              if(tGeom->isValid())
              {
                delete currentVec[k];
                currentVec[k] = tGeom;
                vecGeom[i] = currentVec;
              }
            }
          }
        }
      }
      //task1.pulse();
    }
  }

  //task1.setTotalSteps(vecSize);
  //task1.setCurrentStep(1);
  for(std::size_t i = 0; i < vecSize; ++i)
  {
    std::vector<te::gm::Geometry*> currentVec = vecGeom[i];
    std::size_t c_vecSize = currentVec.size();

    for(std::size_t j = 0; j < c_vecSize; ++j)
    {
      te::mem::DataSetItem* dataSetItem = new te::mem::DataSetItem(outDSet);
      dataSetItem->setInt32(0, pk); //pk
      dataSetItem->setInt32(1, i+1); //level
      dataSetItem->setDouble(2, 0/*distance*(i)*/); //distance
        
      if(currentVec[j]->getGeomTypeId() == te::gm::MultiPolygonType)
      {
        dataSetItem->setGeometry(3, currentVec[j]);
      }
      else
      {
        std::auto_ptr<te::gm::GeometryCollection> mPolygon(new te::gm::GeometryCollection(0, te::gm::MultiPolygonType, currentVec[j]->getSRID()));
        te::gm::GeometryCollection* gcIn = dynamic_cast<te::gm::GeometryCollection*>(currentVec[j]);
        if(gcIn == 0)
          mPolygon->add(currentVec[j]);
        else
          te::vp::SplitGeometryCollection(gcIn, mPolygon.get());

        dataSetItem->setGeometry(3, mPolygon.release());
      }
        
      outDSet->add(dataSetItem);
      ++pk;
    }
    //task1.pulse();
  }
}

