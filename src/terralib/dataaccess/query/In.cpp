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
  \file terralib/dataaccess/query/In.cpp

  \brief A class that represents the IN operator.
*/

// TerraLib
#include "../../common/STLUtils.h"
#include "In.h"
#include "FunctionNames.h"
#include "PropertyName.h"

// STL
#include <cassert>

te::da::In::In(te::da::PropertyName* p)
  : Function(FunctionNames::sm_In),
    m_name(p)
{
}

te::da::In::In(const te::da::PropertyName& p)
  : Function(FunctionNames::sm_In),
    m_name(0)
{
  m_name.reset(static_cast<PropertyName*>(p.clone()));
}

te::da::In::In(const std::string& name)
  : Function(FunctionNames::sm_In),
    m_name(0)
{
  m_name.reset(new te::da::PropertyName(name));
}

te::da::In::In(const In& rhs)
  : Function(rhs),
    m_name(0)
{
  m_name.reset(rhs.m_name.get() ? static_cast<PropertyName*>(rhs.m_name->clone()) : 0);
}

te::da::In::~In() 
{
}

te::da::In& te::da::In::operator=(const In& rhs)
{
  if(this != &rhs)
  {
    Function::operator=(rhs);
    m_name.reset(rhs.m_name.get() ? static_cast<PropertyName*>(rhs.m_name->clone()) : 0);
  }

  return *this;
}

te::da::Expression* te::da::In::clone() const
{
  return new In(*this);
}

te::da::PropertyName* te::da::In::getPropertyName() const
{ 
  return m_name.get(); 
}

void te::da::In::setPropertyName(te::da::PropertyName* p)
{ 
  m_name.reset(p); 
}
