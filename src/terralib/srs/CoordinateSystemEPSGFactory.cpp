#include "CoordinateSystemEPSGFactory.h"

#include "WKTReader.h"

#include <stdlib.h>

#include <sstream> 

te::srs::CoordinateSystemEPSGFactory* te::srs::CoordinateSystemEPSGFactory::sm_factory(0);

std::map<int, std::string> te::srs::CoordinateSystemEPSGFactory::sm_mapEPSG2WKT;


te::srs::CoordinateSystemEPSGFactory::CoordinateSystemEPSGFactory()
  : te::srs::CoordinateSystemFactory("EPSG")
{}

te::srs::CoordinateSystemEPSGFactory::~CoordinateSystemEPSGFactory()
{
}

int te::srs::CoordinateSystemEPSGFactory::suggestCode() const
{
  int fcode = 32768;  // EPSG suggest that users who wish to augment the EPSG 
                      // data with their own information should utilise codes 
                      // greater than 32767.
  std::map<int, std::string>::const_iterator it = sm_mapEPSG2WKT.find(fcode);
  while (it != sm_mapEPSG2WKT.end()) 
  {
    ++fcode;
    it = sm_mapEPSG2WKT.find(fcode);
  }
  return fcode;
}

void te::srs::CoordinateSystemEPSGFactory::add(const std::string& description, int code)
{
   sm_mapEPSG2WKT.insert(std::pair<int, std::string>(code,description));
}

std::string te::srs::CoordinateSystemEPSGFactory::getDescription(int code) const
{
  std::map<int, std::string>::const_iterator it = sm_mapEPSG2WKT.find(code);
  if (it != sm_mapEPSG2WKT.end())
    return it->second;

  return "";
}

void te::srs::CoordinateSystemEPSGFactory::initialize()
{
  finalize();
  sm_factory = new te::srs::CoordinateSystemEPSGFactory();

//TODO: find another way to fill the table.
#include "EPSGDefinitions.h"
}

void te::srs::CoordinateSystemEPSGFactory::finalize()
{
  delete sm_factory;
  sm_factory = 0;

  sm_mapEPSG2WKT.clear();
}

te::srs::CoordinateSystem* te::srs::CoordinateSystemEPSGFactory::build(const std::string& desc) const
{
  try
  {
    return te::srs::WKTReader::read(desc.c_str());
  }
  catch(...)
  {
    return 0;
  }
}

te::srs::CoordinateSystem* te::srs::CoordinateSystemEPSGFactory::build()
{ return 0; }
