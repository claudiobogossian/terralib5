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
  \file terralib/rp/ClassifierKMeansStrategy.cpp

  \brief KMeans strategy for image classification.
*/

// TerraLib
#include "../classification/KMeans.h"
#include "../common/progress/TaskProgress.h"
#include "../geometry/Envelope.h"
#include "../raster/Grid.h"
#include "../raster/PositionIterator.h"
#include "../raster/RasterIterator.h"
#include "../raster/Utils.h"
#include "ClassifierKMeansStrategy.h"
#include "Macros.h"
#include "Functions.h"

// STL
#include <iostream>
#include <stdlib.h>

namespace
{
  static te::rp::ClassifierKMeansStrategyFactory classifierKMeansStrategyFactoryInstance;
}

te::rp::ClassifierKMeansStrategy::Parameters::Parameters()
{
  reset();
}

te::rp::ClassifierKMeansStrategy::Parameters::~Parameters()
{
}

const te::rp::ClassifierKMeansStrategy::Parameters& te::rp::ClassifierKMeansStrategy::Parameters::operator=(const te::rp::ClassifierKMeansStrategy::Parameters& rhs)
{
  reset();

  m_K = rhs.m_K;
  m_maxIterations = rhs.m_maxIterations;
  m_maxInputPoints = rhs.m_maxInputPoints;
  m_epsilon = rhs.m_epsilon;

  return *this;
}

void te::rp::ClassifierKMeansStrategy::Parameters::reset() throw(te::rp::Exception)
{
  m_K = 0;
  m_maxIterations = 0;
  m_maxInputPoints = 0;
  m_epsilon = 0.0;
}

te::common::AbstractParameters* te::rp::ClassifierKMeansStrategy::Parameters::clone() const
{
  return new te::rp::ClassifierKMeansStrategy::Parameters( *this );
}

te::rp::ClassifierKMeansStrategy::ClassifierKMeansStrategy()
{
  m_isInitialized = false;
}

te::rp::ClassifierKMeansStrategy::~ClassifierKMeansStrategy()
{
}

std::vector< int > te::rp::ClassifierKMeansStrategy::getOutputDataTypes() const
{
  std::vector< int > dt;
  dt.push_back( te::dt::UINT32_TYPE );
  return dt;
}

bool te::rp::ClassifierKMeansStrategy::initialize(te::rp::ClassifierStrategyParameters const* const strategyParams) throw(te::rp::Exception)
{
  m_isInitialized = false;

  te::rp::ClassifierKMeansStrategy::Parameters const* paramsPtr = dynamic_cast<te::rp::ClassifierKMeansStrategy::Parameters const*>(strategyParams);

  if(!paramsPtr)
    return false;

  m_parameters = *(paramsPtr);

  TERP_TRUE_OR_RETURN_FALSE(m_parameters.m_K > 1, TE_TR("The value of K (number of clusters) must be at least 2."))
  TERP_TRUE_OR_RETURN_FALSE(m_parameters.m_maxIterations > 0, TE_TR("The number of iterations must be at least 1."))
  TERP_TRUE_OR_RETURN_FALSE(m_parameters.m_epsilon > 0, TE_TR("The stop criteria must be higher than 0."))

  if (m_parameters.m_maxInputPoints < m_parameters.m_K)
    m_parameters.m_maxInputPoints = 1000;

  m_isInitialized = true;

  return true;
}

bool te::rp::ClassifierKMeansStrategy::execute(const te::rst::Raster& inputRaster, const std::vector<unsigned int>& inputRasterBands,
                                              const std::vector<te::gm::Polygon*>& inputPolygons, te::rst::Raster& outputRaster,
                                              const unsigned int outputRasterBand, const bool enableProgressInterface) throw(te::rp::Exception)
{
  TERP_TRUE_OR_RETURN_FALSE(m_isInitialized, TE_TR("Instance not initialized"))

// defining classification parameters
  te::cl::KMeans<te::rst::PointSetIterator<double>, te::rst::RasterIterator<double> >::Parameters classifierParameters;
  classifierParameters.m_K = m_parameters.m_K;
  classifierParameters.m_maxIterations = m_parameters.m_maxIterations;
  classifierParameters.m_epsilon = m_parameters.m_epsilon;
  std::vector<unsigned int> classification;

// define point set iterators for training
  std::vector<te::gm::Point*> randomPoints = te::rst::GetRandomPointsInRaster(inputRaster, m_parameters.m_maxInputPoints);
  te::rst::PointSetIterator<double> pit = te::rst::PointSetIterator<double>::begin(&inputRaster, randomPoints);
  te::rst::PointSetIterator<double> pitend = te::rst::PointSetIterator<double>::end(&inputRaster, randomPoints);

// define raster iterators for classification
  te::rst::RasterIterator<double> rit = te::rst::RasterIterator<double>::begin((te::rst::Raster*) &inputRaster, inputRasterBands);
  te::rst::RasterIterator<double> ritend = te::rst::RasterIterator<double>::end((te::rst::Raster*) &inputRaster, inputRasterBands);

// execute the algorithm
  te::cl::KMeans<te::rst::PointSetIterator<double>, te::rst::RasterIterator<double> > classifier;

  if(!classifier.initialize(classifierParameters))
    throw;
  if(!classifier.train(pit, pitend, inputRasterBands, std::vector<unsigned int>(), true))
    throw;
  if(!classifier.classify(rit, ritend, inputRasterBands, classification, true))
    throw;

// classifying image
  te::common::TaskProgress task;
  task.setTotalSteps(inputRaster.getNumberOfColumns() * inputRaster.getNumberOfRows());
  task.setMessage(TE_TR("KMeans algorithm - classifying image"));
  task.setCurrentStep(0);
  unsigned int i = 0;
  while (rit != ritend)
  {
    outputRaster.setValue(rit.getColumn(), rit.getRow(), classification[i], outputRasterBand);
    ++i;
    ++rit;
    task.pulse();
  }

  return true;
}

te::rp::ClassifierKMeansStrategyFactory::ClassifierKMeansStrategyFactory()
  : te::rp::ClassifierStrategyFactory("kmeans")
{
}

te::rp::ClassifierKMeansStrategyFactory::~ClassifierKMeansStrategyFactory()
{
}

te::rp::ClassifierStrategy* te::rp::ClassifierKMeansStrategyFactory::build()
{
  return new te::rp::ClassifierKMeansStrategy();
}
