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
  \file terralib/rp/MixtureModel.cpp

  \brief Raster decomposition using mixture model.
 */

// TerraLib
#include "../raster/Band.h"
#include "../raster/BandProperty.h"
#include "../raster/Grid.h"
#include "../raster/Raster.h"
#include "../raster/RasterFactory.h"
#include "MixtureModel.h"
#include "MixtureModelStrategy.h"
#include "MixtureModelStrategyFactory.h"
#include "Functions.h"

te::rp::MixtureModel::InputParameters::InputParameters()
  : m_mixtureModelStrategyParamsPtr(0)
{
  reset();
}

te::rp::MixtureModel::InputParameters::InputParameters(const InputParameters& rhs)
  : m_mixtureModelStrategyParamsPtr(0)
{
  reset();

  operator=(rhs);
}

te::rp::MixtureModel::InputParameters::~InputParameters()
{
  reset();
}

void te::rp::MixtureModel::InputParameters::setMixtureModelStrategyParams(const StrategyParameters& p)
{
  if(m_mixtureModelStrategyParamsPtr)
  {
    delete m_mixtureModelStrategyParamsPtr;
    m_mixtureModelStrategyParamsPtr = 0;
  }

  m_mixtureModelStrategyParamsPtr = (StrategyParameters*)p.clone();
}

te::rp::StrategyParameters const* te::rp::MixtureModel::InputParameters::getMixtureModelStrategyParams() const
{
  return m_mixtureModelStrategyParamsPtr;
}

void te::rp::MixtureModel::InputParameters::reset() throw(te::rp::Exception)
{
  m_inputRasterPtr = 0;
  m_inputRasterBands.clear();
  m_inputSensorBands.clear();
  m_components.clear();
  m_strategyName.clear();

  if(m_mixtureModelStrategyParamsPtr)
  {
    delete m_mixtureModelStrategyParamsPtr;
    m_mixtureModelStrategyParamsPtr = 0;
  }
}

const te::rp::MixtureModel::InputParameters& te::rp::MixtureModel::InputParameters::operator=(const te::rp::MixtureModel::InputParameters& params)
{
  reset();

  m_inputRasterPtr = params.m_inputRasterPtr;
  m_inputRasterBands = params.m_inputRasterBands;
  m_inputSensorBands = params.m_inputSensorBands;
  m_components = params.m_components;
  m_strategyName = params.m_strategyName;

  setMixtureModelStrategyParams(*params.m_mixtureModelStrategyParamsPtr);

  return *this;
}

te::common::AbstractParameters* te::rp::MixtureModel::InputParameters::clone() const
{
  return new InputParameters(*this);
}

te::rp::MixtureModel::OutputParameters::OutputParameters()
{
  reset();
}

te::rp::MixtureModel::OutputParameters::OutputParameters(const OutputParameters& rhs)
{
  reset();

  operator=(rhs);
}

te::rp::MixtureModel::OutputParameters::~OutputParameters()
{
  reset();
}

void te::rp::MixtureModel::OutputParameters::reset() throw(te::rp::Exception)
{
  m_rType.clear();
  m_rInfo.clear();
  m_outputRasterPtr.reset();
  m_normalizeOutput = false;
  m_createErrorRaster = false;
}

const te::rp::MixtureModel::OutputParameters& te::rp::MixtureModel::OutputParameters::operator=(const te::rp::MixtureModel::OutputParameters& params)
{
  reset();

  m_rType = params.m_rType;
  m_rInfo = params.m_rInfo;
  m_outputRasterPtr = params.m_outputRasterPtr;
  m_normalizeOutput = params.m_normalizeOutput;
  m_createErrorRaster = params.m_createErrorRaster;

  return *this;
}

te::common::AbstractParameters* te::rp::MixtureModel::OutputParameters::clone() const
{
  return new OutputParameters(*this);
}

te::rp::MixtureModel::MixtureModel()
  : m_instanceInitialized(false)
{
}

te::rp::MixtureModel::~MixtureModel()
{
}

bool te::rp::MixtureModel::execute(AlgorithmOutputParameters& outputParams) throw(te::rp::Exception)
{
  if(!m_instanceInitialized)
    return false;

// creating the output raster
  te::rp::MixtureModel::OutputParameters* outputParamsPtr = dynamic_cast<te::rp::MixtureModel::OutputParameters*>(&outputParams);
  TERP_TRUE_OR_RETURN_FALSE(outputParamsPtr, "Invalid parameters");

  std::vector<te::rst::BandProperty*> bandsProperties;
  std::map<std::string, std::vector<double> >::iterator it;
  for (it = m_inputParameters.m_components.begin(); it != m_inputParameters.m_components.end(); it++)
  {
    te::rst::BandProperty* newbprop = new te::rst::BandProperty(*(m_inputParameters.m_inputRasterPtr->getBand(
      m_inputParameters.m_inputRasterBands[0])->getProperty()));
    newbprop->m_colorInterp = te::rst::GrayIdxCInt;
    newbprop->m_type = te::dt::DOUBLE_TYPE;
    newbprop->m_noDataValue = -1;
    newbprop->m_description = "Mixture model band for class " + it->first;

    bandsProperties.push_back(newbprop);
  }

  if (outputParamsPtr->m_createErrorRaster)
  {
    for (it = m_inputParameters.m_components.begin(); it != m_inputParameters.m_components.end(); it++)
    {
      te::rst::BandProperty* newbprop = new te::rst::BandProperty(*(m_inputParameters.m_inputRasterPtr->getBand(
        m_inputParameters.m_inputRasterBands[0])->getProperty()));
      newbprop->m_colorInterp = te::rst::GrayIdxCInt;
      newbprop->m_type = te::dt::DOUBLE_TYPE;
      newbprop->m_noDataValue = -1;
      newbprop->m_description = "Error raster for class " + it->first;

      bandsProperties.push_back(newbprop);
    }
  }

  te::rst::Grid* newgrid = new te::rst::Grid(*(m_inputParameters.m_inputRasterPtr->getGrid()));

  outputParamsPtr->m_outputRasterPtr.reset(
    te::rst::RasterFactory::make(outputParamsPtr->m_rType, newgrid, bandsProperties, outputParamsPtr->m_rInfo, 0, 0));
  TERP_TRUE_OR_RETURN_FALSE(outputParamsPtr->m_outputRasterPtr.get(),
    "Output raster creation error");

  if (m_inputParameters.m_inputSensorBands.size() == 0)
    for (unsigned b = 0; b < m_inputParameters.m_inputRasterBands.size(); b++)
      m_inputParameters.m_inputSensorBands.push_back("default_sensor");

// instantiating the segmentation strategy
  boost::shared_ptr<te::rp::MixtureModelStrategy> strategyPtr(MixtureModelStrategyFactory::make(m_inputParameters.m_strategyName));
  TERP_TRUE_OR_RETURN_FALSE(strategyPtr.get(), "Unable to create the MixtureModel strategy");
  TERP_TRUE_OR_RETURN_FALSE(strategyPtr->initialize(m_inputParameters.getMixtureModelStrategyParams()),
                            "Unable to initialize the mixture model strategy");
  TERP_TRUE_OR_RETURN_FALSE(strategyPtr->execute(*m_inputParameters.m_inputRasterPtr, m_inputParameters.m_inputRasterBands,
                                                 m_inputParameters.m_inputSensorBands, m_inputParameters.m_components,
                                                 *outputParamsPtr->m_outputRasterPtr, true),
                            "Unable to execute the mixture model strategy");

  if (outputParamsPtr->m_normalizeOutput)
    NormalizeRaster(*outputParamsPtr->m_outputRasterPtr);

  return true;
}

void te::rp::MixtureModel::reset() throw(te::rp::Exception)
{
  m_instanceInitialized = false;

  m_inputParameters.reset();
}

bool te::rp::MixtureModel::initialize(const AlgorithmInputParameters& inputParams) throw(te::rp::Exception)
{
  reset();

  te::rp::MixtureModel::InputParameters const* inputParamsPtr = dynamic_cast<te::rp::MixtureModel::InputParameters const* >(&inputParams);

// check if input raster is ok
  TERP_TRUE_OR_RETURN_FALSE(inputParamsPtr, "Invalid parameters");

  TERP_TRUE_OR_RETURN_FALSE(inputParamsPtr->m_inputRasterPtr, "Invalid raster pointer");

  TERP_TRUE_OR_RETURN_FALSE(inputParamsPtr->m_inputRasterPtr->getAccessPolicy() &
                            te::common::RAccess, "Invalid raster");

// check if input raster and bands fit
  TERP_TRUE_OR_RETURN_FALSE(inputParamsPtr->m_inputRasterBands.size() > 0,
                            "Invalid raster bands number");

// check if input sensor/bands information fits
  if (inputParamsPtr->m_inputSensorBands.size() == 0)
  {
    TERP_LOGWARN("No information about sensors were provided, using defaults.");
  }
  else
  {
    TERP_TRUE_OR_RETURN_FALSE(inputParamsPtr->m_inputSensorBands.size() == inputParamsPtr->m_inputRasterBands.size(),
                              "Invalid raster bands number");
  }

  for(unsigned int i = 0; i < inputParamsPtr->m_inputRasterBands.size(); i++)
  {
    TERP_TRUE_OR_RETURN_FALSE(inputParamsPtr->m_inputRasterBands[i] <
                              inputParamsPtr->m_inputRasterPtr->getNumberOfBands(),
                              "Invalid raster bands" );
  }

  std::map<std::string, std::vector<double> >::const_iterator it;
  for (it = inputParamsPtr->m_components.begin(); it != inputParamsPtr->m_components.end(); ++it)
  {
    TERP_TRUE_OR_RETURN_FALSE(it->second.size() <= inputParamsPtr->m_inputRasterBands.size(),
                              "Endmember's number of channels is bigger from input raster bands number");
  }

  TERP_TRUE_OR_RETURN_FALSE(inputParamsPtr->m_components.size() == inputParamsPtr->m_inputRasterBands.size(),
                            "Number of components must be equal to the number of selected bands");

// everything is ok
  m_instanceInitialized = true;

  m_inputParameters = *inputParamsPtr;

  return true;
}

bool te::rp::MixtureModel::isInitialized() const
{
  return m_instanceInitialized;
}
