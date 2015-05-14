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
  \file terralib/se/RasterSymbolizer.cpp
  
  \brief The RasterSymbolizer describes how to render raster/matrix-coverage data (e.g., satellite photos, DEMs).
*/

// TerraLib
#include "../fe/PropertyName.h"
#include "ChannelSelection.h"
#include "ColorMap.h"
#include "ContrastEnhancement.h"
#include "ImageOutline.h"
#include "ParameterValue.h"
#include "RasterSymbolizer.h"
#include "ShadedRelief.h"

const std::string te::se::RasterSymbolizer::sm_type("RasterSymbolizer");

te::se::RasterSymbolizer::RasterSymbolizer()
  : te::se::Symbolizer(),
    m_geometry(0),
    m_opacity(0),
    m_gain(0),
    m_offset(0),
    m_channelSelection(0),
    m_overlapBehavior(NO_BEHAVIOR),
    m_colorMap(0),
    m_contrastEnhancement(0),
    m_shadedRelief(0),
    m_imageOutline(0)
{
}

te::se::RasterSymbolizer::RasterSymbolizer(const te::se::RasterSymbolizer& rhs)
  : te::se::Symbolizer(rhs),
    m_geometry(0),
    m_opacity(0),
    m_gain(0),
    m_offset(0),
    m_channelSelection(0),
    m_overlapBehavior(rhs.m_overlapBehavior),
    m_colorMap(0),
    m_contrastEnhancement(0),
    m_shadedRelief(0),
    m_imageOutline(0)
{
  if(rhs.m_geometry)
    m_geometry = new te::fe::PropertyName(rhs.m_geometry->getName());

  if(rhs.m_opacity)
    m_opacity = rhs.m_opacity->clone();

  if(rhs.m_gain)
    m_gain = rhs.m_gain->clone();

  if(rhs.m_offset)
    m_offset = rhs.m_offset->clone();

  if(rhs.m_channelSelection)
    m_channelSelection = rhs.m_channelSelection->clone();

  if(rhs.m_colorMap)
    m_colorMap = rhs.m_colorMap->clone();

  if(rhs.m_contrastEnhancement)
    m_contrastEnhancement = rhs.m_contrastEnhancement->clone();

  if(rhs.m_shadedRelief)
    m_shadedRelief = rhs.m_shadedRelief->clone();

  if(rhs.m_imageOutline)
    m_imageOutline = rhs.m_imageOutline->clone();
}

te::se::RasterSymbolizer::~RasterSymbolizer()
{
  delete m_geometry;
  delete m_opacity;
  delete m_gain;
  delete m_offset;
  delete m_channelSelection;
  delete m_colorMap;
  delete m_contrastEnhancement;
  delete m_shadedRelief;
  delete m_imageOutline;
}

void te::se::RasterSymbolizer::setGeometry(te::fe::PropertyName* g)
{
  delete m_geometry;
  m_geometry = g;
}

void te::se::RasterSymbolizer::setOpacity(ParameterValue* p)
{
  delete m_opacity;
  m_opacity = p;
}

void te::se::RasterSymbolizer::setGain(ParameterValue* p)
{
  delete m_gain;
  m_gain = p;
}

void te::se::RasterSymbolizer::setOffset(ParameterValue* p)
{
  delete m_offset;
  m_offset = p;
}

void te::se::RasterSymbolizer::setChannelSelection(ChannelSelection* c)
{
  delete m_channelSelection;
  m_channelSelection = c;
}

void te::se::RasterSymbolizer::setOverlapBehavior(OverlapBehavior b)
{
  m_overlapBehavior = b;
}

void te::se::RasterSymbolizer::setColorMap(ColorMap* c)
{
  delete m_colorMap;
  m_colorMap = c;
}

void te::se::RasterSymbolizer::setContrastEnhancement(ContrastEnhancement* c)
{
  delete m_contrastEnhancement;
  m_contrastEnhancement = c;
}

void te::se::RasterSymbolizer::setShadedRelief(ShadedRelief* s)
{
  delete m_shadedRelief;
  m_shadedRelief = s;
}

void te::se::RasterSymbolizer::setImageOutline(ImageOutline* i)
{
  delete m_imageOutline;
  m_imageOutline = i;
}

const std::string& te::se::RasterSymbolizer::getType() const
{
  return sm_type;
}

te::se::Symbolizer* te::se::RasterSymbolizer::clone() const
{
  return new RasterSymbolizer(*this);
}
