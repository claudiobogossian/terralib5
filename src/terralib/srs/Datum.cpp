#include "Datum.h"

#include <sstream> 

te::srs::Datum::Datum() 
{}

te::srs::Datum::Datum(const std::string& name, const Ellipsoid& ellps)
  : m_name(name),
    m_ellipsoid(ellps)
{}

void te::srs::Datum::setToWGS84Params(const std::vector<double>& params)
{
  size_t i;
  for (i=0; i<params.size(); m_towgs84.push_back(params[i]), ++i);
  while (i<6)
  {
    m_towgs84.push_back(0.0);
    ++i;
  }
}

std::string te::srs::Datum::getWKT() const
{  
  std::ostringstream sstr;
  sstr.precision(10);

  std::string wkt = "DATUM[\"";
  wkt += m_name;
  wkt += "\",";
  wkt += m_ellipsoid.getWKT();
  if (!m_towgs84.empty())
  {
    wkt += ",TOWGS84[";
    sstr << m_towgs84[0];
    wkt += sstr.str();
    sstr.str("");
    size_t i;
    for (i=1; i<m_towgs84.size(); ++i)
    {
      if (i>6)
        break;      
      sstr << m_towgs84[i];
      wkt += ",";
      wkt += sstr.str();
      sstr.str("");
    }
    wkt += "]";
  }
  wkt += "]";
  return wkt;
}
