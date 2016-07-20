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
  \file Text.cpp

  \brief An Text consists of a Envelope and a Text Element array.
 */

// TerraLib
#include "Text.h"
#include "Element.h"
#include "../common/STLUtils.h"

te::at::Text::Text()
{
}

te::at::Text::Text(const Text& rhs)
{
  m_envelope = rhs.m_envelope;
  m_elements = rhs.m_elements;
}

te::at::Text& te::at::Text::operator=(const Text& rhs)
{
  m_envelope = rhs.m_envelope;
  m_elements = rhs.m_elements;
  return *this;
}

te::at::Text::~Text()
{
  te::common::FreeContents(m_elements);
  m_elements.clear();
}

te::at::Text* te::at::Text::clone()
{
  Text* t = new Text;
  t->setEnvelope(m_envelope);
  t->setElements(m_elements);
  return t;
}

void te::at::Text::setEnvelope(const te::gm::Envelope& envelope)
{
  m_envelope = envelope;
}

const te::gm::Envelope& te::at::Text::getEnvelope()
{
  return m_envelope;
}

void te::at::Text::setElements(const std::vector<Element*> elements)
{
  te::common::FreeContents(m_elements);
  m_elements.clear();

  m_elements = elements;
}

void te::at::Text::add(Element* element)
{
  m_elements.push_back(element);
}

const std::vector<te::at::Element*>& te::at::Text::getElements() const
{
  return m_elements;
}