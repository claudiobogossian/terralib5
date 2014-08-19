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
      default:
        te::common::Logger::logDebug("vp", "Geometric Operation - The operation is not valid.");
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
          if(hasMultiGeomColumns) //Condição se o DataSource suporta mais de uma coluna geometrica...
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
            m_outDsetNameVec.push_back(dsTypeVec[dsTypePos]->getName());

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
            m_outDsetNameVec.push_back(dsTypeVec[dsTypePos]->getName());

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
            m_outDsetNameVec.push_back(dsTypeVec[dsTypePos]->getName());

            std::auto_ptr<te::da::DataSetType> outDataSetType(dsTypeVec[dsTypePos]);
            std::auto_ptr<te::mem::DataSet> outDataSet(SetAggregByAttribute(dsTypeVec[dsTypePos], opTab, opGeom));
            result = save(outDataSet, outDataSetType);

            if(!result)
              return result;
          }
        }
        break;
      default:
        te::common::Logger::logDebug("vp", "Geometric Operation - Strategy Not found!");
        return false;
    }

    return result;
  }
  else
  {
    //Descobrir se o DataSource suporta adição de mais de uma coluna geometrica.
    return false;
  }
}


te::mem::DataSet* te::vp::GeometricOpMemory::SetAllObjects( te::da::DataSetType* dsType,
                                                            std::vector<int> tabVec,
                                                            std::vector<int> geoVec)
{
  std::auto_ptr<te::mem::DataSet> outDSet(new te::mem::DataSet(dsType));
  
  int pk = 0;
  std::auto_ptr<te::da::DataSet> inDset = m_inDsrc->getDataSet(m_inDsetName);
  inDset->moveBeforeFirst();
  
  std::size_t geom_pos = te::da::GetFirstSpatialPropertyPos(inDset.get());

  while(inDset->moveNext())
  {
    te::mem::DataSetItem* item = new te::mem::DataSetItem(outDSet.get());
    
    item->setInt32(0, pk);
    bool geom = true;

    if(m_selectedProps.size() > 0)
    {
      for(std::size_t prop_pos = 0; prop_pos < m_selectedProps.size(); ++prop_pos)
      {
        item->setValue(m_selectedProps[prop_pos], inDset->getValue(m_selectedProps[prop_pos]).release());
      }
    }

    std::auto_ptr<te::gm::Geometry> in_geom = inDset->getGeometry(geom_pos);

    if(tabVec.size() > 0)
    {
      for(std::size_t tabPos = 0; tabPos < tabVec.size(); ++tabPos)
      {
        switch(tabVec[tabPos])
        {
          case te::vp::AREA:
            {
              double area = 0;
              area = CalculateTabularOp(tabVec[tabPos], in_geom.get());
              item->setDouble("area", area);
            }
            break;
          case te::vp::LINE:
            {
              double line = 0;
              line = CalculateTabularOp(tabVec[tabPos], in_geom.get());
              item->setDouble("line_length", line);
            }
            break;
          case te::vp::PERIMETER:
            {
              double perimeter = 0;
              perimeter = CalculateTabularOp(tabVec[tabPos], in_geom.get());
              item->setDouble("perimeter", perimeter);
            }
            break;
          default:
            te::common::Logger::logDebug("vp", "Geometric Operation - Could not insert the tabular value.");
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
              std::size_t pos = te::da::GetPropertyPos(dsType, "convex_hull");
              if(pos < dsType->size())
              {
                std::auto_ptr<te::gm::Geometry> convexHull(in_geom->convexHull());
                convexHull->setSRID(in_geom->getSRID());

                if(convexHull->getGeomTypeId() == te::gm::PolygonType)
                {
                  item->setGeometry("convex_hull", convexHull.release());
                  geom = true;
                }
                else
                  geom = false;
              }
            }
            break;
          case te::vp::CENTROID:
            {
              std::size_t pos = te::da::GetPropertyPos(dsType, "centroid");
              if(pos < dsType->size())
              {
                const te::gm::Envelope* env = in_geom->getMBR();
                te::gm::Coord2D center = env->getCenter();
                te::gm::Point* point = new te::gm::Point(center.x, center.y, in_geom->getSRID());
                item->setGeometry("centroid", point);
              }
            }
            break;
          case te::vp::MBR:
            {
              std::size_t pos = te::da::GetPropertyPos(dsType, "mbr");
              if(pos < dsType->size())
              {
                std::auto_ptr<te::gm::Geometry> mbr(in_geom->getEnvelope());
                mbr->setSRID(in_geom->getSRID());
                if(mbr->getGeomTypeId() == te::gm::PolygonType)
                  item->setGeometry("mbr", mbr.release());
                else
                  geom = false;
              }
            }
            break;
          default:
            te::common::Logger::logDebug("vp", "Geometric Operation - Could not insert the geometric value.");
        }
      }
    }
    else
    {
      std::auto_ptr<te::gm::Geometry> g(in_geom.release());
      te::gm::GeometryCollection* teGeomColl = new te::gm::GeometryCollection(0, te::gm::GeometryCollectionType, g->getSRID());

      switch(g->getGeomTypeId())
      {
        case te::gm::PointType:
          {
            if(g->isValid())
              teGeomColl->add(g.release());
          }
          break;
        case te::gm::LineStringType:
          {
            if(g->isValid())
              teGeomColl->add(g.release());
          }
          break;
        case te::gm::PolygonType:
          {
            if(g->isValid())
              teGeomColl->add(g.release());
          }
          break;
        default:
          te::common::Logger::logDebug("vp", "Geometric Operation - Could not insert the geometry in collection.");
      }
      if(teGeomColl->getNumGeometries() != 0)
        item->setGeometry("geom", teGeomColl);
      else
        item->setGeometry("geom", g.release());
    }

    if(geom)
    {
      outDSet->add(item);
      ++pk;
    }
  }

  return outDSet.release();
}


te::mem::DataSet* te::vp::GeometricOpMemory::SetAggregObj(te::da::DataSetType* dsType,
                                                          std::vector<int> tabVec,
                                                          std::vector<int> geoVec)
{
  std::auto_ptr<te::mem::DataSet> outDSet(new te::mem::DataSet(dsType));

  int pk = 0;
  std::auto_ptr<te::da::DataSet> inDset = m_inDsrc->getDataSet(m_inDsetName);
  std::size_t geom_pos = te::da::GetFirstSpatialPropertyPos(inDset.get());
  // move first to take a seed geom.
  inDset->moveFirst();
  te::mem::DataSetItem* item = new te::mem::DataSetItem(outDSet.get());
  
  item->setInt32(0, pk);

  std::auto_ptr<te::gm::Geometry> seedGeom = inDset->getGeometry(geom_pos);
  int srid = seedGeom->getSRID();
  te::gm::GeometryCollection* teGeomColl = new te::gm::GeometryCollection(0, te::gm::GeometryCollectionType, srid);

  if(inDset->size() > 1)
  {
    while(inDset->moveNext())
    {
      std::auto_ptr<te::gm::Geometry> c_geom = inDset->getGeometry(geom_pos);
      if(c_geom->isValid())
        teGeomColl->add(c_geom.release());
    }
  }

  if(teGeomColl->getNumGeometries() > 1)
  {
    seedGeom.reset(seedGeom->Union(teGeomColl));
    seedGeom->setSRID(srid);
  }

  if(tabVec.size() > 0)
  {
    for(std::size_t tabPos = 0; tabPos < tabVec.size(); ++tabPos)
    {
      switch(tabVec[tabPos])
      {
        case te::vp::AREA:
          {
            double area = 0;
            area = CalculateTabularOp(tabVec[tabPos], seedGeom.get());
            item->setDouble("area", area);
          }
          break;
        case te::vp::LINE:
          {
            double line = 0;
            line = CalculateTabularOp(tabVec[tabPos], seedGeom.get());
            item->setDouble("line_length", line);
          }
          break;
        case te::vp::PERIMETER:
          {
            double perimeter = 0;
            perimeter = CalculateTabularOp(tabVec[tabPos], seedGeom.get());
            item->setDouble("perimeter", perimeter);
          }
          break;
        default:
          te::common::Logger::logDebug("vp", "Geometric Operation - Could not insert the tabular value.");
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
            std::size_t pos = te::da::GetPropertyPos(dsType, "convex_hull");
            if(pos < dsType->size())
            {
              std::auto_ptr<te::gm::Geometry> convexHull(seedGeom->convexHull());
              convexHull->setSRID(seedGeom->getSRID());
              item->setGeometry("convex_hull", convexHull.release());
            }
          }
          break;
        case te::vp::CENTROID:
          {
            std::size_t pos = te::da::GetPropertyPos(dsType, "centroid");
            if(pos < dsType->size())
            {
              const te::gm::Envelope* env = seedGeom->getMBR();
              te::gm::Coord2D center = env->getCenter();
              te::gm::Point* point = new te::gm::Point(center.x, center.y, seedGeom->getSRID());
              item->setGeometry("centroid", point);
            }
          }
          break;
        case te::vp::MBR:
          {
            std::size_t pos = te::da::GetPropertyPos(dsType, "mbr");
            if(pos < dsType->size())
            {
              std::auto_ptr<te::gm::Geometry> mbr(seedGeom->getEnvelope());
              mbr->setSRID(seedGeom->getSRID());
              item->setGeometry("mbr", mbr.release());
            }
          }
          break;
        default:
          te::common::Logger::logDebug("vp", "Geometric Operation - Could not insert the geometry in collection.");
      }
    }
  }
  else
  {
    std::auto_ptr<te::gm::Geometry> g(seedGeom.release());
    te::gm::GeometryCollection* teGeomColl = new te::gm::GeometryCollection(0, te::gm::GeometryCollectionType, g->getSRID());

    switch(g->getGeomTypeId())
    {
      case te::gm::PointType:
        {
          if(g->isValid())
            teGeomColl->add(g.release());
        }
        break;
      case te::gm::LineStringType:
        {
          if(g->isValid())
            teGeomColl->add(g.release());
        }
        break;
      case te::gm::PolygonType:
        {
          if(g->isValid())
            teGeomColl->add(g.release());
        }
        break;
      default:
        te::common::Logger::logDebug("vp", "Geometric Operation - Could not insert the geometry in collection.");
    }
    if(teGeomColl->getNumGeometries() != 0)
      item->setGeometry("geom", teGeomColl);
    else
      item->setGeometry("geom", g.release());
  }

  outDSet->add(item);

  return outDSet.release();
}

te::mem::DataSet* te::vp::GeometricOpMemory::SetAggregByAttribute(te::da::DataSetType* dsType,
                                                                  std::vector<int> tabVec,
                                                                  std::vector<int> geoVec)
{
  std::map<std::string, te::gm::Geometry*> geometries;

  std::auto_ptr<te::da::DataSet> inDset = m_inDsrc->getDataSet(m_inDsetName);

  te::gm::GeometryProperty* propGeom = static_cast<te::gm::GeometryProperty*>(m_inDsetType->findFirstPropertyOfType(te::dt::GEOMETRY_TYPE));
  std::size_t geom_pos = te::da::GetFirstSpatialPropertyPos(inDset.get());

  // move first to take a seed geom.
  inDset->moveFirst();
  std::auto_ptr<te::gm::Geometry> seedGeom = inDset->getGeometry(geom_pos);
  int srid = seedGeom->getSRID();

  if(inDset->size() > 1)
  {
    std::map<std::string, std::vector<te::mem::DataSetItem*> > groups;
    std::map<std::string, std::vector<te::mem::DataSetItem*> >::iterator itg;
    size_t nprops = inDset->getNumProperties();

    // move first to take a seed geom.
    inDset->moveBeforeFirst();
    while(inDset->moveNext())
    {
      std::string key = inDset->getAsString(m_attribute);
      te::mem::DataSetItem* item = new te::mem::DataSetItem(inDset.get());
      for(std::size_t j=0; j<nprops; ++j)
      {
        if (!inDset->isNull(j))
        {
          std::auto_ptr<te::dt::AbstractData> val = inDset->getValue(j);
          item->setValue(j,val.release());
        }
      }
      
      itg = groups.find(key);
      if (itg==groups.end())
      {
        std::vector<te::mem::DataSetItem*> dataSetItemVector;
        dataSetItemVector.push_back(item);
        groups.insert(std::pair<std::string, std::vector<te::mem::DataSetItem*> >(key,dataSetItemVector));
      }
      else
        itg->second.push_back(item);
    }

    itg = groups.begin();
    while(itg != groups.end())
    {
      // calculate the spatial aggregation
      te::gm::Geometry* geometry = te::vp::GetGeometryUnion(itg->second, geom_pos, propGeom->getGeometryType());

      if(geometry->isValid())
        geometries.insert(std::pair<std::string, te::gm::Geometry*>(itg->first,geometry));

      ++itg;
    }

  }
  else
  {
    geometries.insert(std::pair<std::string, te::gm::Geometry*>(inDset->getAsString(m_attribute), seedGeom.release()));
  }

// insert result in dataset.
  std::auto_ptr<te::mem::DataSet> outDSet(new te::mem::DataSet(dsType));
  std::map<std::string, te::gm::Geometry*>::iterator itGeom;
  int i = 0;

  itGeom = geometries.begin();
  while(itGeom != geometries.end())
  {
    te::mem::DataSetItem* outItem = new te::mem::DataSetItem(outDSet.get());
    
    // inserting the id in dataSet.
    outItem->setInt32(0, i);
    // inserting aggregated attribute.
    te::dt::Property* prop = m_inDsetType->getProperty(m_attribute);
    switch(prop->getType())
    {
      case te::dt::STRING_TYPE:
          outItem->setString(1, itGeom->first);
          break;
      case te::dt::INT16_TYPE:
          outItem->setInt16(1, boost::lexical_cast<int>(itGeom->first));
          break;
      case te::dt::INT32_TYPE:
          outItem->setInt32(1, boost::lexical_cast<int>(itGeom->first));
          break;
      case te::dt::INT64_TYPE:
          outItem->setInt64(1, boost::lexical_cast<int>(itGeom->first));
          break;
      case te::dt::DOUBLE_TYPE:
          outItem->setDouble(1, boost::lexical_cast<double>(itGeom->first));
          break;
      default:
        te::common::Logger::logDebug("vp", "Geometric Operation - Could not insert the aggregated value.");
    }
    
    // inserting geometries.
    if(tabVec.size() > 0)
    {
      for(std::size_t tabPos = 0; tabPos < tabVec.size(); ++tabPos)
      {
        switch(tabVec[tabPos])
        {
          case te::vp::AREA:
            {
              double area = 0;
              area = CalculateTabularOp(tabVec[tabPos], itGeom->second);
              outItem->setDouble("area", area);
            }
            break;
          case te::vp::LINE:
            {
              double line = 0;
              line = CalculateTabularOp(tabVec[tabPos], itGeom->second);
              outItem->setDouble("line_length", line);
            }
            break;
          case te::vp::PERIMETER:
            {
              double perimeter = 0;
              perimeter = CalculateTabularOp(tabVec[tabPos], itGeom->second);
              outItem->setDouble("perimeter", perimeter);
            }
            break;
          default:
            te::common::Logger::logDebug("vp", "Geometric Operation - Could not insert the tabular value.");
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
              std::size_t pos = te::da::GetPropertyPos(dsType, "convex_hull");
              if(pos < dsType->size())
              {
                std::auto_ptr<te::gm::Geometry> convexHull(itGeom->second->convexHull());
                convexHull->setSRID(seedGeom->getSRID());
                outItem->setGeometry("convex_hull", convexHull.release());
              }
            }
            break;
          case te::vp::CENTROID:
            {
              std::size_t pos = te::da::GetPropertyPos(dsType, "centroid");
              if(pos < dsType->size())
              {
                const te::gm::Envelope* env = itGeom->second->getMBR();
                te::gm::Coord2D center = env->getCenter();
                te::gm::Point* point = new te::gm::Point(center.x, center.y, itGeom->second->getSRID());
                outItem->setGeometry("centroid", point);
              }
            }
            break;
          case te::vp::MBR:
            {
              std::size_t pos = te::da::GetPropertyPos(dsType, "mbr");
              if(pos < dsType->size())
              {
                std::auto_ptr<te::gm::Geometry> mbr(itGeom->second->getEnvelope());
                mbr->setSRID(seedGeom->getSRID());
                outItem->setGeometry("mbr", mbr.release());
              }
            }
            break;
          default:
            te::common::Logger::logDebug("vp", "Geometric Operation - Could not insert the geometric value.");
        }
      }
    }
    else
    {
      te::gm::GeometryCollection* teGeomColl = new te::gm::GeometryCollection(0, te::gm::GeometryCollectionType, itGeom->second->getSRID());

      switch(itGeom->second->getGeomTypeId())
      {
        case te::gm::PointType:
          {
            if(itGeom->second->isValid())
              teGeomColl->add(itGeom->second);
          }
          break;
        case te::gm::LineStringType:
          {
            if(itGeom->second->isValid())
              teGeomColl->add(itGeom->second);
          }
          break;
        case te::gm::PolygonType:
          {
            if(itGeom->second->isValid())
              teGeomColl->add(itGeom->second);
          }
          break;
        default:
          te::common::Logger::logDebug("vp", "Geometric Operation - Could not insert the geometry in collection.");
      }
      if(teGeomColl->getNumGeometries() != 0)
        outItem->setGeometry("geom", teGeomColl);
      else
        outItem->setGeometry("geom", itGeom->second);
    }

    outDSet->add(outItem);
    ++itGeom;
    ++i;
  }

  return outDSet.release();
}


double te::vp::GeometricOpMemory::CalculateTabularOp( int tabOperation, 
                                                      te::gm::Geometry* geom)
{
  double value = 0;
  te::gm::GeomType geomType = geom->getGeomTypeId();

  switch(tabOperation)
  {
    case te::vp::AREA:
      {
        if(geomType == te::gm::PolygonType)
        {
          te::gm::Polygon* pol = dynamic_cast<te::gm::Polygon*>(geom);
          if(pol)
            value = pol->getArea();
        }
        if(geomType == te::gm::MultiPolygonType)
        {
          std::string type = geom->getGeometryType();

          if(type == "GeometryCollection")
          {
            te::gm::GeometryCollection* m_geoColl = dynamic_cast<te::gm::GeometryCollection*>(geom);
            
            if(m_geoColl)
            {
              std::vector<te::gm::Geometry*> geomVec = m_geoColl->getGeometries();
              for(std::size_t i = 0; i < geomVec.size(); ++i)
              {
                te::gm::GeomType inType = geomVec[i]->getGeomTypeId();
                if(inType == te::gm::PolygonType)
                {
                  te::gm::Polygon* m_pol = dynamic_cast<te::gm::Polygon*>(geomVec[i]);
                  if(m_pol)
                    value += m_pol->getArea();
                }
                if(inType == te::gm::MultiPolygonType)
                {
                  te::gm::MultiPolygon* m_pol = dynamic_cast<te::gm::MultiPolygon*>(geomVec[i]);
                  if(m_pol)
                    value += m_pol->getArea();
                }
              }
            }
          }
          else
          {
            te::gm::MultiPolygon* m_pol = dynamic_cast<te::gm::MultiPolygon*>(geom);
            if(m_pol)
              value = m_pol->getArea();
          }
        }
        return value;
      }
      break;
    case te::vp::LINE:
      {
        if(geomType == te::gm::LineStringType)
        {
          te::gm::LineString* lineString = dynamic_cast<te::gm::LineString*>(geom);
          if(lineString)
            value = lineString->getLength();
        }
        if(geomType == te::gm::MultiLineStringType)
        {
          te::gm::MultiLineString* m_lineString = dynamic_cast<te::gm::MultiLineString*>(geom);
          if(m_lineString)
            value = m_lineString->getLength();
        }
        return value;
      }
      break;
    case te::vp::PERIMETER:
      {
        if(geomType == te::gm::PolygonType)
        {
          te::gm::Polygon* pol = dynamic_cast<te::gm::Polygon*>(geom);
          if(pol)
            value = pol->getPerimeter();
        }
        if(geomType == te::gm::MultiPolygonType)
        {
          std::string type = geom->getGeometryType();

          if(type == "GeometryCollection")
          {
            te::gm::GeometryCollection* m_geoColl = dynamic_cast<te::gm::GeometryCollection*>(geom);
            
            if(m_geoColl)
            {
              std::vector<te::gm::Geometry*> geomVec = m_geoColl->getGeometries();
              for(std::size_t i = 0; i < geomVec.size(); ++i)
              {
                te::gm::GeomType inType = geomVec[i]->getGeomTypeId();
                if(inType == te::gm::PolygonType)
                {
                  te::gm::Polygon* m_pol = dynamic_cast<te::gm::Polygon*>(geomVec[i]);
                  if(m_pol)
                    value += m_pol->getPerimeter();
                }
                if(inType == te::gm::MultiPolygonType)
                {
                  te::gm::MultiPolygon* m_pol = dynamic_cast<te::gm::MultiPolygon*>(geomVec[i]);
                  if(m_pol)
                    value += m_pol->getPerimeter();
                }
              }
            }
          }
          else
          {
            te::gm::MultiPolygon* m_pol = dynamic_cast<te::gm::MultiPolygon*>(geom);
            if(m_pol)
              value = m_pol->getPerimeter();
          }
        }
        return value;
      }
      break;
    default:
      te::common::Logger::logDebug("vp", "Geometric Operation - Could not calculate the operation.");
  }

  return value;
}