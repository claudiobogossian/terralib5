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
 \file GeneralOp.cpp
 */

#include "../dataaccess/dataset/DataSet.h"
#include "../dataaccess/dataset/DataSetAdapter.h"
#include "../dataaccess/dataset/DataSetType.h"
#include "../dataaccess/dataset/DataSetTypeConverter.h"
#include "../dataaccess/datasource/DataSource.h"
#include "../dataaccess/datasource/DataSourceCapabilities.h"
#include "../dataaccess/utils/Utils.h"

#include "../datatype/Property.h"
#include "../datatype/StringProperty.h"

#include "../geometry/GeometryProperty.h"

#include "../statistics/core/Utils.h"

#include "GeometricOp.h"

#include <sstream>

te::vp::GeometricOp::GeometricOp():
  m_outDsetName("")
{
}

void te::vp::GeometricOp::setInput(te::da::DataSourcePtr inDsrc,
                                  std::string inDsetName,
                                  std::auto_ptr<te::da::DataSetType> inDsetType)
{
  m_inDsrc = inDsrc;
  m_inDsetName = inDsetName;
  m_inDsetType = inDsetType;
}

void te::vp::GeometricOp::setParams(std::vector<std::string> selectedProps, 
                                  std::vector<te::vp::GeometricOperation> operations,
                                  te::vp::GeometricOpObjStrategy objStrategy,
                                  std::string attribute,
                                  bool outputLayer)
{
  m_selectedProps = selectedProps;
  m_operations = operations;
  m_objStrategy = objStrategy;
  m_attribute = attribute;
  m_outputLayer = outputLayer;
}

void te::vp::GeometricOp::setOutput(std::auto_ptr<te::da::DataSource> outDsrc, std::string dsname)
{
  m_outDsrc = outDsrc;
  m_outDsetName = dsname;
}

bool te::vp::GeometricOp::paramsAreValid()
{
  if (!m_inDsetType.get())
    return false;
  
  if (!m_inDsetType->hasGeom())
    return false;

  if (m_outDsetName.empty() || !m_outDsrc.get())
    return false;
  
  return true;
}

std::vector<std::string> te::vp::GeometricOp::GetOutputDSetNames()
{
  return m_outDsetNameVec;
}

bool  te::vp::GeometricOp::save(std::auto_ptr<te::mem::DataSet> result, std::auto_ptr<te::da::DataSetType> outDsType)
{
  // do any adaptation necessary to persist the output dataset
  te::da::DataSetTypeConverter* converter = new te::da::DataSetTypeConverter(outDsType.get(), m_outDsrc->getCapabilities());
  te::da::DataSetType* dsTypeResult = converter->getResult();
  std::auto_ptr<te::da::DataSetAdapter> dsAdapter(te::da::CreateAdapter(result.get(), converter));
  
  std::map<std::string, std::string> options;
  // create the dataset
  m_outDsrc->createDataSet(dsTypeResult, options);
  
  // copy from memory to output datasource
  result->moveBeforeFirst();
  std::string name = dsTypeResult->getName();
  m_outDsrc->add(dsTypeResult->getName(),result.get(), options);
  
  // create the primary key if it is possible
  if (m_outDsrc->getCapabilities().getDataSetTypeCapabilities().supportsPrimaryKey())
  {
    std::string pk_name = dsTypeResult->getName() + "_pkey";
    te::da::PrimaryKey* pk = new te::da::PrimaryKey(pk_name, dsTypeResult);
    pk->add(dsTypeResult->getProperty(0));
    m_outDsrc->addPrimaryKey(outDsType->getName(), pk);
  }
  
  return true;
}

te::da::DataSetType* te::vp::GeometricOp::GetDataSetType( te::vp::GeometricOpObjStrategy geomOpStrategy,
                                                          bool multiGeomColumns,
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
          break;
        }
      case 1:
        {
          dsType = new te::da::DataSetType(m_outDsetName + "_centroid");
          dsType->setTitle(m_outDsetName + "_centroid");
          break;
        }
      case 2:
        {
          dsType = new te::da::DataSetType(m_outDsetName + "_mbr");
          dsType->setTitle(m_outDsetName + "_mbr");
          break;
        }
    }
  }
  else
  {
    dsType = new te::da::DataSetType(m_outDsetName);
    dsType->setTitle(m_outDsetName);
  }
  // Primary key
  te::dt::SimpleProperty* pkProperty = new te::dt::SimpleProperty(m_outDsetName + "_id", te::dt::INT32_TYPE);
  pkProperty->setAutoNumber(true);
  dsType->add(pkProperty);

  te::da::PrimaryKey* pk = new te::da::PrimaryKey(m_outDsetName + "_pk", dsType);
  pk->add(pkProperty);
  dsType->setPrimaryKey(pk);

  if(geomOpStrategy == te::vp::ALL_OBJ)
  {
      for(std::size_t i = 0; i < m_selectedProps.size(); ++i)
      {
        te::dt::Property* prop = m_inDsetType->getProperty(m_selectedProps[i])->clone();
        prop->setParent(0);
        dsType->add(prop);
      }
  }

  if(geomOpStrategy == te::vp::AGGREG_BY_ATTRIBUTE)
  {
    te::dt::Property* prop = m_inDsetType->getProperty(m_attribute)->clone();
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
          te::dt::SimpleProperty* area = new te::dt::SimpleProperty("area", te::dt::DOUBLE_TYPE);
          dsType->add(area);
        }
        break;
      case te::vp::LINE:
        {
          te::dt::SimpleProperty* line = new te::dt::SimpleProperty("line_length", te::dt::DOUBLE_TYPE);
          dsType->add(line);
        }
        break;
      case te::vp::PERIMETER:
        {
          te::dt::SimpleProperty* perimeter = new te::dt::SimpleProperty("perimeter", te::dt::DOUBLE_TYPE);
          dsType->add(perimeter);
        }
        break;
      default:
        break;
    }
  }
  
//  Geometry property.
  te::gm::GeometryProperty* gp = te::da::GetFirstGeomProperty(m_inDsetType.get());

  if(multiGeomColumns)
  {
    bool flagGeom = false;
    for(std::size_t i = 0; i< m_operations.size(); ++i)
    {
      switch(m_operations[i])
      {
        case te::vp::CONVEX_HULL:
          {
            te::gm::GeometryProperty* convGeom = new te::gm::GeometryProperty("convex_hull");
            convGeom->setGeometryType(te::gm::PolygonType);
            convGeom->setSRID(gp->getSRID());
            dsType->add(convGeom);
            flagGeom = true;
          }
          break;
        case te::vp::CENTROID:
          {
            te::gm::GeometryProperty* centroidGeom = new te::gm::GeometryProperty("centroid");
            centroidGeom->setGeometryType(te::gm::PointType);
            centroidGeom->setSRID(gp->getSRID());
            dsType->add(centroidGeom);
            flagGeom = true;
          }
          break;
        case te::vp::MBR:
          {
            te::gm::GeometryProperty* mbrGeom = new te::gm::GeometryProperty("mbr");
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
        case te::gm::LineStringType:
          geometry->setGeometryType(te::gm::MultiLineStringType);
          break;
        case te::gm::PolygonType:
          geometry->setGeometryType(te::gm::MultiPolygonType);
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
          te::gm::GeometryProperty* geometry = new te::gm::GeometryProperty("convex_hull");
          geometry->setGeometryType(te::gm::PolygonType);
          geometry->setSRID(gp->getSRID());
          dsType->add(geometry);
          flagGeom = true;
        }
        break;
      case te::vp::CENTROID:
        {
          te::gm::GeometryProperty* geometry = new te::gm::GeometryProperty("centroid");
          geometry->setGeometryType(te::gm::PointType);
          geometry->setSRID(gp->getSRID());
          dsType->add(geometry);
          flagGeom = true;
        }
        break;
      case te::vp::MBR:
        {
          te::gm::GeometryProperty* geometry = new te::gm::GeometryProperty("mbr");
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
        case te::gm::LineStringType:
          geometry->setGeometryType(te::gm::MultiLineStringType);
          break;
        case te::gm::PolygonType:
          geometry->setGeometryType(te::gm::MultiPolygonType);
        default:
          geometry->setGeometryType(gp->getGeometryType());
      }
      
      dsType->add(geometry);


    }
  }

  return dsType;
}

