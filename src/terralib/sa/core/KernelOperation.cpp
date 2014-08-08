/*  Copyright (C) 2001-2009 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/sa/core/KernelOperation.cpp

  \brief This file contains a class that represents the kernel operation.

  \reference Adapted from TerraLib4.
*/

//TerraLib
#include "../../dataaccess/utils/Utils.h"
#include "../../dataaccess/datasource/DataSource.h"
#include "../../dataaccess/datasource/DataSourceManager.h"
#include "../../datatype/SimpleProperty.h"
#include "../../geometry/Geometry.h"
#include "../../geometry/GeometryProperty.h"
#include "../../memory/DataSet.h"
#include "../../memory/DataSetItem.h"
#include "../../raster/BandProperty.h"
#include "../../raster/Grid.h"
#include "../../raster/Raster.h"
#include "../../raster/RasterFactory.h"
#include "KernelOperation.h"
#include "Utils.h"

//STL
#include <cassert>

// Boost
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

te::sa::KernelOperation::KernelOperation(te::sa::KernelParams* params)
{
  m_params.reset(params);
}

te::sa::KernelOperation::~KernelOperation()
{
  //clear kernel map
  KernelMap::iterator it = m_kMap.begin();

  while(it != m_kMap.end())
  {
    delete it->second.first;
    ++it;
  }

  m_kMap.clear();
}

void te::sa::KernelOperation::execute()
{
  assert(m_params.get());

  //build tree and map kernel
  buildTree();

  //check the output storage mode
  te::sa::KernelOutputType outType = m_params->m_storageType;

  if(outType == te::sa::Grid)
  {
    runRasterKernel();
  }
  else if(outType == te::sa::Attribute)
  {
    runDataSetKernel();
  }
}

void te::sa::KernelOperation::buildTree()
{
  //get properties information
  te::da::DataSetType* dataSetTYype = m_params->m_dsType.get();

  te::da::PrimaryKey* pk = dataSetTYype->getPrimaryKey();

  if(!pk)
    throw;

  te::gm::GeometryProperty* gmProp = te::da::GetFirstGeomProperty(dataSetTYype);

  if(!gmProp)
    throw;

  te::da::DataSet* dataSet = m_params->m_ds.get();

  dataSet->moveBeforeFirst();

  //create tree and kernel map
  while(dataSet->moveNext())
  {
    int id = dataSet->getInt32(pk->getName());

    te::gm::Geometry* g = dataSet->getGeometry(gmProp->getName()).release();

    double value = 1.; //If there is no properties, assume intensity of one

    if(!m_params->m_intensityAttrName.empty())
    {
      value = te::sa::GetDataValue(dataSet->getValue(m_params->m_intensityAttrName).get());
    }

    std::pair<te::gm::Geometry*, double> pair(g, value);

    m_kMap.insert(KernelMap::value_type(id, pair));

    const te::gm::Envelope* box = g->getMBR();

    m_kTree.insert(*box, id);
  }
}

void te::sa::KernelOperation::runRasterKernel()
{
  //create raster
  std::auto_ptr<te::rst::Raster> raster = buildRaster();

  if(!raster.get())
    throw;

  //check if use adaptative radius or not
  if(m_params->m_useAdaptativeRadius)
  {
    te::sa::GridAdaptRadiusKernel(m_params.get(), m_kTree, m_kMap, raster.get());
  }
  else
  {
    double val = std::max<double>(raster->getGrid()->getExtent()->getWidth(), raster->getGrid()->getExtent()->getHeight());
    double radius = (val * m_params->m_radiusPercentValue) / 100.;

    te::sa::GridStatRadiusKernel(m_params.get(), m_kTree, m_kMap, raster.get(), radius);
  }
}

void te::sa::KernelOperation::runDataSetKernel()
{
  //create datasetype
  std::auto_ptr<te::da::DataSetType> dsType = createDataSetType(m_params->m_dsType.get());

  //create dataset in memory
  std::auto_ptr<te::mem::DataSet> dataSet = createDataSet(m_params->m_ds.get(), dsType.get());

  //get kernel attr index
  std::size_t kernelIdx = dsType->getPropertyPosition(m_params->m_outputAttrName);

  //get geom attr index
  std::size_t geomIdx = te::da::GetFirstPropertyPos(dataSet.get(), te::dt::GEOMETRY_TYPE);

  //check if use adaptative radius or not
  if(m_params->m_useAdaptativeRadius)
  {
    te::sa::DataSetAdaptRadiusKernel(m_params.get(), m_kTree, m_kMap, dataSet.get(), kernelIdx, geomIdx);
  }
  else
  {
    double val = std::max<double>(m_kTree.getMBR().getWidth(), m_kTree.getMBR().getHeight());
    double radius = (val * m_params->m_radiusPercentValue) / 100.;

    te::sa::DataSetStatRadiusKernel(m_params.get(), m_kTree, m_kMap, dataSet.get(), kernelIdx, geomIdx, radius);
  }

  //save dataset
  saveDataSet(dataSet.get(), dsType.get());
}

std::auto_ptr<te::rst::Raster> te::sa::KernelOperation::buildRaster()
{
  //get extent of input data
  te::gm::Envelope* env = new te::gm::Envelope(m_kTree.getMBR());

  //get srid
  int srid = TE_UNKNOWN_SRS;
  
  te::gm::GeometryProperty* gmProp = te::da::GetFirstGeomProperty(m_params->m_dsType.get());

  if(gmProp)
    srid = gmProp->getSRID();

  //create grid
  double val = std::max<double>(env->getWidth(), env->getHeight());

  double res = val / m_params->m_nCols;

  te::rst::Grid* grid = new te::rst::Grid(res, res, env, srid);

  //create bands
  te::rst::BandProperty* bProp = new te::rst::BandProperty(0, te::dt::DOUBLE_TYPE);

  bProp->m_noDataValue = 0.;

  std::vector<te::rst::BandProperty*> vecBandProp;
  vecBandProp.push_back(bProp);

  //create raster info
  std::string fileName = m_params->m_outputPath + "/" + m_params->m_outputDataSetName + ".tif";

  std::map<std::string, std::string> rInfo;
  rInfo["URI"] = fileName;

  //create raster
  std::auto_ptr<te::rst::Raster> rst(te::rst::RasterFactory::make("GDAL", grid, vecBandProp, rInfo));

  return rst;
}

void te::sa::KernelOperation::saveDataSet(te::da::DataSet* dataSet, te::da::DataSetType* dsType)
{
  //create dataset info
  std::string fileName = m_params->m_outputPath + "/" + m_params->m_outputDataSetName + ".shp";

  //connection info
  std::map<std::string, std::string> info;
  info["URI"] = fileName;

  //data source id
  boost::uuids::basic_random_generator<boost::mt19937> gen;
  boost::uuids::uuid u = gen();
  std::string id_ds = boost::uuids::to_string(u);

  //create data source
  te::da::DataSourcePtr ds = te::da::DataSourceManager::getInstance().get(id_ds, "OGR", info);

  //save dataset
  dataSet->moveBeforeFirst();

  std::map<std::string, std::string> options;

  ds->createDataSet(dsType, options);

  ds->add(m_params->m_outputDataSetName, dataSet, options);
}

std::auto_ptr<te::da::DataSetType> te::sa::KernelOperation::createDataSetType(te::da::DataSetType* dsType)
{
  std::auto_ptr<te::da::DataSetType> dataSetType(new te::da::DataSetType(m_params->m_outputDataSetName));

  //create all input dataset properties
  std::vector<te::dt::Property*> propertyVec = dsType->getProperties();

  for(std::size_t t = 0; t < propertyVec.size(); ++t)
  {
    te::dt::Property* prop = propertyVec[t];

    te::dt::Property* newProp = prop->clone();
    newProp->setId(0);
    newProp->setParent(0);

    dataSetType->add(newProp);
  }

  //create kernel property
  te::dt::SimpleProperty* kernelProperty = new te::dt::SimpleProperty(m_params->m_outputAttrName, te::dt::DOUBLE_TYPE);
  dataSetType->add(kernelProperty);

  return dataSetType;
}

std::auto_ptr<te::mem::DataSet> te::sa::KernelOperation::createDataSet(te::da::DataSet* inputDataSet, te::da::DataSetType* dsType)
{
  std::auto_ptr<te::mem::DataSet> outDataset(new te::mem::DataSet(dsType));

  std::size_t nProp = inputDataSet->getNumProperties();

  inputDataSet->moveBeforeFirst();

  while(inputDataSet->moveNext())
  {
    //create dataset item
    te::mem::DataSetItem* outDSetItem = new te::mem::DataSetItem(outDataset.get());

    for(std::size_t t = 0; t < nProp; ++t)
    {
      te::dt::AbstractData* ad = inputDataSet->getValue(t).release();

      outDSetItem->setValue(t, ad);
    }

    //set kernel default value
    outDSetItem->setDouble(m_params->m_outputAttrName, 0.);

    //add item into dataset
    outDataset->add(outDSetItem);
  }

  return outDataset;
}
