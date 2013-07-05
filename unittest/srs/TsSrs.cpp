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
  bool resultBool;
  std::string resultString;
  std::pair<std::string,unsigned int> resultPair;
  
  // --- Test the maps and finds 
  std::string gp4txt = "proj=merc +a=6378137 +b=6378137 +lat_ts=0.0 +lon_0=0.0 +x_0=0.0 +y_0=0 +k=1.0 +units=m";
  std::string gwkt = "PROJCS[\"unnamed\",GEOGCS[\"unnamed ellipse\",DATUM[\"unknown\",SPHEROID[\"unnamed\",6378137,0]],PRIMEM[\"Greenwich\",0],UNIT[\"degree\",0.0174532925199433]],PROJECTION[\"Mercator_2SP\"],PARAMETER[\"standard_parallel_1\",0],PARAMETER[\"central_meridian\",0],PARAMETER[\"false_easting\",0],PARAMETER[\"false_northing\",0],UNIT[\"Meter\",1]";
  CPPUNIT_ASSERT_NO_THROW(te::srs::SpatialReferenceSystemManager::getInstance().add("google",gp4txt,gwkt,96,"sr-org"));
  
  resultBool = te::srs::SpatialReferenceSystemManager::getInstance().recognizes(96,"sr-org");
  CPPUNIT_ASSERT(resultBool);
  
  resultString = te::srs::SpatialReferenceSystemManager::getInstance().getName(96,"sr-org");
  CPPUNIT_ASSERT(!resultString.empty());
  
  resultString = te::srs::SpatialReferenceSystemManager::getInstance().getP4Txt(96,"sr-org");
  CPPUNIT_ASSERT(resultString == gp4txt);
  
  resultString = te::srs::SpatialReferenceSystemManager::getInstance().getWkt(96,"sr-org");
  CPPUNIT_ASSERT(resultString == gwkt); 
  
  CPPUNIT_ASSERT_NO_THROW(resultPair = te::srs::SpatialReferenceSystemManager::getInstance().getIdFromWkt(gwkt));
  CPPUNIT_ASSERT(resultPair.second == 96 && resultPair.first == "sr-org");
  
  CPPUNIT_ASSERT_NO_THROW(resultPair = te::srs::SpatialReferenceSystemManager::getInstance().getIdFromP4Txt(gp4txt));
  CPPUNIT_ASSERT(resultPair.second == 96 && resultPair.first == "sr-org");
  
  te::srs::SpatialReferenceSystemManager::getInstance().remove(96,"sr-org");
  CPPUNIT_ASSERT_THROW(resultPair = te::srs::SpatialReferenceSystemManager::getInstance().getIdFromWkt(gwkt),te::srs::Exception);
  
  resultBool = te::srs::SpatialReferenceSystemManager::getInstance().recognizes(96,"sr-org");
  CPPUNIT_ASSERT(resultBool == false);
  
  // --- Test access to data inserted by initialization from files
  
  resultString = te::srs::SpatialReferenceSystemManager::getInstance().getName(TE_SRS_SAD69);
  CPPUNIT_ASSERT(!resultString.empty());
  
  resultPair = te::srs::SpatialReferenceSystemManager::getInstance().getIdFromName(resultString);
  CPPUNIT_ASSERT(resultPair.second == TE_SRS_SAD69);
  
  resultString = te::srs::SpatialReferenceSystemManager::getInstance().getName(TE_SRS_WGS84_UTM_ZONE_25S);
  CPPUNIT_ASSERT(!resultString.empty());
  
  resultPair = te::srs::SpatialReferenceSystemManager::getInstance().getIdFromName(resultString);
  CPPUNIT_ASSERT(resultPair.second == TE_SRS_WGS84_UTM_ZONE_25S);
  
  te::srs::SpatialReferenceSystemPtr resultSRS; // = 0;
  CPPUNIT_ASSERT_NO_THROW (resultSRS = te::srs::SpatialReferenceSystemManager::getInstance().getSpatialReferenceSystem(TE_SRS_SAD69));
  CPPUNIT_ASSERT(resultSRS.get());
  
  // --- Test the iterator access
  size_t aux1=0;
  std::pair<te::srs::SpatialReferenceSystemManager::iterator,
            te::srs::SpatialReferenceSystemManager::iterator> its = te::srs::SpatialReferenceSystemManager::getInstance().getIterators();
  while(its.first != its.second)
  {
    ++aux1;
    ++its.first;
  }
  CPPUNIT_ASSERT(aux1 = te::srs::SpatialReferenceSystemManager::getInstance().size());  
}

void TsSrs::tcSRSConvertion()
{
  std::auto_ptr<te::srs::Converter> converter(new te::srs::Converter());
//  te::srs::Converter* converter = new te::srs::Converter();

  CPPUNIT_ASSERT_NO_THROW(converter->setSourceSRID(TE_SRS_WGS84));
  CPPUNIT_ASSERT_NO_THROW(converter->setTargetSRID(TE_SRS_WGS84_UTM_ZONE_23S));

  double xin = -45.0;
	double yin = 0.0;
	double xout;
	double yout;
  
  converter->convert(xin,yin,xout,yout);
  
  CPPUNIT_ASSERT_DOUBLES_EQUAL(500000,xout,0.1);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(10000000,yout,0.1);
  
  xin = 500000;
	yin = 10000000;
  
  converter->invert(xin,yin,xout,yout);
  
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-45.0,xout,0.001);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,yout,0.001);
  
  xin = -45.0;
	yin = 0.0;
  
  converter->convert(xin,yin);
  
  CPPUNIT_ASSERT_DOUBLES_EQUAL(500000,xin,0.1);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(10000000,yin,0.1); 
  
  xin = 500000;
	yin = 10000000;
  
  converter->convertToGeographic(xin, yin, TE_SRS_WGS84_UTM_ZONE_23S);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-45.0,xin,0.001);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,yin,0.001);
  
  xin = -45.0;
	yin = 0.0;
  
  converter->convertToProjected(xin, yin, TE_SRS_WGS84_UTM_ZONE_23S);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(500000,xin,0.1);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(10000000,yin,0.1);
  
  std::vector<double> vxin;
  std::vector<double> vyin;
  
  vxin.push_back(-45.0); vxin.push_back(-45.0); vxin.push_back(-45.0);
  vyin.push_back(0); vyin.push_back(0); vyin.push_back(0);
  
  std::vector<double> vxout; vxout.resize(3);
  std::vector<double> vyout; vyout.resize(3);

  converter->convert(&vxin[0],&vyin[0],&vxout[0],&vyout[0],3);
 
  CPPUNIT_ASSERT_DOUBLES_EQUAL(500000,vxout[0],0.1);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(10000000,vyout[0],0.1);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(500000,vxout[1],0.1);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(10000000,vyout[1],0.1);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(500000,vxout[2],0.1);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(10000000,vyout[2],0.1);
  
  converter->convert(&vxin[0],&vyin[0],3);
  
  CPPUNIT_ASSERT_DOUBLES_EQUAL(500000,vxin[0],0.1);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(10000000,vyin[0],0.1);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(500000,vxin[1],0.1);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(10000000,vyin[1],0.1);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(500000,vxin[2],0.1);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(10000000,vyin[2],0.1);
  
  vxin.clear();
  vyin.clear();
  vxout.clear();
  vyout.clear();
}

void TsSrs::tcSRSClear()
{
  CPPUNIT_ASSERT_NO_THROW(te::srs::SpatialReferenceSystemManager::getInstance().clear());
}

void TsSrs::setUp()
{
}


void TsSrs::tearDown() 
{
}
