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
  \file terralib/rp/RasterAttributes.cpp

  \brief Extraction of attributes from Raster, Bands, and Polygons.
*/

// TerraLib
#include "../geometry.h"
#include "../raster/Band.h"
#include "../raster/BandIterator.h"
#include "../raster/Grid.h"
#include "../raster/PositionIterator.h"
#include "../raster/Raster.h"
#include "../statistics.h"
#include "RasterAttributes.h"

te::rp::RasterAttributes::RasterAttributes()
{
}

te::rp::RasterAttributes::~RasterAttributes()
{
}

bool te::rp::RasterAttributes::initialize(const AlgorithmInputParameters& inputParams) throw(te::rp::Exception)
{
  return true;
}

bool te::rp::RasterAttributes::isInitialized() const
{
  return true;
}

bool te::rp::RasterAttributes::execute(AlgorithmOutputParameters& outputParams) throw(te::rp::Exception)
{
  return true;
}

void te::rp::RasterAttributes::reset() throw(te::rp::Exception)
{
}

std::vector<std::complex<double> > te::rp::RasterAttributes::getComplexValuesFromBand(const te::rst::Raster& raster, unsigned int band, const te::gm::Polygon& polygon)
{
  std::vector<double> dvalues = getValuesFromBand(raster, band, polygon);
  std::vector<std::complex<double> > values;

  for (unsigned int i = 0; i < dvalues.size(); i++)
    values.push_back(dvalues[i]);

  return values;
}

std::vector<double> te::rp::RasterAttributes::getValuesFromBand(const te::rst::Raster& raster, unsigned int band, const te::gm::Polygon& polygon)
{
  assert(band < raster.getNumberOfBands());

  std::vector<double> values;

// create iterators for band and polygon
  te::rst::PolygonIterator<double> it = te::rst::PolygonIterator<double>::begin(&raster, &polygon);
  te::rst::PolygonIterator<double> itend = te::rst::PolygonIterator<double>::end(&raster, &polygon);

  while (it != itend)
  {
// using iterator
    values.push_back((*it)[band]);

    ++it;
  }

  return values;
}

std::vector<std::vector<std::complex<double> > > te::rp::RasterAttributes::getComplexValuesFromRaster(const te::rst::Raster& raster, const te::gm::Polygon& polygon, std::vector<unsigned int> bands)
{
  std::vector<std::vector<double> > dallvalues = getValuesFromRaster(raster, polygon, bands);
  std::vector<std::complex<double> > values;
  std::vector<std::vector<std::complex<double> > > allvalues;

  for (unsigned int i = 0; i < dallvalues.size(); i++)
  {
    values.clear();
    for (unsigned int j = 0; j < dallvalues[i].size(); j++)
      values.push_back(dallvalues[i][j]);
    allvalues.push_back(values);
  }

  return allvalues;
}

std::vector<std::vector<double> > te::rp::RasterAttributes::getValuesFromRaster(const te::rst::Raster& raster, const te::gm::Polygon& polygon, std::vector<unsigned int> bands)
{
  assert(bands.size() > 0);
  assert(bands.size() <= raster.getNumberOfBands());

  std::vector<std::vector<double> > allvalues;
  double value;

// create iterators for band and polygon
  te::rst::PolygonIterator<double> it = te::rst::PolygonIterator<double>::begin(&raster, &polygon);
  te::rst::PolygonIterator<double> itend = te::rst::PolygonIterator<double>::end(&raster, &polygon);

  for (unsigned int i = 0; i < bands.size(); i++)
    allvalues.push_back(std::vector<double> ());
  while (it != itend)
  {
    for (unsigned int i = 0; i < bands.size(); i++)
    {
      raster.getValue(it.getColumn(), it.getRow(), value, bands[i]);
      allvalues[i].push_back(value);
    }

    ++it;
  }

  return allvalues;
}

te::stat::NumericStatisticalSummary te::rp::RasterAttributes::getStatistics(std::vector<double>& pixels)
{
  assert(pixels.size() > 0);

  te::stat::NumericStatisticalSummary summary;
  te::stat::GetNumericStatisticalSummary(pixels, summary);

  return summary;
}

boost::numeric::ublas::matrix<double> te::rp::RasterAttributes::getCovarianceMatrix(const std::vector<std::vector<double> >& vpixels, const std::vector<double>& vmeans)
{
  for (unsigned int i = 0; i < vpixels.size(); i++)
    assert(vpixels[i].size() > 0);
  for (unsigned int i = 1; i < vpixels.size(); i++)
    assert(vpixels[0].size() == vpixels[i].size());
  assert(vpixels.size() == vmeans.size());

  unsigned int i;
  unsigned int j;
  unsigned int k;
  unsigned int nbands = vpixels.size();
  unsigned int nvalues = vpixels[0].size();

  boost::numeric::ublas::matrix<double> covariance(nbands, nbands);

// with few values, the covariance is default
  if (nvalues < 2)
  {
    for (i = 0; i < nbands; i++)
    {
      for (j = 0; j < nbands; j++)
        covariance(i, j) = 0.0;
      covariance(i, i) = 1000.0;
    }

    return covariance;
  }

// compute covariance matrix based on values and means
  std::complex<double> sum;
  for (i = 0; i < nbands; i++)
    for (j = 0; j < nbands; j++)
    {
      sum = std::complex<double> (0.0, 0.0);

      for (k = 0; k < nvalues; k++)
        sum += (vpixels[i][k] - vmeans[i]) * (vpixels[j][k] - vmeans[j]);

      covariance(i, j) = sum.real() / (nvalues - 1);
    }

  return covariance;
}

boost::numeric::ublas::matrix<double> te::rp::RasterAttributes::getGLCM(const te::rst::Raster& r, unsigned int b, int dx, int dy)
{
  boost::numeric::ublas::matrix<double> glcm;
  return glcm;
}
        
te::rp::Texture te::rp::RasterAttributes::getGLCMMetrics(boost::numeric::ublas::matrix<double> glcm)
{
  te::rp::Texture metrics;
  return metrics;
}