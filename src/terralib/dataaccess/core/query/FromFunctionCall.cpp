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
  \file terralib/dataaccess/core/query/FromFunctionCall.cpp

  \brief A function that can be used as a source of information in another query.
*/

// TerraLib
#include "FromFunctionCall.h"
#include "Function.h"

te::da::core::FromFunctionCall::FromFunctionCall(Function* f, const std::string& alias)
  : FromItem(alias),
    m_f(f)
{
}

te::da::core::FromFunctionCall::FromFunctionCall(const Function& f, const std::string& alias)
  : FromItem(alias),
    m_f(0)
{
  m_f.reset(static_cast<Function*>(f.clone()));
}

te::da::core::FromFunctionCall::FromFunctionCall(const FromFunctionCall& rhs)
  : FromItem(rhs),
    m_f(0)
{
  m_f.reset(rhs.m_f.get() ? static_cast<Function*>(rhs.m_f->clone()) : 0);
}

te::da::core::FromFunctionCall::~FromFunctionCall()
{
}

te::da::core::FromFunctionCall& te::da::core::FromFunctionCall::operator=(const FromFunctionCall& rhs)
{
  if(this != &rhs)
  {    
    FromItem::operator=(rhs);

    m_f.reset(rhs.m_f.get() ? static_cast<Function*>(rhs.m_f->clone()) : 0);
  }

  return *this;
}

te::da::core::FromItem* te::da::core::FromFunctionCall::clone() const
{
  return new FromFunctionCall(*this);
}

te::da::core::Function* te::da::core::FromFunctionCall::getFunction() const
{
  return m_f.get();
}

void te::da::core::FromFunctionCall::setFunction(Function* f)
{
  m_f.reset(f);
}
