#include "CoordinateSystemWKTFactory.h"
#include "WKTReader.h"

te::srs::CoordinateSystemWKTFactory* te::srs::CoordinateSystemWKTFactory::sm_factory(0);

te::srs::CoordinateSystemWKTFactory::CoordinateSystemWKTFactory()
  : te::srs::CoordinateSystemFactory("WKT")
{}

te::srs::CoordinateSystemWKTFactory::~CoordinateSystemWKTFactory()
{}

te::srs::CoordinateSystem* te::srs::CoordinateSystemWKTFactory::build()
{ return 0; }

void te::srs::CoordinateSystemWKTFactory::add(const std::string& /*description*/, int /*csId*/) 
{}

std::string te::srs::CoordinateSystemWKTFactory::getDescription(int /*csId*/) const 
{ return ""; }

void te::srs::CoordinateSystemWKTFactory::initialize()
{
  finalize();
  sm_factory = new te::srs::CoordinateSystemWKTFactory();
}

void te::srs::CoordinateSystemWKTFactory::finalize()
{
  delete sm_factory;
  sm_factory = 0;
}

int te::srs::CoordinateSystemWKTFactory::suggestCode() const
{
  return -1;
}

//TODO: implement the use of the WKT parser.
te::srs::CoordinateSystem* te::srs::CoordinateSystemWKTFactory::build(const std::string& desc) const
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
