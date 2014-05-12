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

#include "GeometricOpQuery.h"
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

te::vp::GeometricOpQuery::GeometricOpQuery()
{}

te::vp::GeometricOpQuery::~GeometricOpQuery()
{}

bool te::vp::GeometricOpQuery::run()
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
    bool result = false;

    switch(m_objStrategy)
    {
      case te::vp::ALL_OBJ:
        {
          if(hasMultiGeomColumns) //Condição se o DataSource suporta mais de uma soluna geometrica...
          {
            dsTypeVec.push_back(te::vp::GeometricOp::GetDataSetType(te::vp::ALL_OBJ, true));
          }
          else
          {
            if(opGeom.size() > 0)
            {
              for(std::size_t i = 0; i < opGeom.size(); ++i)
                dsTypeVec.push_back(te::vp::GeometricOp::GetDataSetType(te::vp::ALL_OBJ, false, opGeom[i]));
            }
            else
            {
              dsTypeVec.push_back(te::vp::GeometricOp::GetDataSetType(te::vp::ALL_OBJ, false));
            }
          }

          for(std::size_t dsTypePos = 0; dsTypePos < dsTypeVec.size(); ++dsTypePos)
          {
            std::auto_ptr<te::da::DataSetType> outDataSetType(dsTypeVec[dsTypePos]);
            std::auto_ptr<te::mem::DataSet> outDataSet(SetAllObjects(dsTypeVec[dsTypePos], opTab, opGeom));
            result = save(outDataSet, outDataSetType);
            if(!result)
              return result;
          }
        }
        break;
      case te::vp::AGGREG_OBJ:
        {
          if(hasMultiGeomColumns) //Condição se o DataSource suporta mais de uma coluna geometrica...
          {
            dsTypeVec.push_back(te::vp::GeometricOp::GetDataSetType(te::vp::AGGREG_OBJ, true));
          }
          else
          {
            if(opGeom.size() > 0)
            {
              for(std::size_t i = 0; i < opGeom.size(); ++i)
                dsTypeVec.push_back(te::vp::GeometricOp::GetDataSetType(te::vp::AGGREG_OBJ, false, opGeom[i]));
            }
            else
            {
              dsTypeVec.push_back(te::vp::GeometricOp::GetDataSetType(te::vp::AGGREG_OBJ, false));
            }
          }

          for(std::size_t dsTypePos = 0; dsTypePos < dsTypeVec.size(); ++dsTypePos)
          {
            std::auto_ptr<te::da::DataSetType> outDataSetType(dsTypeVec[dsTypePos]);
            std::auto_ptr<te::mem::DataSet> outDataSet(SetAggregObj(dsTypeVec[dsTypePos], opTab, opGeom));
            result = save(outDataSet, outDataSetType);
            if(!result)
              return result;
          }
        }
        break;
      case te::vp::AGGREG_BY_ATTRIBUTE:
        {
          if(hasMultiGeomColumns) //Condição se o DataSource suporta mais de uma coluna geometrica...
          {
            dsTypeVec.push_back(te::vp::GeometricOp::GetDataSetType(te::vp::AGGREG_BY_ATTRIBUTE, true));
          }
          else
          {
            if(opGeom.size() > 0)
            {
              for(std::size_t i = 0; i < opGeom.size(); ++i)
                dsTypeVec.push_back(te::vp::GeometricOp::GetDataSetType(te::vp::AGGREG_BY_ATTRIBUTE, false, opGeom[i]));
            }
            else
            {
              dsTypeVec.push_back(te::vp::GeometricOp::GetDataSetType(te::vp::AGGREG_BY_ATTRIBUTE, false));
            }
          }

          for(std::size_t dsTypePos = 0; dsTypePos < dsTypeVec.size(); ++dsTypePos)
          {
            std::auto_ptr<te::da::DataSetType> outDataSetType(dsTypeVec[dsTypePos]);
            std::auto_ptr<te::mem::DataSet> outDataSet(SetAggregByAttribute(dsTypeVec[dsTypePos], opTab, opGeom));
            result = save(outDataSet, outDataSetType);
            if(!result)
              return result;
          }
        }
        break;
    }

    return result;

  }
  else
  {
    //Descobrir se o DataSource suporta adição de mais de uma coluna geometrica.
    return false;
  }

  return false;
}

te::mem::DataSet* te::vp::GeometricOpQuery::SetAllObjects(te::da::DataSetType* dsType,
                                                          std::vector<int> tabVec,
                                                          std::vector<int> geoVec)
{
  std::auto_ptr<te::mem::DataSet> outDSet(new te::mem::DataSet(dsType));
  std::size_t geom_pos = te::da::GetFirstSpatialPropertyPos(m_inDset.get());

  te::da::Fields* fields = new te::da::Fields;

  if(m_selectedProps.size() > 0)
  {
    for(std::size_t prop_pos = 0; prop_pos < m_selectedProps.size(); ++prop_pos)
    {
      te::da::Field* f_prop = new te::da::Field(m_selectedProps[prop_pos]);
      fields->push_back(f_prop);
    }
  }

  if(tabVec.size() > 0)
  {
    for(std::size_t tabPos = 0; tabPos < tabVec.size(); ++tabPos)
    {
      switch(tabVec[tabPos])
      {
        case te::vp::AREA:
          {
            
          }
          break;
        case te::vp::LINE:
          {
            
          }
          break;
        case te::vp::PERIMETER:
          {
            
          }
          break;
      }
    }
  }

  if(geoVec.size() > 0)
  {
    for(std::size_t geoPos = 0; geoPos < geoVec.size(); ++geoPos)
    {
      switch(geoVec[geoPos])
      {
        case te::vp::CONVEX_HULL:
          {

          }
          break;
        case te::vp::CENTROID:
          {

          }
          break;
        case te::vp::MBR:
          {

          }
          break;
      }
    }
  }
  else
  {
    std::string name = m_inDset->getPropertyName(geom_pos);
    te::da::Field* f_prop = new te::da::Field(name);
    fields->push_back(f_prop);
  }

  te::da::FromItem* fromItem = new te::da::DataSetName(m_inDsetType->getName());
  te::da::From* from = new te::da::From;
  from->push_back(fromItem);

  te::da::Select select(fields, from);
    
  std::auto_ptr<te::da::DataSet> dsQuery = m_inDsrc->query(select);

  if (dsQuery->isEmpty())
    return false;

  SetOutputDSet(dsQuery.get(), outDSet.get());

  return outDSet.release();
}

te::mem::DataSet* te::vp::GeometricOpQuery::SetAggregObj( te::da::DataSetType* dsType,
                                                          std::vector<int> tabVec,
                                                          std::vector<int> geoVec)
{
  std::auto_ptr<te::mem::DataSet> outDSet(new te::mem::DataSet(dsType));



  return outDSet.release();
}

te::mem::DataSet* te::vp::GeometricOpQuery::SetAggregByAttribute( te::da::DataSetType* dsType,
                                                                  std::vector<int> tabVec,
                                                                  std::vector<int> geoVec)
{
  std::auto_ptr<te::mem::DataSet> outDSet(new te::mem::DataSet(dsType));



  return outDSet.release();
}

void te::vp::GeometricOpQuery::SetOutputDSet( te::da::DataSet* inDataSet,
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