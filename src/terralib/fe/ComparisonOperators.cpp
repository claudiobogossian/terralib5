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
  \file ComparisonOperators.cpp
  
  \brief It is used to indicate what types of comparison operators are supported by a service.  
 */

// TerraLib
#include "ComparisonOperators.h"
#include "Globals.h"

// STL
#include <cassert>

std::set<const char*, te::common::LessCmp<const char*> >  te::fe::ComparisonOperators::sm_validComparisonOperators;

te::fe::ComparisonOperators::ComparisonOperators()
{
}

te::fe::ComparisonOperators::~ComparisonOperators()
{
// we will not clear the vector items beacuse they are just reference to an internal list of operators.
}

void te::fe::ComparisonOperators::push_back(const char* c)
{
  assert(c);
  m_comparisonOperators.push_back(c);
}

const char* te::fe::ComparisonOperators::operator[](size_t i) const
{
  assert(i < m_comparisonOperators.size());
  return m_comparisonOperators[i];
}

const char* te::fe::ComparisonOperators::findOperator(const char* o)
{
  assert(o);

  std::set<const char*, te::common::LessCmp<const char*> >::const_iterator it = sm_validComparisonOperators.find(o);

  if(it != sm_validComparisonOperators.end())
    return *it;

  return 0;
}

void te::fe::ComparisonOperators::loadValidOperatorList()
{
  sm_validComparisonOperators.insert(Globals::sm_lessThan);
  sm_validComparisonOperators.insert(Globals::sm_greaterThan);
  sm_validComparisonOperators.insert(Globals::sm_lessThanEqualTo);
  sm_validComparisonOperators.insert(Globals::sm_greaterThanEqualTo);
  sm_validComparisonOperators.insert(Globals::sm_equalTo);  
  sm_validComparisonOperators.insert(Globals::sm_notEqualTo);
  sm_validComparisonOperators.insert(Globals::sm_like);
  sm_validComparisonOperators.insert(Globals::sm_between);
  sm_validComparisonOperators.insert(Globals::sm_nullCheck);
}

void te::fe::ComparisonOperators::clearValidOperatorList()
{
  sm_validComparisonOperators.clear();
}

