/*  Copyright (C) 2001-2009 National Institute For Space Research (INPE) - Brazil.

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
  \file UnitOfMeasure.cpp
 
  \brief A class for representing a unit of measure.
 */

// TerraLib
#include "UnitOfMeasure.h"
#include "StringUtils.h"

// STL
#include <sstream> 

te::common::UnitOfMeasure::UnitOfMeasure(unsigned int id, 
                                         te::common::UnitOfMeasure::Type type)
  : 
    m_id(id),
    m_type(type),
    m_baseUnitId(id)
{
}

te::common::UnitOfMeasure::~UnitOfMeasure()
{
}

unsigned int te::common::UnitOfMeasure::getId() const
{
  return m_id;
}

void te::common::UnitOfMeasure::setName(const std::string& name)
{
  m_name = te::common::Convert2UCase(name);
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

void te::common::UnitOfMeasure::setSymbol(const std::string& s)
{
  m_symbol = s;
}

const std::string& te::common::UnitOfMeasure::getSymbol() const
{
  return m_symbol;
}

void te::common::UnitOfMeasure::setType(te::common::UnitOfMeasure::Type t)
{
  m_type = t;
}

te::common::UnitOfMeasure::Type te::common::UnitOfMeasure::getType() const
{
  return m_type;
}


bool te::common::UnitOfMeasure::isBaseUnit() const
{
  return (m_id == m_baseUnitId);
}

void te::common::UnitOfMeasure::setBaseUnitId(unsigned int baseUnitId)
{
  m_baseUnitId = baseUnitId;
}

const unsigned int te::common::UnitOfMeasure::getBaseUnitId() const
{
  return m_baseUnitId;
}

void te::common::UnitOfMeasure::setConversionFactors(double A, double B, double C, double D)
{
  m_conversionFactors.resize(4);
  m_conversionFactors[0] = A;
  m_conversionFactors[1] = B;
  m_conversionFactors[2] = C;
  m_conversionFactors[3] = D;
}

bool te::common::UnitOfMeasure::getConversionFactors(double& A, double& B, double& C, double& D)
{
  if (!m_conversionFactors.empty())
  {
    A = m_conversionFactors[0];
    B = m_conversionFactors[1];
    C = m_conversionFactors[2];
    D = m_conversionFactors[3];
    return true;
  }
  return false;
}

std::string te::common::UnitOfMeasure::getWKT() const
{
  std::string wkt;
  wkt = "UNIT[\"";
  wkt += this->getName();
  wkt += "\", ";
  if (m_id != m_baseUnitId)
  {
    std::ostringstream sstr;
    sstr.precision(10);
    sstr << m_conversionFactors[0];
    wkt += sstr.str();
  }
  else
    wkt += "1";
  wkt += "]";
  return wkt;
}

