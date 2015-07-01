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
  \file terralib/dataaccess/query/Join.cpp

  \brief A Join clause combines two FromItems.
*/

// TerraLib
#include "Join.h"
#include "JoinCondition.h"

te::da::Join::Join(FromItem* first, FromItem* second, JoinType t, JoinCondition* c)
  : FromItem(""),
    m_first(first),
    m_second(second),
    m_condition(c),
    m_type(t),
    m_isNatural(false)    
{
}

te::da::Join::Join(const FromItem& first, const FromItem& second, JoinType t, const JoinCondition& c)
  : FromItem(""),
    m_first(0),
    m_second(0),
    m_condition(0),
    m_type(t),
    m_isNatural(false)    
{
  m_first.reset(first.clone());
  m_second.reset(second.clone());
  m_condition.reset(c.clone());
}

te::da::Join::Join(const Join& rhs)
  : FromItem(rhs),
    m_first(0),
    m_second(0),
    m_condition(0),
    m_type(rhs.m_type),
    m_isNatural(rhs.m_isNatural)
{  
  m_first.reset(rhs.m_first.get() ? rhs.m_first->clone() : 0);
  m_second.reset(rhs.m_second.get() ? rhs.m_second->clone() : 0);
  m_condition.reset(rhs.m_condition.get() ? rhs.m_condition->clone() : 0);
}

te::da::Join::~Join()
{
}

te::da::Join& te::da::Join::operator=(const Join& rhs)
{
  if(this != &rhs)
  {
    FromItem::operator=(rhs);

    m_first.reset(rhs.m_first.get() ? rhs.m_first->clone() : 0);
    m_second.reset(rhs.m_second.get() ? rhs.m_second->clone() : 0);
    m_condition.reset(rhs.m_condition.get() ? rhs.m_condition->clone() : 0);
    m_type = rhs.m_type;
    m_isNatural = rhs.m_isNatural;
  }

  return *this;
}

te::da::FromItem* te::da::Join::clone() const
{
  return new Join(*this);
}

te::da::FromItem* te::da::Join::getFirst() const
{
  return m_first.get();
}

void te::da::Join::setFirst(FromItem* item)
{
  m_first.reset(item);
}

te::da::FromItem* te::da::Join::getSecond() const
{
  return m_second.get();
}

void te::da::Join::setSecond(FromItem* item)
{
  m_second.reset(item);
}

te::da::JoinType te::da::Join::getType() const
{
  return m_type;
}

void te::da::Join::setType(JoinType t)
{
  m_type = t;
}

te::da::JoinCondition* te::da::Join::getCondition() const
{
  return m_condition.get();
}

void te::da::Join::setCondition(JoinCondition* c)
{
  m_condition.reset(c);
}

