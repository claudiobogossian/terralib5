#include "Module.h"
#include "ProjectedCoordinateSystem.h"

#include "../common/UnitsOfMeasureManager.h"
#include "../common/Translator.h"
#include "../common/Exception.h"

#include <sstream> 

te::srs::ProjectedCoordinateSystem::ProjectedCoordinateSystem() :
  te::srs::CoordinateSystem("","METER"),
  m_geogcs(0)
{}

te::srs::ProjectedCoordinateSystem::ProjectedCoordinateSystem(const std::string& name, 
                                                              const std::string& unitName):
  te::srs::CoordinateSystem(name,unitName),
  m_geogcs(0)
{}

te::srs::ProjectedCoordinateSystem::~ProjectedCoordinateSystem()
{
  if (m_geogcs)
    delete m_geogcs;
}

std::string
te::srs::ProjectedCoordinateSystem::getWKT() const
{
  if (!m_geogcs)
  {
		throw te::common::Exception(TR_SRS("Projected Coordinate System don't have the corresponding Geographic Coordinate System."));
  }
  std::string wkt = "PROJCS[\"";
 	wkt += m_name;
	wkt += "\",";
	wkt += m_geogcs->getWKT();
	wkt += ",PROJECTION[\"";
	wkt += m_projection;
	wkt += "\"]";
	if (!m_params.empty())
	{
    std::map<std::string, double>::const_iterator it = m_params.begin();
		do {
      std::ostringstream sstr;
      sstr.precision(10);
			wkt += ",PARAMETER[\"";
			wkt += it->first;
			wkt += "\",";
      sstr << it->second;
			wkt += sstr.str();
			wkt += "]";
			++it;
		}while (it !=  m_params.end()); 
	}
  te::common::UnitOfMeasure* unit = te::common::UnitsOfMeasureManager::getInstance().findByName(m_unitName);
  if (unit)
  {
	  wkt += ", ";
	  wkt += unit->getWKT();
  }
  if (!m_authority.empty())
  {
    std::ostringstream sstr;
    wkt += ",AUTHORITY[\"";
    wkt += m_authority;
    wkt += "\",\"";
    sstr << m_id;
   	wkt += sstr.str();
    wkt += "\"]";
  }
	wkt += "]";
	return wkt;
}
