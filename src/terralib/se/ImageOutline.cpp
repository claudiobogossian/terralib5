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
  \file terralib/se/ImageOutline.cpp
  
  \brief ImageOutline specifies how individual source rasters in a multi-raster set (such as a set of satellite-image scenes) should be outlined to make the individual-image locations visible.
*/

// TerraLib
#include "ImageOutline.h"
#include "LineSymbolizer.h"
#include "PolygonSymbolizer.h"

te::se::ImageOutline::ImageOutline()
  : m_symbol(0)
{
}

te::se::ImageOutline::~ImageOutline()
{
  delete m_symbol;
}

void te::se::ImageOutline::setSymbolizer(Symbolizer* s)
{
  delete m_symbol;
  m_symbol = s;
}

te::se::Symbolizer* te::se::ImageOutline::getSymbolizer() const
{
  if(m_symbol)
    return m_symbol->clone();
  else
    return 0;
}

te::se::ImageOutline* te::se::ImageOutline::clone() const
{
  ImageOutline* io = new ImageOutline;

  if(m_symbol)
    io->setSymbolizer(m_symbol->clone());

  return io;
}
