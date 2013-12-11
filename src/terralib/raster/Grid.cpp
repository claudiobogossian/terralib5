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
    // Resolution X and Y
    m_geoT[1] = m_extent->getWidth() / static_cast<double>(nCols);
    m_geoT[5] = -1.0 * m_extent->getHeight() / static_cast<double>(nRows);
    
    // Pixell 0,0 upper-left coods
    m_geoT[0] = m_extent->m_llx + ( m_geoT[1] / 2.0 );
    m_geoT[3] = m_extent->m_ury + ( m_geoT[5] / 2.0 );
    
    m_geoT[2] = 0.0;    
    m_geoT[4] = 0.0;
    
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
    // Resolution X and Y
    m_geoT[1] = m_extent->getWidth() / static_cast<double>(nCols);
    m_geoT[5] = -1.0 * m_extent->getHeight() / static_cast<double>(nRows);
    
    // Pixell 0,0 upper-left coods
    m_geoT[0] = m_extent->m_llx + ( m_geoT[1] / 2.0 );
    m_geoT[3] = m_extent->m_ury + ( m_geoT[5] / 2.0 );
    
    m_geoT[2] = 0.0;    
    m_geoT[4] = 0.0;    
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
  m_nCols = static_cast<unsigned int> (mbr->getWidth() / resX + 0.5);
  m_nRows = static_cast<unsigned int> (mbr->getHeight() / resY + 0.5);

  if(mbr)
  {
    // Resolution X and Y
    m_geoT[1] = m_extent->getWidth() / static_cast<double>(m_nCols);
    m_geoT[5] = -1.0 * m_extent->getHeight() / static_cast<double>(m_nRows);
    
    // Pixell 0,0 upper-left coods
    m_geoT[0] = m_extent->m_llx + ( m_geoT[1] / 2.0 );
    m_geoT[3] = m_extent->m_ury + ( m_geoT[5] / 2.0 );    
    
    m_geoT[2] = 0.0;    
    m_geoT[4] = 0.0;
    
  }
  else
  {
    setGeoreference(te::gm::Coord2D(0, 0), srid, resX, resY);
    computeExtent();
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
  memcpy(m_geoT, rhs.m_geoT, sizeof(double) * 6);
  
  if( rhs.m_extent )
  {
    m_extent = new te::gm::Envelope(*rhs.m_extent);
  }
  else
  {
    computeExtent();
  }
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
}

unsigned int te::rst::Grid::getNumberOfColumns() const
{
  return m_nCols;
}

void te::rst::Grid::setNumberOfRows(unsigned int nRows)
{
  m_nRows = nRows;
}

unsigned int te::rst::Grid::getNumberOfRows() const
{
  return m_nRows;
}

void te::rst::Grid::setGeoreference(const te::gm::Coord2D& ulLocation, int srid, double resX, double resY)
{
  m_srid = srid;
  
  // Resolution X and Y
  m_geoT[1] = resX;
  m_geoT[5] = -1.0 * resY;
  
  // Pixell 0,0 upper-left coods
  m_geoT[0] = ulLocation.x + ( m_geoT[1] / 2.0 );
  m_geoT[3] = ulLocation.y + ( m_geoT[5] / 2.0 );
  
  m_geoT[2] = 0.0;
  m_geoT[4] = 0.0;
  
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
  
  computeExtent();
}

const double* te::rst::Grid::getGeoreference() const
{
  return m_geoT;
}

double te::rst::Grid::getResolutionX() const
{
  return std::abs( m_geoT[1] );
}

double te::rst::Grid::getResolutionY() const
{
  return ( -1.0 * m_geoT[5] );
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

  if(!m_extent)
    m_extent = new te::gm::Envelope;

  m_extent->init(std::min(ll.x, ul.x),
                 std::min(ll.y, lr.y),
                 std::max(ur.x, lr.x),
                 std::max(ul.y, ur.y));
}

void te::rst::Grid::gridToGeo(const double& col, const double& row, double& x, double& y) const
{
  x = m_geoT[0] + col * m_geoT[1] + row * m_geoT[2];
  y = m_geoT[3] + col * m_geoT[4] + row * m_geoT[5];
}

void te::rst::Grid::geoToGrid(const double& x, const double& y, double& col, double& row) const
{
  col = (m_geoT[5] * (x - m_geoT[0]) - m_geoT[2] * (y - m_geoT[3])) / (m_geoT[1] * m_geoT[5] - m_geoT[2] * m_geoT[4]);
  row = (y - m_geoT[3] - col * m_geoT[4]) / m_geoT[5];
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
