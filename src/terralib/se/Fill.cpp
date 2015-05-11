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
  \file terralib/se/Fill.cpp
  
  \brief A Fill specifies the pattern for filling an area geometry.
*/

// TerraLib
#include "../common/STLUtils.h"
#include "Fill.h"
#include "Graphic.h"
#include "SvgParameter.h"

const std::string te::se::Fill::sm_fill    = "fill";
const std::string te::se::Fill::sm_opacity = "fill-opacity";

te::se::Fill::Fill()
  : m_graphicFill(0)
{
}

te::se::Fill::~Fill()
{
  delete m_graphicFill;
  te::common::FreeContents(m_svgParams);
}

void te::se::Fill::setGraphicFill(Graphic* g)
{
  delete m_graphicFill;
  m_graphicFill = g;
}

const te::se::Graphic* te::se::Fill::getGraphicFill() const
{
  return m_graphicFill;
}

void te::se::Fill::add(SvgParameter* p)
{
  std::string name = p->getName();
  std::map<std::string, te::se::SvgParameter*>::iterator it = m_svgParams.find(name);
  if(it != m_svgParams.end())
    delete it->second;
  m_svgParams[name] = p;
}

void te::se::Fill::setColor(const std::string& color)
{
  setParameter(sm_fill, color);
}

void te::se::Fill::setOpacity(const std::string& opacity)
{
  setParameter(sm_opacity, opacity);
}

const te::se::SvgParameter* te::se::Fill::getColor() const
{
  return getParameter(sm_fill);
}

const te::se::SvgParameter* te::se::Fill::getOpacity() const
{
  return getParameter(sm_opacity);
}

te::se::Fill* te::se::Fill::clone() const
{
  Fill* fill = new Fill;

  std::map<std::string, SvgParameter*>::const_iterator it;
  for(it = m_svgParams.begin(); it != m_svgParams.end(); ++it)
    fill->add(it->second->clone());

  const Graphic* graphicFill = getGraphicFill();
  if(graphicFill)
    fill->setGraphicFill(graphicFill->clone());

  return fill;
}

void te::se::Fill::setParameter(const std::string& name, const std::string& value)
{
  std::map<std::string, te::se::SvgParameter*>::iterator it = m_svgParams.find(name);
  if(it != m_svgParams.end())
    delete it->second;
  m_svgParams[name] = new te::se::SvgParameter(name, value);
}

const te::se::SvgParameter* te::se::Fill::getParameter(const std::string& name) const
{
  std::map<std::string, te::se::SvgParameter*>::const_iterator it = m_svgParams.find(name);
  if(it != m_svgParams.end())
    return it->second;

  return 0; // Not found
}
