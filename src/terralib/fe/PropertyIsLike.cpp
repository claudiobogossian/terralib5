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
  \file PropertyIsLike.cpp
  
  \brief It is intended to encode a character string comparison operator with pattern matching.
 */

// TerraLib
#include "Globals.h"
#include "Literal.h"
#include "PropertyIsLike.h"
#include "PropertyName.h"

te::fe::PropertyIsLike::PropertyIsLike()
  : ComparisonOp(Globals::sm_propertyIsLike),
    m_property(0),
    m_literalValue(0)
{
}

te::fe::PropertyIsLike::~PropertyIsLike()
{
  delete m_property;
  delete m_literalValue;
}

void te::fe::PropertyIsLike::setPropertyName(PropertyName* p)
{
  delete m_property;
  m_property = p;
}

te::fe::PropertyName* te::fe::PropertyIsLike::getPropertyName() const
{
  return m_property;
}

void te::fe::PropertyIsLike::setLiteral(Literal* l)
{
  delete m_literalValue;
  m_literalValue = l;
}

te::fe::Literal* te::fe::PropertyIsLike::getLiteral() const
{
  return m_literalValue;
}

void te::fe::PropertyIsLike::setWildCard(const std::string& w)
{
  m_wildCard = w;
}

const std::string& te::fe::PropertyIsLike::getWildCard() const
{
  return m_wildCard;
}

void te::fe::PropertyIsLike::setSingleChar(const std::string& s)
{
  m_singleChar = s;
}

const std::string& te::fe::PropertyIsLike::getSingleChar() const
{
  return m_singleChar;
}

void te::fe::PropertyIsLike::setEscapeChar(const std::string& e)
{
  m_escapeChar = e;
}

const std::string& te::fe::PropertyIsLike::getEscapeChar() const
{
  return m_escapeChar;
}

