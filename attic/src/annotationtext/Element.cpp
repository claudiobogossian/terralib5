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
  \file terralib/annotationtext/Element.cpp

  \brief An Text Element consists of a string, location, leader line and may have attribute.
 */

// TerraLib
#include "Element.h"
#include "Attributes.h"
#include "../geometry/Geometry.h"

te::at::Element::Element()
{
  m_value.clear();
  m_location = 0;
  m_leaderLine = 0;
  m_attribute = 0;
}

te::at::Element::Element(const std::string& value, te::gm::Geometry* location, te::gm::Geometry* leaderLine)
{
  m_value = value;
  m_location = location;
  m_leaderLine = leaderLine;
  m_attribute = 0;
}

te::at::Element::~Element()
{
  delete m_location;
  delete m_leaderLine;
  delete m_attribute;
}

te::at::Element* te::at::Element::clone() const
{
  Element* e = new Element;
  e->setValue(m_value);
  e->setLocation(m_location);
  e->setLeaderLine(m_leaderLine);
  e->setAttributes(m_attribute);
  return e;
}

void te::at::Element::setValue(const std::string& value)
{
  m_value = value;
}

const std::string& te::at::Element::getValue() const
{
  return m_value;
}

void te::at::Element::setLocation(te::gm::Geometry* location)
{
  delete m_location;
  m_location = location;
}

const te::gm::Geometry* te::at::Element::getLocation() const
{
  return m_location;
}

void te::at::Element::setLeaderLine(te::gm::Geometry* leaderLine)
{
  delete m_leaderLine;
  m_leaderLine = leaderLine;
}

const te::gm::Geometry* te::at::Element::getLeaderLine() const
{
  return m_leaderLine;
}

void te::at::Element::setAttributes(Attributes* attribute)
{
  delete m_attribute;
  m_attribute = attribute;
}

const te::at::Attributes* te::at::Element::getAttributes() const
{
  return m_attribute;
}

