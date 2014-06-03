/*  Copyright (C) 2008-2013 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/raster/Interpolator.cpp

  \brief It interpolates one pixel based on a selected algorithm.
*/

// TerraLib
#include "Interpolator.h"
#include "Band.h"
#include "BandProperty.h"
#include "Utils.h"

// STL
#include <cassert>

#define BICUBIC_MODULE( x ) ( ( x < 0 ) ? ( -1 * x ) : x )
#define BICUBIC_K1( x , a ) ( ( ( a + 2 ) * x * x * x ) - \
  ( ( a + 3 ) * x * x ) + 1 )
#define BICUBIC_K2( x , a ) ( ( a * x * x * x ) - ( 5 * a * x * x ) + \
  ( 8 * a * x ) - ( 4 * a ) )
#define BICUBIC_RANGES(x,a) \
  ( ( ( 0 <= x ) && ( x <= 1 ) ) ? \
    BICUBIC_K1(x,a) \
  : ( ( ( 1 < x ) && ( x <= 2 ) ) ? \
      BICUBIC_K2(x,a) \
    : 0 ) )
#define BICUBIC_KERNEL( x , a ) BICUBIC_RANGES( BICUBIC_MODULE(x) , a )

te::rst::Interpolator::Interpolator(Raster const* r, int m)
  : m_raster(r),
    m_method(m)
{
  switch(m_method)
  {
    case Bicubic:
      m_function = &te::rst::Interpolator::bicubicGetValue;
      break;
    case Bilinear:
      m_function = &te::rst::Interpolator::bilinearGetValue;
      break;
    case NearestNeighbor:
      m_function = &te::rst::Interpolator::nearestNeighborGetValue;
      break;
  }
  
  // raster no-data values (for each band)
  
  for( unsigned int bandIdx = 0 ; bandIdx < r->getNumberOfBands() ; ++bandIdx )
    m_noDataValues.push_back( r->getBand( bandIdx )->getProperty()->m_noDataValue );
  
  // ancillary values for nearest Neighbor interpolation
  
  m_nnLastRow = ( (double) m_raster->getNumberOfRows() ) - 0.5;
  m_nnLastCol = ( (double) m_raster->getNumberOfColumns() ) - 0.5;

  // ancillary values for bilinear interpolation
  m_bilValues.resize(4, 0);

  m_bilLastRow = (double) m_raster->getNumberOfRows() - 1.0;

  m_bilLastCol = (double) m_raster->getNumberOfColumns() - 1.0;

  // ancillary values for bicubic interpolation
  m_bicKernel = -1.0;

  m_bicRowBound = m_bilLastRow - 1.0;

  m_bicColBound = m_bilLastCol - 1.0;
}

te::rst::Interpolator::~Interpolator()
{
  m_function = 0;
}

void te::rst::Interpolator::getValues(const double& c, const double& r, std::vector<std::complex<double> >& values)
{
  values.clear();

  std::complex<double> v;

  for(std::size_t b = 0; b < m_raster->getNumberOfBands(); b++)
  {
    getValue(c, r, v, b);

    values.push_back(v);
  }
}

te::rst::Interpolator* te::rst::Interpolator::clone() const
{
  te::rst::Interpolator* newInstancePtr = new te::rst::Interpolator( m_raster, m_method );
  return newInstancePtr;
}

void te::rst::Interpolator::nearestNeighborGetValue(const double& c, const double& r, std::complex<double>& v, const std::size_t& b)
{
  if( ( c > (-0.5) ) && ( r > (-0.5) ) && ( c < m_nnLastCol ) && ( r < m_nnLastRow ) )
  {
    m_nnCR = (unsigned int) te::rst::Round(c);
    m_nnRR = (unsigned int) te::rst::Round(r);

    m_raster->getValue(m_nnCR, m_nnRR, v, b);
  }
  else
  {
    v = std::complex<double>( m_noDataValues[ b ], m_noDataValues[ b ] );
  }
}

void te::rst::Interpolator::bilinearGetValue(const double& c, const double& r, std::complex<double>& v, const std::size_t& b)
{
  if( (r < 0.0) || (c < 0.0) || (r > m_bilLastRow) || (c > m_bilLastCol) )
  {
    nearestNeighborGetValue(c, r, v, b);
    return;
  }

  m_bilRowMin = std::floor(r);
  m_bilRowMax = (m_bilRowMin == r)? m_bilRowMin: (m_bilRowMin + 1.0);

  m_bilColMin = std::floor(c);
  m_bilColMax = (m_bilColMin == c)? m_bilColMin: (m_bilColMin + 1.0);

  m_bilRowDifMin = r - m_bilRowMin;
  m_bilRowDifMax = m_bilRowMax - r;

  m_bilColDifMin = c - m_bilColMin;
  m_bilColDifMax = m_bilColMax - c;

  m_bilDistances[0] = std::sqrt((m_bilRowDifMin * m_bilRowDifMin) + (m_bilColDifMin * m_bilColDifMin));
  m_bilDistances[1] = std::sqrt((m_bilRowDifMin * m_bilRowDifMin) + (m_bilColDifMax * m_bilColDifMax));
  m_bilDistances[2] = std::sqrt((m_bilRowDifMax * m_bilRowDifMax) + (m_bilColDifMin * m_bilColDifMin));
  m_bilDistances[3] = std::sqrt((m_bilRowDifMax * m_bilRowDifMax) + (m_bilColDifMin * m_bilColDifMax));

  m_bilWeights[0] = (m_bilDistances[0] == 0)? 1.0: (1 / m_bilDistances[0]);
  m_bilWeights[1] = (m_bilDistances[1] == 0)? 1.0: (1 / m_bilDistances[1]);
  m_bilWeights[2] = (m_bilDistances[2] == 0)? 1.0: (1 / m_bilDistances[2]);
  m_bilWeights[3] = (m_bilDistances[3] == 0)? 1.0: (1 / m_bilDistances[3]);

  m_raster->getValue((unsigned) m_bilColMin, (unsigned) m_bilRowMin, m_bilValues[0], b);
  m_raster->getValue((unsigned) m_bilColMax, (unsigned) m_bilRowMin, m_bilValues[1], b);
  m_raster->getValue((unsigned) m_bilColMin, (unsigned) m_bilRowMax, m_bilValues[2], b);
  m_raster->getValue((unsigned) m_bilColMax, (unsigned) m_bilRowMax, m_bilValues[3], b);

  double vr = ( (m_bilValues[0].real() * m_bilWeights[0]) +
            (m_bilValues[1].real() * m_bilWeights[1]) +
            (m_bilValues[2].real() * m_bilWeights[2]) +
            (m_bilValues[3].real() * m_bilWeights[3]) ) /
            (m_bilWeights[0] + m_bilWeights[1] + m_bilWeights[2] + m_bilWeights[3]);
  double vi = ( (m_bilValues[0].imag() * m_bilWeights[0]) +
            (m_bilValues[1].imag() * m_bilWeights[1]) +
            (m_bilValues[2].imag() * m_bilWeights[2]) +
            (m_bilValues[3].imag() * m_bilWeights[3]) ) /
            (m_bilWeights[0] + m_bilWeights[1] + m_bilWeights[2] + m_bilWeights[3]);
  v = std::complex<double>(vr,vi);

  // if the output value is equal to dummy, call nearest neighbor method
  if( (v.real() == m_raster->getBand(b)->getProperty()->m_noDataValue) ||
      (v.imag() == m_raster->getBand(b)->getProperty()->m_noDataValue) )
    nearestNeighborGetValue(c, r, v, b);
}

void te::rst::Interpolator::bicubicGetValue(const double& c, const double& r, std::complex<double>& v, const std::size_t& b)
{
  if( (r <= 1.0) || (c <= 1.0) || (r >= m_bicRowBound) || (c >= m_bicColBound) )
  {
    nearestNeighborGetValue(c, r, v, b);
    return;
  }

  m_bicGridRow = ((unsigned) std::floor(r)) - 1;
  m_bicGridCol = ((unsigned) std::floor(c)) - 1;

  // fill buffer
  for(m_bicBufRow = 0; m_bicBufRow < 4; ++m_bicBufRow)
  {
    for(m_bicBufCol = 0; m_bicBufCol < 4 ; ++m_bicBufCol)
    {
      m_raster->getValue(m_bicGridCol + m_bicBufCol, m_bicGridRow + m_bicBufRow,
                         m_bicBbufferReal[m_bicBufRow][m_bicBufCol], b);
      m_raster->getIValue(m_bicGridCol + m_bicBufCol, m_bicGridRow + m_bicBufRow,
                          m_bicBbufferImag[m_bicBufRow][m_bicBufCol], b);
    }
  }

  // calcule weights for the required position.
  m_bicOffsetX = c - (double)(m_bicGridCol + 1);
  m_bicOffsetY = r - (double)(m_bicGridRow + 1);

  m_bicHWeights[0] = BICUBIC_KERNEL(1.0 + m_bicOffsetX, m_bicKernel);
  m_bicHWeights[1] = BICUBIC_KERNEL(m_bicOffsetX, m_bicKernel);
  m_bicHWeights[2] = BICUBIC_KERNEL(1.0 - m_bicOffsetX, m_bicKernel);
  m_bicHWeights[3] = BICUBIC_KERNEL(2.0 - m_bicOffsetX, m_bicKernel);

  m_bicVWeights[0] = BICUBIC_KERNEL(1.0 + m_bicOffsetY, m_bicKernel);
  m_bicVWeights[1] = BICUBIC_KERNEL(m_bicOffsetY, m_bicKernel);
  m_bicVWeights[2] = BICUBIC_KERNEL(1.0 - m_bicOffsetY, m_bicKernel);
  m_bicVWeights[3] = BICUBIC_KERNEL(2.0 - m_bicOffsetY, m_bicKernel);

  m_bicHSum = m_bicHWeights[0] + m_bicHWeights[1] + m_bicHWeights[2] + m_bicHWeights[3];
  m_bicVSum = m_bicVWeights[0] + m_bicVWeights[1] + m_bicVWeights[2] + m_bicVWeights[3];

  // interpolating the value
  for(m_bicBufRow = 0 ; m_bicBufRow < 4 ; ++m_bicBufRow)
  {
    m_bicRowAccumReal = 0.0;
    m_bicRowAccumImag = 0.0;
    for(m_bicBufCol = 0; m_bicBufCol < 4; ++m_bicBufCol)
    {
      m_bicRowAccumReal += m_bicBbufferReal[m_bicBufRow][m_bicBufCol] *
                           m_bicHWeights[m_bicBufCol];
      m_bicRowAccumImag += m_bicBbufferImag[m_bicBufRow][m_bicBufCol] *
                           m_bicHWeights[m_bicBufCol];
    }

    m_bicRowsValuesReal[m_bicBufRow] = m_bicRowAccumReal / m_bicHSum;
    m_bicRowsValuesImag[m_bicBufRow] = m_bicRowAccumImag / m_bicHSum;
  }
  double vr = ( m_bicRowsValuesReal[0] * m_bicVWeights[0] +
            m_bicRowsValuesReal[1] * m_bicVWeights[1] +
            m_bicRowsValuesReal[2] * m_bicVWeights[2] +
            m_bicRowsValuesReal[3] * m_bicVWeights[3] ) / m_bicVSum;

  double vi = ( m_bicRowsValuesImag[0] * m_bicVWeights[0] +
            m_bicRowsValuesImag[1] * m_bicVWeights[1] +
            m_bicRowsValuesImag[2] * m_bicVWeights[2] +
               m_bicRowsValuesImag[3] * m_bicVWeights[3] ) / m_bicVSum;
  v = std::complex<double>(vr,vi);

  // if the output value is equal to dummy, call nearest neighbor method
  if( (v.real() == m_raster->getBand(b)->getProperty()->m_noDataValue) ||
      (v.imag() == m_raster->getBand(b)->getProperty()->m_noDataValue) )
    nearestNeighborGetValue(c, r, v, b);
}
