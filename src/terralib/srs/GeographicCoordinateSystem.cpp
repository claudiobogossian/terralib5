#include "GeographicCoordinateSystem.h"
#include "Datum.h"

#include "../common/UnitOfMeasure.h"
#include "../common/UnitsOfMeasureManager.h"
#include "../common/Translator.h"
#include "../common/Exception.h"

#include <sstream> 
#include <assert.h>

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
  te::common::UnitOfMeasure* unit = te::common::UnitsOfMeasureManager::getInstance().findByName(m_unitName);
  if (unit)
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
