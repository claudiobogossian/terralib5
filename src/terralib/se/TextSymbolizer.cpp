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
  \file terralib/se/TextSymbolizer.cpp
  
  \brief A TextSymbolizer is used to render text labels according to various graphical parameters.
*/

// TerraLib
#include "../fe/PropertyName.h"
#include "Fill.h"
#include "Font.h"
#include "Halo.h"
#include "LabelPlacement.h"
#include "ParameterValue.h"
#include "TextSymbolizer.h"

const std::string te::se::TextSymbolizer::sm_type("TextSymbolizer");

te::se::TextSymbolizer::TextSymbolizer()
  : te::se::Symbolizer(),
    m_geometry(0),
    m_label(0),
    m_font(0),
    m_labelPlacement(0),
    m_halo(0),
    m_fill(0)
{
}

te::se::TextSymbolizer::TextSymbolizer(const te::se::TextSymbolizer& rhs)
  : te::se::Symbolizer(rhs),
    m_geometry(0),
    m_label(0),
    m_font(0),
    m_labelPlacement(0),
    m_halo(0),
    m_fill(0)
{
  if(rhs.m_geometry)
    m_geometry = new te::fe::PropertyName(rhs.m_geometry->getName());

  if(rhs.m_label)
    m_label = rhs.m_label->clone();

  if(rhs.m_font)
    m_font = rhs.m_font->clone();

  if(rhs.m_labelPlacement)
    m_labelPlacement = rhs.m_labelPlacement->clone();

  if(rhs.m_halo)
    m_halo = rhs.m_halo->clone();

  if(rhs.m_fill)
    m_fill = rhs.m_fill->clone();
}

te::se::TextSymbolizer::~TextSymbolizer()
{
  delete m_geometry;
  delete m_label;
  delete m_font;
  delete m_labelPlacement;
  delete m_halo;
  delete m_fill;
}

void te::se::TextSymbolizer::setGeometry(te::fe::PropertyName* g)
{
  delete m_geometry;
  m_geometry = g;
}

void te::se::TextSymbolizer::setLabel(ParameterValue* l)
{
  delete m_label;
  m_label = l;
}

const te::se::ParameterValue* te::se::TextSymbolizer::getLabel() const
{
  return m_label;
}

void te::se::TextSymbolizer::setFont(Font* f)
{
  delete m_font;
  m_font = f;
}

const te::se::Font* te::se::TextSymbolizer::getFont() const
{
  return m_font;
}

void te::se::TextSymbolizer::setLabelPlacement(LabelPlacement* l)
{
  delete m_labelPlacement;
  m_labelPlacement = l;
}

const te::se::LabelPlacement* te::se::TextSymbolizer::getLabelPlacement() const
{
  return m_labelPlacement;
}

void te::se::TextSymbolizer::setHalo(Halo* h)
{
  delete m_halo;
  m_halo = h;
}

const te::se::Halo* te::se::TextSymbolizer::getHalo() const
{
  return m_halo;
}

void te::se::TextSymbolizer::setFill(Fill* f)
{
  delete m_fill;
  m_fill = f;
}

const te::se::Fill* te::se::TextSymbolizer::getFill() const
{
  return m_fill;
}

const std::string& te::se::TextSymbolizer::getType() const
{
  return sm_type;
}

te::se::Symbolizer* te::se::TextSymbolizer::clone() const
{
  return new TextSymbolizer(*this);
}
