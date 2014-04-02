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
  \file AggregationQuery.h

  \brief Aggregation Vector Processing functions.
*/

//Terralib

#include "../common/progress/TaskProgress.h"
#include "../common/Logger.h"
#include "../common/Translator.h"

#include "../dataaccess/dataset/DataSet.h"

#include "../datatype/Property.h"
#include "../datatype/SimpleProperty.h"
#include "../datatype/StringProperty.h"

#include "../dataaccess/query/Avg.h"
#include "../dataaccess/query/Count.h"
#include "../dataaccess/query/DataSetName.h"
#include "../dataaccess/query/Expression.h"
#include "../dataaccess/query/Field.h"
#include "../dataaccess/query/Fields.h"
#include "../dataaccess/query/From.h"
#include "../dataaccess/query/FromItem.h"
#include "../dataaccess/query/GroupBy.h"
#include "../dataaccess/query/GroupByItem.h"
#include "../dataaccess/query/Max.h"
#include "../dataaccess/query/Min.h"
#include "../dataaccess/query/PropertyName.h"
#include "../dataaccess/query/Select.h"
#include "../dataaccess/query/Sub.h"
#include "../dataaccess/query/Sum.h"
#include "../dataaccess/query/StdDev.h"
#include "../dataaccess/query/ST_Union.h"
#include "../dataaccess/query/Variance.h"
#include "../dataaccess/utils/Utils.h"

#include "../geometry/Geometry.h"
#include "../geometry/GeometryCollection.h"
#include "../geometry/GeometryProperty.h"
#include "../geometry/Utils.h"

#include "../memory/DataSet.h"
#include "../memory/DataSetItem.h"

#include "../statistics/core/SummaryFunctions.h"
#include "../statistics/core/StringStatisticalSummary.h"
#include "../statistics/core/NumericStatisticalSummary.h"

#include "BasicGeoOpQuery.h"
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

te::vp::BasicGeoOpQuery::BasicGeoOpQuery()
{}

te::vp::BasicGeoOpQuery::~BasicGeoOpQuery()
{}

bool te::vp::BasicGeoOpQuery::run()
{
  std::auto_ptr<te::da::DataSetType> outDSType(GetDataSetType());
  std::auto_ptr<te::mem::DataSet> outDSet(new te::mem::DataSet(outDSType.get()));

  te::gm::GeometryProperty* propGeom = static_cast<te::gm::GeometryProperty*>(m_inDsetType->findFirstPropertyOfType(te::dt::GEOMETRY_TYPE));
  std::string propGeomName = propGeom->getName();
  std::size_t propGeomPos = m_inDsetType->getPropertyPosition(propGeomName);

  // get the geometric operation and/or tabular operation 
  int opGeom = -1;
  std::vector<int> opTab;

  for(std::size_t i = 0; i < m_operations.size(); ++i)
  {
    switch(m_operations[i])
    {
      case te::vp::CENTROID:
        opGeom = te::vp::CENTROID;
        break;
      case te::vp::CONVEX_HULL:
        opGeom = te::vp::CONVEX_HULL;
        break;
      case te::vp::MBR:
        opGeom = te::vp::MBR;
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

  te::da::Fields* fields = new te::da::Fields;
  
  te::da::FromItem* fromItem = new te::da::DataSetName(m_inDsetType->getName());
  te::da::From* from = new te::da::From;
  from->push_back(fromItem);

  if(opGeom == te::vp::MBR)
  {
    
  }
  if(opGeom == te::vp::CONVEX_HULL)
  {

  }
  if(opGeom == te::vp::CENTROID)
  {
    for(std::size_t i = 0; i < m_selectedProps.size(); ++i)
    {
      te::da::Field* f_selProp = new te::da::Field(m_selectedProps[i]);
      fields->push_back(f_selProp);
    }
  }
  else
  {
    for(std::size_t i = 0; i < m_selectedProps.size(); ++i)
    {
      te::da::Field* f_selProp = new te::da::Field(m_selectedProps[i]);
      fields->push_back(f_selProp);
    }

    for(std::size_t i = 0; i < opTab.size(); ++i)
    {
      switch (opTab[i])
      {
      case te::vp::AREA:
        break;
      case te::vp::LINE:
        break;
      case te::vp::PERIMETER:
        break;
      }
    }

    te::da::Field* f_geom = new te::da::Field(propGeomName, " geom");
    fields->push_back(f_geom);

    te::da::Select select(fields, from);
    std::auto_ptr<te::da::DataSet> dsQuery = m_inDsrc->query(select);

    if (dsQuery->isEmpty())
      return false;

    SetOutputDSet(dsQuery, outDSet.get());

    return save(outDSet, outDSType);
  }

  return true;
}


void te::vp::BasicGeoOpQuery::SetOutputDSet(std::auto_ptr<te::da::DataSet> inDataSet,
                                            te::mem::DataSet* outDataSet)
{
  std::size_t numProps = inDataSet->getNumProperties();
  
  inDataSet->moveBeforeFirst();
  while(inDataSet->moveNext())
  {
    te::mem::DataSetItem* dItem = new te::mem::DataSetItem(outDataSet);
    for(std::size_t i = 0; i < numProps; ++i)
    {
      int type = inDataSet->getPropertyDataType(i);
      switch(type)
      {
        case te::dt::INT16_TYPE:
          dItem->setInt16(i, inDataSet->getInt16(i));
          break;
        case te::dt::INT32_TYPE:
          dItem->setInt32(i, inDataSet->getInt32(i));
          break;
        case te::dt::INT64_TYPE:
          dItem->setInt64(i, inDataSet->getInt64(i));
          break;
        case te::dt::DOUBLE_TYPE:
          dItem->setDouble(i, inDataSet->getDouble(i));
          break;
        case te::dt::STRING_TYPE:
          dItem->setString(i, inDataSet->getString(i));
          break;
        case te::dt::GEOMETRY_TYPE:
          {
            te::gm::Geometry* geom = inDataSet->getGeometry(i).release();
            if(geom->isValid())
              dItem->setGeometry(i, geom);
          }
          break;
      }
    }
    outDataSet->add(dItem);
  }
}