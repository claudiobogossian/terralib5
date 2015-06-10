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
  \file terralib/dataaccess/query/UnaryFunction.cpp

  \brief A base class for unary functions.
*/

// TerraLib
#include "UnaryFunction.h"

// STL
#include <cassert>

te::da::UnaryFunction::UnaryFunction(const std::string& name, Expression* arg)
  : Function(name)
{
  assert(arg);
  m_args.push_back(arg);
}

te::da::UnaryFunction::UnaryFunction(const std::string& name, const Expression& arg)
  : Function(name)
{
  m_args.push_back(arg.clone());
}

te::da::UnaryFunction::UnaryFunction(const UnaryFunction& rhs)
  : Function(rhs)
{
}

te::da::UnaryFunction::~UnaryFunction()
{
}

te::da::UnaryFunction& te::da::UnaryFunction::operator=(const UnaryFunction& rhs)
{
  Function::operator=(rhs);
  return *this;
}

te::da::Expression* te::da::UnaryFunction::clone() const
{
  return new UnaryFunction(*this);
}

te::da::Expression* te::da::UnaryFunction::getArgument() const
{
  assert(m_args.size() == 1);
  return m_args[0];
}

void te::da::UnaryFunction::setArgument(Expression* arg)
{
  assert(m_args.size() == 1);
  delete m_args[0];
  m_args[0] = arg;
}

