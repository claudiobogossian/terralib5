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
  \file terralib/classification/ROISet.cpp

  \brief A ROISet is a set of ROI's.
*/

// TerraLib
#include "../common/STLUtils.h"
#include "../dataaccess/dataset/DataSet.h"
#include "../dataaccess/dataset/DataSetAdapter.h"
#include "../dataaccess/dataset/DataSetType.h"
#include "../dataaccess/dataset/DataSetTypeConverter.h"
#include "../dataaccess/datasource/DataSource.h"
#include "../dataaccess/datasource/DataSourceFactory.h"
#include "../dataaccess/utils/Utils.h"
#include "../datatype/StringProperty.h"
#include "../geometry/GeometryProperty.h"
#include "../geometry/MultiPolygon.h"
#include "../geometry/Polygon.h"
#include "../memory/DataSet.h"
#include "../memory/DataSetItem.h"
#include "ROISet.h"

// Boost
#include <boost/filesystem.hpp>

te::cl::ROISet::ROISet()
{
}

te::cl::ROISet::~ROISet()
{
  te::common::FreeContents(m_roiMap);
  m_roiMap.clear();
}

te::cl::ROI* te::cl::ROISet::getROI(std::string label)
{
  std::map<std::string, te::cl::ROI*>::iterator it = m_roiMap.find(label);

  if(it != m_roiMap.end())
    return it->second;

  return 0;
}

void te::cl::ROISet::addROI(te::cl::ROI* roi)
{
  m_roiMap[roi->getLabel()] = roi;
}

void te::cl::ROISet::removeROI(std::string label)
{
  std::map<std::string, te::cl::ROI*>::iterator it = m_roiMap.find(label);

  if(it != m_roiMap.end())
    m_roiMap.erase(it);
}

std::map<std::string, te::cl::ROI*>& te::cl::ROISet::getROISet()
{
  return m_roiMap;
}

void te::cl::ROISet::exportToFile(std::string fileName, int srid)
{
  //get dsType
  std::auto_ptr<te::da::DataSetType> dsType = getDataSetType();

  //create data source
  std::map<std::string, std::string> connInfo;
  connInfo["URI"] = fileName;

  std::auto_ptr<te::da::DataSource> dsOGR = te::da::DataSourceFactory::make("OGR");
  dsOGR->setConnectionInfo(connInfo);
  dsOGR->open();

  //create converter
  te::da::DataSetTypeConverter* converter = new te::da::DataSetTypeConverter(dsType.get(), dsOGR->getCapabilities());

  te::da::DataSetType* dsTypeResult = converter->getResult();

  boost::filesystem::path uri(fileName);

  std::string val = uri.stem().string();

  dsTypeResult->setName(val);

  //get dataset
  std::auto_ptr<te::da::DataSet> dataset = getDataSet();

  //exchange
  std::map<std::string,std::string> nopt;

  dsOGR->createDataSet(dsTypeResult, nopt);

  std::auto_ptr<te::da::DataSetAdapter> dsAdapter(te::da::CreateAdapter(dataset.get(), converter));

  dsAdapter->setSRID(srid);

  if(dataset->moveBeforeFirst())
    dsOGR->add(dsTypeResult->getName(), dsAdapter.get(), dsOGR->getConnectionInfo());

  dsOGR->close();
}

te::cl::ROISet* te::cl::ROISet::createROISet(std::auto_ptr<te::da::DataSet> ds)
{
  //check the input dataset
  assert(ds.get());
  assert(ds->getNumProperties() == 5);
  assert(ds->getPropertyName(1) == TE_CL_ROI_GEOM_ID_NAME);
  assert(ds->getPropertyName(2) == TE_CL_ROI_LABEL_NAME);
  assert(ds->getPropertyName(3) == TE_CL_ROI_COLOR_NAME);
  //assert(ds->getPropertyName(4) == TE_CL_ROI_GEOM_NAME); //OGR_GEOMETRY

  //move the data set to begin
  ds->moveBeforeFirst();

  te::cl::ROISet* rs = new te::cl::ROISet();

  while(ds->moveNext())
  {
    std::string label = ds->getString(TE_CL_ROI_LABEL_NAME);

    te::cl::ROI* roi = rs->getROI(label);

    if(roi)
    {
      std::string pId = ds->getString(TE_CL_ROI_GEOM_ID_NAME);

      te::gm::MultiPolygon* mp = (te::gm::MultiPolygon*)ds->getGeometry(4).release();
      te::gm::Polygon* p = (te::gm::Polygon*)mp->getGeometries()[0];

      roi->addPolygon(p, pId);
    }
    else
    {
      std::string color = ds->getString(TE_CL_ROI_COLOR_NAME);
      std::string pId = ds->getString(TE_CL_ROI_GEOM_ID_NAME);

      te::gm::MultiPolygon* mp = (te::gm::MultiPolygon*)ds->getGeometry(4).release();
      te::gm::Polygon* p = (te::gm::Polygon*)mp->getGeometries()[0];

      te::cl::ROI* r = new te::cl::ROI(label);
      r->setColor(color);
      r->addPolygon(p, pId);

      rs->addROI(r);
    }
  }

  return rs;
}

std::auto_ptr<te::da::DataSetType> te::cl::ROISet::getDataSetType()
{
  std::auto_ptr<te::da::DataSetType> dsType;

  te::dt::StringProperty* geomIdProp = new te::dt::StringProperty(TE_CL_ROI_GEOM_ID_NAME);
  te::dt::StringProperty* labelProp  = new te::dt::StringProperty(TE_CL_ROI_LABEL_NAME);
  te::dt::StringProperty* colorProp  = new te::dt::StringProperty(TE_CL_ROI_COLOR_NAME);
  te::gm::GeometryProperty* geomProp = new te::gm::GeometryProperty(TE_CL_ROI_GEOM_NAME);

  te::da::PrimaryKey* pk = new te::da::PrimaryKey(TE_CL_ROI_PK_NAME, dsType.get());
  pk->add(geomIdProp);
  
  te::da::DataSetType* dst = new te::da::DataSetType("ROISet");
  dst->add(geomIdProp);
  dst->add(labelProp);
  dst->add(colorProp);
  dst->add(geomProp);
  dst->add(pk);

  dsType.reset(dst);

  return dsType;
}

std::auto_ptr<te::da::DataSet> te::cl::ROISet::getDataSet()
{
  std::auto_ptr<te::da::DataSet> ds;

  std::auto_ptr<te::da::DataSetType> dsType = getDataSetType();

  te::da::DataSet* dsMem = new te::mem::DataSet(dsType.get());

  std::map<std::string, te::cl::ROI*>::iterator it = m_roiMap.begin();

  while(it != m_roiMap.end())
  {
    te::cl::ROI* roi = it->second;

    std::map<std::string, te::gm::Polygon*> roiMap = roi->getPolygons();

    std::map<std::string, te::gm::Polygon*>::iterator itPols = roiMap.begin();

    while(itPols != roiMap.end())
    {
      te::mem::DataSetItem* dsItem = new te::mem::DataSetItem(dsMem);

      dsItem->setString(TE_CL_ROI_GEOM_ID_NAME, itPols->first);
      dsItem->setString(TE_CL_ROI_LABEL_NAME, roi->getLabel());
      dsItem->setString(TE_CL_ROI_COLOR_NAME, roi->getColor());

      te::gm::Polygon* poly = (te::gm::Polygon*)itPols->second->clone();

      dsItem->setGeometry(TE_CL_ROI_GEOM_NAME, (te::gm::Geometry*)poly);

      ((te::mem::DataSet*)dsMem)->add(dsItem);

      ++itPols;
    }

    ++it;
  }

  ds.reset(dsMem);

  return ds;
}
