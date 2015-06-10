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
 \file CI_Citation.cpp
*/

// TerraLib
#include "CI_Citation.h"

// STL
#include <cassert>

te::md::CI_Citation::CI_Citation(const std::string& title, boost::gregorian::date date, CI_DateTypeCode dType):
  m_title(title),
  m_date(date), 
  m_dateType(dType)
{}

te::md::CI_Citation::~CI_Citation()
{}

const 
std::string& te::md::CI_Citation::getTitle() const
{
  return m_title;
}

const 
boost::gregorian::date& te::md::CI_Citation::getDate() const
{
  return m_date;
}

te::md::CI_DateTypeCode 
te::md::CI_Citation::getDateType() const
{
  return m_dateType;
}

te::md::CI_Citation* te::md::CI_Citation::clone() const
{
  return new te::md::CI_Citation(m_title, m_date, m_dateType);
}