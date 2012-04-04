#include "CoordinateSystemProj4Factory.h"
#include "GeographicCoordinateSystem.h"
#include "ProjectedCoordinateSystem.h"

#include "../common/StringUtils.h"
#include "../common/UnitsOfMeasureManager.h"

#include <cassert>
#include <cstdlib>

#ifdef TE_USE_PROJ4

te::srs::CoordinateSystemProj4Factory* te::srs::CoordinateSystemProj4Factory::sm_factory(0);

std::map<int, std::string> te::srs::CoordinateSystemProj4Factory::sm_mapCode2Proj4;

te::srs::CoordinateSystemProj4Factory::~CoordinateSystemProj4Factory()
{
}

te::srs::CoordinateSystemProj4Factory::CoordinateSystemProj4Factory()
  : te::srs::CoordinateSystemFactory("PROJ4")
{
}

void te::srs::CoordinateSystemProj4Factory::initialize()
{
  finalize();
  sm_factory = new te::srs::CoordinateSystemProj4Factory();

//TODO: initialize it somewhere else.
#include "Proj4Definitions.h"
}

void te::srs::CoordinateSystemProj4Factory::finalize()
{
  delete sm_factory;
  sm_factory = 0;
}

int te::srs::CoordinateSystemProj4Factory::suggestCode() const
{
  int fcode = 32768;  // to garantee that there is no conflict with EPSG.
  std::map<int, std::string>::const_iterator it = sm_mapCode2Proj4.find(fcode);
  while (it != sm_mapCode2Proj4.end()) 
  {
    ++fcode;
    it = sm_mapCode2Proj4.find(fcode);
  }
  return fcode;
}

void te::srs::CoordinateSystemProj4Factory::add(const std::string& description, int csId)
{
   sm_mapCode2Proj4.insert(std::pair<int, std::string>(csId,description));
}

std::string te::srs::CoordinateSystemProj4Factory::getDescription(int csId) const
{
  std::map<int, std::string>::const_iterator it = sm_mapCode2Proj4.find(csId);
  if (it != sm_mapCode2Proj4.end())
    return it->second;

  return "";
}

te::srs::CoordinateSystem* te::srs::CoordinateSystemProj4Factory::build(const std::string& desc) const
{
  std::map<std::string, std::string> kvp;
  te::common::ExtractKVP(desc, kvp," ");
  if (kvp.empty())
     return 0;

  // try to stablish the ellipsoid and/or datum
  te::srs::Datum datum;
  te::srs::Ellipsoid ellps("WGS 84",6378137.0,298.257223563); // default WGS84
  std::map<std::string, std::string>::const_iterator itkvp = kvp.find("+ellps");
  if (itkvp != kvp.end())
  {
    ellps.setName(itkvp->second);
    if (itkvp->second == "intl")
    {
      ellps.setName("International 1924");
      ellps.setInverseFlattening(297.0);
      ellps.setRadium(6378388.0);
      datum.setName("Corrego_Alegre");
    }
    else if (itkvp->second == "GRS80")
    {
      ellps.setName("GRS 1980");
      ellps.setInverseFlattening(298.257222101);
      ellps.setRadium(6378137.0);
      datum.setName("Sistema_de_Referencia_Geocentrico_para_America_del_Sur_2000");
    }
    else if (itkvp->second == "WGS84")
    {
      ellps.setName("WGS_1984");
      ellps.setInverseFlattening(298.257223563);
      ellps.setRadium(6378137.0);
      datum.setName("WGS_1984");
    }
    else if (itkvp->second == "aust_SA")
    {
      ellps.setName("GRS 1967 (SAD69)");     
      ellps.setInverseFlattening(298.25);
      ellps.setRadium(6378160.0);
      datum.setName("South_American_Datum_1969");
    }
    else if (itkvp->second == "GRS67")
    {
      ellps.setName("GRS 1967");
      ellps.setInverseFlattening(298.2471674270);
      ellps.setRadium(6378160.0);
      datum.setName("South_American_Datum_1969");
    }
  }
  else
  {
    ellps.setName("Unknown");
    itkvp = kvp.find("+a");
    if (itkvp != kvp.end())
      ellps.setRadium(atof(itkvp->second.c_str()));

    itkvp = kvp.find("+b");
    if (itkvp != kvp.end())
      ellps.setInverseFlattening(atof(itkvp->second.c_str()));
  }
  
  itkvp = kvp.find("+datum");
  if (itkvp != kvp.end())
    datum.setName(itkvp->second);

  datum.setEllipsoid(ellps);
  itkvp = kvp.find("+towgs84");
  if (itkvp != kvp.end())
  {
    std::vector<std::string> tokens;
    te::common::Tokenize(itkvp->second,tokens,",");
    std::vector<double> towgs84;
    for (size_t i=0; i<tokens.size(); towgs84.push_back(atof(tokens[i].c_str())),++i);
    datum.setToWGS84Params(towgs84);
  }
  //TODO: check for alternative prime meridian  
  te::srs::GeographicCoordinateSystem* geogcs = new te::srs::GeographicCoordinateSystem(datum.getName(),datum);    
    
  // it is a geographic datum
  itkvp = kvp.find("+proj");
  if (itkvp == kvp.end() || itkvp->second == "latlong" || itkvp->second == "longlat")
    return geogcs;
  
  // Decoding general parameters
  std::map<std::string, double> params;
  std::string projcsname = datum.getName();  
  std::string projname;
  itkvp = kvp.find("+lat_0");
  if (itkvp != kvp.end())
    params["latitude_of_origin"] = atof(itkvp->second.c_str());
  else
    params["latitude_of_origin"] = 0.0;

  itkvp = kvp.find("+lat_ts");
  if (itkvp != kvp.end())
    params["latitude_of_origin"]=atof(itkvp->second.c_str()); 
    
  itkvp = kvp.find("+lon_0");
  if (itkvp != kvp.end())
    params["central_meridian"] = atof(itkvp->second.c_str());

  itkvp = kvp.find("+x_0");
  if (itkvp != kvp.end())
    params["false_easting"]=atof(itkvp->second.c_str());

  itkvp = kvp.find("+y_0");
  if (itkvp != kvp.end())
    params["false_northing"]=atof(itkvp->second.c_str());

  itkvp = kvp.find("+k_0");
  if (itkvp == kvp.end())
    itkvp = kvp.find("+k");
  if (itkvp != kvp.end())
    params["scale_factor"]=atof(itkvp->second.c_str());

  itkvp = kvp.find("+proj");
  if (itkvp->second == "utm")
  {
    bool isSouth=false;
    projname = "Transverse_Mercator";
    params["false_easting"]=500000;
    itkvp = kvp.find("+south");
    if (itkvp != kvp.end())
    {
      params["false_northing"]=10000000;
      isSouth = true;
    }
    else
      params["false_northing"]=0;
    
  
    projcsname += " / UTM ";    
    itkvp = kvp.find("+zone");    
    if (itkvp != kvp.end())
    {
      projcsname += "zone ";
      projcsname += itkvp->second;
      if (isSouth)
        projcsname += "S";
      params["central_meridian"] = (atoi(itkvp->second.c_str())*6)-183;
    }
  }
  else if (itkvp->second == "stere")
  {
    projname = "Polar_Stereographic";
    projcsname += " / Antarctic Polar Stereographic";
  }
  else if (itkvp->second == "poly")
  {
    projname = "Polyconic";
    projcsname += "/ Brazil Polyconic";
  }
  else
  {
    projname = itkvp->second;
    projcsname += " / ";
    projcsname += itkvp->second;
  }

  te::srs::ProjectedCoordinateSystem* projcs = new te::srs::ProjectedCoordinateSystem(projcsname);
  projcs->setProjection(projname);
  projcs->setParameters(params);
  itkvp = kvp.find("+unit");
  if (itkvp != kvp.end())
    projcs->setUnitName(itkvp->second);

  projcs->setGeographicCoordinateSystem(geogcs);

  return projcs;
}

#endif

te::srs::CoordinateSystem* te::srs::CoordinateSystemProj4Factory::build()
{  return 0; }
