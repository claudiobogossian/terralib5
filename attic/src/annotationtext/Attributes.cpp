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
  \file Attributes.cpp

  \brief An Text Element consists of a string, location, leader line and may have attribute.
 */

// TerraLib
#include "Attributes.h"

te::at::Attributes::Attributes()
{
  m_family = "Verdana"; 
  m_pointSize = 12.;
  m_style = NormalStyle;
  m_weight = NormalWeight;
  m_decoration = None;
  m_letterSpacing = 0.;
  m_wordSpacing = 0.;
  m_fillColor = "black";
  m_fillOpacity = 1.;
  m_strokeColor.clear();
  m_strokeWidth = 0.;
  m_strokeOpacity = 1.;
  m_hAlign = Start;
  m_vAlign = Baseline;
  m_multiLineJustification = Left;
  m_multiLineSpacing = 0.;
}

te::at::Attributes::Attributes(const std::string& family,
                               const double pointSize,
                               FontStyle style,
                               FontWeight weight,
                               HorizontalAlignment ha,
                               VerticalAlignment va)
  : m_family(family),
    m_pointSize(pointSize),
    m_style(style),
    m_weight(weight)
{
  m_decoration = None;
  m_letterSpacing = 0.;
  m_wordSpacing = 0.;
  m_fillColor = "black";
  m_fillOpacity = 1.;
  m_strokeColor.clear();
  m_strokeWidth = 0.;
  m_strokeOpacity = 1.;
  m_multiLineJustification = Left;
  m_multiLineSpacing = 0.;
  m_hAlign = ha;
  m_vAlign = va;
}

te::at::Attributes::Attributes(const Attributes& rhs)
{
  m_family = rhs.m_family; 
  m_pointSize = rhs.m_pointSize;
  m_style = rhs.m_style;
  m_weight = rhs.m_weight;
  m_decoration = rhs.m_decoration;
  m_letterSpacing = rhs.m_letterSpacing;
  m_wordSpacing = rhs.m_wordSpacing;
  m_fillColor = rhs.m_fillColor;
  m_fillOpacity = rhs.m_fillOpacity;
  m_strokeColor = rhs.m_strokeColor;
  m_strokeWidth = rhs.m_strokeWidth;
  m_strokeOpacity = rhs.m_strokeOpacity;
  m_hAlign = rhs.m_hAlign;
  m_vAlign = rhs.m_vAlign;
  m_multiLineJustification = rhs.m_multiLineJustification;
  m_multiLineSpacing = rhs.m_multiLineSpacing;
}

te::at::Attributes& te::at::Attributes::operator=(const Attributes& rhs)
{
  m_family = rhs.m_family; 
  m_pointSize = rhs.m_pointSize;
  m_style = rhs.m_style;
  m_weight = rhs.m_weight;
  m_decoration = rhs.m_decoration;
  m_letterSpacing = rhs.m_letterSpacing;
  m_wordSpacing = rhs.m_wordSpacing;
  m_fillColor = rhs.m_fillColor;
  m_fillOpacity = rhs.m_fillOpacity;
  m_strokeColor = rhs.m_strokeColor;
  m_strokeWidth = rhs.m_strokeWidth;
  m_strokeOpacity = rhs.m_strokeOpacity;
  m_hAlign = rhs.m_hAlign;
  m_vAlign = rhs.m_vAlign;
  m_multiLineJustification = rhs.m_multiLineJustification;
  m_multiLineSpacing = rhs.m_multiLineSpacing;
  return *this;
}

te::at::Attributes::~Attributes()
{
}

void te::at::Attributes::setFamily(const std::string& family)
{
  m_family = family;
}

const std::string& te::at::Attributes::getFamily() const
{
  return m_family;
}

void te::at::Attributes::setPointSize(const double& ps)
{
  m_pointSize = ps;
}

const double& te::at::Attributes::getPointSize() const
{
  return m_pointSize;
}

void te::at::Attributes::setStyle(FontStyle style)
{
  m_style = style;
}

te::at::FontStyle te::at::Attributes::getStyle() const
{
  return m_style;
}

void te::at::Attributes::setWeight(FontWeight weight)
{
  m_weight = weight;
}

te::at::FontWeight te::at::Attributes::getWeight() const
{
  return m_weight;
}

void te::at::Attributes::setDecoration(TextDecoration decoration)
{
  m_decoration = decoration;
}

te::at::TextDecoration te::at::Attributes::getDecoration() const
{
  return m_decoration;
}

void te::at::Attributes::setLetterSpacing(const double& spacing)
{
  m_letterSpacing = spacing;
}

const double& te::at::Attributes::getLetterSpacing() const
{
  return m_letterSpacing;
} 

void te::at::Attributes::setWordSpacing(const double& spacing)
{
  m_wordSpacing = spacing;
}

const double& te::at::Attributes::getWordSpacing() const
{
  return m_wordSpacing;
}

void te::at::Attributes::setTextColor(const std::string& color)
{
  m_fillColor = color;
}

const std::string& te::at::Attributes::getTextColor() const
{
  return m_fillColor;
}

void te::at::Attributes::setTextOpacity(double& opacity)
{
  m_fillOpacity = opacity;
}

const double& te::at::Attributes::getTextOpacity() const
{
  return m_fillOpacity;
}

void te::at::Attributes::setTextStrokecolor(const std::string& color)
{
  m_strokeColor = color;
}

const std::string& te::at::Attributes::getTextStrokeColor() const
{
  return m_strokeColor;
}

void te::at::Attributes::setTextStrokeWidth(const double& width)
{
  m_strokeWidth = width;
}

const double& te::at::Attributes::getTextStrokeWidth() const
{
  return m_strokeWidth;
}

void te::at::Attributes::setTextStrokeOpacity(const double& opacity)
{
  m_strokeOpacity = opacity;
}

const double& te::at::Attributes::getTextStrokeOpacity() const
{
  return m_strokeOpacity;
}

void te::at::Attributes::setHorizontalAlignment(HorizontalAlignment ha)
{
  m_hAlign = ha;
}

te::at::HorizontalAlignment te::at::Attributes::getHorizontalAlignment() const
{
  return m_hAlign;
}

void te::at::Attributes::setVerticalAlignment(VerticalAlignment va)
{
  m_vAlign = va;
}

te::at::VerticalAlignment te::at::Attributes::getVerticalAlignment() const
{
  return m_vAlign;
}

void te::at::Attributes::setLineJustification(LineJustification just)
{
  m_multiLineJustification = just;
}

te::at::LineJustification te::at::Attributes::getLineJustification() const
{
  return m_multiLineJustification;
}

void te::at::Attributes::setLineSpacing(const double& spacing)
{
  m_multiLineSpacing = spacing;
}

const double& te::at::Attributes::getLineSpacing() const
{
  return m_multiLineSpacing;
}

