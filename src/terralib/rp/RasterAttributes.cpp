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
  \file terralib/rp/RasterAttributes.cpp

  \brief Extraction of attributes from Raster, Bands, and Polygons.
*/

// TerraLib
#include "../common/progress/TaskProgress.h"
#include "../geometry.h"
#include "../raster/Band.h"
#include "../raster/BandIterator.h"
#include "../raster/Grid.h"
#include "../raster/PositionIterator.h"
#include "../raster/Raster.h"
#include "../raster/Utils.h"
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

std::vector<std::vector<std::complex<double> > > te::rp::RasterAttributes::getComplexValuesFromRaster(const te::rst::Raster& raster, const te::gm::Polygon& polygon, std::vector<unsigned int> bands,
																									  unsigned int rowstep, unsigned int colstep)
{
  assert(bands.size() > 0);
  assert(bands.size() <= raster.getNumberOfBands());

  assert(rowstep > 0);
  assert(colstep > 0);

  std::vector<std::vector<std::complex<double> > > allvalues;
  std::complex<double> value;

// create iterators for band and polygon
  te::rst::PolygonIterator<double> it = te::rst::PolygonIterator<double>::begin(&raster, &polygon);
  te::rst::PolygonIterator<double> itend = te::rst::PolygonIterator<double>::end(&raster, &polygon);

  //unsigned int initrow = it.getRow();

  for (unsigned int i = 0; i < bands.size(); i++)
    allvalues.push_back(std::vector<std::complex< double > > ());
  
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
	
	
	/*  std::vector<std::vector<double> > dallvalues = getValuesFromRaster(raster, polygon, bands);
  std::vector<std::complex<double> > values;
  std::vector<std::vector<std::complex<double> > > allvalues;

  for (unsigned int i = 0; i < dallvalues.size(); i++)
  {
    values.clear();
    for (unsigned int j = 0; j < dallvalues[i].size(); j++)
      values.push_back(dallvalues[i][j]);
    allvalues.push_back(values);
  }

  return allvalues;*/
}

std::vector<std::vector<double> > te::rp::RasterAttributes::getValuesFromRaster(const te::rst::Raster& raster, const te::gm::Polygon& polygon, std::vector<unsigned int> bands,
	                                                                            unsigned int rowstep, unsigned int colstep)
{
  assert(bands.size() > 0);
  assert(bands.size() <= raster.getNumberOfBands());

  assert(rowstep > 0);
  assert(colstep > 0);

  std::vector<std::vector<double> > allvalues;
  double value;

// create iterators for band and polygon
  te::rst::PolygonIterator<double> it = te::rst::PolygonIterator<double>::begin(&raster, &polygon);
  te::rst::PolygonIterator<double> itend = te::rst::PolygonIterator<double>::end(&raster, &polygon);

  unsigned int initrow = it.getRow(), initcol;

  for (unsigned int i = 0; i < bands.size(); i++)
    allvalues.push_back(std::vector<double> ());
  while (it != itend)
  {
	  if(!((it.getRow() - initrow) % rowstep))
	  {
		for (unsigned int i = 0; i < bands.size(); i++)
		{
		  raster.getValue(it.getColumn(), it.getRow(), value, bands[i]);
		  allvalues[i].push_back(value);
		}
	  }

	  unsigned posstep = it.getColumn() + colstep;

	  initcol = it.getColumn();

	  for(initcol; initcol < posstep; initcol++)
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

te::stat::NumericStatisticalComplexSummary te::rp::RasterAttributes::getComplexStatistics(std::vector<std::complex < double > >& pixels)
{
  assert(pixels.size() > 0);

  te::stat::NumericStatisticalComplexSummary summary;
  te::stat::GetNumericComplexStatisticalSummary(pixels, summary);

  return summary;
}

boost::numeric::ublas::matrix<std::complex < double> > te::rp::RasterAttributes::getComplexCovarianceMatrix(const std::vector<std::vector<std::complex < double > > >& vpixels, const std::vector<std::complex < double > >& vmeans)
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

	  boost::numeric::ublas::matrix<std::complex < double > > covariance(nbands, nbands);

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
	  te::common::TaskProgress taskProgress("Computing covariance complex matrix", te::common::TaskProgress::UNDEFINED, nbands * nbands);
	  std::complex<double> sum;
	  for (i = 0; i < nbands; i++)
		for (j = 0; j < nbands; j++)
		{
		  taskProgress.pulse();
      
		  sum = std::complex<double> (0.0, 0.0);

		  for (k = 0; k < nvalues; k++)
			sum += (vpixels[i][k] - vmeans[i]) * (vpixels[j][k] - vmeans[j]);

		  covariance(i, j) = sum / (nvalues - 1.0);
		}

	  return covariance;

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
  te::common::TaskProgress taskProgress("Computing covariance matrix", te::common::TaskProgress::UNDEFINED, nbands * nbands);
  std::complex<double> sum;
  for (i = 0; i < nbands; i++)
    for (j = 0; j < nbands; j++)
    {
      taskProgress.pulse();
      
      sum = std::complex<double> (0.0, 0.0);

      for (k = 0; k < nvalues; k++)
        sum += (vpixels[i][k] - vmeans[i]) * (vpixels[j][k] - vmeans[j]);

      covariance(i, j) = sum.real() / (nvalues - 1);
    }

  return covariance;
}

boost::numeric::ublas::matrix<double> te::rp::RasterAttributes::getGLCM(const te::rst::Raster& rin, unsigned int band, int dx, int dy)
{
  assert(band < rin.getNumberOfBands());
  
  double minPixel;
  double maxPixel;
  te::rst::GetDataTypeRanges(rin.getBandDataType(band), minPixel, maxPixel);
  
  boost::numeric::ublas::matrix<double> glcm (maxPixel + 1, maxPixel + 1);
  glcm.clear();
  double pixel;
  double neighborPixel;
  double noDataValue = rin.getBand(band)->getProperty()->m_noDataValue;
  double N = 0.0;
  
// defining limits for iteration  
  int row_start = 0;
  int row_end = rin.getNumberOfRows();
  int column_start = 0;
  int column_end = rin.getNumberOfColumns();
  
  if (dy > 0)
    row_end -= dy;
  else
    row_start -= dy;

  if (dx > 0)
    column_end -= dx;
  else
    column_start -= dx;
  
// computing GLCM
  te::common::TaskProgress taskProgress("Computing the GLCM", te::common::TaskProgress::UNDEFINED, (row_end - row_start) * (column_end - column_start));
  for (int r = row_start; r < row_end; r++)
  {
    for (int c = column_start; c < column_end; c++)
    {
      taskProgress.pulse();
      
// get central pixel      
      rin.getValue(c, r, pixel, band);
      if (pixel == noDataValue)
        continue;
      
// get neighbor pixel      
      rin.getValue(c + dx, r + dy, neighborPixel, band);
      if (neighborPixel == noDataValue)
        continue;

// update GLCM matrix
      glcm(pixel, neighborPixel) = glcm(pixel, neighborPixel) + 1;
      N++;
    }
  }

  if (N > 0.0)
  {
    for (unsigned int i = 0; i < glcm.size1(); i++)
      for (unsigned int j = 0; j < glcm.size2(); j++)
        glcm(i, j) = glcm(i, j) / N;
  }
  
  return glcm;
}
        
boost::numeric::ublas::matrix<double> te::rp::RasterAttributes::getGLCM(const te::rst::Raster& rin, unsigned int band, int dx, int dy, const te::gm::Polygon& polygon)
{
  // create raster crop with polygon
  std::map<std::string, std::string> rcropinfo;
  rcropinfo["FORCE_MEM_DRIVER"] = "TRUE";
  te::rst::RasterPtr rcrop = te::rst::CropRaster(rin, polygon, rcropinfo, "MEM");
  
  // call previous method using crop
  return getGLCM(*rcrop.get(), band, dx, dy);
}
        
te::rp::Texture te::rp::RasterAttributes::getGLCMMetrics(boost::numeric::ublas::matrix<double> glcm)
{
  te::rp::Texture metrics;
  
  unsigned int i;
  unsigned int j;
  double di;
  double dj;
  double di_minus_dj;
  double square_di_minus_dj;
  te::common::TaskProgress taskProgress("Computing GLCM Metrics", te::common::TaskProgress::UNDEFINED, glcm.size1() * glcm.size2());
  for (i = 0, di = 0.0; i < glcm.size1(); i++, di++)
  {
    for (j = 0, dj = 0.0; j < glcm.size2(); j++, dj++)
    {
      taskProgress.pulse();
      
      di_minus_dj = (di - dj);
      square_di_minus_dj = di_minus_dj * di_minus_dj;
      
      metrics.m_contrast += glcm(i, j) * square_di_minus_dj;
      metrics.m_dissimilarity += glcm(i, j) * std::abs(di_minus_dj);
      metrics.m_energy += glcm(i, j) * glcm(i, j);
      if (glcm(i, j) > 0)
        metrics.m_entropy += glcm(i, j) * (-1.0 * log(glcm(i, j)));
      metrics.m_homogeneity += glcm(i, j) / (1 + square_di_minus_dj);
    }
  }

  if (metrics.m_energy > 0)
    metrics.m_energy = std::sqrt(metrics.m_energy);

  return metrics;
}