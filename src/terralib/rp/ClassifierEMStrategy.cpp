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
  \file terralib/rp/ClassifierEMStrategy.cpp

  \brief EM (Expectation-Maximization) strategy for pixel-based classification.
*/

// TerraLib
#include "../common/MatrixUtils.h"
#include "../common/progress/TaskProgress.h"
#include "../raster/Grid.h"
#include "../raster/PositionIterator.h"
#include "../raster/RasterIterator.h"
#include "../raster/Utils.h"
#include "ClassifierEMStrategy.h"
#include "Macros.h"
#include "Functions.h"

// STL
#include <complex>
#include <iostream>
#include <map>

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
  m_maxInputPoints = rhs.m_maxInputPoints;
  m_epsilon = rhs.m_epsilon;
  m_clustersMeans = rhs.m_clustersMeans;

  return *this;
}

void te::rp::ClassifierEMStrategy::Parameters::reset() throw(te::rp::Exception)
{
  m_numberOfClusters = 0;
  m_maxIterations = 0;
  m_maxInputPoints = 0;
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

std::vector< int > te::rp::ClassifierEMStrategy::getOutputDataTypes() const
{
  std::vector< int > dt;
  dt.push_back( te::dt::UINT32_TYPE );
  return dt;
}

bool te::rp::ClassifierEMStrategy::initialize(te::rp::ClassifierStrategyParameters const* const strategyParams) throw(te::rp::Exception)
{
  m_isInitialized = false;

  te::rp::ClassifierEMStrategy::Parameters const* paramsPtr = dynamic_cast<te::rp::ClassifierEMStrategy::Parameters const*>(strategyParams);

  if(!paramsPtr)
    return false;

  m_parameters = *(paramsPtr);

  TERP_TRUE_OR_RETURN_FALSE(m_parameters.m_numberOfClusters > 1, TE_TR("The number of clusters must be at least 2."))
  TERP_TRUE_OR_RETURN_FALSE(m_parameters.m_maxIterations > 0, TE_TR("The number of iterations must be at least 1."))
  if (m_parameters.m_maxInputPoints == 0)
    m_parameters.m_maxInputPoints = 1000;
  TERP_TRUE_OR_RETURN_FALSE(m_parameters.m_maxInputPoints >= m_parameters.m_numberOfClusters, TE_TR("The maximum number of points must be at least higher than the number of clusters."))
  TERP_TRUE_OR_RETURN_FALSE(m_parameters.m_epsilon > 0, TE_TR("The stop criteria must be higher than 0."))

  m_isInitialized = true;

  return true;
}

bool te::rp::ClassifierEMStrategy::execute(const te::rst::Raster& inputRaster, const std::vector<unsigned int>& inputRasterBands,
                                           const std::vector<te::gm::Polygon*>& inputPolygons, te::rst::Raster& outputRaster,
                                           const unsigned int outputRasterBand, const bool enableProgressInterface) throw(te::rp::Exception)
{
  TERP_TRUE_OR_RETURN_FALSE(m_isInitialized, TE_TR("Instance not initialized"))

// create a vector of points with random positions inside raster to obtain input data
  std::vector<te::gm::Point*> randomPoints = te::rst::GetRandomPointsInRaster(inputRaster, m_parameters.m_maxInputPoints);

// M is the number of clusters
  const unsigned int M = m_parameters.m_numberOfClusters;
// N is the number of vectors used to estimate the probabilities
  const unsigned int N = randomPoints.size();
// S is the number of elements inside each vector
  const unsigned int S = inputRasterBands.size();

// get the input data
  boost::numeric::ublas::matrix<double> Xk = boost::numeric::ublas::matrix<double>(N, S);

  te::rst::PointSetIterator<double> pit = te::rst::PointSetIterator<double>::begin(&inputRaster, randomPoints);
  te::rst::PointSetIterator<double> pitend = te::rst::PointSetIterator<double>::end(&inputRaster, randomPoints);
  unsigned int k = 0;
  double max_pixel_value = 0.0;
  while (pit != pitend)
  {
    for (unsigned int l = 0; l < S; l++)
    {
      inputRaster.getValue(pit.getColumn(), pit.getRow(), Xk(k, l), inputRasterBands[l]);
      if (Xk(k, l) > max_pixel_value)
        max_pixel_value = Xk(k, l);
    }

    ++k;
    ++pit;
  }

  srand((unsigned) time(0));
// the parameter vector of means for each cluster
  boost::numeric::ublas::matrix<double> MUj = boost::numeric::ublas::matrix<double>(M, S);
  boost::numeric::ublas::matrix<double> previous_MUj = boost::numeric::ublas::matrix<double>(M, S);
  if (m_parameters.m_clustersMeans.size() > 0)
  {
    for (unsigned int j = 0; j < M; j++)
      for (unsigned int l = 0; l < S; l++)
        MUj(j, l) = m_parameters.m_clustersMeans[j][l];
  }
  else
  {
// define vector of means randomly, in the interval [0, max_pixel_value].
    for (unsigned int j = 0; j < M; j++)
      for (unsigned int l = 0; l < S; l++)
        MUj(j, l) = rand() % (int) ceil(max_pixel_value);
  }
  previous_MUj = MUj;

// the parameter vector of covariance matrices for each cluster
  std::vector<boost::numeric::ublas::matrix<double> > SIGMAj;
  for (unsigned int j = 0; j < M; j++)
  {
    boost::numeric::ublas::matrix<double> tmp_sigma(S, S);
    for (unsigned int l = 0; l < S; l++)
    {
      for (unsigned int m = 0; m < S; m++)
        tmp_sigma(l, m) = 0.0;
      tmp_sigma(l, l) = 10.0;
    }
    SIGMAj.push_back(tmp_sigma);
  }

// variables used to estimate the cluster's probabilities
  boost::numeric::ublas::matrix<double> Pj = boost::numeric::ublas::matrix<double>(M, 1);
  boost::numeric::ublas::matrix<double> PCj_Xk = boost::numeric::ublas::matrix<double>(M, N);
  for (unsigned int j = 0; j < M; j++)
  {
    Pj(j, 0) = 1 / (double) M;
    for (unsigned int k = 0; k < N; k++)
      PCj_Xk(j, k) = 0.0;
  }

// estimating cluster's probabilities
  double sum_PCj_Xk;
  double numerator_PCj_Xk;
  double denominator_PCj_Xk;
  double det_SIGMAj;
  double det_SIGMAj2;
  double distance_MUj;

  boost::numeric::ublas::matrix<double> Xk_minus_MUj(S, 1);
  boost::numeric::ublas::matrix<double> Xk_minus_MUj_T(1, S);
  boost::numeric::ublas::matrix<double> product_NETAj(1, 1);
  boost::numeric::ublas::matrix<double> product_NETAj2(1, 1);
  boost::numeric::ublas::matrix<double> inverse_SIGMAj(S, S);
  boost::numeric::ublas::matrix<double> inverse_SIGMAj2(S, S);
  boost::numeric::ublas::matrix<double> product_Xk_minusMUj(S, S);
  boost::numeric::ublas::matrix<double> sum_product_Xk_minusMUj(S, S);

  te::common::TaskProgress task(TE_TR("Expectation Maximization algorithm - estimating clusters"), te::common::TaskProgress::UNDEFINED, m_parameters.m_maxIterations);
  for (unsigned int i = 0; i < m_parameters.m_maxIterations; i++)
  {
// computing PCj_Xk
    for (unsigned int j = 0; j < M; j++)
    {
      te::common::GetDeterminant(SIGMAj[j], det_SIGMAj);
      if (det_SIGMAj >= 0.0)
        det_SIGMAj = pow(det_SIGMAj, -0.5);
      else
        det_SIGMAj = 1.0;

      for (unsigned int k = 0; k < N; k++)
      {
// computing numerator of equation for PCj_Xk
        numerator_PCj_Xk = 0.0;
        for (unsigned int l = 0; l < S; l++)
          Xk_minus_MUj(l, 0) = Xk(k, l) - MUj(j, l);
        Xk_minus_MUj_T = boost::numeric::ublas::trans(Xk_minus_MUj);

        te::common::GetInverseMatrix(SIGMAj[j], inverse_SIGMAj);

        product_NETAj = prod(Xk_minus_MUj_T, inverse_SIGMAj);
        product_NETAj = prod(product_NETAj, Xk_minus_MUj);
        product_NETAj *= -0.5;

        numerator_PCj_Xk = det_SIGMAj * exp(product_NETAj(0, 0)) * Pj(j, 0);

// computing denominator of equation for PCj_Xk
        denominator_PCj_Xk = 0.0;
        for (unsigned int j2 = 0; j2 < M; j2++)
        {
          te::common::GetDeterminant(SIGMAj[j2], det_SIGMAj2);
          if (det_SIGMAj2 >= 0.0)
            det_SIGMAj2 = pow(det_SIGMAj2, -0.5);
          else
            det_SIGMAj2 = 1.0;

          for (unsigned int l = 0; l < S; l++)
            Xk_minus_MUj(l, 0) = Xk(k, l) - MUj(j2, l);
          Xk_minus_MUj_T = boost::numeric::ublas::trans(Xk_minus_MUj);

          te::common::GetInverseMatrix(SIGMAj[j2], inverse_SIGMAj2);

          product_NETAj2 = prod(Xk_minus_MUj_T, inverse_SIGMAj2);
          product_NETAj2 = prod(product_NETAj2, Xk_minus_MUj);
          product_NETAj2 *= -0.5;

          denominator_PCj_Xk += det_SIGMAj2 * exp(product_NETAj2(0, 0)) * Pj(j2, 0);
        }
        if (denominator_PCj_Xk == 0.0)
          denominator_PCj_Xk = 0.0000000001;

// computing PCj_Xk
        PCj_Xk(j, k) = numerator_PCj_Xk / denominator_PCj_Xk;
      }
    }

// computing SIGMAj for t + 1
    for (unsigned int j = 0; j < M; j++)
    {
      sum_PCj_Xk = 0.0;
      for (unsigned int l = 0; l < S; l++)
        for (unsigned int l2 = 0; l2 < S; l2++)
          sum_product_Xk_minusMUj(l, l2) = 0.0;
      for (unsigned int k = 0; k < N; k++)
      {
        sum_PCj_Xk += PCj_Xk(j, k);

        for (unsigned int l = 0; l < S; l++)
          Xk_minus_MUj(l, 0) = Xk(k, l) - MUj(j, l);
        Xk_minus_MUj_T = boost::numeric::ublas::trans(Xk_minus_MUj);

        product_Xk_minusMUj = prod(Xk_minus_MUj, Xk_minus_MUj_T);
        product_Xk_minusMUj *= PCj_Xk(j, k);

        sum_product_Xk_minusMUj += product_Xk_minusMUj;
      }
      if (sum_PCj_Xk == 0.0)
        sum_PCj_Xk = 0.0000000001;
      SIGMAj[j] = sum_product_Xk_minusMUj / sum_PCj_Xk;
    }

// computing MUj for t + 1
    for (unsigned int j = 0; j < M; j++)
    {
      sum_PCj_Xk = 0.0;
      for (unsigned int l = 0; l < S; l++)
        MUj(j, l) = 0.0;
      for (unsigned int k = 0; k < N; k++)
      {
        for (unsigned int l = 0; l < S; l++)
          MUj(j, l) += PCj_Xk(j, k) * Xk(k, l);
        sum_PCj_Xk += PCj_Xk(j, k);
      }
      if (sum_PCj_Xk == 0.0)
        sum_PCj_Xk = 0.0000000001;
      for (unsigned int l = 0; l < S; l++)
        MUj(j, l) /= sum_PCj_Xk;
    }

// computing Pj for t + 1
    for (unsigned int j = 0; j < M; j++)
    {
      Pj(j, 0) = 0.0;
      for (unsigned int k = 0; k < N; k++)
        Pj(j, 0) += PCj_Xk(j, k);
      Pj(j, 0) /= N;
    }

// checking convergence
    distance_MUj = 0.0;
    double a_minus_b;
    for (unsigned int j = 0; j < M; j++)
      for (unsigned int l = 0; l < S; l++)
      {
        a_minus_b = MUj(j, l) - previous_MUj(j, l);
        distance_MUj += a_minus_b * a_minus_b;
      }
    distance_MUj = sqrt(distance_MUj);
    if (distance_MUj < m_parameters.m_epsilon)
      break;
    previous_MUj = MUj;

    task.pulse();
  }

// classifying image
  te::rst::RasterIterator<double> it = te::rst::RasterIterator<double>::begin((te::rst::Raster*) &inputRaster, inputRasterBands);
  te::rst::RasterIterator<double> itend = te::rst::RasterIterator<double>::end((te::rst::Raster*) &inputRaster, inputRasterBands);

  boost::numeric::ublas::matrix<double> X = boost::numeric::ublas::matrix<double>(1, S);
  boost::numeric::ublas::matrix<double> PCj_X = boost::numeric::ublas::matrix<double>(M, 1);

  double max_PCj_X;
  unsigned int cluster;

  double numerator_PCj_X;
  double denominator_PCj_X;

  boost::numeric::ublas::matrix<double> X_minus_MUj(S, 1);
  boost::numeric::ublas::matrix<double> X_minus_MUj_T(1, S);

  task.setTotalSteps(inputRaster.getNumberOfColumns() * inputRaster.getNumberOfRows());
  task.setMessage(TE_TR("Expectation Maximization algorithm - classifying image"));
  task.setCurrentStep(0);
  while (it != itend)
  {
    for (unsigned int l = 0; l < S; l++)
      X(0, l) = (*it)[l];

// computing PCj_X
    for (unsigned int j = 0; j < M; j++)
    {
// computing numerator of equation for PCj_X
      te::common::GetDeterminant(SIGMAj[j], det_SIGMAj);
      if (det_SIGMAj >= 0.0)
        det_SIGMAj = pow(det_SIGMAj, -0.5);
      else
        det_SIGMAj = 1.0;

      numerator_PCj_X = 0.0;
      for (unsigned int l = 0; l < S; l++)
        X_minus_MUj(l, 0) = X(0, l) - MUj(j, l);
      X_minus_MUj_T = boost::numeric::ublas::trans(X_minus_MUj);

      te::common::GetInverseMatrix(SIGMAj[j], inverse_SIGMAj);

      product_NETAj = prod(X_minus_MUj_T, inverse_SIGMAj);
      product_NETAj = prod(product_NETAj, X_minus_MUj);
      product_NETAj *= -0.5;

      numerator_PCj_X = det_SIGMAj * exp(product_NETAj(0, 0)) * Pj(j, 0);

// computing denominator of equation for PCj_X
      denominator_PCj_X = 0.0;
      for (unsigned int j2 = 0; j2 < M; j2++)
      {
        te::common::GetDeterminant(SIGMAj[j2], det_SIGMAj2);
        if (det_SIGMAj2 >= 0.0)
          det_SIGMAj2 = pow(det_SIGMAj2, -0.5);
        else
          det_SIGMAj2 = 1.0;

        for (unsigned int l = 0; l < S; l++)
          X_minus_MUj(l, 0) = X(0, l) - MUj(j2, l);
        X_minus_MUj_T = boost::numeric::ublas::trans(X_minus_MUj);

        te::common::GetInverseMatrix(SIGMAj[j2], inverse_SIGMAj2);

        product_NETAj2 = prod(X_minus_MUj_T, inverse_SIGMAj2);
        product_NETAj2 = prod(product_NETAj2, X_minus_MUj);
        product_NETAj2 *= -0.5;

        denominator_PCj_X += det_SIGMAj2 * exp(product_NETAj2(0, 0)) * Pj(j2, 0);
      }
      if (denominator_PCj_X == 0.0)
        denominator_PCj_X = 0.0000000001;

      PCj_X(j, 0) = numerator_PCj_X / denominator_PCj_X;
    }

    max_PCj_X = 0.0;
    cluster = 0;
    for (unsigned int j = 0; j < M; j++)
      if (PCj_X(j, 0) > max_PCj_X)
      {
        max_PCj_X = PCj_X(j, 0);
        cluster = j + 1;
      }

// save cluster information in output raster
    outputRaster.setValue(it.getColumn(), it.getRow(), cluster, outputRasterBand);

    ++it;
    task.pulse();
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
