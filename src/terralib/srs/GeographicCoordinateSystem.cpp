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
 \file terralib/srs/GeographicCoordinateSystem.cpp
*/

// TerraLib
#include "../common/Exception.h"
#include "../common/Translator.h"
#include "../common/UnitOfMeasure.h"
#include "../common/UnitsOfMeasureManager.h"
#include "Datum.h"
#include "GeographicCoordinateSystem.h"

//STL
#include <cassert>
#include <sstream> 

te::srs::GeographicCoordinateSystem::GeographicCoordinateSystem(const std::string& name, 
                                                                const std::string unitName, 
                                                                double primem) :
  te::srs::SpatialReferenceSystem(name, unitName),
  m_datum(0),
  m_primem(primem)
{}

te::srs::GeographicCoordinateSystem::~GeographicCoordinateSystem()
{
  delete m_datum;
}
 
void te::srs::GeographicCoordinateSystem::setDatum(te::srs::Datum* datum)
{
  assert(datum);
  
  delete m_datum;
  m_datum = datum;
}

const te::srs::Datum* te::srs::GeographicCoordinateSystem::getDatum() const
{
  return m_datum;
}

double te::srs::GeographicCoordinateSystem::getPrimem() const
{
  return m_primem;
}

void te::srs::GeographicCoordinateSystem::setPrimem(double primen)
{
  m_primem = primen;
}

std::string te::srs::GeographicCoordinateSystem::getWKT() const
{
  std::ostringstream sstr;
  sstr.precision(10);

  std::string wkt = "GEOGCS[\"";
	wkt += m_name;
  if (m_datum)
  {
	  wkt += "\",";
	  wkt += m_datum->getWKT();
  }
	wkt += ",";
	wkt += "PRIMEM[\"";
  if (m_primem == 0)
    wkt += "Greenwich";
  else
    wkt += "Unknown";
  wkt += "\",";
  sstr << m_primem;
	wkt += sstr.str();
	wkt += "]";
  te::common::UnitOfMeasurePtr unit = te::common::UnitsOfMeasureManager::getInstance().find(m_unitName);
  if (unit.get())
  {
	  wkt += ", ";
	  wkt += unit->getWKT();
  }
  if (m_srid.first > 0 && !m_srid.second.empty())
  {
    wkt += ",AUTHORITY[\"";
    wkt += m_srid.second;
    wkt += "\",\"";
    sstr.str("");
    sstr << m_srid.first;
   	wkt += sstr.str();
    wkt += "\"]";
  }
	wkt += "]";
	return wkt;
}

bool 
te::srs::GeographicCoordinateSystem::isGeographic() const
{
  return true;
}
