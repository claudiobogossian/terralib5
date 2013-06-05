/*  Copyright (C) 2008-2011 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/dataaccess/query/BinaryFunction.cpp

  \brief A base class for binary functions.
*/

// TerraLib
#include "BinaryFunction.h"

// STL
#include <cassert>

te::da::core::BinaryFunction::BinaryFunction(const std::string& name, Expression* arg1, Expression* arg2)
  : Function(name)
{
  assert(arg1 && arg2);
  m_args.push_back(arg1);
  m_args.push_back(arg2);
}

te::da::core::BinaryFunction::BinaryFunction(const std::string& name, const Expression& arg1, const Expression& arg2)
  : Function(name)
{
  m_args.push_back(arg1.clone());
  m_args.push_back(arg2.clone());
}

te::da::core::BinaryFunction::BinaryFunction(const BinaryFunction& rhs)
  : Function(rhs)
{
}

te::da::core::BinaryFunction::~BinaryFunction()
{
}

te::da::core::BinaryFunction& te::da::core::BinaryFunction::operator=(const BinaryFunction& rhs)
{
  Function::operator=(rhs);
  return *this;
}

te::da::core::Expression* te::da::core::BinaryFunction::clone() const
{
  return new BinaryFunction(*this);
}

te::da::core::Expression* te::da::core::BinaryFunction::getFirst() const
{
  assert(m_args.size() == 2);
  return m_args[0];
}

void te::da::core::BinaryFunction::setFirst(Expression* arg)
{
  assert(m_args.size() == 2);
  delete m_args[0];
  m_args[0] = arg;
}

te::da::core::Expression* te::da::core::BinaryFunction::getSecond() const
{
  assert(m_args.size() == 2);
  return m_args[1];
}

void te::da::core::BinaryFunction::setSecond(Expression* arg)
{
  assert(m_args.size() == 2);
  delete m_args[1];
  m_args[1] = arg;
}
