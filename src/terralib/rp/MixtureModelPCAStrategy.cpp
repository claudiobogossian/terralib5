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
  \file terralib/rp/MixtureModelPCAStrategy.cpp

  \brief PCA (Principal Component Analysis) strategy for mixture model.
*/

// TerraLib
#include "../raster/Grid.h"
#include "../raster/Utils.h"
#include "MixtureModelPCAStrategy.h"
#include "Macros.h"
#include "PositionIterator.h"
#include "RasterAttributes.h"

// STL
#include <complex>
#include <iostream>
#include <set>
#include <stdlib.h>

// Boost
#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/lu.hpp>
#include <boost/numeric/ublas/matrix.hpp>

// STL
#include <cfloat>

namespace
{
  static te::rp::MixtureModelPCAStrategyFactory mixtureModelPCAStrategyFactoryInstance;
}

te::rp::MixtureModelPCAStrategy::Parameters::Parameters()
{
  reset();
}

te::rp::MixtureModelPCAStrategy::Parameters::~Parameters()
{
}

const te::rp::MixtureModelPCAStrategy::Parameters& te::rp::MixtureModelPCAStrategy::Parameters::operator=(const te::rp::MixtureModelPCAStrategy::Parameters& rhs)
{
  reset();

  m_acceptanceThreshold = rhs.m_acceptanceThreshold;

  return *this;
}

void te::rp::MixtureModelPCAStrategy::Parameters::reset() throw(te::rp::Exception)
{
  m_acceptanceThreshold = 0;
}

te::common::AbstractParameters* te::rp::MixtureModelPCAStrategy::Parameters::clone() const
{
  return new te::rp::MixtureModelPCAStrategy::Parameters(*this);
}

te::rp::MixtureModelPCAStrategy::MixtureModelPCAStrategy()
{
  m_isInitialized = false;
}

te::rp::MixtureModelPCAStrategy::~MixtureModelPCAStrategy()
{
}

bool te::rp::MixtureModelPCAStrategy::initialize(te::rp::StrategyParameters const* const strategyParams) throw(te::rp::Exception)
{
  m_isInitialized = false;

  te::rp::MixtureModelPCAStrategy::Parameters const* paramsPtr = dynamic_cast<te::rp::MixtureModelPCAStrategy::Parameters const*>(strategyParams);

  if(!paramsPtr)
    return false;

  m_parameters = *(paramsPtr);

  TERP_TRUE_OR_RETURN_FALSE(m_parameters.m_acceptanceThreshold > 0 && m_parameters.m_acceptanceThreshold <= 100, "Invalid acceptance threshold [0, 100].")

  m_isInitialized = true;

  return true;
}

bool te::rp::MixtureModelPCAStrategy::execute(const te::rst::Raster& inputRaster, const std::vector<unsigned int>& inputRasterBands,
                                              const std::map<std::string, std::vector<double> >& components, te::rst::Raster& outputRaster,
                                              const bool enableProgressInterface) throw(te::rp::Exception)
{
  TERP_TRUE_OR_RETURN_FALSE(m_isInitialized, "Instance not initialized")

  return true;
}

te::rp::MixtureModelPCAStrategyFactory::MixtureModelPCAStrategyFactory()
  : te::rp::MixtureModelStrategyFactory("pca")
{
}

te::rp::MixtureModelPCAStrategyFactory::~MixtureModelPCAStrategyFactory()
{
}

te::rp::MixtureModelStrategy* te::rp::MixtureModelPCAStrategyFactory::build()
{
  return new te::rp::MixtureModelPCAStrategy();
}
