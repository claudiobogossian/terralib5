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
  \file terralib/sa/core/KernelMapOperation.cpp

  \brief This file contains a class that represents the kernel map operation.

  \reference Adapted from TerraLib4.
*/

//TerraLib
#include "../../dataaccess/utils/Utils.h"
#include "../../geometry/GeometryProperty.h"
#include "../../memory/DataSet.h"
#include "../../raster/Raster.h"
#include "KernelMapOperation.h"
#include "Utils.h"

//STL
#include <cassert>

te::sa::KernelMapOperation::KernelMapOperation()
{

}

te::sa::KernelMapOperation::~KernelMapOperation()
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

void te::sa::KernelMapOperation::execute()
{
  assert(m_inputParams.get());
  assert(m_outputParams.get());

  //build tree and map kernel
  buildTree();

  //check the output storage mode
  te::sa::KernelOutputType outType = m_outputParams->m_storageType;

  if(outType == te::sa::Grid)
  {
    //create raster
    std::auto_ptr<te::rst::Raster> raster = buildRaster(m_inputParams.get(), m_kTree, "GDAL");

    //run kernel
    runRasterKernel(m_inputParams.get(), m_kTree, m_kMap, raster.get());
  }
  else if(outType == te::sa::Attribute)
  {
    //create datasetype
    std::auto_ptr<te::da::DataSetType> dsType = createDataSetType(m_inputParams->m_dsType.get());

    //run kernel
    std::auto_ptr<te::mem::DataSet> dataSet = runDataSetKernel(m_inputParams.get(), m_kTree, m_kMap, dsType.get());

    //save dataset
    saveDataSet(dataSet.get(), dsType.get());
  }
}

void te::sa::KernelMapOperation::buildTree()
{
  //get properties information
  te::da::DataSetType* dataSetType = m_inputParams->m_dsType.get();

  te::da::PrimaryKey* pk = dataSetType->getPrimaryKey();

  if(!pk || pk->getProperties().empty())
    throw;

  std::string idxName = pk->getProperties()[0]->getName();

  te::gm::GeometryProperty* gmProp = te::da::GetFirstGeomProperty(dataSetType);

  if(!gmProp)
    throw;

  te::da::DataSet* dataSet = m_inputParams->m_ds.get();

  dataSet->moveBeforeFirst();

  //create tree and kernel map
  while(dataSet->moveNext())
  {
    int id = dataSet->getInt32(idxName);

    te::gm::Geometry* g = dataSet->getGeometry(gmProp->getName()).release();

    const te::gm::Envelope* box = g->getMBR();

    m_kTree.insert(*box, id);

    double value = 1.; //If there is no properties, assume intensity of one

    if(!m_inputParams->m_intensityAttrName.empty())
    {
      value = te::sa::GetDataValue(dataSet->getValue(m_inputParams->m_intensityAttrName).get());
    }

    std::pair<te::gm::Geometry*, double> pair(g, value);

    m_kMap.insert(KernelMap::value_type(id, pair));
  }
}
