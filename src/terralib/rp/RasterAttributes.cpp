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
#include "../geometry/Coord2D.h"
#include "../geometry/Envelope.h"
#include "../geometry/Point.h"
#include "../raster/Band.h"
#include "../raster/BandIterator.h"
#include "../raster/Grid.h"
#include "../raster/Raster.h"
#include "RasterAttributes.h"

te::rp::RasterAttributes::RasterAttributes()
{
}

te::rp::RasterAttributes::~RasterAttributes()
{
}

bool te::rp::RasterAttributes::initialize(const AlgorithmInputParameters& inputParams) throw(te::rp::Exception)
{
}

bool te::rp::RasterAttributes::execute(AlgorithmOutputParameters& outputParams) throw(te::rp::Exception)
{
}

void te::rp::RasterAttributes::reset() throw(te::rp::Exception)
{
}

std::vector<std::complex<double> > te::rp::RasterAttributes::getValuesFromBand(const te::rst::Band& band, const te::gm::Geometry& geometry)
{
  std::vector<std::complex<double> > values;

// create iterators for band and geometry
  te::rst::ConstBandIterator<double> it = te::rst::ConstBandIterator<double>::begin(&band, &geometry);
  te::rst::ConstBandIterator<double> itend = te::rst::ConstBandIterator<double>::end(&band, &geometry);

  while (it != itend)
  {
// using iterator
    values.push_back(*it);

    ++it;
  }

  return values;
}

std::vector<std::vector<std::complex<double> > > te::rp::RasterAttributes::getValuesFromRaster(const te::rst::Raster& raster, const te::gm::Geometry& geometry, std::vector<unsigned int> bands)
{
  assert(bands.size() > 0);
  assert(bands.size() <= raster.getNumberOfBands());

  std::vector<std::vector<std::complex<double> > > allvalues;
  std::vector<std::complex<double> > values;
  std::complex<double> value;

// create iterators for band and geometry
  te::rst::ConstBandIterator<double> it = te::rst::ConstBandIterator<double>::begin(raster.getBand(bands[0]), &geometry);
  te::rst::ConstBandIterator<double> itend = te::rst::ConstBandIterator<double>::end(raster.getBand(bands[0]), &geometry);

  while (it != itend)
  {
    values.clear();

    for (unsigned int i = 0; i < bands.size(); i++)
    {
      raster.getValue(it.getCol(), it.getRow(), value, bands[i]);

      values.push_back(value);
    }

    allvalues.push_back(values);

    ++it;
  }

  return allvalues;
}

std::complex<double> te::rp::RasterAttributes::getMean(const te::rst::Band& band, const te::gm::Geometry& geometry)
{
  std::vector<unsigned int> b;

  b.push_back(band.getProperty()->m_idx);

  return getMeans(*band.getRaster(), geometry, b)[0];
}

std::vector<std::complex<double> > te::rp::RasterAttributes::getMeans(const te::rst::Raster& raster, const te::gm::Geometry& geometry, std::vector<unsigned int> bands)
{
  assert(bands.size() > 0);
  assert(bands.size() <= raster.getNumberOfBands());

  std::vector<std::complex<double> > means;

  for (unsigned int i = 0; i < bands.size(); i++)
    means.push_back(0.0);

  unsigned int nvalues = 0;

  std::complex<double> value;

  te::rst::ConstBandIterator<unsigned> it = te::rst::ConstBandIterator<unsigned>::begin(raster.getBand(bands[0]), &geometry);
  te::rst::ConstBandIterator<unsigned> itend = te::rst::ConstBandIterator<unsigned>::end(raster.getBand(bands[0]), &geometry);

  while (it != itend)
  {
    for (unsigned int i = 0; i < bands.size(); i++)
    {
      raster.getValue(it.getCol(), it.getRow(), value, bands[i]);

      means[i] += value;
    }

    ++nvalues;

    ++it;
  }

  for (unsigned int i = 0; i < means.size() && nvalues != 0; i++)
    means[i] /= nvalues;

  return means;
}

boost::numeric::ublas::matrix<double> te::rp::RasterAttributes::getCovarianceMatrix(const te::rst::Raster& raster, const te::gm::Geometry& geometry, std::vector<unsigned int> bands)
{
  assert(bands.size() > 1);
  assert(bands.size() <= raster.getNumberOfBands());

  unsigned int i;
  unsigned int j;
  unsigned int k;
  unsigned int nbands = bands.size();

  std::vector<std::vector<std::complex<double> > > valuesperband = getValuesFromRaster(raster, geometry, bands);

  unsigned int nvalues = valuesperband.size();

  boost::numeric::ublas::matrix<double> covariance(nbands, nbands);

  std::vector<std::complex<double> > meansperband = getMeans(raster, geometry, bands);

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

  std::complex<double> sum;

  for (i = 0; i < nbands; i++)
    for (j = 0; j < nbands; j++)
    {
      sum = std::complex<double> (0.0, 0.0);

      for (k = 0; k < nvalues; k++)
        sum += (valuesperband[k][i] - meansperband[i]) * (valuesperband[k][j] - meansperband[j]);

      covariance(i, j) = sum.real() / (nvalues - 1);
    }

  return covariance;
}
