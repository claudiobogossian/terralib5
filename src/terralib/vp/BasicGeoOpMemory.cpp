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

#include "BasicGeoOpMemory.h"
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

te::vp::BasicGeoOpMemory::BasicGeoOpMemory()
{}

te::vp::BasicGeoOpMemory::~BasicGeoOpMemory()
{}


bool te::vp::BasicGeoOpMemory::run()
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

  if(opGeom == te::vp::MBR)
  {
    te::gm::Geometry* resultGeom(0);
    std::auto_ptr<te::gm::Envelope> env = m_inDset->getExtent(propGeomPos);
    te::mem::DataSetItem* item = new te::mem::DataSetItem(outDSet.get());
    item->setInt32(0, 0);

    if(opTab.size() > 0)
    {
      te::gm::GeometryCollection* teGeomColl = new te::gm::GeometryCollection(0, te::gm::GeometryCollectionType, propGeom->getSRID());

// move first to take a seed geom.
      m_inDset->moveFirst();
      std::auto_ptr<te::gm::Geometry> seedGeom = m_inDset->getGeometry(propGeomName);

      if(m_inDset->size() > 1)
      {
        while(m_inDset->moveNext())
        {
          std::auto_ptr<te::gm::Geometry> c_geom = m_inDset->getGeometry(propGeomName);
          if(c_geom->isValid())
            teGeomColl->add(c_geom.release());
        }
        resultGeom = seedGeom->Union(teGeomColl);
      }
      else
      {
        resultGeom = seedGeom.release();
      }

      te::gm::GeomType geomType = resultGeom->getGeomTypeId();

      for(int i = 0; i < opTab.size(); ++i)
      {
        switch(opTab[i])
        {
          case te::vp::AREA:
            {
              double area = 0;
              area = CalculateTabularOp(opTab[i], resultGeom);
              item->setDouble("area", area);
            }
            break;
          case te::vp::LINE:
            {
              double line = 0;
              line = CalculateTabularOp(opTab[i], resultGeom);
              item->setDouble("line_length", line);
            }
            break;
          case te::vp::PERIMETER:
            {
              double perimeter = 0;
              perimeter = CalculateTabularOp(opTab[i], resultGeom);
              item->setDouble("perimeter", perimeter);
            }
            break;
        }
      }
    }

    te::gm::Geometry* geom = te::gm::GetGeomFromEnvelope(env.get(), propGeom->getSRID());
    item->setGeometry("geom", geom);
    outDSet->add(item);

    delete resultGeom;
  }

  if(opGeom == te::vp::CONVEX_HULL)
  {
    te::gm::Geometry* resultGeom(0);
    te::mem::DataSetItem* item = new te::mem::DataSetItem(outDSet.get());
    item->setInt32(0, 0);

    if(opTab.size() > 0)
    {
      te::gm::GeometryCollection* teGeomColl = new te::gm::GeometryCollection(0, te::gm::GeometryCollectionType, propGeom->getSRID());

// move first to take a seed geom.
      m_inDset->moveFirst();
      std::auto_ptr<te::gm::Geometry> seedGeom = m_inDset->getGeometry(propGeomName);

      if(m_inDset->size() > 1)
      {
        while(m_inDset->moveNext())
        {
          std::auto_ptr<te::gm::Geometry> c_geom = m_inDset->getGeometry(propGeomName);
          if(c_geom->isValid())
            teGeomColl->add(c_geom.release());
        }
        resultGeom = seedGeom->Union(teGeomColl);
      }
      else
      {
        resultGeom = seedGeom.release();
      }

      te::gm::GeomType geomType = resultGeom->getGeomTypeId();

      for(int i = 0; i < opTab.size(); ++i)
      {
        switch(opTab[i])
        {
          case te::vp::AREA:
            {
              double area = 0;
              area = CalculateTabularOp(opTab[i], resultGeom);
              item->setDouble("area", area);
            }
            break;
          case te::vp::LINE:
            {
              double line = 0;
              line = CalculateTabularOp(opTab[i], resultGeom);
              item->setDouble("line_length", line);
            }
            break;
          case te::vp::PERIMETER:
            {
              double perimeter = 0;
              perimeter = CalculateTabularOp(opTab[i], resultGeom);
              item->setDouble("perimeter", perimeter);
            }
            break;
        }
      }
    }

    te::gm::Geometry* geom = resultGeom->convexHull();
    item->setGeometry("geom", geom);
    outDSet->add(item);

    delete resultGeom;
  }

  if(opGeom == te::vp::CENTROID)
  {
    int id = 0;

    m_inDset->moveBeforeFirst();
    while(m_inDset->moveNext())
    {
      te::mem::DataSetItem* item = new te::mem::DataSetItem(outDSet.get());
      item->setInt32(0, id);

      for(std::size_t i = 0; i < m_selectedProps.size(); ++i)
      {
        std::size_t pos = m_inDsetType->getPropertyPosition(m_selectedProps[i]);
        int type = m_inDset->getPropertyDataType(pos);

        switch(type)
        {
          case te::dt::INT16_TYPE:
            item->setInt16(m_selectedProps[i], m_inDset->getInt16(m_selectedProps[i]));
            break;
          case te::dt::INT32_TYPE:
            item->setInt32(m_selectedProps[i], m_inDset->getInt32(m_selectedProps[i]));
            break;
          case te::dt::INT64_TYPE:
            item->setInt64(m_selectedProps[i], m_inDset->getInt64(m_selectedProps[i]));
            break;
          case te::dt::DOUBLE_TYPE:
            item->setDouble(m_selectedProps[i], m_inDset->getDouble(m_selectedProps[i]));
            break;
          case te::dt::STRING_TYPE:
            item->setString(m_selectedProps[i], m_inDset->getString(m_selectedProps[i]));
            break;
        }
      }

      std::auto_ptr<te::gm::Geometry> c_geom = m_inDset->getGeometry(propGeomName);
      if(c_geom->isValid())
      {
        for(int i = 0; i < opTab.size(); ++i)
        {
          switch(opTab[i])
          {
            case te::vp::AREA:
              {
                double area = 0;
                area = CalculateTabularOp(opTab[i], c_geom.get());
                item->setDouble("area", area);
              }
              break;
            case te::vp::LINE:
              {
                double line = 0;
                line = CalculateTabularOp(opTab[i], c_geom.get());
                item->setDouble("line_length", line);
              }
              break;
            case te::vp::PERIMETER:
              {
                double perimeter = 0;
                perimeter = CalculateTabularOp(opTab[i], c_geom.get());
                item->setDouble("perimeter", perimeter);
              }
              break;
          }
        }

        const te::gm::Envelope* env = c_geom->getMBR();
        te::gm::Coord2D center = env->getCenter();
        te::gm::Point* point = new te::gm::Point(center.x, center.y, propGeom->getSRID());
        item->setGeometry("geom", point);
      }

      outDSet->add(item);
      ++id;
    }
  }

  else
  {
    int id = 0;

    m_inDset->moveBeforeFirst();
    while(m_inDset->moveNext())
    {
      te::mem::DataSetItem* item = new te::mem::DataSetItem(outDSet.get());
      item->setInt32(0, id);

      for(std::size_t i = 0; i < m_selectedProps.size(); ++i)
      {
        std::size_t pos = m_inDsetType->getPropertyPosition(m_selectedProps[i]);
        int type = m_inDset->getPropertyDataType(pos);

        switch(type)
        {
          case te::dt::INT16_TYPE:
            item->setInt16(m_selectedProps[i], m_inDset->getInt16(m_selectedProps[i]));
            break;
          case te::dt::INT32_TYPE:
            item->setInt32(m_selectedProps[i], m_inDset->getInt32(m_selectedProps[i]));
            break;
          case te::dt::INT64_TYPE:
            item->setInt64(m_selectedProps[i], m_inDset->getInt64(m_selectedProps[i]));
            break;
          case te::dt::DOUBLE_TYPE:
            item->setDouble(m_selectedProps[i], m_inDset->getDouble(m_selectedProps[i]));
            break;
          case te::dt::STRING_TYPE:
            item->setString(m_selectedProps[i], m_inDset->getString(m_selectedProps[i]));
            break;
        }
      }

      std::auto_ptr<te::gm::Geometry> c_geom = m_inDset->getGeometry(propGeomName);
      if(c_geom->isValid())
      {
        for(int i = 0; i < opTab.size(); ++i)
        {
          switch(opTab[i])
          {
            case te::vp::AREA:
              {
                double area = 0;
                area = CalculateTabularOp(opTab[i], c_geom.get());
                item->setDouble("area", area);
              }
              break;
            case te::vp::LINE:
              {
                double line = 0;
                line = CalculateTabularOp(opTab[i], c_geom.get());
                item->setDouble("line_length", line);
              }
              break;
            case te::vp::PERIMETER:
              {
                double perimeter = 0;
                perimeter = CalculateTabularOp(opTab[i], c_geom.get());
                item->setDouble("perimeter", perimeter);
              }
              break;
          }
        }

        item->setGeometry("geom", c_geom.release());
      }

      outDSet->add(item);
      ++id;
    }
  }

// save the result
  return save(outDSet, outDSType);
}

double te::vp::BasicGeoOpMemory::CalculateTabularOp(int tabOperation, 
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
          te::gm::MultiPolygon* m_pol = dynamic_cast<te::gm::MultiPolygon*>(geom);
          if(m_pol)
            value = m_pol->getArea();
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
          te::gm::MultiPolygon* m_pol = dynamic_cast<te::gm::MultiPolygon*>(geom);
          if(m_pol)
            value = m_pol->getPerimeter();
        }
        return value;
      }
      break;
  }
}
