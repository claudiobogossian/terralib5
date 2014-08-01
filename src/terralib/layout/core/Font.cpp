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
  \file Font.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "Font.h"

te::layout::Font::Font() :
m_family("Arial"),
m_pointSize(12),
m_bold(false),
m_italic(false),
m_underline(false),
m_strikeout(false),
m_kerning(false),
m_textAlign(TPTopLeft)
{

}

te::layout::Font::~Font()
{

}

void te::layout::Font::setFamily( std::string family )
{
  m_family = family;
}

std::string te::layout::Font::getFamily()
{
  return m_family;
}

void te::layout::Font::setPointSize( int point )
{
  m_pointSize = point;
}

int te::layout::Font::getPointSize()
{
  return m_pointSize;
}

void te::layout::Font::setBold( bool bold )
{
  m_bold = bold;
}

bool te::layout::Font::isBold()
{
  return m_bold;
}

void te::layout::Font::setItalic( bool italic )
{
  m_italic = italic;
}

bool te::layout::Font::isItalic()
{
  return m_italic;
}

void te::layout::Font::setUnderline( bool underline )
{
  m_underline = underline;
}

bool te::layout::Font::isUnderline()
{
  return m_underline;
}

void te::layout::Font::setStrikeout( bool strikeout )
{
  m_strikeout = strikeout;
}

bool te::layout::Font::isStrikeout()
{
  return m_strikeout;
}

void te::layout::Font::setKerning( bool kerning )
{
  m_kerning = kerning;
}

bool te::layout::Font::isKerning()
{
  return m_kerning;
}
