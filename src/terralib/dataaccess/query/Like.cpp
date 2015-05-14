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
  \file terralib/dataaccess/query/Like.cpp

  \brief It is intended to encode a character string comparison operator with pattern matching.
*/

// TerraLib
#include "Expression.h"
#include "FunctionNames.h"
#include "Like.h"
#include "LiteralString.h"

// STL
#include <cassert>

te::da::Like::Like(Expression* str,
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

te::da::Like::Like(const Expression& str,
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

te::da::Like::~Like()
{
}

te::da::Like::Like(const Like& rhs)
  : Function(rhs),
    m_pattern(rhs.m_pattern),
    m_wildCard(rhs.m_wildCard),
    m_singleChar(rhs.m_singleChar),
    m_escapeChar(rhs.m_escapeChar)
{
}

te::da::Like& te::da::Like::operator=(const Like& rhs)
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

te::da::Expression* te::da::Like::clone() const
{
  return new Like(*this);
}

te::da::Expression* te::da::Like::getString() const
{
  return m_args[0];
}

void te::da::Like::setString(Expression* str)
{
  delete m_args[0];
  m_args[0] = str;
}

const std::string& te::da::Like::getPattern()
{
  return m_pattern;
}

void te::da::Like::setPattern(const std::string& p)
{
  m_pattern = p;
}

void te::da::Like::setWildCard(const std::string& w)
{
  m_wildCard = w;
}

const std::string& te::da::Like::getWildCard() const
{
  return m_wildCard;
}

void te::da::Like::setSingleChar(const std::string& s)
{
  m_singleChar = s;
}

const std::string& te::da::Like::getSingleChar() const
{
  return m_singleChar;
}

void te::da::Like::setEscapeChar(const std::string& e)
{
  m_escapeChar = e;
}

const std::string& te::da::Like::getEscapeChar() const
{
  return m_escapeChar;
}

