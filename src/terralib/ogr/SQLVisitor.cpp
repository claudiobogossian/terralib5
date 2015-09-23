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
  \file terralib/ogr/SQLVisitor.cpp

  \brief A visitor for building an SQL statement using OGR dialect.
*/

// TerraLib
#include "../common/StringUtils.h"
#include "../geometry/Envelope.h"
#include "../dataaccess/query/DataSetName.h"
#include "../dataaccess/query/Function.h"
#include "../dataaccess/query/Having.h"
#include "../dataaccess/query/Join.h"
#include "../dataaccess/query/JoinConditionOn.h"
#include "../dataaccess/query/LiteralEnvelope.h"
#include "../dataaccess/query/PropertyName.h"
#include "../dataaccess/query/Select.h"
#include "../dataaccess/query/Where.h"
#include "../dataaccess/Enums.h"
#include "SQLVisitor.h"

// STL
#include <cassert>
#include <string>
#include <vector>

te::ogr::SQLVisitor::SQLVisitor(const te::da::SQLDialect& dialect, std::string& sql)
  : te::da::SQLVisitor(dialect, sql),
    m_bbox(0)
{
}

void te::ogr::SQLVisitor::visit(const te::da::DataSetName& visited)
{
  m_sql += "\"" + visited.getName() + "\"";
}

void te::ogr::SQLVisitor::visit(const te::da::LiteralEnvelope& visited)
{
  m_bbox = visited.getValue();

  m_sql += "BBOX)";
}

void te::ogr::SQLVisitor::visit(const te::da::PropertyName& visited)
{
  std::vector<std::string> values;
  te::common::Tokenize(visited.getName(), values, ".");

  if(values.size() == 1)
    m_sql += visited.getName();
  else
    m_sql += values[values.size() - 1];
}

void te::ogr::SQLVisitor::visit(const te::da::Select& visited)
{
  m_sql += "SELECT FID, ";

  if(visited.getDistinct())
  {
    visitDistinct(*(visited.getDistinct()));
    m_sql += " ";
  }

  if(visited.getFields())
  {
    te::da::SQLVisitor::visit(*(visited.getFields()));
    m_sql += " ";
  }

  if(visited.getFrom())
  {
    te::da::SQLVisitor::visit(*(visited.getFrom()));
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
    te::da::SQLVisitor::visit(*(visited.getGroupBy()));
    m_sql += " ";
  }

  if(visited.getHaving())
  {
    m_sql += "HAVING ";
    visited.getHaving()->getExp()->accept(*this);
    m_sql += " ";
  }

  if(visited.getOrderBy())
    te::da::SQLVisitor::visit(*(visited.getOrderBy()));
}

void te::ogr::SQLVisitor::visit(const te::da::Join& visited)
{
  assert(visited.getFirst() && visited.getSecond());

  visited.getFirst()->accept(*this);

  if(visited.isNatural())
    m_sql += " NATURAL ";

  switch(visited.getType())
  {
    case te::da::JOIN :
      m_sql += " JOIN ";
    break;

    case te::da::INNER_JOIN :
      m_sql += " INNER JOIN ";
    break;

    case te::da::LEFT_JOIN :
      m_sql += " LEFT JOIN ";
    break;

    case te::da::RIGHT_JOIN :
      m_sql += " RIGHT JOIN ";
    break;

    case te::da::FULL_OUTER_JOIN :
      m_sql += " FULL OUTER JOIN ";
    break;

    case te::da::CROSS_JOIN :
      m_sql += " CROSS JOIN ";
    break;

    case te::da::NATURAL_JOIN :
      m_sql += " NATURAL JOIN ";
    break;
  }

  visited.getSecond()->accept(*this);

  if(visited.getCondition())
  {
    m_sql += " ";
    visited.getCondition()->accept(*this);
  }
}

void te::ogr::SQLVisitor::visit(const te::da::JoinConditionOn& visited)
{
  assert(visited.getCondition());  
  m_sql += "ON ";

  te::da::Function func = dynamic_cast< te::da::Function&>(*visited.getCondition());
  const std::string& fname = func.getName();

  assert(func.getNumArgs() == 2);
  func[0]->accept(*this);
  m_sql += " ";
  m_sql += fname;
  m_sql += " ";
  func[1]->accept(*this);
}

void te::ogr::SQLVisitor::visitDistinct(const te::da::Distinct& visited)
{
  /* TODO: http://www.gdal.org/ogr/ogr_sql.html

  A special form of the field list uses the DISTINCT keyword. This returns a list of all the distinct values
  of the named attribute. When the DISTINCT keyword is used, only one attribute may appear in the field list.
  The DISTINCT keyword may be used against any type of field. Currently the distinctness test against a string value
  is case insensitive in OGR SQL. The result of a SELECT with a DISTINCT keyword is a layer with one column
  (named the same as the field operated on), and one feature per distinct value.
  Geometries are discarded. The distinct values are assembled in memory, so alot of memory may be used for datasets with a large number of distinct values. */
  
  m_sql = "SELECT DISTINCT ";
  
  for (std::size_t i = 0; i < visited.size(); ++i)
  {
    te::da::Expression* exp = visited[i].clone();
    te::da::PropertyName* propName = new te::da::PropertyName(dynamic_cast<te::da::PropertyName&>(*exp));

    if (propName)
    {
      const std::string& att = propName->getName();
      m_sql += att;
      m_sql += ", ";
    }
  }
  int size = m_sql.size();
  m_sql.erase(m_sql.begin() + (size - 2), m_sql.end() - 1);
}

te::gm::Envelope* te::ogr::SQLVisitor::getMBR()
{
  return m_bbox;
}
