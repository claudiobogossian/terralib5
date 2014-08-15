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
  \file terralib/sa/core/KernelRatioOperation.cpp

  \brief This file contains a class that represents the kernel ratio operation.

  \reference Adapted from TerraLib4.
*/

//TerraLib
#include "../../dataaccess/utils/Utils.h"
#include "../../geometry/GeometryProperty.h"
#include "../../memory/DataSet.h"
#include "../../raster/Raster.h"
#include "KernelRatioOperation.h"
#include "Utils.h"

//STL
#include <cassert>

te::sa::KernelRatioOperation::KernelRatioOperation()
{

}

te::sa::KernelRatioOperation::~KernelRatioOperation()
{
  //clear kernel map A
  KernelMap::iterator itA = m_kMapA.begin();

  while(itA != m_kMapA.end())
  {
    delete itA->second.first;
    ++itA;
  }

  m_kMapA.clear();

  //clear kernel map B
  KernelMap::iterator itB = m_kMapB.begin();

  while(itB != m_kMapB.end())
  {
    delete itB->second.first;
    ++itB;
  }

  m_kMapB.clear();
}

void te::sa::KernelRatioOperation::execute()
{
  assert(m_inputParams.get());
  assert(m_outputParams.get());

  //build tree and map kernel 
  buildTree();

  //check the output storage mode
  te::sa::KernelOutputType outType = m_outputParams->m_storageType;

  if(outType == te::sa::Grid)
  {
    //create raster A
    std::auto_ptr<te::rst::Raster> rasterA = buildRaster(m_inputParams.get(), m_kTree, "MEM");

    //run kernel A
    runRasterKernel(m_inputParams.get(), m_kTree, m_kMapA, rasterA.get());

    //create raster B
    std::auto_ptr<te::rst::Raster> rasterB = buildRaster(m_inputParams.get(), m_kTree, "MEM");

    //run kernel B
    runRasterKernel(m_inputParams.get(), m_kTree, m_kMapB, rasterB.get());

    //create raster out
    std::auto_ptr<te::rst::Raster> rasterOut = buildRaster(m_inputParams.get(), m_kTree, "GDAL");

    //run kernel ratio
    te::sa::GridRatioKernel(m_inputParams.get(), rasterA.get(), rasterB.get(), rasterOut.get());
  }
  else if(outType == te::sa::Attribute)
  {
    //create datasetype A
    std::auto_ptr<te::da::DataSetType> dsTypeA = createDataSetType(m_inputParams->m_dsType.get());

    //run kernel A
    std::auto_ptr<te::mem::DataSet> dataSetA = runDataSetKernel(m_inputParams.get(), m_kTree, m_kMapA, dsTypeA.get());

    //create datasetype B
    std::auto_ptr<te::da::DataSetType> dsTypeB = createDataSetType(m_inputParams->m_dsType.get());

    //run kernel B
    std::auto_ptr<te::mem::DataSet> dataSetB = runDataSetKernel(m_inputParams.get(), m_kTree, m_kMapB, dsTypeB.get());

    //create datasetype out
    std::auto_ptr<te::da::DataSetType> dsTypeOut = createDataSetType(m_inputParams->m_dsType.get());

    //create dataset in memory
    std::auto_ptr<te::mem::DataSet> dataSetOut = createDataSet(m_inputParams->m_ds.get(), dsTypeOut.get());

    //get kernel attr index
    std::size_t kernelIdx = dsTypeOut->getPropertyPosition(m_outputParams->m_outputAttrName);

    //get geom attr index
    std::size_t geomIdx = te::da::GetFirstPropertyPos(dataSetOut.get(), te::dt::GEOMETRY_TYPE);

    //run kernel ratio
    te::sa::DataSetRatioKernel(m_inputParams.get(), dataSetA.get(), dataSetB.get(), dataSetOut.get(), kernelIdx, geomIdx);

    //save dataset
    saveDataSet(dataSetOut.get(), dsTypeOut.get());
  }
}

void te::sa::KernelRatioOperation::buildTree()
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

    std::auto_ptr<te::gm::Geometry> g = dataSet->getGeometry(gmProp->getName());

    const te::gm::Envelope* box = g->getMBR();

    m_kTree.insert(*box, id);

    double valueA = 1.; //If there is no properties, assume intensity of one

    if(!m_inputParams->m_intensityAttrName.empty())
    {
      valueA = te::sa::GetDataValue(dataSet->getValue(m_inputParams->m_intensityAttrName).get());
    }

    std::pair<te::gm::Geometry*, double> pairA((te::gm::Geometry*)g->clone(), valueA);

    m_kMapA.insert(KernelMap::value_type(id, pairA));

    double valueB = 1.; //If there is no properties, assume intensity of one

    if(!m_inputParams->m_intensityAttrName2.empty())
    {
      valueB = te::sa::GetDataValue(dataSet->getValue(m_inputParams->m_intensityAttrName2).get());
    }

    std::pair<te::gm::Geometry*, double> pairB((te::gm::Geometry*)g->clone(), valueB);

    m_kMapB.insert(KernelMap::value_type(id, pairB));
  }
}
