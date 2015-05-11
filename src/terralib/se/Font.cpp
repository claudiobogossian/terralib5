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
  \file terralib/se/Font.cpp
  
  \brief A Font specifies the text font to use in a text symbolizer.
*/

// TerraLib
#include "../common/STLUtils.h"
#include "Font.h"
#include "SvgParameter.h"

std::map<te::se::Font::FontStyleType, std::string>  te::se::Font::sm_fontStyleMap;
std::map<te::se::Font::FontWeightType, std::string> te::se::Font::sm_fontWeightMap;

const std::string te::se::Font::sm_family = "font-family";
const std::string te::se::Font::sm_style  = "font-style";
const std::string te::se::Font::sm_weight = "font-weight";
const std::string te::se::Font::sm_size   = "font-size";

te::se::Font::Font()
{
  // FontStyle
  sm_fontStyleMap[te::se::Font::StyleNormal]  = "normal";
  sm_fontStyleMap[te::se::Font::StyleItalic]  = "italic";
  sm_fontStyleMap[te::se::Font::StyleOblique] = "oblique";
  
  // FontWeightType
  sm_fontWeightMap[te::se::Font::WeightNormal]  = "normal";
  sm_fontWeightMap[te::se::Font::WeightBold]    = "bold";
}

te::se::Font::~Font()
{
  te::common::FreeContents(m_svgParams);
}

void te::se::Font::add(te::se::SvgParameter* p)
{
  std::string name = p->getName();
  std::map<std::string, te::se::SvgParameter*>::iterator it = m_svgParams.find(name);
  if(it != m_svgParams.end())
    delete it->second;
  m_svgParams[name] = p;
}

void te::se::Font::setFamily(const std::string& family)
{
  setParameter(sm_family, family);
}

void te::se::Font::setStyle(const te::se::Font::FontStyleType& style)
{
  setParameter(sm_style, sm_fontStyleMap[style]);
}

void te::se::Font::setWeight(const FontWeightType& weight)
{
  setParameter(sm_weight, sm_fontWeightMap[weight]);
}

void te::se::Font::setSize(const std::string& size)
{
  setParameter(sm_size, size);
}

const te::se::SvgParameter* te::se::Font::getFamily() const
{
  return getParameter(sm_family);
}

const te::se::SvgParameter* te::se::Font::getStyle() const
{
  return getParameter(sm_style);
}

const te::se::SvgParameter* te::se::Font::getWeight() const
{
  return getParameter(sm_weight);
}

const te::se::SvgParameter* te::se::Font::getSize() const
{
  return getParameter(sm_size);
}

te::se::Font* te::se::Font::clone() const
{
  Font* font = new Font;

  std::map<std::string, SvgParameter*>::const_iterator it;
  for(it = m_svgParams.begin(); it != m_svgParams.end(); ++it)
    font->add(it->second->clone());

  return font;
}

void te::se::Font::setParameter(const std::string& name, const std::string& value)
{
  std::map<std::string, te::se::SvgParameter*>::iterator it = m_svgParams.find(name);
  if(it != m_svgParams.end())
    delete it->second;
  m_svgParams[name] = new te::se::SvgParameter(name, value);
}

const te::se::SvgParameter* te::se::Font::getParameter(const std::string& name) const
{
  std::map<std::string, te::se::SvgParameter*>::const_iterator it = m_svgParams.find(name);
  if(it != m_svgParams.end())
    return it->second;

  return 0; // Not found
}
