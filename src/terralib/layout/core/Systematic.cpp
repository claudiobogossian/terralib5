/*  Copyright (C) 2001-2014 National Institute For Space Research (INPE) - Brazil.

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
  \file Systematic.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "Systematic.h"


te::layout::Systematic::Systematic() :
  m_height(0.),
  m_width(0.),
  m_scale(0),
  m_planarGap(0.),
  m_geoGap(0.)
{

}

te::layout::Systematic::~Systematic()
{

}

void te::layout::Systematic::setWidth( double w )
{
  m_width = w;
}

double te::layout::Systematic::getWidth()
{
  return m_width;
}

void te::layout::Systematic::setHeight( double h )
{
  m_height = h;
}

double te::layout::Systematic::getHeight()
{
  return m_height;
}

void te::layout::Systematic::setScale( int scale )
{
  m_scale = scale;
}

int te::layout::Systematic::getScale()
{
  return m_scale;
}

void te::layout::Systematic::setPlanarGap( double gap )
{
  m_planarGap = gap;
}

double te::layout::Systematic::getPlanarGap()
{
  return m_planarGap;
}

void te::layout::Systematic::setGeoGap( double gap )
{
  m_geoGap = gap;
}

double te::layout::Systematic::getGeoGap()
{
  return m_geoGap;
}
