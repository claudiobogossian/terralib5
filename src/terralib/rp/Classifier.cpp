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
  \file terralib/rp/Classifier.cpp

  \brief Raster classification.
 */

// TerraLib
#include "../geometry/Envelope.h"
#include "../geometry/Polygon.h"
#include "../raster/Raster.h"
#include "../raster/RasterFactory.h"
#include "../raster/Band.h"
#include "../raster/BandProperty.h"
#include "../raster/Grid.h"
#include "Algorithm.h"
#include "Classifier.h"
#include "ClassifierStrategy.h"
#include "ClassifierStrategyFactory.h"
#include "Config.h"
#include "Matrix.h"

te::rp::Classifier::InputParameters::InputParameters()
  : m_classifierStrategyParamsPtr(0)
{
  reset();
}

te::rp::Classifier::InputParameters::InputParameters(const InputParameters& rhs)
  : m_classifierStrategyParamsPtr(0)
{
  reset();

  operator=(rhs);
}

te::rp::Classifier::InputParameters::~InputParameters()
{
  reset();
}

void te::rp::Classifier::InputParameters::setClassifierStrategyParams(const StrategyParameters& p)
{
  if(m_classifierStrategyParamsPtr)
  {
    delete m_classifierStrategyParamsPtr;
    m_classifierStrategyParamsPtr = 0;
  }

  m_classifierStrategyParamsPtr = (StrategyParameters*)p.clone();
}

te::rp::StrategyParameters const* te::rp::Classifier::InputParameters::getClassifierStrategyParams() const
{
  return m_classifierStrategyParamsPtr;
}

void te::rp::Classifier::InputParameters::reset() throw(te::rp::Exception)
{
  m_inputRasterPtr = 0;
  m_inputRasterBands.clear();
  m_inputPolygons.clear();
  m_strategyName.clear();

  if(m_classifierStrategyParamsPtr)
  {
    delete m_classifierStrategyParamsPtr;
    m_classifierStrategyParamsPtr = 0;
  }
}

const te::rp::Classifier::InputParameters& te::rp::Classifier::InputParameters::operator=(const te::rp::Classifier::InputParameters& params)
{
  reset();

  m_inputRasterPtr = params.m_inputRasterPtr;
  m_inputRasterBands = params.m_inputRasterBands;
  m_inputPolygons = params.m_inputPolygons;
  m_strategyName = params.m_strategyName;

  setClassifierStrategyParams(*params.m_classifierStrategyParamsPtr);

  return *this;
}

te::common::AbstractParameters* te::rp::Classifier::InputParameters::clone() const
{
  return new InputParameters(*this);
}

te::rp::Classifier::OutputParameters::OutputParameters()
{
  reset();
}

te::rp::Classifier::OutputParameters::OutputParameters(const OutputParameters& rhs)
{
  reset();

  operator=(rhs);
}

te::rp::Classifier::OutputParameters::~OutputParameters()
{
  reset();
}

void te::rp::Classifier::OutputParameters::reset() throw(te::rp::Exception)
{
  m_rType.clear();
  m_rInfo.clear();
  m_outputRasterPtr.reset();
}

const te::rp::Classifier::OutputParameters& te::rp::Classifier::OutputParameters::operator=(const te::rp::Classifier::OutputParameters& params)
{
  reset();

  m_rType = params.m_rType;
  m_rInfo = params.m_rInfo;
  m_outputRasterPtr = params.m_outputRasterPtr;

  return *this;
}

te::common::AbstractParameters* te::rp::Classifier::OutputParameters::clone() const
{
  return new OutputParameters(*this);
}

te::rp::Classifier::Classifier()
  : m_instanceInitialized(false)
{
}

te::rp::Classifier::~Classifier()
{
}

bool te::rp::Classifier::execute(AlgorithmOutputParameters& outputParams) throw(te::rp::Exception)
{
  if(!m_instanceInitialized)
    return false;

// creating the output raster
  te::rp::Classifier::OutputParameters* outputParamsPtr = dynamic_cast<te::rp::Classifier::OutputParameters*>(&outputParams);
  TERP_TRUE_OR_RETURN_FALSE(outputParamsPtr, "Invalid parameters");

  std::vector<te::rst::BandProperty*> bandsProperties;
  bandsProperties.push_back(new te::rst::BandProperty(*(m_inputParameters.m_inputRasterPtr->getBand(
    m_inputParameters.m_inputRasterBands[0])->getProperty())));
  bandsProperties[0]->m_colorInterp = te::rst::GrayIdxCInt;
  bandsProperties[0]->m_type = te::dt::UINT32_TYPE;
  bandsProperties[0]->m_noDataValue = 0;

  te::rst::Grid* newgrid = new te::rst::Grid(*(m_inputParameters.m_inputRasterPtr->getGrid()));

  outputParamsPtr->m_outputRasterPtr.reset(
    te::rst::RasterFactory::make(outputParamsPtr->m_rType, newgrid, bandsProperties, outputParamsPtr->m_rInfo, 0, 0));
  TERP_TRUE_OR_RETURN_FALSE(outputParamsPtr->m_outputRasterPtr.get(),
    "Output raster creation error");

// instantiating the classification strategy
  boost::shared_ptr<te::rp::ClassifierStrategy> strategyPtr(ClassifierStrategyFactory::make(m_inputParameters.m_strategyName));
  TERP_TRUE_OR_RETURN_FALSE(strategyPtr.get(), "Unable to create the classifier strategy");
  TERP_TRUE_OR_RETURN_FALSE(strategyPtr->initialize(m_inputParameters.getClassifierStrategyParams()),
                            "Unable to initialize the classification strategy");
  TERP_TRUE_OR_RETURN_FALSE(strategyPtr->execute(*m_inputParameters.m_inputRasterPtr, m_inputParameters.m_inputRasterBands,
                                                 m_inputParameters.m_inputPolygons, *outputParamsPtr->m_outputRasterPtr, 0, true),
                            "Unable to execute the classification strategy");

  return true;
}

void te::rp::Classifier::reset() throw(te::rp::Exception)
{
  m_instanceInitialized = false;

  m_inputParameters.reset();
}

bool te::rp::Classifier::initialize(const AlgorithmInputParameters& inputParams) throw(te::rp::Exception)
{
  reset();

  te::rp::Classifier::InputParameters const* inputParamsPtr = dynamic_cast<te::rp::Classifier::InputParameters const* >(&inputParams);

// check if input raster is ok
  TERP_TRUE_OR_RETURN_FALSE(inputParamsPtr, "Invalid parameters");

  TERP_TRUE_OR_RETURN_FALSE(inputParamsPtr->m_inputRasterPtr, "Invalid raster pointer");

  TERP_TRUE_OR_RETURN_FALSE(inputParamsPtr->m_inputRasterPtr->getAccessPolicy() &
                            te::common::RAccess, "Invalid raster");

// check if input raster and bands fit
  TERP_TRUE_OR_RETURN_FALSE(inputParamsPtr->m_inputRasterBands.size() > 0,
                            "Invalid raster bands number");

  for(unsigned int i = 0; i < inputParamsPtr->m_inputRasterBands.size(); i++)
    TERP_TRUE_OR_RETURN_FALSE(inputParamsPtr->m_inputRasterBands[i] <
                              inputParamsPtr->m_inputRasterPtr->getNumberOfBands(),
                              "Invalid raster bands" );

// check if input raster and geometries fit
  for (unsigned i = 0; i > inputParamsPtr->m_inputPolygons.size(); i++)
  {
    TERP_TRUE_OR_RETURN_FALSE(inputParamsPtr->m_inputPolygons[i]->getMBR()->within(*inputParamsPtr->m_inputRasterPtr->getExtent()),
                              "Raster and Polygons does not fit");
  }

// everything is ok
  m_instanceInitialized = true;

  m_inputParameters = *inputParamsPtr;

  return true;
}

bool te::rp::Classifier::isInitialized() const
{
  return m_instanceInitialized;
}
