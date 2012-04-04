#include "TsProj4.h"

#include <terralib/srs.h>
#include <terralib/proj4.h>

#include <string>

CPPUNIT_TEST_SUITE_REGISTRATION( TsProj4 );

void TsProj4::setUp()
{}


void TsProj4::tearDown() 
{}

void TsProj4::tcGetConverter()
{
  te::srs::Converter* conv = te::srs::ConverterManager::getInstance().getConverter();
	CPPUNIT_ASSERT_MESSAGE("Couldn't obtain a valid converter from the SRS support",conv);

	CPPUNIT_ASSERT_NO_THROW(conv->setSourceSRS(4326));       // Geographic Coordinate System based on WGS84. 
  CPPUNIT_ASSERT_EQUAL(4326,conv->getSourceSRS());

	CPPUNIT_ASSERT_NO_THROW(conv->setTargetSRS(32723));      // Projected Coordinate System, WGS84 UTM Zone 23S. 
  CPPUNIT_ASSERT_EQUAL(32723,conv->getTargetSRS());

  delete conv;
}

void TsProj4::tcConvertCoordinate()
{
  te::srs::Converter* conv = te::srs::ConverterManager::getInstance().getConverter();
	CPPUNIT_ASSERT_MESSAGE("Couldn't obtain a valid converter from the SRS support",conv);

	CPPUNIT_ASSERT_NO_THROW(conv->setSourceSRS(4326));       // Geographic Coordinate System based on WGS84. 
	CPPUNIT_ASSERT_NO_THROW(conv->setTargetSRS(32723));      // Projected Coordinate System, WGS84 UTM Zone 23S. 

	double llX = -45.5;
	double llY = -23.0;

	double xyX;
	double xyY;

	conv->convert(llX,llY,xyX,xyY);
	CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("LL/WGS84 to XY UTM/WGS84 is not working properly (X)",448758.781653,xyX,0.01);
	CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("LL/WGS84 to XY UTM/WGS84 is not working properly (Y)",7456392.87602,xyY,0.01);
		
	conv->invert(xyX,xyY,llX,llY);
	CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("XY/WGS84 to UTM/WGS84 to LL is not working properly (LONG)",-45.5,llX,0.0001);
	CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE("XY/WGS84 to UTM/WGS84 to LL is not working properly (LAT)", -23.0,llY,0.0001);
}

void TsProj4::tcConvertVectorCoordinates()
{
	double* xs = new double[3];
	double* ys = new double[3];

 	xs[0] = -45.5;
	xs[1] = -45.6;
	xs[2] = -45.7;

	ys[0] = -23.0;
	ys[1] = -23.1;
	ys[2] = -23.2;

  te::srs::Converter* conv = te::srs::ConverterManager::getInstance().getConverter();
	CPPUNIT_ASSERT_MESSAGE("Couldn't obtain a valid converter from the SRS support",conv);

	CPPUNIT_ASSERT_NO_THROW(conv->setSourceSRS(4326));       // Geographic Coordinate System based on WGS84. 
	CPPUNIT_ASSERT_NO_THROW(conv->setTargetSRS(32723));      // Projected Coordinate System, WGS84 UTM Zone 23S. 

	conv->convert(xs,ys,3,1);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(448758.781,xs[0],0.001);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(438555.689,xs[1],0.001);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(428367.818,xs[2],0.001);

	CPPUNIT_ASSERT_DOUBLES_EQUAL(7456392.874,ys[0],0.001);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(7445283.968,ys[1],0.001);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(7434167.628,ys[2],0.001);

	delete xs;
	delete ys;

	double* xy = new double[6];
	xy[0] = -45.5;
	xy[2] = -45.6;
	xy[4] = -45.7;

	xy[1] = -23.0;
	xy[3] = -23.1;
	xy[5] = -23.2;

	conv->convert(xy,&xy[1],3,2);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(448758.781,xy[0],0.001);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(438555.689,xy[2],0.001);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(428367.818,xy[4],0.001);

	CPPUNIT_ASSERT_DOUBLES_EQUAL(7456392.874,xy[1],0.001);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(7445283.968,xy[3],0.001);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(7434167.628,xy[5],0.001);
	
  delete xy;
	
  delete conv;
}
