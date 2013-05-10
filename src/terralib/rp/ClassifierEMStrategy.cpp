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
  \file terralib/rp/ClassifierEMStrategy.cpp

  \brief EM (Expectation-Maximization) strategy for pixel-based classification.
*/

// TerraLib
#include "../raster/Grid.h"
#include "../raster/PositionIterator.h"
#include "../raster/Utils.h"
#include "ClassifierEMStrategy.h"
#include "Functions.h"
#include "Macros.h"

// STL
#include <complex>
#include <iostream>

// Boost
#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/lu.hpp>
#include <boost/numeric/ublas/matrix.hpp>

namespace
{
  static te::rp::ClassifierEMStrategyFactory classifierEMStrategyFactoryInstance;
}

te::rp::ClassifierEMStrategy::Parameters::Parameters()
{
  reset();
}

te::rp::ClassifierEMStrategy::Parameters::~Parameters()
{
}

const te::rp::ClassifierEMStrategy::Parameters& te::rp::ClassifierEMStrategy::Parameters::operator=(const te::rp::ClassifierEMStrategy::Parameters& rhs)
{
  reset();

  m_numberOfClusters = rhs.m_numberOfClusters;
  m_maxIterations = rhs.m_maxIterations;
  m_epsilon = rhs.m_epsilon;
  m_clustersMeans = rhs.m_clustersMeans;

  return *this;
}

void te::rp::ClassifierEMStrategy::Parameters::reset() throw(te::rp::Exception)
{
  m_numberOfClusters = 0;
  m_maxIterations = 0;
  m_epsilon = 0.0;
  m_clustersMeans.clear();
}

te::common::AbstractParameters* te::rp::ClassifierEMStrategy::Parameters::clone() const
{
  return new te::rp::ClassifierEMStrategy::Parameters(*this);
}

te::rp::ClassifierEMStrategy::ClassifierEMStrategy()
{
  m_isInitialized = false;
}

te::rp::ClassifierEMStrategy::~ClassifierEMStrategy()
{
}

bool te::rp::ClassifierEMStrategy::initialize(te::rp::StrategyParameters const* const strategyParams) throw(te::rp::Exception)
{
  m_isInitialized = false;

  te::rp::ClassifierEMStrategy::Parameters const* paramsPtr = dynamic_cast<te::rp::ClassifierEMStrategy::Parameters const*>(strategyParams);

  if(!paramsPtr)
    return false;

  m_parameters = *(paramsPtr);

  TERP_TRUE_OR_RETURN_FALSE(m_parameters.m_numberOfClusters > 1, "The number of clusters must be at least 2.")
  TERP_TRUE_OR_RETURN_FALSE(m_parameters.m_maxIterations > 0, "The number of iterations must be at least 1.")
  TERP_TRUE_OR_RETURN_FALSE(m_parameters.m_epsilon > 0, "The stop criteria must be higher than 0.")

  m_isInitialized = true;

  return true;
}

bool te::rp::ClassifierEMStrategy::execute(const te::rst::Raster& inputRaster, const std::vector<unsigned int>& inputRasterBands,
                                           const std::vector<te::gm::Polygon*>& inputPolygons, te::rst::Raster& outputRaster,
                                           const unsigned int outputRasterBand, const bool enableProgressInterface) throw(te::rp::Exception)
{
  TERP_TRUE_OR_RETURN_FALSE(m_isInitialized, "Instance not initialized")

  unsigned int c;
  unsigned int r;
  unsigned int pattern;
  const unsigned int nrows = outputRaster.getNumberOfRows();
  const unsigned int ncols = outputRaster.getNumberOfColumns();

  for(r = 0; r < nrows; r++)
  {
    for(c = 0; c < ncols; c++)
    {
// generate random classification according to m_numberOfClusters
      pattern = rand() % m_parameters.m_numberOfClusters + 1;

      outputRaster.setValue(c, r, pattern, outputRasterBand);
    }
  }

  return true;
}

te::rp::ClassifierEMStrategyFactory::ClassifierEMStrategyFactory()
  : te::rp::ClassifierStrategyFactory("em")
{
}

te::rp::ClassifierEMStrategyFactory::~ClassifierEMStrategyFactory()
{
}

te::rp::ClassifierStrategy* te::rp::ClassifierEMStrategyFactory::build()
{
  return new te::rp::ClassifierEMStrategy();
}
