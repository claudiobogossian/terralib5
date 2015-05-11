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
  \file terralib/se/Mark.cpp
  
  \brief A Mark specifies a geometric shape and applies coloring to it.
*/

// TerraLib
#include "../xlink/SimpleLink.h"
#include "Fill.h"
#include "InlineContent.h"
#include "Mark.h"
#include "Stroke.h"

te::se::Mark::Mark()
  : m_wellKnownName(0),
    m_onlineResource(0),
    m_inlineContent(0),
    m_format(0),
    m_markIndex(-1),
    m_fill(0),
    m_stroke(0)
{
}

te::se::Mark::~Mark()
{
  delete m_wellKnownName;
  delete m_onlineResource;
  delete m_inlineContent;
  delete m_format;
  delete m_fill;
  delete m_stroke;
}

void te::se::Mark::setWellKnownName(std::string* name)
{
  delete m_wellKnownName;
  m_wellKnownName = name;
}

const std::string* te::se::Mark::getWellKnownName() const
{
  return m_wellKnownName;
}

void te::se::Mark::setOnlineResource(te::xl::SimpleLink* link)
{
  delete m_onlineResource;
  m_onlineResource = link;
}

const te::xl::SimpleLink* te::se::Mark::getOnlineResource() const
{
  return m_onlineResource;
}

void te::se::Mark::setInlineContent(InlineContent* iContent)
{
  delete m_inlineContent;
  m_inlineContent = iContent;
}

te::se::InlineContent* te::se::Mark::getInlineContent() const
{
  return m_inlineContent;
}

void te::se::Mark::setFormat(std::string* f)
{
  delete m_format;
  m_format = f;
}

const std::string* te::se::Mark::getFormat() const
{
  return m_format;
}

void te::se::Mark::setMarkIndex(int i)
{
  m_markIndex = i;
}

int te::se::Mark::getMarkIndex() const
{
  return m_markIndex;
}

void te::se::Mark::setFill(Fill* f)
{
  delete m_fill;
  m_fill = f;
}

const te::se::Fill* te::se::Mark::getFill() const
{
  return m_fill;
}

void te::se::Mark::setStroke(Stroke* s)
{
  delete m_stroke;
  m_stroke = s;
}

const te::se::Stroke* te::se::Mark::getStroke() const
{
  return m_stroke;
}

te::se::Mark* te::se::Mark::clone() const
{
  Mark* mark = new Mark;

  mark->setMarkIndex(m_markIndex);

  if(m_wellKnownName)
    mark->setWellKnownName(new std::string(*m_wellKnownName));
  
  if(m_stroke)
    mark->setStroke(m_stroke->clone());

  if(m_fill)
    mark->setFill(m_fill->clone());

  if(m_onlineResource)
    mark->setOnlineResource(new te::xl::SimpleLink(*m_onlineResource));

  if(m_inlineContent)
    mark->setInlineContent(m_inlineContent->clone());

  if(m_format)
    mark->setFormat(new std::string(*m_format));

  return mark;
}
