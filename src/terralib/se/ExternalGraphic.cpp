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
  \file terralib/se/ExternalGraphic.cpp
  
  \brief The ExternalGraphic allows a reference to be made to an external graphic file with a Web URL or to in-line content.
*/

// TerraLib
#include "../common/STLUtils.h"
#include "../xlink/SimpleLink.h"
#include "ExternalGraphic.h"
#include "InlineContent.h"
#include "Recode.h"

te::se::ExternalGraphic::ExternalGraphic()
  : m_onlineResource(0),
    m_inlineContent(0)
{
}

te::se::ExternalGraphic::~ExternalGraphic()
{
  delete m_onlineResource;
  delete m_inlineContent;

  te::common::FreeContents(m_colorReplacements);
}

void te::se::ExternalGraphic::setOnlineResource(te::xl::SimpleLink* onlineResource)
{
  delete m_onlineResource;
  m_onlineResource = onlineResource;
}

const te::xl::SimpleLink* te::se::ExternalGraphic::getOnlineResource() const
{
  return m_onlineResource;
}

void te::se::ExternalGraphic::setInlineContent(InlineContent* inlineContent)
{
  delete m_inlineContent;
  m_inlineContent = inlineContent;
}

const te::se::InlineContent* te::se::ExternalGraphic::getInlineContent() const
{
  return m_inlineContent;
}

void te::se::ExternalGraphic::setFormat(const std::string& f)
{
  m_format = f;
}

const std::string& te::se::ExternalGraphic::getFormat() const
{
  return m_format;
}

void te::se::ExternalGraphic::add(Recode* r)
{
  m_colorReplacements.push_back(r);
}

te::se::ExternalGraphic* te::se::ExternalGraphic::clone() const
{
  ExternalGraphic* externalGraphic = new ExternalGraphic;

  externalGraphic->setFormat(m_format);

  if(m_onlineResource)
    externalGraphic->setOnlineResource(new te::xl::SimpleLink(*m_onlineResource));

  if(m_inlineContent)
    externalGraphic->setInlineContent(m_inlineContent->clone());

  // TODO: std::vector<Recode*>

  return externalGraphic;
}
