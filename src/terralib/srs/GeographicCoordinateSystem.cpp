#include "GeographicCoordinateSystem.h"

#include "../common/UnitsOfMeasureManager.h"
#include "../common/Translator.h"
#include "../common/Exception.h"

#include <sstream> 

te::srs::GeographicCoordinateSystem::GeographicCoordinateSystem() :
  te::srs::CoordinateSystem(),
  m_primem(0.0)
{}

te::srs::GeographicCoordinateSystem::GeographicCoordinateSystem(const std::string& name, const Datum& datum, 
                                   double primem, const std::string& unitName) :
  te::srs::CoordinateSystem(name, unitName),
  m_datum(datum),
  m_primem(primem)
{}
 

std::string te::srs::GeographicCoordinateSystem::getWKT() const
{
  std::ostringstream sstr;
  sstr.precision(10);

  std::string wkt = "GEOGCS[\"";
	wkt += m_name;
	wkt += "\",";
	wkt += m_datum.getWKT();
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
  if (!m_authority.empty())
  {
    wkt += ",AUTHORITY[\"";
    wkt += m_authority;
    wkt += "\",\"";
    sstr.str("");
    sstr << m_id;
   	wkt += sstr.str();
    wkt += "\"]";
  }
	wkt += "]";
	return wkt;
}

