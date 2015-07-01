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
  \file terralib/se/Stroke.cpp
  
  \brief A Stroke specifies the appearance of a linear geometry.
*/

// TerraLib
#include "../common/STLUtils.h"
#include "Graphic.h"
#include "GraphicStroke.h"
#include "Stroke.h"
#include "SvgParameter.h"

// STL
#include <cassert>

const std::string te::se::Stroke::sm_stroke     = "stroke";
const std::string te::se::Stroke::sm_opacity    = "stroke-opacity";
const std::string te::se::Stroke::sm_width      = "stroke-width";
const std::string te::se::Stroke::sm_linejoin   = "stroke-linejoin";
const std::string te::se::Stroke::sm_linecap    = "stroke-linecap";
const std::string te::se::Stroke::sm_dasharray  = "stroke-dasharray";
const std::string te::se::Stroke::sm_dashoffset = "stroke-dashoffset";

te::se::Stroke::Stroke()
  : m_fill(0),
    m_stroke(0)
{
}

te::se::Stroke::~Stroke()
{
  delete m_fill;
  delete m_stroke;
  te::common::FreeContents(m_svgParams);
}

void te::se::Stroke::setGraphicFill(Graphic* fill)
{
  delete m_fill;
  m_fill = fill;
}

const te::se::Graphic* te::se::Stroke::getGraphicFill() const
{
  return m_fill;
}

void te::se::Stroke::setGraphicStroke(GraphicStroke* stroke)
{
  delete m_stroke;
  m_stroke = stroke;
}

const te::se::GraphicStroke* te::se::Stroke::getGraphicStroke() const
{
  return m_stroke;
}

void te::se::Stroke::add(te::se::SvgParameter* p)
{
  std::string name = p->getName();
  std::map<std::string, te::se::SvgParameter*>::iterator it = m_svgParams.find(name);
  if(it != m_svgParams.end())
    delete it->second;
  m_svgParams[name] = p;
}

void te::se::Stroke::setColor(const std::string& color)
{
  setParameter(sm_stroke, color);
}

void te::se::Stroke::setOpacity(const std::string& opacity)
{
  setParameter(sm_opacity, opacity);
}

void te::se::Stroke::setWidth(const std::string& width)
{
  setParameter(sm_width, width);
}

void te::se::Stroke::setLineJoin(const std::string& join)
{
  setParameter(sm_linejoin, join);
}

void te::se::Stroke::setLineCap(const std::string& cap)
{
  setParameter(sm_linecap, cap);
}

void te::se::Stroke::setDashArray(const std::string& dasharray)
{
  setParameter(sm_dasharray, dasharray);
}

void te::se::Stroke::setDashOffset(const std::string& offset)
{
  setParameter(sm_dashoffset, offset);
}

const te::se::SvgParameter* te::se::Stroke::getColor() const
{
  return getParameter(sm_stroke);
}

const te::se::SvgParameter* te::se::Stroke::getOpacity() const
{
  return getParameter(sm_opacity);
}

const te::se::SvgParameter* te::se::Stroke::getWidth() const
{
  return getParameter(sm_width);
}

const te::se::SvgParameter* te::se::Stroke::getLineJoin() const
{
  return getParameter(sm_linejoin);
}

const te::se::SvgParameter* te::se::Stroke::getLineCap() const
{
  return getParameter(sm_linecap);
}

const te::se::SvgParameter* te::se::Stroke::getDashArray() const
{
  return getParameter(sm_dasharray);
}

const te::se::SvgParameter* te::se::Stroke::setDashOffset() const
{
  return getParameter(sm_dashoffset);
}

te::se::Stroke* te::se::Stroke::clone() const
{
  Stroke* stroke = new Stroke;

  std::map<std::string, SvgParameter*>::const_iterator it;
  for(it = m_svgParams.begin(); it != m_svgParams.end(); ++it)
    stroke->add(it->second->clone());

  const Graphic* graphicFill = getGraphicFill();
  if(graphicFill)
    stroke->setGraphicFill(graphicFill->clone());

  const GraphicStroke* graphicStroke = getGraphicStroke();
  if(graphicStroke)
    stroke->setGraphicStroke(graphicStroke->clone());

  return stroke;
}

void te::se::Stroke::setParameter(const std::string& name, const std::string& value)
{
  std::map<std::string, te::se::SvgParameter*>::iterator it = m_svgParams.find(name);
  if(it != m_svgParams.end())
    delete it->second;
  m_svgParams[name] = new te::se::SvgParameter(name, value);
}

const te::se::SvgParameter* te::se::Stroke::getParameter(const std::string& name) const
{
  std::map<std::string, te::se::SvgParameter*>::const_iterator it = m_svgParams.find(name);
  if(it != m_svgParams.end())
    return it->second;

  return 0; // Not found
}
