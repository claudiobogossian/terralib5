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
  \file terralib/memory/TiledBand.cpp
 
  \brief A tiled band implementation for the In-Memory Raster.
*/

// TerraLib
#include "../common/Translator.h" 
#include "../raster/BandProperty.h"
#include "Exception.h"
#include "Raster.h"
#include "TiledBand.h"

// STL
#include <cassert>
#include <cstring>

te::mem::TiledBand::TiledBand(Raster* r, te::rst::BandProperty* p, std::size_t idx)
  : te::rst::Band(p, idx),
    m_raster(r),
    m_buff(0),
    m_getBuff(0),
    m_getBuffI(0),
    m_setBuff(0),
    m_setBuffI(0),
    m_ncols(0),
    m_nrows(0),
    m_blksize(0),
    m_nblksx(0),
    m_nblksy(0),
    m_blkw(0),
    m_blkh(0)
{
  m_blksize = getBlockSize();

  m_blkw = p->m_blkw;
  m_blkh = p->m_blkh;

  m_nblksx = p->m_nblocksx;
  m_nblksy = p->m_nblocksy;

  m_buff = new unsigned char**[m_nblksy];

  for(int i = 0; i < m_nblksy; ++i)
  {
    m_buff[i] = new unsigned char*[m_nblksx];

    for(int j = 0; j < m_nblksx; ++j)
    {
      m_buff[i][j] = new unsigned char[m_blksize];
    }
  }

  m_ncols = getRaster()->getNumberOfColumns();
  m_nrows = getRaster()->getNumberOfRows();

  te::rst::SetBlockFunctions(&m_getBuff, &m_getBuffI, &m_setBuff, &m_setBuffI, m_property->getType());
}

te::mem::TiledBand::TiledBand(const TiledBand& rhs)
  : te::rst::Band(rhs),
    m_raster(0),
    m_buff(0),
    m_getBuff(rhs.m_getBuff),
    m_getBuffI(rhs.m_getBuffI),
    m_setBuff(rhs.m_setBuff),
    m_setBuffI(rhs.m_setBuffI),
    m_ncols(rhs.m_ncols),
    m_nrows(rhs.m_nrows),
    m_blksize(rhs.m_blksize),
    m_nblksx(rhs.m_nblksx),
    m_nblksy(rhs.m_nblksy),
    m_blkw(rhs.m_blkw),
    m_blkh(rhs.m_blkh)
{
  m_buff = new unsigned char**[m_nblksy];

  for(int i = 0; i < m_nblksy; ++i)
  {
    m_buff[i] = new unsigned char*[m_nblksx];

    for(int j = 0; j < m_nblksx; ++j)
    {
      m_buff[i][j] = new unsigned char[m_blksize];

      memcpy(m_buff[i][j], rhs.m_buff[i][j], m_blksize);
    }
  }
}

te::mem::TiledBand::~TiledBand()
{
  for(int i = 0; i < m_nblksy; ++i)
  {
    for(int j = 0; j < m_nblksx; ++j)
    {
// kill tiles
      delete [] m_buff[i][j];
    }

// kill rows
    delete [] m_buff[i];
  }

// kill col
  delete [] m_buff;
}

te::rst::Raster* te::mem::TiledBand::getRaster() const
{
  return m_raster;
}

te::mem::TiledBand& te::mem::TiledBand::operator=(const TiledBand& rhs)
{
  if(&rhs != this)
  {
    if((m_blksize != rhs.m_blksize) ||
       (m_nblksx != rhs.m_nblksx) ||
       (m_nblksy != rhs.m_nblksy))
      throw Exception(TE_TR("You can not assign a band to another one with different block size or tile dimesinsions"));

    te::rst::Band::operator=(rhs);

    for(int i = 0; i < m_nblksy; ++i)
    {
      m_buff[i] = new unsigned char*[m_nblksx];

      for(int j = 0; j < m_nblksx; ++j)
      {
        m_buff[i][j] = new unsigned char[m_blksize];

        memcpy(m_buff[i][j], rhs.m_buff[i][j], m_blksize);
      }
    }

    m_getBuff = rhs.m_getBuff;
    m_getBuffI = rhs.m_getBuffI;
    m_setBuff = rhs.m_setBuff;
    m_setBuffI = rhs.m_setBuffI;
    m_ncols = rhs.m_ncols;
    m_nrows = rhs.m_nrows;
  }

  return *this;
}

void te::mem::TiledBand::getValue(unsigned int c, unsigned int r, double& value) const
{
  int blkx = c / m_blkw;

  int blky = r / m_blkh;

  int pos = c % m_blkw + ((r % m_blkh) * m_blkw);

  assert(pos < m_blksize);

  m_getBuff(pos, m_buff[blky][blkx], &value);
}

void te::mem::TiledBand::setValue(unsigned int c, unsigned int r, const double value)
{
  int blkx = c / m_blkw;

  int blky = r / m_blkh;

  int pos = c % m_blkw + ((r % m_blkh) * m_blkw);

  assert(pos < m_blksize);

  m_setBuff(pos, m_buff[blky][blkx], &value);
}

void te::mem::TiledBand::getIValue(unsigned int c, unsigned int r, double& value) const
{
  int blkx = c / m_blkw;

  int blky = r / m_blkh;

  int pos = c % m_blkw + ((r % m_blkh) * m_blkw);

  assert(pos < m_blksize);

  m_getBuffI(pos, m_buff[blky][blkx], &value);
}

void te::mem::TiledBand::setIValue(unsigned int c, unsigned int r, const double value)
{
  int blkx = c / m_blkw;

  int blky = r / m_blkh;

  int pos = c % m_blkw + ((r % m_blkh) * m_blkw);

  assert(pos < m_blksize);

  m_setBuffI(pos, m_buff[blky][blkx], &value);
}

void te::mem::TiledBand::read(int x, int y, void* buffer) const
{
  assert(x < m_nblksx && y < m_nblksy);

  memcpy(buffer, m_buff[y][x], m_blksize);
}

void* te::mem::TiledBand::read(int x, int y)
{
  assert(x < m_nblksx && y < m_nblksy);

  return m_buff[y][x];
}

void te::mem::TiledBand::write(int x, int y, void* buffer)
{
  assert(x < m_nblksx && y < m_nblksy);

  memcpy(m_buff[y][x], buffer, m_blksize);
}

void te::mem::TiledBand::setRaster(Raster* r)
{
  m_raster = r;
}
