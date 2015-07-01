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
  \file terralib/common/UnitOfMeasure.cpp

  \brief A class for representing a unit of measure.
*/

// TerraLib
#include "UnitOfMeasure.h"

// STL
#include <cassert>
#include <sstream>

// Boost
#include <boost/algorithm/string/case_conv.hpp>

te::common::UnitOfMeasure::UnitOfMeasure(unsigned int id, const std::string& name, const std::string& symbol,
                                         te::common::MeasureType type, const std::string& description)
  : m_id(id),
    m_symbol(symbol),
    m_type(type),
    m_description(description),
    m_baseUnitId(id),
    m_a(1.0),
    m_b(0.0),
    m_c(0.0),
    m_d(1.0)
{
  m_name = boost::to_upper_copy(name);
}

te::common::UnitOfMeasure::UnitOfMeasure(unsigned int id, const std::string& name, const std::string& symbol,
                                         te::common::MeasureType type, unsigned int baseUnitId,
                                         double A, double B, double C , double D,
                                         const std::string& description)
  : m_id(id),
    m_symbol(symbol),
    m_type(type),
    m_description(description),
    m_baseUnitId(baseUnitId),
    m_a(A),
    m_b(B),
    m_c(C),
    m_d(D)
{
  assert((m_c + m_d) != 0);

  m_name = boost::to_upper_copy(name);
}

te::common::UnitOfMeasure::~UnitOfMeasure()
{
}

unsigned int te::common::UnitOfMeasure::getId() const
{
  return m_id;
}

const std::string& te::common::UnitOfMeasure::getName() const
{
  return m_name;
}

void te::common::UnitOfMeasure::setDescription(const std::string& description)
{
  m_description = description;
}

const std::string& te::common::UnitOfMeasure::getDescription() const
{
  return m_description;
}

const std::string& te::common::UnitOfMeasure::getSymbol() const
{
  return m_symbol;
}

te::common::MeasureType te::common::UnitOfMeasure::getType() const
{
  return m_type;
}

bool te::common::UnitOfMeasure::isBaseUnit() const
{
  return (m_id == m_baseUnitId);
}


const unsigned int te::common::UnitOfMeasure::getBaseUnitId() const
{
  return m_baseUnitId;
}


void te::common::UnitOfMeasure::getConversionFactors(double& A, double& B, double& C, double& D) const
{
  A = m_a;
  B = m_b;
  C = m_c;
  D = m_d;
}

double te::common::UnitOfMeasure::getConversionValue() const
{
  return ((m_a + m_b) / (m_c + m_d));
}

std::string te::common::UnitOfMeasure::getWKT() const
{
  double convf = (m_a + m_b) / (m_c + m_d);
  std::string wkt;
  wkt = "UNIT[\"";
  wkt += this->getName();
  wkt += "\", ";
  std::ostringstream sstr;
  sstr.precision(10);
  sstr << convf;
  wkt += sstr.str();
  wkt += "]";
  return wkt;
}

