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
  \file terralib/sa/core/SamplePointsGeneratorAbstract.cpp

  \brief This file contains a virtual class to generate samples points.

  \reference Adapted from TerraLib4.
*/

// Terralib Includes
#include "../../geometry/Point.h"
#include "../../memory/DataSet.h"
#include "../../srs/Config.h"
#include "SamplePointsGeneratorAbstract.h"

te::sa::SamplePointsGeneratorAbstract::SamplePointsGeneratorAbstract() : m_srid(TE_UNKNOWN_SRS), m_outputDataSetName("")
{
  m_distReal = boost::random::uniform_real_distribution<>(0, 1);
}

te::sa::SamplePointsGeneratorAbstract::~SamplePointsGeneratorAbstract()
{
}

void te::sa::SamplePointsGeneratorAbstract::execute()
{
  std::auto_ptr<te::da::DataSetType> dsType = createDataSetType();

  std::auto_ptr<te::mem::DataSet> dsMem = generateSamples(dsType.get());

  saveDataSet(dsMem.get(), dsType.get());
}

void te::sa::SamplePointsGeneratorAbstract::setSRID(int srid)
{
  m_srid = srid;
}

void te::sa::SamplePointsGeneratorAbstract::setEnvelope(te::gm::Envelope env)
{
  m_env = env;
}

void te::sa::SamplePointsGeneratorAbstract::setOutputDataSetName(std::string dataSetName)
{
  m_outputDataSetName = dataSetName;
}

void te::sa::SamplePointsGeneratorAbstract::setOutputDataSource(te::da::DataSourcePtr ds)
{
  m_ds = ds;
}

void te::sa::SamplePointsGeneratorAbstract::saveDataSet(te::mem::DataSet* dataSet, te::da::DataSetType* dsType)
{
  assert(dataSet);
  assert(dsType);

  //save dataset
  dataSet->moveBeforeFirst();

  std::map<std::string, std::string> options;

  m_ds->createDataSet(dsType, options);

  m_ds->add(m_outputDataSetName, dataSet, options);
}

te::gm::Point* te::sa::SamplePointsGeneratorAbstract::getPoint(const te::gm::Envelope* env)
{
  te::gm::Point* p = new te::gm::Point(m_srid);

  double diffX = env->getUpperRightX() - env->getLowerLeftX();
  double randX = m_distReal(m_gen);
  double x =  randX * diffX + env->getLowerLeftX();
  p->setX(x);

  double diffY = env->getUpperRightY() - env->getLowerLeftY();
  double randY = m_distReal(m_gen);
  double y = randY * diffY + env->getLowerLeftY();
  p->setY(y);

  return p;
}

