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
  \file terralib/dataaccess/query/FromItem.cpp

  \brief An abstract class that models a source of data in a query.
*/

// TerraLib
#include "FromItem.h"

te::da::FromItem::FromItem(const FromItem& rhs)
  : m_alias(rhs.m_alias)
{
}

te::da::FromItem& te::da::FromItem::operator=(const FromItem& rhs)
{
  if(this != &rhs)
    m_alias = rhs.m_alias;

  return *this;
}

void te::da::FromItem::setAlias(const std::string& alias)
{
  m_alias = alias;
}

const std::string& te::da::FromItem::getAlias() const
{
  return m_alias;
}

