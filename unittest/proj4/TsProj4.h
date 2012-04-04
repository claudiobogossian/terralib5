#ifndef TS_PROJ4_H
#define TS_PROJ4_H

#include <cppunit/extensions/HelperMacros.h>

class TsProj4 : public CPPUNIT_NS::TestFixture
{
  protected:


    //! Tests access to the ConverterManager to build a Converter.
    void tcGetConverter();

    //! Tests the conversion of one coordinate.
    void tcConvertCoordinate();

    void tcCoordinateSystemBuilding();

	  //! Test the conversion of a vector of coordinates.
    void tcConvertVectorCoordinates();

  public :

    //! It sets the environment for a test case (actually a method).
	  void setUp();

    //! It finalizes the environment after a test case (actually a method) had run.
    void tearDown();

    CPPUNIT_TEST_SUITE( TsProj4 );

    CPPUNIT_TEST( tcGetConverter );
    CPPUNIT_TEST( tcConvertCoordinate );
    CPPUNIT_TEST( tcConvertVectorCoordinates );

	  CPPUNIT_TEST_SUITE_END();  	
};

#endif  // TS_PROJ4_H

