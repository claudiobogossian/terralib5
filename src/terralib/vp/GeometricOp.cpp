/*  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

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
 \file GeometricOp.cpp
 */

#include "../dataaccess/utils/Utils.h"
#include "../geometry/GeometryProperty.h"

#include "GeometricOp.h"

te::vp::GeometricOp::GeometricOp()
{
}

void te::vp::GeometricOp::setInput(
    te::da::DataSourcePtr inDsrc, std::string inDsetName,
    std::unique_ptr<te::da::DataSetTypeConverter> converter)
{
  m_inDsrc = inDsrc;
  m_inDsetName = inDsetName;
  m_converter = std::move(converter);
}

void te::vp::GeometricOp::setParams(
    std::vector<std::string> selectedProps,
    std::vector<te::vp::GeometricOperation> operations,
    te::vp::GeometricOpObjStrategy objStrategy, std::string attribute)
{
  m_selectedProps = selectedProps;
  m_operations = operations;
  m_objStrategy = objStrategy;
  m_attribute = attribute;
}

void te::vp::GeometricOp::setOutput(std::unique_ptr<da::DataSource> outDsrc,
                                    std::string dsname)
{
  m_outDsrc = std::move(outDsrc);
  m_outDsetName = dsname;
}

bool te::vp::GeometricOp::paramsAreValid()
{
  if (!m_converter.get())
    return false;
  
  if (!m_converter->getResult()->hasGeom())
    return false;

  if (m_outDsetName.empty() || !m_outDsrc.get())
    return false;
  
  return true;
}

std::vector<std::string> te::vp::GeometricOp::GetOutputDSetNames()
{
  return m_outDsetNameVec;
}

te::da::DataSetType* te::vp::GeometricOp::GetDataSetType(
    te::vp::GeometricOpObjStrategy geomOpStrategy, bool multiGeomColumns,
    int geomOp)
{
  te::da::DataSetType* dsType = 0;

  if(geomOp != -1)
  {
    switch(geomOp)
    {
      case 0:
        {
          dsType = new te::da::DataSetType(m_outDsetName + "_convex_hull");
          dsType->setTitle(m_outDsetName + "_convex_hull");

          // Primary key
          te::dt::SimpleProperty* pkProperty = new te::dt::SimpleProperty(
              m_outDsetName + "_id", te::dt::INT32_TYPE);
          pkProperty->setAutoNumber(true);
          dsType->add(pkProperty);

          te::da::PrimaryKey* pk =
              new te::da::PrimaryKey(m_outDsetName + "_convex_hull_pk", dsType);
          pk->add(pkProperty);
          dsType->setPrimaryKey(pk);

          break;
        }
      case 1:
        {
          dsType = new te::da::DataSetType(m_outDsetName + "_centroid");
          dsType->setTitle(m_outDsetName + "_centroid");

          // Primary key
          te::dt::SimpleProperty* pkProperty = new te::dt::SimpleProperty(
              m_outDsetName + "_id", te::dt::INT32_TYPE);
          pkProperty->setAutoNumber(true);
          dsType->add(pkProperty);

          te::da::PrimaryKey* pk =
              new te::da::PrimaryKey(m_outDsetName + "_centroid_pk", dsType);
          pk->add(pkProperty);
          dsType->setPrimaryKey(pk);

          break;
        }
      case 2:
        {
          dsType = new te::da::DataSetType(m_outDsetName + "_mbr");
          dsType->setTitle(m_outDsetName + "_mbr");

          // Primary key
          te::dt::SimpleProperty* pkProperty = new te::dt::SimpleProperty(
              m_outDsetName + "_id", te::dt::INT32_TYPE);
          pkProperty->setAutoNumber(true);
          dsType->add(pkProperty);

          te::da::PrimaryKey* pk =
              new te::da::PrimaryKey(m_outDsetName + "_mbr_pk", dsType);
          pk->add(pkProperty);
          dsType->setPrimaryKey(pk);

          break;
        }
    }
  }
  else
  {
    dsType = new te::da::DataSetType(m_outDsetName);
    dsType->setTitle(m_outDsetName);

    // Primary key
    te::dt::SimpleProperty* pkProperty =
        new te::dt::SimpleProperty(m_outDsetName + "_id", te::dt::INT32_TYPE);
    pkProperty->setAutoNumber(true);
    dsType->add(pkProperty);

    te::da::PrimaryKey* pk =
        new te::da::PrimaryKey(m_outDsetName + "_res_pk", dsType);
    pk->add(pkProperty);
    dsType->setPrimaryKey(pk);
  }


  if(geomOpStrategy == te::vp::ALL_OBJ)
  {
      for(std::size_t i = 0; i < m_selectedProps.size(); ++i)
      {
        te::dt::Property* prop =
            m_converter->getResult()->getProperty(m_selectedProps[i])->clone();
        prop->setParent(0);
        dsType->add(prop);
      }
  }

  if(geomOpStrategy == te::vp::AGGREG_BY_ATTRIBUTE)
  {
    te::dt::Property* prop =
        m_converter->getResult()->getProperty(m_attribute)->clone();
    prop->setParent(0);
    dsType->add(prop);
  }

  for(std::size_t i = 0; i < m_operations.size(); ++i)
  {
    int op = m_operations[i];
    switch(op)
    {
      case te::vp::AREA:
        {
          te::dt::SimpleProperty* area =
              new te::dt::SimpleProperty("area", te::dt::DOUBLE_TYPE);
          dsType->add(area);
        }
        break;
      case te::vp::LINE:
        {
          te::dt::SimpleProperty* line =
              new te::dt::SimpleProperty("line_length", te::dt::DOUBLE_TYPE);
          dsType->add(line);
        }
        break;
      case te::vp::PERIMETER:
        {
          te::dt::SimpleProperty* perimeter =
              new te::dt::SimpleProperty("perimeter", te::dt::DOUBLE_TYPE);
          dsType->add(perimeter);
        }
        break;
      default:
        break;
    }
  }
  
//  Geometry property.
  te::gm::GeometryProperty* gp =
      te::da::GetFirstGeomProperty(m_converter->getResult());

  if(multiGeomColumns)
  {
    bool flagGeom = false;
    for(std::size_t i = 0; i< m_operations.size(); ++i)
    {
      switch(m_operations[i])
      {
        case te::vp::CONVEX_HULL:
          {
            te::gm::GeometryProperty* convGeom =
                new te::gm::GeometryProperty("convex_hull");
            convGeom->setGeometryType(te::gm::PolygonType);
            convGeom->setSRID(gp->getSRID());
            dsType->add(convGeom);
            flagGeom = true;
          }
          break;
        case te::vp::CENTROID:
          {
            te::gm::GeometryProperty* centroidGeom =
                new te::gm::GeometryProperty("centroid");
            centroidGeom->setGeometryType(te::gm::PointType);
            centroidGeom->setSRID(gp->getSRID());
            dsType->add(centroidGeom);
            flagGeom = true;
          }
          break;
        case te::vp::MBR:
          {
            te::gm::GeometryProperty* mbrGeom =
                new te::gm::GeometryProperty("mbr");
            mbrGeom->setGeometryType(te::gm::PolygonType);
            mbrGeom->setSRID(gp->getSRID());
            dsType->add(mbrGeom);
            flagGeom = true;
          }
          break;
        default:
          break;
      }
    }

    if(!flagGeom)
    {
      te::gm::GeometryProperty* geometry = new te::gm::GeometryProperty("geom");
      geometry->setSRID(gp->getSRID());

      switch(gp->getGeometryType())
      {
        case te::gm::PointType:
          geometry->setGeometryType(te::gm::MultiPointType);
          break;
        case te::gm::PointMType:
          geometry->setGeometryType(te::gm::MultiPointMType);
          break;
        case te::gm::PointZType:
          geometry->setGeometryType(te::gm::MultiPointZType);
          break;
        case te::gm::PointZMType:
          geometry->setGeometryType(te::gm::MultiPointZMType);
          break;
        case te::gm::LineStringType:
          geometry->setGeometryType(te::gm::MultiLineStringType);
          break;
        case te::gm::LineStringMType:
          geometry->setGeometryType(te::gm::MultiLineStringMType);
          break;
        case te::gm::LineStringZType:
          geometry->setGeometryType(te::gm::MultiLineStringZType);
          break;
        case te::gm::LineStringZMType:
          geometry->setGeometryType(te::gm::MultiLineStringZMType);
          break;
        case te::gm::PolygonType:
          geometry->setGeometryType(te::gm::MultiPolygonType);
          break;
        case te::gm::PolygonZType:
          geometry->setGeometryType(te::gm::MultiPolygonZType);
          break;
        case te::gm::PolygonMType:
          geometry->setGeometryType(te::gm::MultiPolygonMType);
          break;
        case te::gm::PolygonZMType:
          geometry->setGeometryType(te::gm::MultiPolygonZMType);
          break;
        default:
          geometry->setGeometryType(gp->getGeometryType());
      }
      
      dsType->add(geometry);
    }

  }
  else
  {
    bool flagGeom = false;
    switch(geomOp)
    {
      case te::vp::CONVEX_HULL:
        {
          te::gm::GeometryProperty* geometry =
              new te::gm::GeometryProperty("convex_hull");
          geometry->setGeometryType(te::gm::PolygonType);
          geometry->setSRID(gp->getSRID());
          dsType->add(geometry);
          flagGeom = true;
        }
        break;
      case te::vp::CENTROID:
        {
          te::gm::GeometryProperty* geometry =
              new te::gm::GeometryProperty("centroid");
          geometry->setGeometryType(te::gm::PointType);
          geometry->setSRID(gp->getSRID());
          dsType->add(geometry);
          flagGeom = true;
        }
        break;
      case te::vp::MBR:
        {
          te::gm::GeometryProperty* geometry =
              new te::gm::GeometryProperty("mbr");
          geometry->setGeometryType(te::gm::PolygonType);
          geometry->setSRID(gp->getSRID());
          dsType->add(geometry);
          flagGeom = true;
        }
        break;
      default:
        break;

    }

    if(!flagGeom)
    {
      te::gm::GeometryProperty* geometry = new te::gm::GeometryProperty("geom");
      geometry->setSRID(gp->getSRID());

      switch(gp->getGeometryType())
      {
        case te::gm::PointType:
          geometry->setGeometryType(te::gm::MultiPointType);
          break;
        case te::gm::PointMType:
          geometry->setGeometryType(te::gm::MultiPointMType);
          break;
        case te::gm::PointZType:
          geometry->setGeometryType(te::gm::MultiPointZType);
          break;
        case te::gm::PointZMType:
          geometry->setGeometryType(te::gm::MultiPointZMType);
          break;
        case te::gm::LineStringType:
          geometry->setGeometryType(te::gm::MultiLineStringType);
          break;
        case te::gm::LineStringMType:
          geometry->setGeometryType(te::gm::MultiLineStringMType);
          break;
        case te::gm::LineStringZType:
          geometry->setGeometryType(te::gm::MultiLineStringZType);
          break;
        case te::gm::LineStringZMType:
          geometry->setGeometryType(te::gm::MultiLineStringZMType);
          break;
        case te::gm::PolygonType:
          geometry->setGeometryType(te::gm::MultiPolygonType);
          break;
        case te::gm::PolygonZType:
          geometry->setGeometryType(te::gm::MultiPolygonZType);
          break;
        case te::gm::PolygonMType:
          geometry->setGeometryType(te::gm::MultiPolygonMType);
          break;
        case te::gm::PolygonZMType:
          geometry->setGeometryType(te::gm::MultiPolygonZMType);
          break;
        default:
          geometry->setGeometryType(gp->getGeometryType());
      }
      
      dsType->add(geometry);


    }
  }

  return dsType;
}

