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
  \file terralib/dataaccess/query/Substring.cpp

  \brief Allows the extraction of substrings.
*/

// TerraLib
#include "FunctionNames.h"
#include "Substring.h"

// STL
#include <cassert>

te::da::Substring::Substring(Expression* str, Expression* start, Expression* len)
  : Function(FunctionNames::sm_Substring)
{
  m_args.push_back(str);
  m_args.push_back(start);
  m_args.push_back(len);
}

te::da::Substring::Substring(const Substring& rhs)
  : Function(rhs)
{
}

te::da::Substring& te::da::Substring::operator=(const Substring& rhs)
{
  Function::operator=(rhs);
  return *this;
}

te::da::Expression* te::da::Substring::clone() const
{
  return new Substring(*this);
}

te::da::Expression* te::da::Substring::getString() const
{
  assert(m_args.size() == 3);
  return m_args[0];
}

void te::da::Substring::setString(Expression* str)
{
  assert(m_args.size() == 3);
  delete m_args[0];
  m_args[0] = str;
}

te::da::Expression* te::da::Substring::getStartPosition()
{
  assert(m_args.size() == 3);
  return m_args[1];
}

void te::da::Substring::setStartPosition(Expression* start)
{
  assert(m_args.size() == 3);
  delete m_args[1];
  m_args[1] = start;
}

te::da::Expression* te::da::Substring::getLength()
{
  assert(m_args.size() == 3);
  return m_args[2];
}

void te::da::Substring::setLength(Expression* len)
{
  assert(m_args.size() == 3);
  delete m_args[2];
  m_args[2] = len;
}

