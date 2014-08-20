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
  \file terralib/sa/core/SamplePointsGeneratorRandom.cpp

  \brief This file contains a class to generate samples points using random strategy.

  \reference Adapted from TerraLib4.
*/

// Terralib Includes
#include "../../datatype/SimpleProperty.h"
#include "../../geometry/Geometry.h"
#include "../../geometry/GeometryProperty.h"
#include "../../memory/DataSet.h"
#include "../../memory/DataSetItem.h"
#include "../Enums.h"
#include "SamplePointsGeneratorRandom.h"

te::sa::SamplePointsGeneratorRandom::SamplePointsGeneratorRandom() : te::sa::SamplePointsGeneratorAbstract(), m_nPoints(0)
{
  m_type = te::sa::Random;
}

te::sa::SamplePointsGeneratorRandom::~SamplePointsGeneratorRandom()
{
}

void te::sa::SamplePointsGeneratorRandom::setNumberOfPoints(int nPoints)
{
  m_nPoints = nPoints;
}

std::auto_ptr<te::da::DataSetType> te::sa::SamplePointsGeneratorRandom::createDataSetType()
{
  std::auto_ptr<te::da::DataSetType> dsType(new te::da::DataSetType(m_outputDataSetName));

  //create id property
  te::dt::SimpleProperty* idProperty = new te::dt::SimpleProperty(TE_SA_SPG_ATTR_ID_NAME, te::dt::INT32_TYPE);
  dsType->add(idProperty);

  //create geometry property
  te::gm::GeometryProperty* geomProperty = new te::gm::GeometryProperty(TE_SA_SPG_ATTR_GEOM_NAME, m_srid, te::gm::PointType);
  dsType->add(geomProperty);

  //create primary key
  te::da::PrimaryKey* pk = new te::da::PrimaryKey(TE_SA_SPG_ATTR_PK_NAME, dsType.get());
  pk->add(idProperty);

  return dsType;
}

std::auto_ptr<te::mem::DataSet> te::sa::SamplePointsGeneratorRandom::generateSamples(te::da::DataSetType* dsType)
{
  std::auto_ptr<te::mem::DataSet> ds(new te::mem::DataSet(dsType));

  for(int i = 0; i < m_nPoints; ++i)
  {
    //create dataset item
    te::mem::DataSetItem* item = new te::mem::DataSetItem(ds.get());

    //set id
    item->setInt32(TE_SA_SPG_ATTR_ID_NAME, i);

    //set geometry
    item->setGeometry(TE_SA_SPG_ATTR_GEOM_NAME, getPoint(&m_env));

    ds->add(item);
  }

  return ds;
}
