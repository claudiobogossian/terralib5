/*  Copyright (C) 2008-2013 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/dataaccess/core/query/QueryCapabilities.cpp

  \brief A class that informs the query support of a given data source.
*/

// TerraLib
#include "QueryCapabilities.h"

te::da::core::QueryCapabilities::QueryCapabilities()
  : m_sqlDialect(false),
    m_insert(false),
    m_update(false),
    m_delete(false),
    m_create(false),
    m_drop(false),
    m_alter(false),
    m_select(false),
    m_selectInto(false)
{
}

te::da::core::QueryCapabilities::~QueryCapabilities()
{
}

bool te::da::core::QueryCapabilities::supportsSQLDialect() const
{
  return m_sqlDialect;
}

void te::da::core::QueryCapabilities::setSupportSQLDialect(const bool& support)
{
  m_sqlDialect = support;
}

bool te::da::core::QueryCapabilities::supportsInsert() const
{
  return m_insert;
}

void te::da::core::QueryCapabilities::setSupportInsert(const bool& support)
{
  m_insert = support;
}

bool te::da::core::QueryCapabilities::supportsUpdate() const
{
  return m_update;
}

void te::da::core::QueryCapabilities::setSupportUpdate(const bool& support)
{
  m_update = support;
}

bool te::da::core::QueryCapabilities::supportsDelete() const
{
  return m_delete;
}

void te::da::core::QueryCapabilities::setSupportDelete(const bool& support)
{
  m_delete = support;
}

bool te::da::core::QueryCapabilities::supportsCreate() const
{
  return m_create;
}

void te::da::core::QueryCapabilities::setSupportCreate(const bool& support)
{
  m_create = support;
}

bool te::da::core::QueryCapabilities::supportsDrop() const
{
  return m_drop;
}

void te::da::core::QueryCapabilities::setSupportDrop(const bool& support)
{
  m_drop = support;
}

bool te::da::core::QueryCapabilities::supportsAlter() const
{
  return m_alter;
}

void te::da::core::QueryCapabilities::setSupportAlter(const bool& support)
{
  m_alter = support;
}

bool te::da::core::QueryCapabilities::supportsSelect() const
{
  return m_select;
}

void te::da::core::QueryCapabilities::setSupportSelect(const bool& support)
{
  m_select = support;
}

bool te::da::core::QueryCapabilities::supportsSelectInto() const
{
  return m_selectInto;
}

void te::da::core::QueryCapabilities::setSupportSelectInto(const bool& support)
{
  m_selectInto = support;
}

void te::da::core::QueryCapabilities::setSupportAll()
{
  m_sqlDialect = true;
  m_insert = true;
  m_update = true;
  m_delete = true;
  m_create = true;
  m_drop = true;
  m_alter = true;
  m_select = true;
  m_selectInto = true;
}

const std::set<std::string>& te::da::core::QueryCapabilities::getSpatialOperators() const
{
  return m_spatialOperators;
}

void te::da::core::QueryCapabilities::addSpatialOperator(const std::string& op)
{
  m_spatialOperators.insert(op);
}

const std::set<std::string>& te::da::core::QueryCapabilities::getLogicalOperators() const
{
  return m_logicalOperators;
}

void te::da::core::QueryCapabilities::addLogicalOperator(const std::string& op)
{
  m_logicalOperators.insert(op);
}

const std::set<std::string>& te::da::core::QueryCapabilities::getComparsionOperators() const
{
  return m_comparsionOperators;
}

void te::da::core::QueryCapabilities::addComparsionOperator(const std::string& op)
{
  m_comparsionOperators.insert(op);
}

const std::set<std::string>& te::da::core::QueryCapabilities::getArithmeticOperators() const
{
  return m_arithmeticOperators;
}

void te::da::core::QueryCapabilities::addArithmeticOperator(const std::string& op)
{
  m_arithmeticOperators.insert(op);
}

const std::set<std::string>& te::da::core::QueryCapabilities::getFunctions() const
{
  return m_functions;
}

void te::da::core::QueryCapabilities::addFunction(const std::string& op)
{
  m_functions.insert(op);
}

const std::set<te::gm::GeomType>& te::da::core::QueryCapabilities::getGeometryOperands() const
{
  return m_geomOperands;
}

void te::da::core::QueryCapabilities::addGeometryOperand(const te::gm::GeomType& type)
{
  m_geomOperands.insert(type);
}
