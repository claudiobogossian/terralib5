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
  \file Font.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "Font.h"

// STL
#include <sstream>
#include <string> 
#include <iostream>
#include <vector>
#include <cstdlib>

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

std::string te::layout::Font::toString()
{
  std::string s_convert;
  std::stringstream ss;//create a stringstream

  s_convert = m_family;
  
  ss << m_pointSize;
  s_convert += "," + ss.str();
  s_convert += ",";

  s_convert += toString(m_bold);
  s_convert += ",";
  s_convert += toString(m_italic);
  s_convert += ",";
  s_convert += toString(m_underline);
  s_convert += ",";
  s_convert += toString(m_strikeout);
  s_convert += ",";
  s_convert += toString(m_kerning);

  return s_convert;
}

void te::layout::Font::fromString( std::string font )
{
  std::vector<std::string> strings;
  std::istringstream f(font);
  std::string s;    
  while (std::getline(f, s, ',')) 
  {
    strings.push_back(s);
  }

  if(strings.empty() || strings.size() > 7)
    return;

  m_family = strings[0];
  m_pointSize = std::atoi(strings[1].c_str());
  m_bold = toBool(strings[2]);
  m_italic = toBool(strings[3]);;
  m_underline = toBool(strings[4]);;
  m_strikeout = toBool(strings[5]);;
  m_kerning = toBool(strings[6]);;  
}

std::string te::layout::Font::toString( bool flag )
{
  return flag ? "true" : "false"; 
}

bool te::layout::Font::toBool( std::string str )
{
  if(str.compare("true") == 0)
  {
    return true;
  }
  else
  {
    return false;
  }
}
