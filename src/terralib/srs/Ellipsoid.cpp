#include "Ellipsoid.h"

#include "../common/Translator.h"

#include <sstream> 

te::srs::Ellipsoid::Ellipsoid(const std::string& name, double rad, double invflat):
  m_name(name),
  m_radium(rad),
  m_invFlattening(invflat)
{}
  
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

