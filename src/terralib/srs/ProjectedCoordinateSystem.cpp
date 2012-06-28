#include "Module.h"
#include "ProjectedCoordinateSystem.h"
#include "GeographicCoordinateSystem.h"

#include "../common/UnitOfMeasure.h"
#include "../common/UnitsOfMeasureManager.h"
#include "../common/Translator.h"
#include "../common/Exception.h"

#include <sstream> 
#include <assert.h>

te::srs::ProjectedCoordinateSystem::ProjectedCoordinateSystem(const std::string& name, 
                                                              const std::string& unitName,
                                                              te::srs::GeographicCoordinateSystem* geogcs,
                                                              const std::string& projName):
  te::srs::SpatialReferenceSystem(name,unitName),
  m_geogcs(geogcs),
  m_projection(projName)
{}

te::srs::ProjectedCoordinateSystem::~ProjectedCoordinateSystem()
{
  delete m_geogcs;
}

void 
te::srs::ProjectedCoordinateSystem::setGeographicCoordinateSystem(te::srs::GeographicCoordinateSystem* geogcs) 
{ 
  assert(geogcs);
  
  delete geogcs;
  m_geogcs = geogcs; 
}

const te::srs::GeographicCoordinateSystem* 
te::srs::ProjectedCoordinateSystem::getGeographicCoordinateSystem() const 
{ 
  return m_geogcs; 
}

void 
te::srs::ProjectedCoordinateSystem::setProjection(const std::string& projname)  
{ 
  m_projection = projname; 
} 

const std::string& 
te::srs::ProjectedCoordinateSystem::getProjection() const 
{ 
  return m_projection; 
} 

void 
te::srs::ProjectedCoordinateSystem::setParameters(const std::map<std::string, double>& params) 
{ 
  m_params = params; 
}

const std::map<std::string, double>& 
te::srs::ProjectedCoordinateSystem::getParameters() const 
{ 
  return m_params; 
}

std::string
te::srs::ProjectedCoordinateSystem::getWKT() const
{
  std::ostringstream sstr;
  sstr.precision(10);

  std::string wkt = "PROJCS[\"";
 	wkt += m_name;
  if (m_geogcs)
  {
	  wkt += "\",";
	  wkt += m_geogcs->getWKT();
  }
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
te::srs::ProjectedCoordinateSystem::isGeographic() const
{ 
  return false; 
}
