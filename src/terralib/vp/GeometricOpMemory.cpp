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
  \file GeographicOpMemory.h

  \brief Geographic Vector Processing functions.
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
#include "../geometry/GeometryProperty.h"
#include "../geometry/GeometryCollection.h"
#include "../geometry/LineString.h"
#include "../geometry/MultiLineString.h"
#include "../geometry/MultiPolygon.h"
#include "../geometry/Point.h"
#include "../geometry/Polygon.h"
#include "../geometry/Utils.h"

#include "../memory/DataSet.h"
#include "../memory/DataSetItem.h"

#include "GeometricOpMemory.h"
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

te::vp::GeometricOpMemory::GeometricOpMemory()
{}

te::vp::GeometricOpMemory::~GeometricOpMemory()
{}


bool te::vp::GeometricOpMemory::run()
{
  // get the geometric operation and/or tabular operation. 
  std::vector<int> opGeom;
  std::vector<int> opTab;
  std::vector<te::da::DataSetType*> dsTypeVec;

  for(std::size_t i = 0; i < m_operations.size(); ++i)
  {
    switch(m_operations[i])
    {
      case te::vp::CENTROID:
        opGeom.push_back(te::vp::CENTROID);
        break;
      case te::vp::CONVEX_HULL:
        opGeom.push_back(te::vp::CONVEX_HULL);
        break;
      case te::vp::MBR:
        opGeom.push_back(te::vp::MBR);
        break;
      case te::vp::AREA:
        opTab.push_back(te::vp::AREA);
        break;
      case te::vp::LINE:
        opTab.push_back(te::vp::LINE);
        break;
      case te::vp::PERIMETER:
        opTab.push_back(te::vp::PERIMETER);
        break;
    }
  }

  if(m_outputLayer)
  {
    bool hasMultiGeomColumns = false;

    switch(m_objStrategy)
    {
      case te::vp::ALL_OBJ:
        {
          if(hasMultiGeomColumns) //Condição se o DataSource suporta mais de uma coluna geometrica...
          {
            dsTypeVec[0] = te::vp::GeometricOp::GetDataSetType(te::vp::ALL_OBJ, true);
          }
          else
          {
            for(std::size_t i = 0; i < opGeom.size(); ++i)
              dsTypeVec.push_back(te::vp::GeometricOp::GetDataSetType(te::vp::ALL_OBJ, false, opGeom[i]));
          }


        }
        break;
      case te::vp::AGGREG_OBJ:
        {
          if(hasMultiGeomColumns) //Condição se o DataSource suporta mais de uma coluna geometrica...
          {
            dsTypeVec[0] = te::vp::GeometricOp::GetDataSetType(te::vp::AGGREG_OBJ, true);
          }
          else
          {
            for(std::size_t i = 0; i < opGeom.size(); ++i)
              dsTypeVec.push_back(te::vp::GeometricOp::GetDataSetType(te::vp::AGGREG_OBJ, false, opGeom[i]));
          }
        }
        break;
      case te::vp::AGGREG_BY_ATTRIBUTE:
        {
          if(hasMultiGeomColumns) //Condição se o DataSource suporta mais de uma coluna geometrica...
          {
            dsTypeVec[0] = te::vp::GeometricOp::GetDataSetType(te::vp::AGGREG_BY_ATTRIBUTE, true);
          }
          else
          {
            for(std::size_t i = 0; i < opGeom.size(); ++i)
              dsTypeVec.push_back(te::vp::GeometricOp::GetDataSetType(te::vp::AGGREG_BY_ATTRIBUTE, false, opGeom[i]));
          }
        }
        break;
    }


    //Baseado no DSType....adicionar os valores no DataSet.....


  }
  else
  {
    //Descobrir se o DataSource suporta adição de mais de uma coluna geometrica.
    return false;
  }
  return true;
}


void te::vp::GeometricOpMemory::SetAllObjects(std::vector<te::da::DataSetType*> dsTypeVec, 
                                              std::vector<int> tabVec, 
                                              std::vector<int> geoVec)
{
  for(std::size_t i = 0; i < dsTypeVec.size(); ++i)
  {
    std::auto_ptr<te::da::DataSetType> dsType(dsTypeVec[0]);
    std::auto_ptr<te::mem::DataSet> outDSet(new te::mem::DataSet(dsType.get()));

    m_inDset->moveBeforeFirst();

    while(m_inDset->moveNext())
    {
      te::mem::DataSetItem* item = new te::mem::DataSetItem(outDSet.get());
      for(std::size_t item_pos = 0; item_pos < item->getNumProperties(); ++item_pos)
      {
        item->setInt32(0, item_pos);

        if(m_selectedProps.size() > 0)
        {
          for(std::size_t prop_pos = 0; prop_pos < m_selectedProps.size(); ++prop_pos)
          {
            item->setValue(m_selectedProps[prop_pos], m_inDset->getValue(m_selectedProps[prop_pos]).get());
          }
        }


      }
    }
  }
}
