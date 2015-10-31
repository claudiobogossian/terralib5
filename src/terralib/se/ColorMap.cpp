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
  \file terralib/se/ColorMap.cpp
  
  \brief A ColorMap defines either the colors of a pallette-type raster source or the mapping of numeric pixel values to colors.
*/

// TerraLib
#include "Categorize.h"
#include "ColorMap.h"
#include "Interpolate.h"
#include "Recode.h"

te::se::ColorMap::ColorMap()
  : m_categorize(0),
    m_interpolate(0),
    m_recode(0)
{
}

te::se::ColorMap::ColorMap(const ColorMap& rhs)
  : m_categorize(0),
    m_interpolate(0),
    m_recode(0)
{
  if(rhs.m_categorize)
  {
    m_categorize = rhs.m_categorize->clone();
  }

  if(rhs.m_interpolate)
  {
    m_interpolate = rhs.m_interpolate->clone();
  }

  if (rhs.m_recode)
  {
    m_recode = rhs.m_recode->clone();
  }
}

te::se::ColorMap::~ColorMap()
{
  delete m_categorize;
  delete m_interpolate;
  delete m_recode;
}

void te::se::ColorMap::setCategorize(Categorize* c)
{
  delete m_categorize;
  m_categorize = c;
}

te::se::Categorize* te::se::ColorMap::getCategorize() const
{
  return m_categorize;
}

void te::se::ColorMap::setInterpolate(Interpolate* i)
{
  delete m_interpolate;
  m_interpolate = i;
}

te::se::Interpolate* te::se::ColorMap::getInterpolate() const
{
  return m_interpolate;
}

void te::se::ColorMap::setRecode(Recode* i)
{
  delete m_recode;
  m_recode = i;
}

te::se::Recode* te::se::ColorMap::getRecode() const
{
  return m_recode;
}

te::se::ColorMap* te::se::ColorMap::clone() const
{
  return new ColorMap(*this);
}

