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

#include "BasicGeoOp.h"

te::vp::BasicGeoOp::BasicGeoOp():
  m_outDsetName("")
{
}

void te::vp::BasicGeoOp::setInput(te::da::DataSourcePtr inDsrc,
                                  std::auto_ptr<te::da::DataSet> inDset,
                                  std::auto_ptr<te::da::DataSetType> inDsetType)
{
  m_inDsrc = inDsrc;
  m_inDset = inDset;
  m_inDsetType = inDsetType;
}

void te::vp::BasicGeoOp::setParams(std::vector<std::string> selectedProps, 
                                  std::vector<te::vp::GeographicOperation> operations)
{
  m_selectedProps = selectedProps;
  m_operations = operations;
}

void te::vp::BasicGeoOp::setOutput(std::auto_ptr<te::da::DataSource> outDsrc, std::string dsname)
{
  m_outDsrc = outDsrc;
  m_outDsetName = dsname;
}

bool te::vp::BasicGeoOp::paramsAreValid()
{
  if (!m_inDset.get() || !m_inDsetType.get())
    return false;
  
  if (!m_inDsetType->hasGeom())
    return false;
  
  //if (m_operations.empty())
  //  return false;

  if (m_outDsetName.empty() || !m_outDsrc.get())
    return false;
  
  return true;
}

bool  te::vp::BasicGeoOp::save(std::auto_ptr<te::mem::DataSet> result, std::auto_ptr<te::da::DataSetType> outDsType)
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
  m_outDsrc->add(dsTypeResult->getName(),result.get(), options);
  
  // create the primary key if it is possible
  if (m_outDsrc->getCapabilities().getDataSetTypeCapabilities().supportsPrimaryKey())
  {
    std::string pk_name = dsTypeResult->getName() + "_pkey";
    te::da::PrimaryKey* pk = new te::da::PrimaryKey(pk_name, dsTypeResult);
    pk->add(dsTypeResult->getProperty(0));
    m_outDsrc->addPrimaryKey(m_outDsetName,pk);
  }
  
  return true;
}

te::da::DataSetType* te::vp::BasicGeoOp::GetDataSetType()
{
  te::da::DataSetType* dsType = new te::da::DataSetType(m_outDsetName);

  bool convexHull = false;
  bool mbr = false;
  for(std::size_t i = 0; i < m_operations.size(); ++i)
  {
    if(m_operations[i] == te::vp::CONVEX_HULL)
      convexHull = true;
    if(m_operations[i] == te::vp::MBR)
      mbr = true;
  }

  if(convexHull == false && mbr == false)
  {
    for(std::size_t i = 0; i < m_selectedProps.size(); ++i)
    {
      te::dt::Property* prop = m_inDsetType->getProperty(m_selectedProps[i])->clone();
      prop->setParent(0);
      dsType->add(prop);
    }
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
    }
  }

  //Geometry property.
  te::gm::GeometryProperty* gp = te::da::GetFirstGeomProperty(m_inDsetType.get());
  te::gm::GeometryProperty* geometry = new te::gm::GeometryProperty("geom");

  for(std::size_t i = 0; i < m_operations.size(); ++i)
  {
    bool flagGeo;
    int op = m_operations[i];
    switch(op)
    {
      case te::vp::CONVEX_HULL:
        geometry->setGeometryType(te::gm::MultiPolygonType);
        break;
      case te::vp::CENTROID:
        geometry->setGeometryType(te::gm::PointType);
        break;
      case te::vp::MBR:
        geometry->setGeometryType(te::gm::MultiPolygonType);
        break;
      default:
        geometry->setGeometryType(gp->getGeometryType());
    }
  }

  if(m_operations.empty())
  {
    geometry->setGeometryType(gp->getGeometryType());
  }

  geometry->setSRID(gp->getSRID());
  dsType->add(geometry);

  return dsType;
}
