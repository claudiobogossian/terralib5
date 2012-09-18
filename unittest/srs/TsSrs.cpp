#include "TsSrs.h"


#include <terralib/common.h>
#include <terralib/srs.h>
#include <terralib/srs/Module.h>

#include <string>
#include <iomanip>
#include <iostream>

CPPUNIT_TEST_SUITE_REGISTRATION( TsSrs );

void TsSrs::tcSRSManager()
{
  std::string name = te::srs::SpatialReferenceSystemManager::getInstance().getName(TE_SRS_SAD69);
  CPPUNIT_ASSERT_MESSAGE("Fail to find name for TE_SRS_SAD69",(!name.empty()));
  
  std::pair<std::string,unsigned int> srid = te::srs::SpatialReferenceSystemManager::getInstance().getIdFromName(name);
  CPPUNIT_ASSERT_MESSAGE("Fail to access geographic SRS",(srid.second == TE_SRS_SAD69));
  
  name = te::srs::SpatialReferenceSystemManager::getInstance().getName(TE_SRS_WGS84_UTM_ZONE_25S);
  CPPUNIT_ASSERT_MESSAGE("Fail to find name for TE_SRS_WGS84_UTM_ZONE_25S",(!name.empty()));
  
  srid = te::srs::SpatialReferenceSystemManager::getInstance().getIdFromName(name);
  CPPUNIT_ASSERT_MESSAGE("Fail to access projected SRS",(srid.second == TE_SRS_WGS84_UTM_ZONE_25S));
}

void TsSrs::tcSRSConvertion()
{
  std::auto_ptr<te::srs::Converter> converter(new te::srs::Converter());
  CPPUNIT_ASSERT_NO_THROW_MESSAGE("Fail to recognize TE_SRS_WGS84",converter->setSourceSRID(TE_SRS_WGS84));
  CPPUNIT_ASSERT_NO_THROW_MESSAGE("Fail to recognize TE_SRS_WGS84_UTM_ZONE_23S",converter->setTargetSRID(TE_SRS_WGS84_UTM_ZONE_23S));

  double llX = -45.0;
	double llY = 0.0;
	double xyX;
	double xyY;
  
  converter->convert(llX,llY,xyX,xyY);
  
  CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Conversion lat long to  X Y fails in X component.",500000,xyX,0.1);
  CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Conversion lat long to  X Y fails in Y component.",10000000,xyY,0.1);
  
  xyX = 500000;
	xyY = 10000000;
  
  converter->invert(xyX,xyY,llX,llY);
  
  CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Conversion X Y to lat long fails in X component.",-45.0,llX,0.001);
  CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("Conversion X Y to lat long fails in Y component.",0.0,llY,0.001);
}

void TsSrs::setUp()
{
}


void TsSrs::tearDown() 
{
}
