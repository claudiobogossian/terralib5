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
#include "../../geometry/Geometry.h"
#include "../../geometry/GeometryProperty.h"
#include "../../raster/BandProperty.h"
#include "../../raster/Grid.h"
#include "../../raster/Raster.h"
#include "../../raster/RasterFactory.h"
#include "KernelOperation.h"
#include "Utils.h"

//STL
#include <cassert>

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

std::auto_ptr<te::rst::Raster> te::sa::KernelOperation::buildRaster()
{
  //get extent of input data
  te::gm::Envelope* env = new te::gm::Envelope(m_kTree.getMBR());

  //create grid
  double val = std::max<double>(env->getWidth(), env->getHeight());

  double res = val / m_params->m_nCols;

  te::rst::Grid* grid = new te::rst::Grid(res, res, env);

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
