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
  \file terralib/rp/MixtureModelPCAStrategy.cpp

  \brief PCA (Principal Component Analysis) strategy for mixture model.
*/

// TerraLib
#include "../common/MatrixUtils.h"
#include "../common/progress/TaskProgress.h"
#include "../raster/Grid.h"
#include "../raster/Utils.h"
#include "Functions.h"
#include "Macros.h"
#include "MixtureModelPCAStrategy.h"
#include "Functions.h"

// Boost
#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/matrix.hpp>

bool gaussElimination(boost::numeric::ublas::matrix<double>& Z, std::vector<int>& row, unsigned int nComponents);
bool fowardBackSubstitution(boost::numeric::ublas::matrix<double>& Z, std::vector<double>& y, unsigned int ys, std::vector<int>& row, unsigned int rows, std::vector<double>& x, unsigned int xs);

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

  return *this;
}

void te::rp::MixtureModelPCAStrategy::Parameters::reset() throw(te::rp::Exception)
{
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

  // TERP_TRUE_OR_RETURN_FALSE(m_parameters.m_acceptanceThreshold > 0 && m_parameters.m_acceptanceThreshold <= 100, "Invalid acceptance threshold [0, 100].")

  m_isInitialized = true;

  return true;
}

bool te::rp::MixtureModelPCAStrategy::execute(const te::rst::Raster& inputRaster, const std::vector<unsigned int>& inputRasterBands,
                                              const std::vector<std::string>& inputSensorBands, const std::map<std::string, std::vector<double> >& components,
                                              te::rst::Raster& outputRaster, const bool enableProgressInterface) throw(te::rp::Exception)
{
// this source code is based on TerraLib 4
  TERP_TRUE_OR_RETURN_FALSE(m_isInitialized, "Instance not initialized")

  const unsigned int nComponents = components.size();
  const unsigned int nBands = inputSensorBands.size();

  bool computeErrorRasters = false;
  if (nComponents < outputRaster.getNumberOfBands())
    computeErrorRasters = true;

// indexes variables
  unsigned int i, j, k, m;

  std::vector<double> Qmax;
  for (i = 0; i < inputSensorBands.size(); i++)
    Qmax.push_back(GetDigitalNumberBandMax(inputSensorBands[i]));

// buildind the SpectralBandsComponents Matrix
  boost::numeric::ublas::matrix<double> SpectralBandsComponents(nBands, nComponents);
  std::map<std::string, std::vector<double> >::const_iterator it;
  for(i = 0; i < nBands; i++)
    for (j = 0, it = components.begin(); it != components.end(); j++, it++)
      SpectralBandsComponents(i, j) = it->second[i] / Qmax[i];

// Part I : mathematical processing independend on image data

// SpectralBandsComponentsTransposed stores SpectralBandsComponents and other things after
  boost::numeric::ublas::matrix<double> SpectralBandsComponentsTransposed(nComponents, nBands);

// Initializing SpectralBandsComponentsTransposed Matrix = Transpose of SpectralBandsComponents
  SpectralBandsComponentsTransposed = boost::numeric::ublas::trans(SpectralBandsComponents);

// compute componentsMean vector as the mean coefficient value per band and subtract the componentsMeanAfter from the coefficients matrix
// creating two double vectors to store de means of the spectralBands of each component
  std::vector<double> componentsMean(nBands, 0.0);
  std::vector<double> componentsMeanAfter(nBands, 0.0);

  for (j = 0; j < nBands; j++)
  {
// compute original mean (componentsMean)
    componentsMean[j] = 0;
    for(i = 0; i < nComponents; i++)
      componentsMean[j] = componentsMean[j] + SpectralBandsComponentsTransposed(i, j);
    componentsMean[j] = componentsMean[j] / (double) nComponents;

// subtract mean from SpectralBandsComponents and compute new matrix mean (componentsMeanAfter)
    componentsMeanAfter[j] = 0;
    for(i = 0; i < nComponents; i++)
    {
      SpectralBandsComponentsTransposed(i, j) = SpectralBandsComponentsTransposed(i, j) - componentsMean[j];
      componentsMeanAfter[j] = componentsMeanAfter[j] + SpectralBandsComponentsTransposed(i, j);
    }
    componentsMeanAfter[j] = componentsMeanAfter[j] / (double) nComponents;
  }

// compute covarianceVector vector
  std::vector<double> covarianceVector(nBands * nBands, 0.0);
  for(k = 0; k < nBands * nBands; k++)
    covarianceVector[k] = 0;

  k = 0 ;
  for(j = 0; j < nBands; j++)
    for(m = 0; m < j + 1; m++)
    {
      for(i = 0; i < nComponents; i++)
        covarianceVector[k] = covarianceVector[k] + (SpectralBandsComponentsTransposed(i, j) - componentsMeanAfter[j]) *
                                                    (SpectralBandsComponentsTransposed(i, m) - componentsMeanAfter[m]);
      covarianceVector[k] = covarianceVector[k] / (double) nComponents;

      k++;
    }

  k = 0;
  boost::numeric::ublas::matrix<double> covarianceMatrix(nBands, nBands);
  for (i = 0; i < nBands; i++)
    for (j = 0; j <= i; j++)
      covarianceMatrix(i, j) = covarianceVector[k++];

// compute eigenvectors -> auxMatrix and eigenvalues -> tmpMatrix
  boost::numeric::ublas::matrix<double> auxMatrix(covarianceMatrix);
  boost::numeric::ublas::matrix<double> tmpMatrix;

  if(!te::common::EigenVectors(covarianceMatrix, auxMatrix, tmpMatrix))
    return false; // print error in eigenvectors

// keep only significant eigenvectors (nComponents - 1): results eigenReduced
  boost::numeric::ublas::matrix<double> eigenReduced(nBands, nComponents - 1);
  for (j = 0; j < nBands; j++)
    for (i = 0; i < nComponents - 1; i++)
      eigenReduced(j, i) = auxMatrix(j, i);

// rotate SpectralBandsComponentsTransposed to PCA space; result SpectralBandsComponentsPCA
  boost::numeric::ublas::matrix<double> SpectralBandsComponentsPCA(SpectralBandsComponentsTransposed.size1(), eigenReduced.size2()); // (nrow x ncol)???
  SpectralBandsComponentsPCA = boost::numeric::ublas::prod(SpectralBandsComponentsTransposed, eigenReduced);

// Initialize one more column to SpectralBandsComponentsPCA to incorporate sum restriction to equations; results auxMatrix
  auxMatrix.resize(SpectralBandsComponentsPCA.size1(), SpectralBandsComponentsPCA.size2() + 1); // nrow, ncol + 1 ???
  auxMatrix.clear();

  for (j = 0; j < SpectralBandsComponentsPCA.size1(); j++) // nrow
  {
    for (i = 0; i < SpectralBandsComponentsPCA.size2(); i++) // ncol
      auxMatrix(j, i) = SpectralBandsComponentsPCA(j, i);

    auxMatrix(j, SpectralBandsComponentsPCA.size2()) = 1.0;
  }

// Transpose auxMatrix; results SpectralBandsComponentsPCA
  SpectralBandsComponentsPCA.clear();
  SpectralBandsComponentsPCA = boost::numeric::ublas::trans(auxMatrix);

// invert matrix SpectralBandsComponentsPCA; results SpectralBandsComponentsPCA
  std::vector<int> rows(nComponents, 0);

  if (!gaussElimination(SpectralBandsComponentsPCA, rows, nComponents))
    return false; // print "Error in gauss elimination");

// Part II : mathematical processing dependend on image data

// Initialize matrices that will help to prepare vector Y
  auxMatrix.resize(1, nBands);
  auxMatrix.clear();
  boost::numeric::ublas::matrix<double> ymat(nComponents - 1, nComponents - 1);

// initialize proportion vector X and image dependent values for the linear equations Y
  std::vector<double> x(nComponents, 0.0);
  std::vector<double> y(nComponents, 0.0);

  unsigned int prop;
  std::vector<unsigned int> rowsIn(nBands, 0);
  std::vector<unsigned int> columnIn(nBands, 0);
  std::vector<unsigned int> columnsIn(nBands, 0);
  std::vector<double> spectralBandsError(nBands, 0.0);

  boost::numeric::ublas::matrix<double> rasterRowsIn(nBands, inputRaster.getNumberOfColumns());
  boost::numeric::ublas::matrix<double> rasterRowsOut(nComponents, inputRaster.getNumberOfColumns());

  te::common::TaskProgress task(TE_TR("PCA Mixture Model"), te::common::TaskProgress::UNDEFINED, inputRaster.getNumberOfRows());
  double value;
  for (unsigned int rowout = 0; rowout < inputRaster.getNumberOfRows(); rowout++)
  {
// read input row for each band
    for (i = 0; i < nBands; i++)
    {
      for (j = 0; j < inputRaster.getNumberOfColumns(); j++)
      {
        inputRaster.getValue(j, rowsIn[i], value, inputRasterBands[i]);
        rasterRowsIn(i, j) = value;
      }

// update next row to be read for band i
      rowsIn[i]++;

// reinitialize first column for band i
      columnIn[i] = columnsIn[i];
    }

// compute proportions for each column
    for (unsigned int colout = 0; colout < inputRaster.getNumberOfColumns(); colout++)
    {
// prepare y
      for (i = 0; i < nBands; i++)
        auxMatrix(0, i) = (double) (rasterRowsIn(i, columnIn[i])) / 255.0 - componentsMean[i];

      ymat = boost::numeric::ublas::prod(auxMatrix, eigenReduced);

      for (i = 0; i < nComponents - 1; i++)
        y[i] = ymat(0, i);

// compute proportions
      if (!fowardBackSubstitution(SpectralBandsComponentsPCA, y, nComponents, rows, nComponents, x, nComponents))
        return false; // print "error in function fowardBackSubstitution"

// store proportion in buffers
      for (i = 0; i < nComponents; i++)
      {
        prop = (short) (x[i] * 100 + 100);
        rasterRowsOut(i, colout) = (unsigned char) prop;
      }

      double aux;
      double error;

// verify if it is necessary to compute the error
      if (computeErrorRasters)
      {
        for (i = 0; i < nBands; i++)
        {
// compute digital value from the proportions
          aux = 0.0;
          for (j = 0; j < nComponents; j++)
            aux += x[j] * SpectralBandsComponents(i, j);

// compute error as module of the difference between the original value and aux
          error = (long) (rasterRowsIn(i, columnIn[i])) / 255.0 - aux;
          if (error < 0)
            error *= -1;

          if (computeErrorRasters)
            rasterRowsIn(i, colout) = error * 255.0;
        }
      }

// update current column number
      for (i = 0; i < nBands; i++)
        columnIn[i]++;

    }

// write processed row to disk
    for (i = 0; i < nComponents; i++)
      for (j = 0; j < inputRaster.getNumberOfColumns(); j++)
        outputRaster.setValue(j, rowout, rasterRowsOut(i, j), i);

// store output error rasters
    if (computeErrorRasters)
    {
      for (i = 0; i < nComponents; i++)
        for (j = 0; j < inputRaster.getNumberOfColumns(); j++)
          outputRaster.setValue(j, rowout, rasterRowsIn(i, j), i + nComponents); // ???
    }
    task.pulse();
  }

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

bool gaussElimination(boost::numeric::ublas::matrix<double>& Z, std::vector<int>& row, unsigned int nComponents)
{
// verify matrix and vector sizes
  assert(Z.size1() >= Z.size2());
  unsigned int nrows = Z.size1(); // rows = size2?
  assert(nComponents >= nrows);

  unsigned int i;
  unsigned int j;
  unsigned int k;
  unsigned int aux;
  double auxd;
  double m;

// initialize changed rows indicator
  for (i = 0; i < nrows; i++)
    row[i] = i;

// diagonalization
  for (k = 0; k < nrows; k++)
  {
    i = k;
    while (Z(i, k) == 0.0)
    {
      if (i == nrows - 1)
        return false; // print "Singular matrix"

      i++;
    }

    if (k != i)
    {
// update changed rows indicator
      aux = row[i];
      row[i] = row[k];
      row[k] = aux;

// change rows
      for (j=0; j < nrows; j ++)
      {
        auxd = Z(k, j);
        Z(k, j) = Z (i, j);
        Z (i, j) = auxd;
      }
    }

// recompute rows i = k + 1,..., nComponents - 1
    for (i = k + 1; i < nrows; i++)
    {
      m = Z(i, k)/Z(k, k);
      Z(i, k) = m;
      for (j = k + 1; j < nrows; j++)
        Z (i, j) = Z (i, j) - m * Z(k, j);
    }

  }

  if (Z(nrows - 1, nrows - 1) == 0.0)
    return false; // print "Singular Matrix"

  return true;
}

bool fowardBackSubstitution(boost::numeric::ublas::matrix<double>& Z, std::vector<double>& y, unsigned int ys, std::vector<int>& row, unsigned int rows, std::vector<double>& x, unsigned int xs)
{
// verify matrix size
  assert(Z.size1() >= Z.size2());
  unsigned int nrows = Z.size1(); // rows = size2?

  if (((rows < nrows) || (ys < nrows) || (xs < nrows)))
    return false; // print "Vector Size"

  int j, k;
  double aux;
  std::vector<double> F(nrows, 0.0);

// foward substuitution
  for (k = 0; k < (int) nrows; k++)
  {
    aux = 0.0;
    for (j = 0; j <= k - 1; j++)
      aux = aux + Z(k, j) * F[j];

    F[k] = y[row[k]] - aux;
  }

// backward substitution
  for (k = ((int) nrows) - 1; k >= 0; k--)
  {
    aux = 0.0;
    for (j = k + 1; j < (int) nrows; j++)
      aux = aux + Z(k, j) * x[j];

    x[k] = (F[k] - aux) / Z(k, k);
  }

  return true;
}
