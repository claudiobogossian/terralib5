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
  \file terralib/rp/MixtureModelLinearStrategy.cpp

  \brief Raster linear strategy for mixture model classification.
*/

// TerraLib
#include "../raster/Grid.h"
#include "../raster/Utils.h"
#include "Functions.h"
#include "MixtureModelLinearStrategy.h"
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
  static te::rp::MixtureModelLinearStrategyFactory mixtureModelLinearStrategyFactoryInstance;
}

te::rp::MixtureModelLinearStrategy::Parameters::Parameters()
{
  reset();
}

te::rp::MixtureModelLinearStrategy::Parameters::~Parameters()
{
}

const te::rp::MixtureModelLinearStrategy::Parameters& te::rp::MixtureModelLinearStrategy::Parameters::operator=(const te::rp::MixtureModelLinearStrategy::Parameters& rhs)
{
  reset();

  return *this;
}

void te::rp::MixtureModelLinearStrategy::Parameters::reset() throw(te::rp::Exception)
{
}

te::common::AbstractParameters* te::rp::MixtureModelLinearStrategy::Parameters::clone() const
{
  return new te::rp::MixtureModelLinearStrategy::Parameters(*this);
}

te::rp::MixtureModelLinearStrategy::MixtureModelLinearStrategy()
{
  m_isInitialized = false;
}

te::rp::MixtureModelLinearStrategy::~MixtureModelLinearStrategy()
{
}

bool te::rp::MixtureModelLinearStrategy::initialize(te::rp::StrategyParameters const* const strategyParams) throw(te::rp::Exception)
{
  m_isInitialized = false;

  te::rp::MixtureModelLinearStrategy::Parameters const* paramsPtr = dynamic_cast<te::rp::MixtureModelLinearStrategy::Parameters const*>(strategyParams);

  if(!paramsPtr)
    return false;

  m_parameters = *(paramsPtr);

  // TERP_TRUE_OR_RETURN_FALSE(m_parameters.m_acceptanceThreshold > 0 && m_parameters.m_acceptanceThreshold <= 100, "Invalid acceptance threshold [0, 100].")

  m_isInitialized = true;

  return true;
}

bool te::rp::MixtureModelLinearStrategy::execute(const te::rst::Raster& inputRaster, const std::vector<unsigned int>& inputRasterBands,
                                                 const std::map<std::string, std::vector<double> >& components, te::rst::Raster& outputRaster,
                                                 const bool enableProgressInterface) throw(te::rp::Exception)
{
  TERP_TRUE_OR_RETURN_FALSE(m_isInitialized, "Instance not initialized")

  const unsigned int nComponents = components.size();
  boost::numeric::ublas::matrix<double> matrixA (nComponents, nComponents);

  std::map<std::string, std::vector<double> >::const_iterator it;
  unsigned i;
  for (i = 0, it = components.begin(); it != components.end(); it++, i++)
    for (unsigned j = 0; j < matrixA.size2 (); j++)
      matrixA(i, j) = it->second[j];

  std::cout << matrixA << std::endl;

  boost::numeric::ublas::matrix<double> invertA = boost::numeric::ublas::matrix<double>(matrixA.size1(), matrixA.size2());

  InvertMatrix(matrixA, invertA);

  return true;
}

te::rp::MixtureModelLinearStrategyFactory::MixtureModelLinearStrategyFactory()
  : te::rp::MixtureModelStrategyFactory("linear")
{
}

te::rp::MixtureModelLinearStrategyFactory::~MixtureModelLinearStrategyFactory()
{
}

te::rp::MixtureModelStrategy* te::rp::MixtureModelLinearStrategyFactory::build()
{
  return new te::rp::MixtureModelLinearStrategy();
}
