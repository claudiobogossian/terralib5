/*  Copyright (C) 2011-2012 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/mysql/SQLVisitor.cpp

  \brief A visitor for building an SQL statement from a given Query hierarchy.
*/

// TerraLib
#include "../common/Translator.h"
#include "../dataaccess/query/LiteralByteArray.h"
#include "../dataaccess/query/LiteralDateTime.h"
#include "../dataaccess/query/LiteralEnvelope.h"
#include "../dataaccess/query/LiteralGeom.h"
#include "Exception.h"
#include "SQLVisitor.h"

// STL
#include <cassert>

te::mysql::SQLVisitor::SQLVisitor(const te::da::SQLDialect& dialect, std::string& sql)
  : te::da::SQLVisitor(dialect, sql)
{
}

void te::mysql::SQLVisitor::visit(const te::da::LiteralByteArray& /*visited*/)
{
  throw Exception(TR_MYSQL("Not implemented yet!"));
}

void te::mysql::SQLVisitor::visit(const te::da::LiteralDateTime& /*visited*/)
{
  throw Exception(TR_MYSQL("Not implemented yet!"));
}

void te::mysql::SQLVisitor::visit(const te::da::LiteralEnvelope& /*visited*/)
{
  throw Exception(TR_MYSQL("Not implemented yet!"));
}

void te::mysql::SQLVisitor::visit(const te::da::LiteralGeom& /*visited*/)
{
  throw Exception(TR_MYSQL("Not implemented yet!"));
}

