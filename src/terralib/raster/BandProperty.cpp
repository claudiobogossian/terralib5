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
  \file terralib/raster/BandProperty.cpp

  \brief It describes the range of one dimension of a raster.
*/

// TerraLib
#include "../common/UnitOfMeasure.h"
#include "../datatype/Enums.h"
#include "BandProperty.h"
#include "Enums.h"

// STL
#include <limits>

te::rst::BandProperty::BandProperty(std::size_t idx, int t, const std::string& desc)
  : m_idx(idx),
    m_type(t),
    m_description(desc),
    m_noDataValue(std::numeric_limits<double>::max()),
    m_valuesOffset(0.0, 0.0),
    m_valuesScale(1.0, 0.0),
    m_colorInterp(te::rst::UndefCInt),
    m_paletteInterp(te::rst::UndefPalInt),
    m_blkw(0),
    m_blkh(0),
    m_nblocksx(0),
    m_nblocksy(0)
{
}

te::rst::BandProperty::BandProperty(const te::rst::BandProperty& rhs)
  : m_idx(rhs.m_idx),
    m_type(rhs.m_type),
    m_description(rhs.m_description),
    m_metadata(rhs.m_metadata),
    m_noDataValue(rhs.m_noDataValue),
    m_valuesOffset(rhs.m_valuesOffset),
    m_valuesScale(rhs.m_valuesScale),
    m_categoryNames(rhs.m_categoryNames),
    m_colorInterp(rhs.m_colorInterp),
    m_paletteInterp(rhs.m_paletteInterp),
    m_palette(rhs.m_palette),
    m_blkw(rhs.m_blkw),
    m_blkh(rhs.m_blkh),
    m_nblocksx(rhs.m_nblocksx),
    m_nblocksy(rhs.m_nblocksy),
    m_unit(rhs.m_unit)
{
}

te::rst::BandProperty::~BandProperty()
{
}

te::rst::BandProperty& te::rst::BandProperty::operator=(const BandProperty& rhs)
{
  if(this != &rhs)
  {
    m_idx = rhs.m_idx;
    m_type = rhs.m_type;
    m_description = rhs.m_description;
    m_metadata = rhs.m_metadata;
    m_noDataValue = rhs.m_noDataValue;
    m_valuesOffset = rhs.m_valuesOffset;
    m_valuesScale = rhs.m_valuesScale;
    m_categoryNames = rhs.m_categoryNames;
    m_colorInterp = rhs.m_colorInterp;
    m_paletteInterp = rhs.m_paletteInterp;
    m_palette = rhs.m_palette;
    m_blkw = rhs.m_blkw;
    m_blkh = rhs.m_blkh;
    m_nblocksx = rhs.m_nblocksx;
    m_nblocksy = rhs.m_nblocksy;
    m_unit = rhs.m_unit;
  }

  return *this;
}

bool te::rst::BandProperty::isComplex() const
{
  return (m_type == te::dt::CDOUBLE_TYPE || m_type == te::dt::CFLOAT_TYPE ||
          m_type == te::dt::CINT16_TYPE || m_type == te::dt::CINT32_TYPE);
}
