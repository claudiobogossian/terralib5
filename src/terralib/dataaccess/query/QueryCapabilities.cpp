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
  \file terralib/dataaccess/query/QueryCapabilities.cpp

  \brief A class that informs the query support of a given data source.
*/

// TerraLib
#include "QueryCapabilities.h"

te::da::QueryCapabilities::QueryCapabilities()
  : m_sqlDialect(false),
    m_spatialSQLDialect(false),
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

te::da::QueryCapabilities::~QueryCapabilities()
{
}

bool te::da::QueryCapabilities::supportsSQLDialect() const
{
  return m_sqlDialect;
}

void te::da::QueryCapabilities::setSupportSQLDialect(const bool& support)
{
  m_sqlDialect = support;
}

bool te::da::QueryCapabilities::supportsSpatialSQLDialect() const
{
  return m_spatialSQLDialect;
}

void te::da::QueryCapabilities::setSupportSpatialSQLDialect(const bool& support)
{
  m_spatialSQLDialect = support;
}

bool te::da::QueryCapabilities::supportsInsert() const
{
  return m_insert;
}

void te::da::QueryCapabilities::setSupportInsert(const bool& support)
{
  m_insert = support;
}

bool te::da::QueryCapabilities::supportsUpdate() const
{
  return m_update;
}

void te::da::QueryCapabilities::setSupportUpdate(const bool& support)
{
  m_update = support;
}

bool te::da::QueryCapabilities::supportsDelete() const
{
  return m_delete;
}

void te::da::QueryCapabilities::setSupportDelete(const bool& support)
{
  m_delete = support;
}

bool te::da::QueryCapabilities::supportsCreate() const
{
  return m_create;
}

void te::da::QueryCapabilities::setSupportCreate(const bool& support)
{
  m_create = support;
}

bool te::da::QueryCapabilities::supportsDrop() const
{
  return m_drop;
}

void te::da::QueryCapabilities::setSupportDrop(const bool& support)
{
  m_drop = support;
}

bool te::da::QueryCapabilities::supportsAlter() const
{
  return m_alter;
}

void te::da::QueryCapabilities::setSupportAlter(const bool& support)
{
  m_alter = support;
}

bool te::da::QueryCapabilities::supportsSelect() const
{
  return m_select;
}

void te::da::QueryCapabilities::setSupportSelect(const bool& support)
{
  m_select = support;
}

bool te::da::QueryCapabilities::supportsSelectInto() const
{
  return m_selectInto;
}

void te::da::QueryCapabilities::setSupportSelectInto(const bool& support)
{
  m_selectInto = support;
}

void te::da::QueryCapabilities::setSupportAll()
{
  m_sqlDialect = true;
  m_spatialSQLDialect = true;
  m_insert = true;
  m_update = true;
  m_delete = true;
  m_create = true;
  m_drop = true;
  m_alter = true;
  m_select = true;
  m_selectInto = true;
}

const std::set<std::string>& te::da::QueryCapabilities::getSpatialOperators() const
{
  return m_spatialOperators;
}

void te::da::QueryCapabilities::addSpatialOperator(const std::string& op)
{
  m_spatialOperators.insert(op);
}

const std::set<std::string>& te::da::QueryCapabilities::getSpatialTopologicOperators() const
{
  return m_spatialTopologicOperators;
}

void te::da::QueryCapabilities::addSpatialTopologicOperator(const std::string& op)
{
  m_spatialTopologicOperators.insert(op);
}

const std::set<std::string>& te::da::QueryCapabilities::getSpatialMetricOperators() const
{
  return m_spatialMetricOperators;
}

void te::da::QueryCapabilities::addSpatialMetricOperator(const std::string& op)
{
  m_spatialMetricOperators.insert(op);
}

const std::set<std::string>& te::da::QueryCapabilities::getSpatialNewGeomOperators() const
{
  return m_spatialNewGeomOperators;
}

void te::da::QueryCapabilities::addSpatialNewGeomOperator(const std::string& op)
{
  m_spatialNewGeomOperators.insert(op);
}

const std::set<std::string>& te::da::QueryCapabilities::getLogicalOperators() const
{
  return m_logicalOperators;
}

void te::da::QueryCapabilities::addLogicalOperator(const std::string& op)
{
  m_logicalOperators.insert(op);
}

const std::set<std::string>& te::da::QueryCapabilities::getComparsionOperators() const
{
  return m_comparsionOperators;
}

void te::da::QueryCapabilities::addComparsionOperator(const std::string& op)
{
  m_comparsionOperators.insert(op);
}

const std::set<std::string>& te::da::QueryCapabilities::getArithmeticOperators() const
{
  return m_arithmeticOperators;
}

void te::da::QueryCapabilities::addArithmeticOperator(const std::string& op)
{
  m_arithmeticOperators.insert(op);
}

const std::set<std::string>& te::da::QueryCapabilities::getFunctions() const
{
  return m_functions;
}

void te::da::QueryCapabilities::addFunction(const std::string& op)
{
  m_functions.insert(op);
}

const std::set<te::gm::GeomType>& te::da::QueryCapabilities::getGeometryOperands() const
{
  return m_geomOperands;
}

void te::da::QueryCapabilities::addGeometryOperand(const te::gm::GeomType& type)
{
  m_geomOperands.insert(type);
}
