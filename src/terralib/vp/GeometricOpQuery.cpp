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
#include "../dataaccess/query/Literal.h"
#include "../dataaccess/query/LiteralBool.h"
#include "../dataaccess/query/PropertyName.h"
#include "../dataaccess/query/Select.h"
#include "../dataaccess/query/ST_Area.h"
#include "../dataaccess/query/ST_Centroid.h"
#include "../dataaccess/query/ST_ConvexHull.h"
#include "../dataaccess/query/ST_Envelope.h"
#include "../dataaccess/query/ST_Length.h"
#include "../dataaccess/query/ST_Perimeter.h"
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
  std::auto_ptr<te::da::DataSet> inDset = m_inDsrc->getDataSet(m_inDsetName);
  std::size_t geom_pos = te::da::GetFirstSpatialPropertyPos(inDset.get());

  te::da::Fields* fields = new te::da::Fields;

  if(m_selectedProps.size() > 0)
  {
    for(std::size_t prop_pos = 0; prop_pos < m_selectedProps.size(); ++prop_pos)
    {
      te::da::Field* f_prop = new te::da::Field(m_selectedProps[prop_pos]);
      fields->push_back(f_prop);
    }
  }

  std::string name = inDset->getPropertyName(geom_pos);

  if(tabVec.size() > 0)
  {
    for(std::size_t tabPos = 0; tabPos < tabVec.size(); ++tabPos)
    {
      te::gm::GeometryProperty* geom = te::da::GetFirstGeomProperty(m_inDsetType.get());

      te::da::Expression* ex;
      te::da::Field* f;

      switch(tabVec[tabPos])
      {
        case te::vp::AREA:
          {
            ex = new te::da::ST_Area(te::da::PropertyName(name));
            f = new te::da::Field(*ex, "area");
            fields->push_back(f);
          }
          break;
        case te::vp::LINE:
          {
            ex = new te::da::ST_Length(te::da::PropertyName(name));
            f = new te::da::Field(*ex, "line_length");
            fields->push_back(f);
          }
          break;
        case te::vp::PERIMETER:
          {
            ex = new te::da::ST_Perimeter(te::da::PropertyName(name));
            f = new te::da::Field(*ex, "perimeter");
            fields->push_back(f);
          }
          break;
      }
    }
  }

  if(geoVec.size() > 0)
  {
    for(std::size_t geoPos = 0; geoPos < geoVec.size(); ++geoPos)
    {
      te::da::Expression* ex_geom;
      te::da::Field* f_geom;

      switch(geoVec[geoPos])
      {
        case te::vp::CONVEX_HULL:
          {
            std::size_t pos = dsType->getPropertyPosition("convex_hull");
            if( pos < outDSet->getNumProperties() && pos > 0)
            {
              ex_geom = new te::da::ST_ConvexHull(te::da::PropertyName(name));
              f_geom = new te::da::Field(*ex_geom, "convex_hull");
              fields->push_back(f_geom);
            }
          }
          break;
        case te::vp::CENTROID:
          {
            std::size_t pos = dsType->getPropertyPosition("centroid");
            if( pos < outDSet->getNumProperties() && pos > 0)
            {
              ex_geom = new te::da::ST_Centroid(te::da::PropertyName(name));
              f_geom = new te::da::Field(*ex_geom, "centroid");
              fields->push_back(f_geom);
            }
          }
          break;
        case te::vp::MBR:
          {
            std::size_t pos = dsType->getPropertyPosition("mbr");
            if( pos < outDSet->getNumProperties() && pos > 0)
            {
              ex_geom = new te::da::ST_Envelope(te::da::PropertyName(name));
              f_geom = new te::da::Field(*ex_geom, "mbr");
              fields->push_back(f_geom);
            }
          }
          break;
      }
    }
  }
  else
  {
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
  
  std::auto_ptr<te::da::DataSet> inDset = m_inDsrc->getDataSet(m_inDsetName);
  std::size_t geom_pos = te::da::GetFirstSpatialPropertyPos(inDset.get());
  std::string name = inDset->getPropertyName(geom_pos);
  
  te::da::Fields* fields = new te::da::Fields;

  if(tabVec.size() > 0)
  {
    for(std::size_t tabPos = 0; tabPos < tabVec.size(); ++tabPos)
    {
      te::gm::GeometryProperty* geom = te::da::GetFirstGeomProperty(m_inDsetType.get());

      te::da::Expression* ex;
      te::da::Field* f;

      switch(tabVec[tabPos])
      {
        case te::vp::AREA:
          {
            te::da::Expression* e_union = new te::da::ST_Union(te::da::PropertyName(name));
            ex = new te::da::ST_Area(*e_union);
            f = new te::da::Field(*ex, "area");
            fields->push_back(f);
          }
          break;
        case te::vp::LINE:
          {
            te::da::Expression* e_union = new te::da::ST_Union(te::da::PropertyName(name));
            ex = new te::da::ST_Length(*e_union);
            f = new te::da::Field(*ex, "line_length");
            fields->push_back(f);
          }
          break;
        case te::vp::PERIMETER:
          {
            te::da::Expression* e_union = new te::da::ST_Union(te::da::PropertyName(name));
            ex = new te::da::ST_Perimeter(*e_union);
            f = new te::da::Field(*ex, "perimeter");
            fields->push_back(f);
          }
          break;
      }
    }
  }

  if(geoVec.size() > 0)
  {
    for(std::size_t geoPos = 0; geoPos < geoVec.size(); ++geoPos)
    {
      te::da::Expression* ex_geom;
      te::da::Field* f_geom;

      switch(geoVec[geoPos])
      {
        case te::vp::CONVEX_HULL:
          {
            std::size_t pos = dsType->getPropertyPosition("convex_hull");
            if( pos < outDSet->getNumProperties() && pos > 0)
            {
              te::da::Expression* e_union = new te::da::ST_Union(te::da::PropertyName(name));
              ex_geom = new te::da::ST_ConvexHull(*e_union);
              f_geom = new te::da::Field(*ex_geom, "convex_hull");
              fields->push_back(f_geom);
            }
          }
          break;
        case te::vp::CENTROID:
          {
            std::size_t pos = dsType->getPropertyPosition("centroid");
            if( pos < outDSet->getNumProperties() && pos > 0)
            {
              te::da::Expression* e_union = new te::da::ST_Union(te::da::PropertyName(name));
              ex_geom = new te::da::ST_Centroid(*e_union);
              f_geom = new te::da::Field(*ex_geom, "centroid");
              fields->push_back(f_geom);
            }
          }
          break;
        case te::vp::MBR:
          {
            std::size_t pos = dsType->getPropertyPosition("mbr");
            if( pos < outDSet->getNumProperties() && pos > 0)
            {
              te::da::Expression* e_union = new te::da::ST_Union(te::da::PropertyName(name));
              ex_geom = new te::da::ST_Envelope(*e_union);
              f_geom = new te::da::Field(*ex_geom, "mbr");
              fields->push_back(f_geom);
            }
          }
          break;
      }
    }
  }
  else
  {
    te::da::Expression* e_union = new te::da::ST_Union(te::da::PropertyName(name));
    te::da::Field* f_prop = new te::da::Field(*e_union, name);
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

te::mem::DataSet* te::vp::GeometricOpQuery::SetAggregByAttribute( te::da::DataSetType* dsType,
                                                                  std::vector<int> tabVec,
                                                                  std::vector<int> geoVec)
{
  std::auto_ptr<te::mem::DataSet> outDSet(new te::mem::DataSet(dsType));
  std::auto_ptr<te::da::DataSet> inDset = m_inDsrc->getDataSet(m_inDsetName);
  std::size_t geom_pos = te::da::GetFirstSpatialPropertyPos(inDset.get());
  std::string name = inDset->getPropertyName(geom_pos);
  
  te::da::Fields* fields = new te::da::Fields;

  if(tabVec.size() > 0)
  {
    for(std::size_t tabPos = 0; tabPos < tabVec.size(); ++tabPos)
    {
      te::gm::GeometryProperty* geom = te::da::GetFirstGeomProperty(m_inDsetType.get());

      te::da::Expression* ex;
      te::da::Field* f;

      switch(tabVec[tabPos])
      {
        case te::vp::AREA:
          {
            te::da::Expression* e_union = new te::da::ST_Union(te::da::PropertyName(name));
            ex = new te::da::ST_Area(*e_union);
            f = new te::da::Field(*ex, "area");
            fields->push_back(f);
          }
          break;
        case te::vp::LINE:
          {
            te::da::Expression* e_union = new te::da::ST_Union(te::da::PropertyName(name));
            ex = new te::da::ST_Length(*e_union);
            f = new te::da::Field(*ex, "line_length");
            fields->push_back(f);
          }
          break;
        case te::vp::PERIMETER:
          {
            te::da::Expression* e_union = new te::da::ST_Union(te::da::PropertyName(name));
            ex = new te::da::ST_Perimeter(*e_union);
            f = new te::da::Field(*ex, "perimeter");
            fields->push_back(f);
          }
          break;
      }
    }
  }

  if(geoVec.size() > 0)
  {
    for(std::size_t geoPos = 0; geoPos < geoVec.size(); ++geoPos)
    {
      te::da::Expression* ex_geom;
      te::da::Field* f_geom;

      switch(geoVec[geoPos])
      {
        case te::vp::CONVEX_HULL:
          {
            std::size_t pos = dsType->getPropertyPosition("convex_hull");
            if( pos < outDSet->getNumProperties() && pos > 0)
            {
              te::da::Expression* e_union = new te::da::ST_Union(te::da::PropertyName(name));
              ex_geom = new te::da::ST_ConvexHull(*e_union);
              f_geom = new te::da::Field(*ex_geom, "convex_hull");
              fields->push_back(f_geom);
            }
          }
          break;
        case te::vp::CENTROID:
          {
            std::size_t pos = dsType->getPropertyPosition("centroid");
            if( pos < outDSet->getNumProperties() && pos > 0)
            {
              te::da::Expression* e_union = new te::da::ST_Union(te::da::PropertyName(name));
              ex_geom = new te::da::ST_Centroid(*e_union);
              f_geom = new te::da::Field(*ex_geom, "centroid");
              fields->push_back(f_geom);
            }
          }
          break;
        case te::vp::MBR:
          {
            std::size_t pos = dsType->getPropertyPosition("mbr");
            if( pos < outDSet->getNumProperties() && pos > 0)
            {
              te::da::Expression* e_union = new te::da::ST_Union(te::da::PropertyName(name));
              ex_geom = new te::da::ST_Envelope(*e_union);
              f_geom = new te::da::Field(*ex_geom, "mbr");
              fields->push_back(f_geom);
            }
          }
          break;
      }
    }
  }
  else
  {
    te::da::Expression* e_union = new te::da::ST_Union(te::da::PropertyName(name));
    te::da::Field* f_prop = new te::da::Field(*e_union, name);
    fields->push_back(f_prop);
  }

  te::da::FromItem* fromItem = new te::da::DataSetName(m_inDsetType->getName());
  te::da::From* from = new te::da::From;
  from->push_back(fromItem);

  te::da::Select select(fields, from);

  te::da::GroupBy* groupBy = new te::da::GroupBy();
  te::da::GroupByItem* e_groupBy = new te::da::GroupByItem(m_attribute);
  groupBy->push_back(e_groupBy);
  
  select.setGroupBy(groupBy);

  std::auto_ptr<te::da::DataSet> dsQuery = m_inDsrc->query(select);

  if (dsQuery->isEmpty())
    return false;

  SetOutputDSet(dsQuery.get(), outDSet.get());

  return outDSet.release();
}

void te::vp::GeometricOpQuery::SetOutputDSet( te::da::DataSet* inDataSet,
                                              te::mem::DataSet* outDataSet)
{
  std::size_t numProps = inDataSet->getNumProperties();
  int pk = 0;
  
  bool geomFlag = true;

  inDataSet->moveBeforeFirst();
  while(inDataSet->moveNext())
  {
    geomFlag = true;

    te::mem::DataSetItem* dItem = new te::mem::DataSetItem(outDataSet);
    std::size_t size = dItem->getNumProperties();

    dItem->setInt32(0, pk);

    for(std::size_t i = 0; i < numProps; ++i)
    {
      int type = inDataSet->getPropertyDataType(i);
      switch(type)
      {
        case te::dt::INT16_TYPE:
          dItem->setInt16(i+1, inDataSet->getInt16(i));
          break;
        case te::dt::INT32_TYPE:
          dItem->setInt32(i+1, inDataSet->getInt32(i));
          break;
        case te::dt::INT64_TYPE:
          dItem->setInt64(i+1, inDataSet->getInt64(i));
          break;
        case te::dt::DOUBLE_TYPE:
          dItem->setDouble(i+1, inDataSet->getDouble(i));
          break;
        case te::dt::STRING_TYPE:
          dItem->setString(i+1, inDataSet->getString(i));
          break;
        case te::dt::GEOMETRY_TYPE:
          {
            std::string inPropName = inDataSet->getPropertyName(i);

            if(inPropName == "convex_hull")
            {
              te::gm::Geometry* geom = inDataSet->getGeometry(i).release();
              if(geom->isValid())
              {
                if(geom->getGeomTypeId() == te::gm::PolygonType)
                  dItem->setGeometry("convex_hull", geom);
                else
                  geomFlag = false;
              }
            }
            else if(inPropName == "centroid")
            {
              te::gm::Geometry* geom = inDataSet->getGeometry(i).release();
              if(geom->isValid())
                dItem->setGeometry("centroid", geom);
            }
            else if(inPropName == "mbr")
            {
              te::gm::Geometry* geom = inDataSet->getGeometry(i).release();
              if(geom->isValid())
              {
                if(geom->getGeomTypeId() == te::gm::PolygonType)
                  dItem->setGeometry("mbr", geom);
                else
                  geomFlag = false;
              }
            }
            else
            {
              te::gm::Geometry* geom = inDataSet->getGeometry(i).release();

              switch(geom->getGeomTypeId())
              {
                case te::gm::PointType:
                  {
                    te::gm::GeometryCollection* teGeomColl = new te::gm::GeometryCollection(1, te::gm::MultiPointType, geom->getSRID());
                    if(geom->isValid())
                    {
                      teGeomColl->setGeometryN(0, geom);
                      dItem->setGeometry(i+1, teGeomColl);
                    }
                  }
                  break;
                case te::gm::LineStringType:
                  {
                    te::gm::GeometryCollection* teGeomColl = new te::gm::GeometryCollection(1, te::gm::MultiLineStringType, geom->getSRID());
                    if(geom->isValid())
                    {
                      teGeomColl->setGeometryN(0, geom);
                      dItem->setGeometry(i+1, teGeomColl);
                    }
                  }
                  break;
                case te::gm::PolygonType:
                  {
                    te::gm::GeometryCollection* teGeomColl = new te::gm::GeometryCollection(1, te::gm::MultiPolygonType, geom->getSRID());
                    if(geom->isValid())
                    {
                      teGeomColl->setGeometryN(0, geom);
                      dItem->setGeometry(i+1, teGeomColl);
                    }
                  }
                  break;
                default:
                  {
                    dItem->setGeometry(i+1, geom);
                  }
                  break;
              }
            }
          }
          break;
      }
    }
    if(geomFlag)
    {
      ++pk;
      outDataSet->add(dItem);
    }
  }
}