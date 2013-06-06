/*  Copyright (C) 2008-2013 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/dataaccess/core/query/Like.cpp

  \brief It is intended to encode a character string comparison operator with pattern matching.
*/

// TerraLib
#include "Expression.h"
#include "FunctionNames.h"
#include "Like.h"
#include "LiteralString.h"

// STL
#include <cassert>

te::da::core::Like::Like(Expression* str,
                   const std::string& pattern,
                   const std::string& wildCard,
                   const std::string& singleChar,
                   const std::string& escapeChar)
  : Function(FunctionNames::sm_Like),
    m_pattern(pattern),
    m_wildCard(wildCard),
    m_singleChar(singleChar),
    m_escapeChar(escapeChar)
{
  m_args.push_back(str);
}

te::da::core::Like::Like(const Expression& str,
                   const std::string& pattern,
                   const std::string& wildCard,
                   const std::string& singleChar,
                   const std::string& escapeChar)
  : Function(FunctionNames::sm_Like),
    m_pattern(pattern),
    m_wildCard(wildCard),
    m_singleChar(singleChar),
    m_escapeChar(escapeChar)
{
  m_args.push_back(str.clone());
}

te::da::core::Like::~Like()
{
}

te::da::core::Like::Like(const Like& rhs)
  : Function(rhs),
    m_pattern(rhs.m_pattern),
    m_wildCard(rhs.m_wildCard),
    m_singleChar(rhs.m_singleChar),
    m_escapeChar(rhs.m_escapeChar)
{
}

te::da::core::Like& te::da::core::Like::operator=(const Like& rhs)
{
  if(this != &rhs)
  {
    Function::operator=(rhs);
    m_pattern = rhs.m_pattern;
    m_wildCard = rhs.m_wildCard;
    m_singleChar = rhs.m_singleChar;
    m_escapeChar = rhs.m_escapeChar;
  }

  return *this;
}

te::da::core::Expression* te::da::core::Like::clone() const
{
  return new Like(*this);
}

te::da::core::Expression* te::da::core::Like::getString() const
{
  return m_args[0];
}

void te::da::core::Like::setString(Expression* str)
{
  delete m_args[0];
  m_args[0] = str;
}

const std::string& te::da::core::Like::getPattern()
{
  return m_pattern;
}

void te::da::core::Like::setPattern(const std::string& p)
{
  m_pattern = p;
}

void te::da::core::Like::setWildCard(const std::string& w)
{
  m_wildCard = w;
}

const std::string& te::da::core::Like::getWildCard() const
{
  return m_wildCard;
}

void te::da::core::Like::setSingleChar(const std::string& s)
{
  m_singleChar = s;
}

const std::string& te::da::core::Like::getSingleChar() const
{
  return m_singleChar;
}

void te::da::core::Like::setEscapeChar(const std::string& e)
{
  m_escapeChar = e;
}

const std::string& te::da::core::Like::getEscapeChar() const
{
  return m_escapeChar;
}
