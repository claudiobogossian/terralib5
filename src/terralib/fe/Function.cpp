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
  \file Function.cpp
  
  \brief A function is a named procedure that performs a distinct computation.  
 */

// TerraLib
#include "../common/STLUtils.h"
#include "Function.h"

// STL
#include <cassert>

te::fe::Function::Function(const std::string& name)
  : m_name(name)
{
}

te::fe::Function::~Function()
{
  te::common::FreeContents(m_argumentList);
}

const std::string& te::fe::Function::getName() const
{
  return m_name;
}

void te::fe::Function::setName(const std::string& n)
{
  m_name = n;
}

te::fe::Expression* te::fe::Function::getArgument(size_t i) const
{
  assert(i < m_argumentList.size());
  return m_argumentList[i];
}

void te::fe::Function::add(Expression* argument)
{
  assert(argument);
  m_argumentList.push_back(argument);
}

void te::fe::Function::setArgument(size_t i, Expression* argument)
{
  assert(i < m_argumentList.size());
  delete m_argumentList[i];
  m_argumentList[i] = argument;
}

void te::fe::Function::removeArgument(size_t i)
{
  assert(i < m_argumentList.size());
  delete m_argumentList[i];
  m_argumentList.erase(m_argumentList.begin() + i);
}

te::fe::Expression* te::fe::Function::clone() const
{
  te::fe::Function* f = new te::fe::Function(m_name);
  for(std::size_t i = 0; i < m_argumentList.size(); ++i)
  {
    te::fe::Expression* e = m_argumentList[i];
    if(e)
      f->add(e->clone());
  }

  return f;
}
