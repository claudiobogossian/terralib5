#ifndef TS_SRS_H
#define TS_SRS_H

#include <cppunit/extensions/HelperMacros.h>

class TsSrs : public CPPUNIT_NS::TestFixture
{
  
  protected:

    void tcInitialization();

    void tcSRSManager();
  
    void tcSRSConvertion();

  public :

    //! It sets the environment for a test case (actually a method).
	  void setUp();

    //! It finalizes the environment after a test case (actually a method) had run.
    void tearDown();

    CPPUNIT_TEST_SUITE( TsSrs );

    CPPUNIT_TEST( tcSRSManager );
  
    CPPUNIT_TEST( tcSRSConvertion );

	  CPPUNIT_TEST_SUITE_END();  	
};

#endif

