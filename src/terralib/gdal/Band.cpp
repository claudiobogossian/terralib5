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
  \file terralib/gdal/Band.cpp
 
  \brief This class represents raster band description.
*/

// TerraLib
#include "../common/Translator.h"
#include "../raster/RasterProperty.h"
#include "../raster/Utils.h"
#include "Band.h"
#include "Exception.h"
#include "Raster.h"
#include "Utils.h"

// STL
#include <cassert>
#include <limits>

// GDAL
#include <gdal_priv.h>

te::gdal::Band::Band(Raster* rstPtr, std::size_t idx, GDALRasterBand* gdalRasterBandPtr )
  : te::rst::Band(0, idx),
    m_raster(rstPtr),
    m_rasterBand(gdalRasterBandPtr)
{
  m_gdaltype = m_rasterBand->GetRasterDataType();
  
  m_property = GetBandProperty(m_rasterBand, idx);

  te::rst::SetBlockFunctions(&m_getBuff, &m_getBuffI, &m_setBuff, &m_setBuffI, m_property->getType());

  m_buffer = new unsigned char[getBlockSize()];

  m_x = std::numeric_limits<int>::max();

  m_y = std::numeric_limits<int>::max();

  m_update_buffer = false;  
}

te::gdal::Band::Band(const Band& rhs)
  : te::rst::Band(rhs),
    m_raster(rhs.m_raster),
    m_rasterBand(rhs.m_rasterBand),
    m_getBuff(rhs.m_getBuff),
    m_gdaltype(rhs.m_gdaltype)
{
  m_buffer = new unsigned char[getBlockSize()];

  m_x = std::numeric_limits<int>::max();

  m_y = std::numeric_limits<int>::max();

  m_update_buffer = false;
}

te::gdal::Band::~Band()
{
  if (m_update_buffer)
  {
    m_rasterBand->WriteBlock(m_x, m_y, m_buffer);

    m_rasterBand->FlushCache();
  }

  unsigned char* buff = (unsigned char*)m_buffer;

  delete [] buff;
}

te::rst::Raster* te::gdal::Band::getRaster() const
{
  return m_raster;
}

te::gdal::Band& te::gdal::Band::operator=(const Band& rhs)
{
  assert(m_property->getType() == rhs.m_property->getType());

  if(this != &rhs)
  {
    te::rst::Band::operator=(rhs);

    m_getBuff = rhs.m_getBuff;

    unsigned char* buffer = new unsigned char[rhs.getBlockSize()];

    for (int x = 0; x < m_property->m_nblocksx; ++x)
      for (int y = 0; y < m_property->m_nblocksy; ++y)
      {
        rhs.read(x, y, buffer);

        write(x, y, buffer);
      }

    delete [] buffer;
  }

  return *this;
}

void te::gdal::Band::getValue(unsigned int c, unsigned int r, double& value) const
{
  m_i = placeBuffer(c, r);

  m_getBuff(m_i, m_buffer, &value);
}

void te::gdal::Band::setValue(unsigned int c, unsigned int r, const double value)
{
  m_i = placeBuffer(c, r);

  m_setBuff(m_i, m_buffer, &value);

  m_update_buffer = true;
}

void te::gdal::Band::getIValue(unsigned int c, unsigned int r, double& value) const
{
  m_i = placeBuffer(c, r);

  m_getBuffI(m_i, m_buffer, &value);
}

void te::gdal::Band::setIValue(unsigned int c, unsigned int r, const double value)
{
  m_i = placeBuffer(c, r);

  m_setBuffI(m_i, m_buffer, &value);

  m_update_buffer = true;
}

void te::gdal::Band::read(int x, int y, void* buffer) const
{
  m_rasterBand->ReadBlock(x, y, buffer);
  
  // If there is a palette, the values must be interpreted as
  // Palette indexes
  
  if( m_rasterBand->GetColorInterpretation() == GCI_PaletteIndex )
  {
    const int bufferSize = m_property->m_blkh * m_property->m_blkw;
    
    const GDALColorTable& cTable = *( m_rasterBand->GetColorTable() );
    double value = 0;
    GDALColorEntry const * cEntryPtr = 0;
    
    switch( te::rst::Band::m_idx )
    {
      case 0 :
        for( int bufferIdx = 0 ; bufferIdx < bufferSize ; ++bufferIdx )
        {
          m_getBuff( bufferIdx, buffer, &value );
          
          cEntryPtr = cTable.GetColorEntry( (int)value );
          assert(cEntryPtr );
          
          value = (double)cEntryPtr->c1;
          
          m_setBuff( bufferIdx, buffer, &value );
        }
        break;
      case 1 :
        for( int bufferIdx = 0 ; bufferIdx < bufferSize ; ++bufferIdx )
        {
          m_getBuff( bufferIdx, buffer, &value );
          
          cEntryPtr = cTable.GetColorEntry( (int)value );
          assert(cEntryPtr );
          
          value = (double)cEntryPtr->c2;
          
          m_setBuff( bufferIdx, buffer, &value );
        }
        break;
      case 2 :
        for( int bufferIdx = 0 ; bufferIdx < bufferSize ; ++bufferIdx )
        {
          m_getBuff( bufferIdx, buffer, &value );
          
          cEntryPtr = cTable.GetColorEntry( (int)value );
          assert(cEntryPtr );
          
          value = (double)cEntryPtr->c3;
          
          m_setBuff( bufferIdx, buffer, &value );
        }
        break;
      case 3 :
        for( int bufferIdx = 0 ; bufferIdx < bufferSize ; ++bufferIdx )
        {
          m_getBuff( bufferIdx, buffer, &value );
          
          cEntryPtr = cTable.GetColorEntry( (int)value );
          assert(cEntryPtr );
          
          value = (double)cEntryPtr->c4;
          
          m_setBuff( bufferIdx, buffer, &value );
        }
        break;
      default :
        throw Exception(TE_TR("Invalid band index"));
        break;          
    }
  }   
}

void* te::gdal::Band::read(int x, int y)
{
  if( ( x != m_x ) || ( y != m_y ) )
  {
    if (m_update_buffer)
    {
      m_rasterBand->WriteBlock(m_x, m_y, m_buffer);

      m_rasterBand->FlushCache();

      m_update_buffer = false;
    }
        
    read( x, y, m_buffer );
    m_x = x;
    m_y = y;
  }
  
  return m_buffer;
}

void te::gdal::Band::write(int x, int y, void* buffer)
{
  m_rasterBand->WriteBlock(x, y, buffer);
}

int te::gdal::Band::placeBuffer(unsigned c, unsigned r) const
{
  assert(c >= 0 && c < m_raster->getNumberOfColumns());
  assert(r >= 0 && r < m_raster->getNumberOfRows());

  m_currX = c / m_property->m_blkw;

  m_currY = r / m_property->m_blkh;

  m_currC = c % m_property->m_blkw;

  m_currR = r % m_property->m_blkh;

  if (m_currX != m_x || m_currY != m_y)
  {
    if (m_update_buffer)
    {
      m_rasterBand->WriteBlock(m_x, m_y, m_buffer);

      m_rasterBand->FlushCache();

      m_update_buffer = false;
    }

    read(m_currX, m_currY, m_buffer);

    m_x = m_currX;

    m_y = m_currY;
  }

// calculates and returns the value of m_i
  return (m_currC + m_currR * m_property->m_blkw);
}
