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
#include "../common/MatrixUtils.h"
#include "../common/progress/TaskProgress.h"
#include "../raster/Grid.h"
#include "../raster/Utils.h"
#include "Functions.h"
#include "Macros.h"
#include "MixtureModelLinearStrategy.h"
#include "Functions.h"

// Boost
#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/matrix.hpp>

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

  m_isInitialized = true;

  return true;
}

bool te::rp::MixtureModelLinearStrategy::execute(const te::rst::Raster& inputRaster, const std::vector<unsigned int>& inputRasterBands,
                                                 const std::vector<std::string>& inputSensorBands, const std::map<std::string, std::vector<double> >& components,
                                                 te::rst::Raster& outputRaster, const bool enableProgressInterface) throw(te::rp::Exception)
{
  TERP_TRUE_OR_RETURN_FALSE(m_isInitialized, "Instance not initialized")

  const unsigned int nComponents = components.size();
  const unsigned int nBands = inputRasterBands.size();

  boost::numeric::ublas::matrix<double> transposeA = boost::numeric::ublas::matrix<double>(nComponents + 1, nBands + 1);
  boost::numeric::ublas::matrix<double> matrixA (transposeA.size2(), transposeA.size1());

// by definition, A * X = R, we have A, so that X = inv(A' * A) * A' * R
// A is the set of known reflectances for each component
// R is the reflectances of a specific pixel
// X is the proportion of each component

  std::vector<double> Qmax;
  for (unsigned i = 0; i < inputSensorBands.size(); i++)
    Qmax.push_back(GetDigitalNumberBandMax(inputSensorBands[i]));

// retrieve the transposed A matrix, normalizing values using band/sensor info
  unsigned row = 0;
  for (unsigned j = 0; j < nBands; j++)
    transposeA(row, j) = 1.0;
  transposeA(row, nBands) = 0.0;
  std::map<std::string, std::vector<double> >::const_iterator it;
  for (row = 1, it = components.begin(); it != components.end(); row++, it++)
  {
    for (unsigned j = 0; j < nBands; j++)
      transposeA(row, j) = it->second[j] / Qmax[j];
    transposeA(row, nBands) = 1.0;
  }

// calculate the matrixA from the transpose of A
  matrixA = boost::numeric::ublas::trans(transposeA);

// calculate the product A' * A
  boost::numeric::ublas::matrix<double> productAtA = boost::numeric::ublas::matrix<double>(transposeA.size1(), matrixA.size2());
  productAtA = boost::numeric::ublas::prod(transposeA, matrixA);

// calculate the inverse of A' * A
  boost::numeric::ublas::matrix<double> invertAtA = boost::numeric::ublas::matrix<double>(productAtA.size1(), productAtA.size2());
  te::common::GetInverseMatrix(productAtA, invertAtA);

  boost::numeric::ublas::matrix<double> matrixR = boost::numeric::ublas::matrix<double>(matrixA.size1(), 1);
  boost::numeric::ublas::matrix<double> productAtR = boost::numeric::ublas::matrix<double>(transposeA.size1(), matrixR.size2());
  boost::numeric::ublas::matrix<double> matrixX = boost::numeric::ublas::matrix<double>(invertAtA.size1(), productAtR.size2());
  boost::numeric::ublas::matrix<double> productAX = boost::numeric::ublas::matrix<double>(matrixA.size1(), matrixX.size2());
  boost::numeric::ublas::matrix<double> matrixE = boost::numeric::ublas::matrix<double>(matrixR.size1(), matrixR.size2());

  te::common::TaskProgress task(TR_RP("Linear Mixture Model"), te::common::TaskProgress::UNDEFINED, inputRaster.getNumberOfRows());
  double value;
  for (unsigned r = 0; r < inputRaster.getNumberOfRows(); r++)
  {
    for (unsigned c = 0; c < inputRaster.getNumberOfColumns(); c++)
    {
// read input values
      for (unsigned b = 0; b < nBands; b++)
      {
        inputRaster.getValue(c, r, value, inputRasterBands[b]);
        matrixR(b, 0) = value / Qmax[b];
      }
      matrixR(nBands, 0) = 0.0;
// calculate the product A' * R
      productAtR = boost::numeric::ublas::prod(transposeA, matrixR);
// compute matrix X
      matrixX = boost::numeric::ublas::prod(invertAtA, productAtR);
// write output fractions
      for (unsigned b = 0; b < nComponents; b++)
        outputRaster.setValue(c, r, matrixX(b + 1, 0), b);

// compute error matrix
      if (nComponents < outputRaster.getNumberOfBands())
      {
        productAX = boost::numeric::ublas::prod(matrixA, matrixX);
        matrixE = matrixR - productAX;

// write output errors
        for (unsigned b = nComponents, e = 0; b < outputRaster.getNumberOfBands(); b++, e++)
          outputRaster.setValue(c, r, matrixE(e, 0), b);
      }
    }
    task.pulse();
  }

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
