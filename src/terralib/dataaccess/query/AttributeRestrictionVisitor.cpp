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
  \file terralib/dataaccess/query/AttributeRestrictionVisitor.cpp

  \brief A visitor that retrieves attribute restrictions from a Query hierarchy.
*/

// TerraLib
#include "../../common/STLUtils.h"
#include "../../common/Translator.h"
#include "../Exception.h"
#include "AttributeRestrictionVisitor.h"
#include "Expression.h"
#include "Function.h"
#include "FunctionNames.h"
#include "PropertyName.h"
#include "Select.h"
#include "Where.h"

// STL
#include <cassert>

te::da::AttributeRestriction::AttributeRestriction()
  : m_index(0),
    m_op(""),
    m_function(0)
{
}

te::da::AttributeRestriction::~AttributeRestriction()
{
}

te::da::AttributeRestrictionVisitor::AttributeRestrictionVisitor()
  : m_index(0)
{
  initialize();
}

te::da::AttributeRestrictionVisitor::~AttributeRestrictionVisitor()
{
  te::common::FreeContents(m_attrRestrictions);
}

void te::da::AttributeRestrictionVisitor::visit(const Function& visited)
{
  if(isAttributeRestrictionFunction(visited))
  {
    addAttributeRestriction(visited);
    return;
  }

  std::size_t size = visited.getNumArgs();
  for(std::size_t i = 0; i < size; ++i)
  {
    assert(visited[i]);
    visited[i]->accept(*this);
  }
}

void te::da::AttributeRestrictionVisitor::visit(const Select& visited)
{
  Where* restriction = visited.getWhere();

  if(!restriction)
    return;

  Expression* e = restriction->getExp();
  if(e)
    e->accept(*this);
}

bool te::da::AttributeRestrictionVisitor::hasAttributeRestrictions() const
{
  return !m_attrRestrictions.empty();
}

const std::vector<te::da::AttributeRestriction*>& te::da::AttributeRestrictionVisitor::getAttributeRestrictions() const
{
  return m_attrRestrictions;
}

void te::da::AttributeRestrictionVisitor::initialize()
{
  // Initializes the map of attribute restriction functions
  m_attrFunctions[FunctionNames::sm_EqualTo             ] = "=";
  m_attrFunctions[FunctionNames::sm_NotEqualTo          ] = "<>";
  m_attrFunctions[FunctionNames::sm_GreaterThan         ] = ">";
  m_attrFunctions[FunctionNames::sm_GreaterThanOrEqualTo] = ">=";
  m_attrFunctions[FunctionNames::sm_LessThan            ] = "<";
  m_attrFunctions[FunctionNames::sm_LessThanOrEqualTo   ] = "<=";
  // ... more?!
}

bool te::da::AttributeRestrictionVisitor::isAttributeRestrictionFunction(const Function& f) const
{
  return m_attrFunctions.find(f.getName()) != m_attrFunctions.end();
}

std::string te::da::AttributeRestrictionVisitor::getOperator(const Function& f) const
{
  assert(isAttributeRestrictionFunction(f));

  return m_attrFunctions.find(f.getName())->second;
}

void te::da::AttributeRestrictionVisitor::addAttributeRestriction(const Function& f)
{
  // Creates the attribute restriction
  AttributeRestriction* restriction = new AttributeRestriction;
  restriction->m_index = m_index++;
  restriction->m_op = getOperator(f);
  restriction->m_function = dynamic_cast<const Function*>(&f);

  // Adds the attribute restriction
  m_attrRestrictions.push_back(restriction);
}
