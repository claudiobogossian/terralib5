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
  \file terralib/dataaccess/query/Function.cpp

  \brief A class that models a Function expression.
*/

// TerraLib
#include "../../common/STLUtils.h"
#include "Function.h"

// STL
#include <cassert>

te::da::Function::Function(const Function& rhs)
  : m_name(rhs.m_name)
{
  te::common::Clone(rhs.m_args, m_args);
}

te::da::Function::~Function()
{
  te::common::FreeContents(m_args);
}

te::da::Function& te::da::Function::operator=(const Function& rhs)
{
  if(this != &rhs)
  {
    m_name = rhs.m_name;
    te::common::FreeContents(m_args);
    m_args.clear();
    te::common::Clone(rhs.m_args, m_args);
  }

  return *this;
}

te::da::Expression* te::da::Function::clone() const
{
  return new Function(*this);
}

std::size_t te::da::Function::getNumArgs() const
{
  return m_args.size();
}

te::da::Expression* te::da::Function::getArg(size_t i) const
{
  assert(i < m_args.size());
  return m_args[i];
}

te::da::Expression* te::da::Function::operator[](size_t i) const
{
  assert(i < m_args.size());
  return m_args[i];
}

void te::da::Function::add(Expression* arg)
{
  m_args.push_back(arg);
}

