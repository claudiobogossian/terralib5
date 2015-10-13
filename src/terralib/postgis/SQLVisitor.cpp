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
  \file terralib/postgis/SQLVisitor.cpp

  \brief A visitor for building an SQL statement from a given Query hierarchy.
*/

// TerraLib
#include "../dataaccess/query/LiteralByteArray.h"
#include "../dataaccess/query/LiteralDateTime.h"
#include "../dataaccess/query/LiteralEnvelope.h"
#include "../dataaccess/query/LiteralGeom.h"
#include "SQLVisitor.h"
#include "Utils.h"

// STL
#include <cassert>

te::pgis::SQLVisitor::SQLVisitor(const te::da::SQLDialect& dialect, std::string& sql, PGconn* conn)
  : te::da::SQLVisitor(dialect, sql),
    m_conn(conn)
{
}

void te::pgis::SQLVisitor::visit(const te::da::LiteralByteArray& visited)
{
  assert(visited.getValue() != 0);
  assert(false);  //TODO
}

void te::pgis::SQLVisitor::visit(const te::da::LiteralDateTime& visited)
{
  assert(visited.getValue() != 0);
  assert(false);  //TODO
}

void te::pgis::SQLVisitor::visit(const te::da::LiteralEnvelope& visited)
{
  assert(visited.getValue() != 0);
  Convert2PostGIS(visited.getValue(), visited.getSRID(), m_sql);
}

void te::pgis::SQLVisitor::visit(const te::da::LiteralGeom& visited)
{
  assert(visited.getValue() != 0);
  Convert2PostGIS(m_conn, static_cast<te::gm::Geometry*>(visited.getValue()), m_sql);
}

void te::pgis::SQLVisitor::visitDistinct(const te::da::Distinct& visited)
{
  m_sql += "DISTINCT (";

  for (std::size_t i = 0; i < visited.size(); ++i)
  {
    if (i != 0)
      m_sql += ", ";

    visited[i].accept(*this);
  }

  m_sql += ")";
}