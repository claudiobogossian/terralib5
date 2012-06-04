#include "TsSrs.h"


#include <terralib/common.h>
#include <terralib/srs.h>

#include <string>
#include <iomanip>
#include <iostream>

CPPUNIT_TEST_SUITE_REGISTRATION( TsSrs );

void TsSrs::tcEPSGFactory()
{
  te::srs::CoordinateSystem* cs = te::srs::CoordinateSystemFactory::make("EPSG", TE_SRS_SAD69);
  CPPUNIT_ASSERT_MESSAGE("Couldn't obtain a valid Coordinate System",cs);
  std::string name = cs->getName();
  CPPUNIT_ASSERT_MESSAGE("Couldn't obtain the right coordinate system",(name == "SAD 69"));
  std::string wkt = cs->getWKT();
  delete cs;

  cs = te::srs::CoordinateSystemFactory::make("EPSG", TE_SRS_SAD69_UTM_ZONE_18N);
  CPPUNIT_ASSERT_MESSAGE("Couldn't obtain a valid Coordinate System",cs);
  name = cs->getName();
  CPPUNIT_ASSERT_MESSAGE("Couldn't obtain the right coordinate system",(name == "SAD 69 / UTM zone 18N"));
  wkt = cs->getWKT();
  delete cs;

  cs = te::srs::CoordinateSystemFactory::make("EPSG", TE_SRS_WGS84_UTM_ZONE_17S);
  CPPUNIT_ASSERT_MESSAGE("Couldn't obtain a valid Coordinate System",cs);
  name = cs->getName();
  CPPUNIT_ASSERT_MESSAGE("Couldn't obtain the right coordinate system",(name == "WGS 84 / UTM zone 17S"));
  wkt = cs->getWKT();
  delete cs;
}


void TsSrs::setUp()
{
}


void TsSrs::tearDown() 
{
}
