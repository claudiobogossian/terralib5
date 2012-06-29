#include "Datum.h"
#include "Ellipsoid.h"

#include <assert.h>
#include <sstream> 

te::srs::Datum::Datum(const std::string& name, te::srs::Ellipsoid* ellps):
  m_name(name),
  m_ellipsoid(ellps)
{}

te::srs::Datum::~Datum()
{
  delete m_ellipsoid;
}

const std::string& te::srs::Datum::getName() const 
{ 
  return m_name; 
}

void te::srs::Datum::setName(const std::string& name) 
{ 
  m_name = name; 
}

const te::srs::Ellipsoid* te::srs::Datum::getEllipsoid() const 
{ 
  return m_ellipsoid; 
}

void te::srs::Datum::setEllipsoid(te::srs::Ellipsoid* ellps) 
{
  assert(ellps);
  
  delete m_ellipsoid;
  m_ellipsoid = ellps; 
}

void te::srs::Datum::setToWGS84Params(const std::vector<double>& params)
{
  m_towgs84.clear();
  
  size_t i;
  for (i=0; i<params.size(); m_towgs84.push_back(params[i]), ++i);
  while (i<6)
  {
    m_towgs84.push_back(0.0);
    ++i;
  }
}

const std::vector<double>& te::srs::Datum::getToWGS84Params() const 
{
  return m_towgs84; 
}

std::string te::srs::Datum::getWKT() const
{  
  std::ostringstream sstr;
  sstr.precision(10);

  std::string wkt = "DATUM[\"";
  wkt += m_name;
  wkt += "\",";
  wkt += m_ellipsoid->getWKT();
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
