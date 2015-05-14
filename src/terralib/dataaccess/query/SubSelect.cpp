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
  \file terralib/dataaccess/query/SubSelect.cpp

  \brief A Select can be used as a source of information in another query.
*/

// TerraLib
#include "Select.h"
#include "SubSelect.h"

te::da::SubSelect::SubSelect(Select* s, const std::string& alias)
  : FromItem(alias),
    m_select(s)
{
}

te::da::SubSelect::SubSelect(const Select& s, const std::string& alias)
  : FromItem(alias),
    m_select(static_cast<Select*>(s.clone()))
{
}

te::da::SubSelect::SubSelect(const SubSelect& rhs)
  : FromItem(rhs),
    m_select(0)
{
  m_select.reset(rhs.m_select.get() ? static_cast<Select*>(rhs.m_select->clone()) : 0);
}

te::da::SubSelect::~SubSelect()
{
}

te::da::SubSelect& te::da::SubSelect::operator=(const SubSelect& rhs)
{
  if(this != &rhs)
  {
    FromItem::operator=(rhs);
    m_select.reset(rhs.m_select.get() ? static_cast<Select*>(rhs.m_select->clone()) : 0);
  }

  return *this;
}

te::da::FromItem* te::da::SubSelect::clone() const
{
  return new SubSelect(*this);
}

te::da::Select* te::da::SubSelect::getSelect() const
{
  return m_select.get();
}

void te::da::SubSelect::setSelect(Select* s)
{
  m_select.reset(s);
}

