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
  \file terralib/memory/Band.cpp

  \brief Band implementation for the In-Memory Raster.
*/

// TerraLib
#include "../common/Translator.h"
#include "../raster/BandProperty.h"
#include "../raster/Utils.h"
#include "Band.h"
#include "Exception.h"
#include "Raster.h"

// STL
#include <cassert>
#include <cstring>

te::mem::Band::Band(Raster* r, te::rst::BandProperty* p, std::size_t idx, void* externalBuffer)
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
    m_releaseBuffer(true)
{
  int pxlsize = te::rst::GetPixelSize(m_property->getType());

  m_blksize = m_property->m_blkw * m_property->m_blkh * pxlsize;

// fixing block sizes for special cases (1, 2, and 4 bits raster data type)
  if (m_property->getType() == te::dt::R4BITS_TYPE)
    m_blksize = te::rst::Round(m_blksize / 2);
  if (m_property->getType() == te::dt::R2BITS_TYPE)
    m_blksize = te::rst::Round(m_blksize / 4);
  if (m_property->getType() == te::dt::R1BIT_TYPE)
    m_blksize = te::rst::Round(m_blksize / 8);

  if(externalBuffer == 0)
    m_buff = new unsigned char[m_blksize];
  else
  {
    m_buff = static_cast<unsigned char*>(externalBuffer);
    m_releaseBuffer = false;
  }

  m_ncols = getRaster()->getNumberOfColumns();
  m_nrows = getRaster()->getNumberOfRows();

  te::rst::SetBlockFunctions(&m_getBuff, &m_getBuffI, &m_setBuff, &m_setBuffI, m_property->getType());
}

te::mem::Band::Band(const Band& rhs)
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
    m_releaseBuffer(true)
{
  m_buff = new unsigned char[m_blksize];

  memcpy(m_buff, rhs.m_buff, m_blksize);
}

te::mem::Band::~Band()
{
  if(m_releaseBuffer)
    delete [] m_buff;
}

te::rst::Raster* te::mem::Band::getRaster() const
{
  return m_raster;
}

te::mem::Band& te::mem::Band::operator=(const Band& rhs)
{
  if(&rhs != this)
  {
    if(m_blksize != rhs.m_blksize)
      throw Exception(TE_TR("You can not assign a band to another one with different block size"));

    te::rst::Band::operator=(rhs);

    memcpy(m_buff, rhs.m_buff, m_blksize);

    m_getBuff = rhs.m_getBuff;
    m_getBuffI = rhs.m_getBuffI;
    m_setBuff = rhs.m_setBuff;
    m_setBuffI = rhs.m_setBuffI;
    m_ncols = rhs.m_ncols;
    m_nrows = rhs.m_nrows;
  }

  return *this;
}

void te::mem::Band::getValue(unsigned int c, unsigned int r, double& value) const
{
  int pos = c + r * m_ncols;

  m_getBuff(pos, m_buff, &value);
}

void te::mem::Band::setValue(unsigned int c, unsigned int r, const double value)
{
  int pos = c + r * m_ncols;

  m_setBuff(pos, m_buff, &value);
}

void te::mem::Band::getIValue(unsigned int c, unsigned int r, double& value) const
{
  int pos = c + r * m_ncols;

  m_getBuffI(pos, m_buff, &value);
}

void te::mem::Band::setIValue(unsigned int c, unsigned int r, const double value)
{
  int pos = c + r * m_ncols;

  m_setBuffI(pos, m_buff, &value);
}

void te::mem::Band::read(int x, int y, void* buffer) const
{
  assert(x == 0 && y == 0);

  memcpy(buffer, m_buff, m_blksize);
}

void* te::mem::Band::read(int x, int y)
{
  assert(x == 0 && y == 0);

  return m_buff;
}

void te::mem::Band::write(int x, int y, void* buffer)
{
  assert(x == 0 && y == 0);

  memcpy(m_buff, buffer, m_blksize);
}

void te::mem::Band::setRaster(Raster* r)
{
  m_raster = r;
}

int te::mem::Band::getBlockSize() const
{
  return m_blksize;
}
