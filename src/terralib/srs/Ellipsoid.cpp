#include "Ellipsoid.h"

#include "../common/Translator.h"

#include <sstream> 

te::srs::Ellipsoid::Ellipsoid(const std::string& name, double rad, double invflat):
  m_name(name),
  m_radium(rad),
  m_invFlattening(invflat)
{}
  
const std::string& te::srs::Ellipsoid::getName() const 
{ 
  return m_name; 
}

void te::srs::Ellipsoid::setName(const std::string& name) 
{ 
  m_name = name; 
}

double te::srs::Ellipsoid::getRadium() const 
{ 
  return m_radium; 
}

void te::srs::Ellipsoid::setRadium(const double& rad) 
{ 
  m_radium = rad; 
}

double te::srs::Ellipsoid::getInverseFlattening() const 
{ 
  return m_invFlattening; 
}

void te::srs::Ellipsoid::setInverseFlattening(const double& invflat) 
{ 
  m_invFlattening = invflat; 
}

std::string 
te::srs::Ellipsoid::getWKT() const
{
  std::ostringstream sstr;
  sstr.precision(10);

  std::string wkt = "SPHEROID[\"";
	wkt += m_name;
	wkt += "\",";
  sstr << m_radium;
	wkt += sstr.str();	
	wkt += ",";
  sstr.str("");
  sstr << m_invFlattening;
	wkt += sstr.str();
	wkt += "]";
	return wkt;
}

