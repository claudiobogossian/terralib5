#ifndef TS_SRS_H
#define TS_SRS_H

#include <cppunit/extensions/HelperMacros.h>

class TsSrs : public CPPUNIT_NS::TestFixture
{
  protected:

    void tcInitialization();

    void tcEPSGFactory();

  public :

    //! It sets the environment for a test case (actually a method).
	  void setUp();

    //! It finalizes the environment after a test case (actually a method) had run.
    void tearDown();

    CPPUNIT_TEST_SUITE( TsSrs );

    CPPUNIT_TEST( tcEPSGFactory );

	  CPPUNIT_TEST_SUITE_END();  	
};

#endif

