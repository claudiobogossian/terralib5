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
  \file terralib/dataaccess/query/FromFunctionCall.cpp

  \brief A function can be used as a source of information in another query.
*/

// TerraLib
#include "FromFunctionCall.h"
#include "Function.h"

te::da::FromFunctionCall::FromFunctionCall(Function* f, const std::string& alias)
  : FromItem(alias),
    m_f(f)
{
}

te::da::FromFunctionCall::FromFunctionCall(const Function& f, const std::string& alias)
  : FromItem(alias),
    m_f(0)
{
  m_f.reset(static_cast<Function*>(f.clone()));
}

te::da::FromFunctionCall::FromFunctionCall(const FromFunctionCall& rhs)
  : FromItem(rhs),
    m_f(0)
{
  m_f.reset(rhs.m_f.get() ? static_cast<Function*>(rhs.m_f->clone()) : 0);
}

te::da::FromFunctionCall::~FromFunctionCall()
{
}

te::da::FromFunctionCall& te::da::FromFunctionCall::operator=(const FromFunctionCall& rhs)
{
  if(this != &rhs)
  {    
    FromItem::operator=(rhs);

    m_f.reset(rhs.m_f.get() ? static_cast<Function*>(rhs.m_f->clone()) : 0);
  }

  return *this;
}

te::da::FromItem* te::da::FromFunctionCall::clone() const
{
  return new FromFunctionCall(*this);
}

te::da::Function* te::da::FromFunctionCall::getFunction() const
{
  return m_f.get();
}

void te::da::FromFunctionCall::setFunction(Function* f)
{
  m_f.reset(f);
}

