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
  \file terralib/raster/Grid.cpp

  \brief A rectified grid is the spatial support for raster data.
*/

// TerraLib
#include "../geometry/Coord2D.h"
#include "../geometry/Envelope.h"
#include "Grid.h"
#include "Exception.h"

// STL
#include <algorithm>
#include <cmath>
#include <cstring>

te::rst::Grid::Grid(unsigned int nCols,
                    unsigned int nRows,
                    te::gm::Envelope* mbr,
                    int srid)
  : m_extent(mbr),
    m_nCols(nCols),
    m_nRows(nRows),
    m_srid(srid)
{
  if(m_extent)
  {
    if( ! computeAffineParameters( m_extent->getWidth(), m_extent->getHeight(),
      nCols, nRows, m_extent->m_llx, m_extent->m_ury, m_geoT, m_geoTInverse ) )
    {
      throw te::rst::Exception("Unable to compute affine parameters");
    }
  }
  else
  {
    setGeoreference(te::gm::Coord2D(0, 0), srid, 1.0, 1.0);
    computeExtent();
  }
}

te::rst::Grid::Grid(unsigned int nCols, unsigned int nRows,
                    double resX, double resY,
                    const te::gm::Coord2D* ulc, int srid)
  : m_extent(0),
    m_nCols(nCols),
    m_nRows(nRows),
    m_srid(srid)
{
  if(ulc)
    setGeoreference(*ulc, srid, resX, resY);
  else
    setGeoreference(te::gm::Coord2D(0, 0), srid, resX, resY);
  computeExtent();
}

te::rst::Grid::Grid(unsigned int nCols, unsigned int nRows,
                    double resX, double resY,
                    te::gm::Envelope* mbr, int srid)
  : m_extent(mbr),
    m_nCols(nCols),
    m_nRows(nRows),
    m_srid(srid)
{
  if(m_extent)
  {
    if( ! computeAffineParameters( m_extent->getWidth(), m_extent->getHeight(),
      nCols, nRows, m_extent->m_llx, m_extent->m_ury, m_geoT, m_geoTInverse ) )
    {
      throw te::rst::Exception("Unable to compute affine parameters");
    }    
  }
  else
  {
    setGeoreference(te::gm::Coord2D(0, 0), srid, resX, resY);
    computeExtent();
  }
}

te::rst::Grid::Grid(double resX,
                    double resY,
                    te::gm::Envelope* mbr,
                    int srid)
  : m_extent(mbr),
    m_nCols(0),
    m_nRows(0),
    m_srid(srid)
{
  if( m_extent == 0 )
  {
    throw te::rst::Exception("Missing extent");
  }
  
  m_nCols = static_cast<unsigned int> (mbr->getWidth() / resX + 0.5);
  m_nRows = static_cast<unsigned int> (mbr->getHeight() / resY + 0.5);

  if( ! computeAffineParameters( m_extent->getWidth(), m_extent->getHeight(),
    m_nCols, m_nRows, m_extent->m_llx, m_extent->m_ury, m_geoT, m_geoTInverse ) )
  {
    throw te::rst::Exception("Unable to compute affine parameters");
  }    
}

te::rst::Grid::Grid( const double geoTrans[], unsigned int nCols, unsigned int nRows,
                     int srid)
  : m_extent(0),
    m_nCols(nCols),
    m_nRows(nRows),
    m_srid(srid)
{
  setGeoreference(geoTrans, srid);
  computeExtent();
}

te::rst::Grid::Grid(const Grid& rhs)
  : m_extent(0),
    m_nCols(rhs.m_nCols),
    m_nRows(rhs.m_nRows),
    m_srid(rhs.m_srid)
{
  operator=( rhs );
}

te::rst::Grid::~Grid()
{
  if (m_extent)
  {
    delete m_extent;
  }
}

te::rst::Grid& te::rst::Grid::operator=(const Grid& rhs)
{
  if(this != &rhs)
  {
    m_nCols = rhs.m_nCols;

    m_nRows = rhs.m_nRows;

    m_srid = rhs.m_srid;

    memcpy(m_geoT, rhs.m_geoT, sizeof(double) * 6);
    memcpy(m_geoTInverse, rhs.m_geoTInverse, sizeof(double) * 6);
    
    if (m_extent)
    {
      delete m_extent;
      m_extent = 0;
    }
    if( rhs.m_extent )
    {
      m_extent = new te::gm::Envelope(*rhs.m_extent);
    }
    else
    {
      computeExtent();
    }    
  }

  return *this;
}

void te::rst::Grid::setNumberOfColumns(unsigned int nCols)
{
  m_nCols = nCols;

  if( ! computeAffineParameters( m_extent->getWidth(), m_extent->getHeight(),
    m_nCols, m_nRows, m_extent->m_llx, m_extent->m_ury, m_geoT, m_geoTInverse ) )
  {
    throw te::rst::Exception("Unable to compute affine parameters");
  }     
}

unsigned int te::rst::Grid::getNumberOfColumns() const
{
  return m_nCols;
}

void te::rst::Grid::setNumberOfRows(unsigned int nRows)
{
  m_nRows = nRows;
  
  if( ! computeAffineParameters( m_extent->getWidth(), m_extent->getHeight(),
    m_nCols, m_nRows, m_extent->m_llx, m_extent->m_ury, m_geoT, m_geoTInverse ) )
  {
    throw te::rst::Exception("Unable to compute affine parameters");
  }    
}

unsigned int te::rst::Grid::getNumberOfRows() const
{
  return m_nRows;
}

void te::rst::Grid::setGeoreference(const te::gm::Coord2D& ulLocation, int srid, double resX, double resY)
{
  m_srid = srid;
  
  const double extentWidth = ((double)m_nCols) * resX;
  const double extentheight = ((double)m_nRows) * resY;
  
  if( ! computeAffineParameters( extentWidth, extentheight,
    m_nCols, m_nRows, ulLocation.x, ulLocation.y, m_geoT, m_geoTInverse ) )
  {
    throw te::rst::Exception("Unable to compute affine parameters");
  }  
  
  computeExtent();
}

void te::rst::Grid::setGeoreference(const double geoTrans[], int srid)
{
  m_srid = srid;
  m_geoT[0] = geoTrans[0];
  m_geoT[1] = geoTrans[1];
  m_geoT[2] = geoTrans[2];
  m_geoT[3] = geoTrans[3];
  m_geoT[4] = geoTrans[4];
  m_geoT[5] = geoTrans[5];
  
  if( ! computeInverseParameters( m_geoT, m_geoTInverse ) )
  {
    throw te::rst::Exception("Unable to compute the inverse affine parameters");
  }
  
  computeExtent();
}

const double* te::rst::Grid::getGeoreference() const
{
  return m_geoT;
}

double te::rst::Grid::getResolutionX() const
{
  assert( m_extent );
  return m_extent->getWidth() / ((double)m_nCols );
}

double te::rst::Grid::getResolutionY() const
{
  assert( m_extent );
  return m_extent->getHeight() / ((double)m_nRows );
}

int te::rst::Grid::getSRID() const
{
  return m_srid;
}

void te::rst::Grid::setSRID(int srid)
{
  m_srid = srid;
}

te::gm::Envelope* te::rst::Grid::getExtent()
{
  return m_extent;
}

const te::gm::Envelope* te::rst::Grid::getExtent() const
{
  return m_extent;
}

void te::rst::Grid::computeExtent() const
{
  te::gm::Coord2D ll = gridToGeo( -0.5, ((double)m_nRows) - 0.5 );
  te::gm::Coord2D lr = gridToGeo( ((double)m_nCols) - 0.5, ((double)m_nRows) - 0.5 );
  te::gm::Coord2D ur = gridToGeo( ((double)m_nCols) - 0.5, -0.5 );
  te::gm::Coord2D ul = gridToGeo( -0.5, -0.5 );

  if(m_extent)
  {
    delete m_extent;
  }
    
  m_extent = new te::gm::Envelope( std::min(ll.x, ul.x), std::min(ll.y, lr.y),
    std::max(ur.x, lr.x), std::max(ul.y, ur.y) );
}

void te::rst::Grid::gridToGeo(const double& col, const double& row, double& x, double& y) const
{
  x = col * m_geoT[0] + row * m_geoT[1] + m_geoT[2];
  y = col * m_geoT[3] + row * m_geoT[4] + m_geoT[5];
}

void te::rst::Grid::geoToGrid(const double& x, const double& y, double& col, double& row) const
{
  col = x * m_geoTInverse[0] + y * m_geoTInverse[1] + m_geoTInverse[2];
  row = x * m_geoTInverse[3] + y * m_geoTInverse[4] + m_geoTInverse[5];
}

bool te::rst::Grid::operator==(const Grid& rhs) const
{
  computeExtent();
  rhs.computeExtent();

  if (!m_extent->equals(*rhs.m_extent) ||
      m_nCols != rhs.m_nCols ||
      m_nRows != rhs.m_nRows ||
      m_srid != rhs.m_srid ||
      getGeoreference()[0] != rhs.getGeoreference()[0] ||
      getGeoreference()[1] != rhs.getGeoreference()[1] ||
      getGeoreference()[2] != rhs.getGeoreference()[2] ||
      getGeoreference()[3] != rhs.getGeoreference()[3] ||
      getGeoreference()[4] != rhs.getGeoreference()[4] ||
      getGeoreference()[5] != rhs.getGeoreference()[5])
    return false;
  else
    return true;
}

bool te::rst::Grid::computeAffineParameters( const double extentWidth,
  const double extentHeight, const unsigned int nColumns,
  const unsigned int nRows, const double extentULX,
  const double extentULY, double* affineParamsPtr,
  double* inverseAffineParamsPtr ) const
{
  affineParamsPtr[0] = extentWidth / ((double)nColumns);
  affineParamsPtr[1] = 0.0;
  affineParamsPtr[2] = extentULX + ( extentWidth / static_cast<double>(nColumns) / 2.0 );
  affineParamsPtr[3] = 0.0;
  affineParamsPtr[4] = -1.0 * extentHeight / ((double)nRows);
  affineParamsPtr[5] = extentULY - ( extentHeight / static_cast<double>(nRows) / 2.0 );

  return computeInverseParameters( affineParamsPtr, inverseAffineParamsPtr );
}

bool te::rst::Grid::computeInverseParameters( double* const affineParamsPtr,
  double* inverseAffineParamsPtr ) const
{
  const double determinant = ( affineParamsPtr[0] * affineParamsPtr[4] ) -
    ( affineParamsPtr[1] * affineParamsPtr[3] );
  if( determinant == 0.0 )
  {
    return false;
  }
  
  inverseAffineParamsPtr[0] = affineParamsPtr[4] / determinant;
  inverseAffineParamsPtr[1] = ( -1.0 * affineParamsPtr[1] ) / determinant;
  inverseAffineParamsPtr[2] = 
    ( 
      ( affineParamsPtr[1] * affineParamsPtr[5] ) 
      - 
      ( affineParamsPtr[2] * affineParamsPtr[4] ) 
    ) 
    / 
    determinant;
  inverseAffineParamsPtr[3] = ( -1.0 * affineParamsPtr[3] ) / determinant;
  inverseAffineParamsPtr[4] = ( affineParamsPtr[0] ) / determinant;
  inverseAffineParamsPtr[5] = 
    (
      ( -1.0 * ( affineParamsPtr[0] * affineParamsPtr[5] ) )
      + 
      ( affineParamsPtr[3] * affineParamsPtr[2] )
    )
    /
    determinant;   
    
  return true;
}
