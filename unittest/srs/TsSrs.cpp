#include "TsSrs.h"


#include <terralib/common.h>
#include <terralib/srs.h>

#include <string>
#include <iomanip>
#include <iostream>

CPPUNIT_TEST_SUITE_REGISTRATION( TsSrs );

void TsSrs::tcEPSGFactory()
{
  te::srs::CoordinateSystem* cs = te::srs::CoordinateSystemFactory::make("EPSG", 4618);
  CPPUNIT_ASSERT_MESSAGE("Couldn't obtain a valid Coordinate System",cs);
  std::string name = cs->getName();
  CPPUNIT_ASSERT_MESSAGE("Couldn't obtain the right coordinate system",(name == "SAD 69"));
  std::string wkt = cs->getWKT();
  delete cs;

  cs = te::srs::CoordinateSystemFactory::make("EPSG", 29168);
  CPPUNIT_ASSERT_MESSAGE("Couldn't obtain a valid Coordinate System",cs);
  name = cs->getName();
  CPPUNIT_ASSERT_MESSAGE("Couldn't obtain the right coordinate system",(name == "SAD 69 / UTM zone 18N"));
  wkt = cs->getWKT();
  delete cs;

  cs = te::srs::CoordinateSystemFactory::make("EPSG", 32717);
  CPPUNIT_ASSERT_MESSAGE("Couldn't obtain a valid Coordinate System",cs);
  name = cs->getName();
  CPPUNIT_ASSERT_MESSAGE("Couldn't obtain the right coordinate system",(name == "WGS 84 / UTM zone 17S"));
  wkt = cs->getWKT();
  delete cs;
}


void TsSrs::setUp()
{
  te::common::UnitOfMeasure* m = new te::common::UnitOfMeasure(0,te::common::UnitOfMeasure::LengthMeasure);
  m->setName("METER");
  m->setSymbol("m");
  std::vector<std::string> mNames;
  mNames.push_back("METRE");  
  te::common::UnitsOfMeasureManager::getInstance().insert(m,mNames);  

  te::common::UnitOfMeasure* km = new te::common::UnitOfMeasure(1,te::common::UnitOfMeasure::LengthMeasure);
  km->setName("KILOMETER");
  km->setSymbol("km");
  std::vector<std::string> kmNames;
  kmNames.push_back("KILOMETRE");
  km->setBaseUnitId(0);
  km->setConversionFactors(0.001);
  te::common::UnitsOfMeasureManager::getInstance().insert(km,kmNames);  

  te::common::UnitOfMeasure* rad = new te::common::UnitOfMeasure(2,te::common::UnitOfMeasure::AngleMeasure);
  rad->setName("RADIAN");
  rad->setSymbol("rad");
  te::common::UnitsOfMeasureManager::getInstance().insert(rad);  

  te::common::UnitOfMeasure* deg = new te::common::UnitOfMeasure(3,te::common::UnitOfMeasure::AngleMeasure);
  deg->setName("DEGREE");
  deg->setSymbol("deg");
  deg->setBaseUnitId(2);
  deg->setConversionFactors(.0174532925199432958); 
  te::common::UnitsOfMeasureManager::getInstance().insert(deg); 
  
}


void TsSrs::tearDown() 
{
}
