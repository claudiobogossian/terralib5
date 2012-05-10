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

te::se::TextSymbolizer::TextSymbolizer()
  : m_geometry(0),
    m_label(0),
    m_font(0),
    m_labelPlacement(0),
    m_halo(0),
    m_fill(0)
{
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

void te::se::TextSymbolizer::setFont(Font* f)
{
  delete m_font;
  m_font = f;
}

void te::se::TextSymbolizer::setLabelPlacement(LabelPlacement* l)
{
  delete m_labelPlacement;
  m_labelPlacement = l;
}

void te::se::TextSymbolizer::setHalo(Halo* h)
{
  delete m_halo;
  m_halo = h;
}

void te::se::TextSymbolizer::setFill(Fill* f)
{
  delete m_fill;
  m_fill = f;
}

