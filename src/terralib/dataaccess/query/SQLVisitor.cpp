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
  \file terralib/dataaccess/query/SQLVisitor.cpp

  \brief A visitor for building an SQL statement from a given Query hierarchy.
*/

// TerraLib
#include "../../common/StringUtils.h"
#include "../../common/Translator.h"
#include "../../datatype/AbstractData.h"
#include "../Exception.h"
#include "DataSetName.h"
#include "Distinct.h"
#include "Expression.h"
#include "Field.h"
#include "Function.h"
#include "GroupBy.h"
#include "GroupByItem.h"
#include "Having.h"
#include "In.h"
#include "Insert.h"
#include "Join.h"
#include "JoinCondition.h"
#include "JoinConditionOn.h"
#include "JoinConditionUsing.h"
#include "Literal.h"
#include "LiteralBool.h"
#include "LiteralByteArray.h"
#include "LiteralDateTime.h"
#include "LiteralDouble.h"
#include "LiteralGeom.h"
#include "LiteralInt16.h"
#include "LiteralInt32.h"
#include "LiteralInt64.h"
#include "LiteralString.h"
#include "OrderByItem.h"
#include "PropertyName.h"
#include "Select.h"
#include "SQLDialect.h"
#include "SQLFunctionEncoder.h"
#include "SQLVisitor.h"
#include "ST_Dump.h"
#include "SubSelect.h"
#include "Where.h"

// STL
#include <cassert>

void te::da::SQLVisitor::visit(const Expression& /*visited*/)
{
}

void te::da::SQLVisitor::visit(const DataSetName& visited)
{
  m_sql += visited.getName();

  if(visited.getAlias().empty() == false)
  {
    m_sql += " AS ";
    m_sql += visited.getAlias();
  }
}

void te::da::SQLVisitor::visit(const FromItem& /*visited*/)
{
}

void te::da::SQLVisitor::visit(const Function& visited)
{
  const std::string& fname = visited.getName();

  const SQLFunctionEncoder* encoder = m_dialect.find(fname);

  if(encoder == 0)
    throw Exception(TE_TR("The informed function is not supported by this driver!"));

  encoder->toSQL(visited, m_sql, *this);
}

void te::da::SQLVisitor::visit(const Insert& visited)
{
  m_sql += "INSERT INTO ";

  if (visited.getDataSetName())
  {
    visit(*(visited.getDataSetName()));
    m_sql += " ";
  }

  if (visited.getFields())
  {
    m_sql += "( ";
    visit(*(visited.getFields()));
    m_sql += " ) ";
  }

  if (visited.getSelect())
  {
    visit(*(visited.getSelect()));
    m_sql += " ";
  }
}

void te::da::SQLVisitor::visit(const Join& visited)
{
  assert(visited.getFirst() && visited.getSecond());

  m_sql += "(";
  visited.getFirst()->accept(*this);

  if(visited.isNatural())
    m_sql += " NATURAL ";

  switch(visited.getType())
  {
    case JOIN :
      m_sql += " JOIN ";
    break;

    case INNER_JOIN :
      m_sql += " INNER JOIN ";
    break;

    case LEFT_JOIN :
      m_sql += " LEFT JOIN ";
    break;

    case RIGHT_JOIN :
      m_sql += " RIGHT JOIN ";
    break;

    case FULL_OUTER_JOIN :
      m_sql += " FULL OUTER JOIN ";
    break;

    case CROSS_JOIN :
      m_sql += " CROSS JOIN ";
    break;

    case NATURAL_JOIN :
      m_sql += " NATURAL JOIN ";
    break;
  }

  visited.getSecond()->accept(*this);

  if(visited.getCondition())
  {
    m_sql += " ";
    visited.getCondition()->accept(*this);
  }

  m_sql += ")";
}

void te::da::SQLVisitor::visit(const JoinCondition& /*visited*/)
{ 
}

void te::da::SQLVisitor::visit(const JoinConditionOn& visited)
{
  assert(visited.getCondition());  
  m_sql += "ON (";
  visited.getCondition()->accept(*this);
  m_sql += ")";
}

void te::da::SQLVisitor::visit(const JoinConditionUsing& visited)
{
  m_sql += "USING (";

  size_t ncols = visited.getNumFields();

  for(size_t i = 0; i < ncols; ++i)
  {
    if(i != 0)
      m_sql += ", ";

    assert(visited[i]);
    visited[i]->accept(*this);
  }

  m_sql += ")";
}

void te::da::SQLVisitor::visit(const Literal& visited)
{
  if(visited.getValue())
    m_sql += visited.getValue()->toString();
}

void te::da::SQLVisitor::visit(const LiteralBool& visited)
{
  if(visited.getValue())
    m_sql += "bool(" + visited.getValue()->toString() + ")";
}

void te::da::SQLVisitor::visit(const LiteralByteArray& visited)
{
  if(visited.getValue())
    m_sql += visited.getValue()->toString();
}

void te::da::SQLVisitor::visit(const LiteralDateTime& visited)
{
  if(visited.getValue())
    m_sql += visited.getValue()->toString();
}

void te::da::SQLVisitor::visit(const LiteralDouble& visited)
{
  if(visited.getValue())
    m_sql += visited.getValue()->toString();
}

void te::da::SQLVisitor::visit(const LiteralEnvelope& /*visited*/)
{
}

void te::da::SQLVisitor::visit(const LiteralGeom& visited)
{
  if(visited.getValue())
    m_sql += visited.getValue()->toString();
}

void te::da::SQLVisitor::visit(const LiteralInt16& visited)
{
  if(visited.getValue())
    m_sql += visited.getValue()->toString();
}

void te::da::SQLVisitor::visit(const LiteralInt32& visited)
{
  if(visited.getValue())
    m_sql += visited.getValue()->toString();
}

void te::da::SQLVisitor::visit(const LiteralInt64& visited)
{
  if(visited.getValue())
    m_sql += visited.getValue()->toString();
}

void te::da::SQLVisitor::visit(const LiteralString& visited)
{
  if(visited.getValue())
  {
    m_sql += "'";
    m_sql += visited.getValue()->toString();
    m_sql += "'";
  }
}

void te::da::SQLVisitor::visit(const PropertyName& visited)
{
  m_sql += visited.getName();
}

void te::da::SQLVisitor::visit(const Query& /*visited*/)
{
}

void te::da::SQLVisitor::visit(const Select& visited)
{
  m_sql += "SELECT ";

  if(visited.getDistinct())
  {
    visitDistinct(*(visited.getDistinct()));
    m_sql += " ";
  }

  if(visited.getFields())
  {
    visit(*(visited.getFields()));
    m_sql += " ";
  }

  if(visited.getFrom())
  {
    visit(*(visited.getFrom()));
    m_sql += " ";
  }

  if(visited.getWhere())
  {
    m_sql += "WHERE ";
    visited.getWhere()->getExp()->accept(*this);
    m_sql += " ";
  }

  if(visited.getGroupBy())
  {
    visit(*(visited.getGroupBy()));
    m_sql += " ";
  }

  if(visited.getHaving())
  {
    m_sql += "HAVING ";
    visited.getHaving()->getExp()->accept(*this);
    m_sql += " ";
  }

  if(visited.getOrderBy())
    visit(*(visited.getOrderBy()));
}

void te::da::SQLVisitor::visit(const SubSelect& visited)
{
  assert(visited.getSelect());

  m_sql += "(";
  visited.getSelect()->accept(*this);
  m_sql += ")";

  if(visited.getAlias().empty() == false)
  {
    m_sql += "AS ";
    m_sql += visited.getAlias();
  }  
}

void te::da::SQLVisitor::visit(const In& visited)
{
  assert(visited.getPropertyName());
  visited.getPropertyName()->accept(*this);
  
  m_sql += " IN";
  m_sql += "(";

  for(size_t i = 0; i < visited.getNumArgs(); ++i)
  {
    if(i != 0)
      m_sql += ", ";

    assert(visited[i]);
    visited[i]->accept(*this);
  }

  m_sql += ")";
}

void te::da::SQLVisitor::visitDistinct(const te::da::Distinct& visited)
{
  if(visited.empty())
  {
    m_sql += "DISTINCT";
  }
  else
  {
    m_sql += "DISTINCT ON("; 

    std::size_t ncols = visited.size();

    for(std::size_t i = 0; i < ncols; ++i)
    {
      if(i != 0)
        m_sql += ", ";

      visited[i].accept(*this);
    }

    m_sql += ")";
  }
}

void te::da::SQLVisitor::visit(const te::da::Fields& visited)
{
  std::size_t size = visited.size();

  for(size_t i = 0; i < size; ++i)
  {
    if(i != 0)
      m_sql += ", ";

    visited[i].getExpression()->accept(*this);

    if (visited[i].getAlias())
    {
      m_sql += " AS ";
      m_sql += *(visited[i].getAlias());
    }
  }
}

void te::da::SQLVisitor::visit(const te::da::From& visited)
{
  std::size_t size = visited.size();

  m_sql += "FROM ";

  for(std::size_t i = 0; i < size; ++i)
  {
    if(i != 0)
      m_sql += ", ";

    visited[i].accept(*this);
  }
}

void te::da::SQLVisitor::visit(const te::da::GroupBy& visited)
{
  m_sql += "GROUP BY ";

  std::size_t ncols = visited.size();

  for(std::size_t i = 0; i < ncols; ++i)
  {
    if(i != 0)
      m_sql += ", ";

    visited[i].getExpression()->accept(*this);
  }
}

void te::da::SQLVisitor::visit(const te::da::OrderBy& visited)
{
  std::size_t size = visited.size();

  m_sql += "ORDER BY ";

  for(std::size_t i = 0; i < size; ++i)
  {
    if(i != 0)
      m_sql += ", ";

    visited[i].getExpression()->accept(*this);
    
    if(visited[i].getSortOrder() == te::da::ASC)
      m_sql += " ASC";
    else
      m_sql += " DESC";
  }
}
