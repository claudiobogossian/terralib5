#include "../Config.h"

// cppUnit include files
#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>
#include <cppunit/XmlOutputter.h>
#include <cppunit/TextOutputter.h>

#include <terralib/srs/Platform.h>
#include <terralib/proj4/Platform.h>

// Unit-Test TerraLib includes by platform
#include "../Config.h"

#include <cstdlib>

int main(int /*argc*/, char** /*argv*/)
{
  te::srs::Platform::initialize();
  te::proj4::Platform::initialize();

// it creates the event manager and test controller
  CPPUNIT_NS::TestResult controller;

// it adds a listener that colllects test result 
  CPPUNIT_NS::TestResultCollector result;

  controller.addListener(&result);

// it adds a listener that print dots as test run.
  CPPUNIT_NS::BriefTestProgressListener progress;

  controller.addListener(&progress);

// it adds the top suite to the test runner
  CppUnit::Test* suite = CppUnit::TestFactoryRegistry::getRegistry().makeTest();

  CPPUNIT_NS::TestRunner runner;

  runner.addTest(suite);

  runner.run(controller);

  CPPUNIT_NS::CompilerOutputter outputter( &result, CPPUNIT_NS::stdCOut() );

  outputter.write();

  te::proj4::Platform::finalize(); 
  te::srs::Platform::finalize();
  
// Testing  different outputs

// Print only fail results in a txt file (the same you can see in DOS window)  
  std::ofstream file1( TE_DATA_UNITTEST_LOCALE "/testsResult_proj4_dos.txt" );
  CPPUNIT_NS::CompilerOutputter outputter1( &result, file1);
  outputter1.write();
  file1.close();

// Printing testResult in XML file
  // NOTE: copy StyleSheet "report.xsl" from the cppunit library to the same dir of testResult
  // in order to be able use a web-browser to see the testResult.   
  CPPUNIT_NS::OFileStream file2( TE_DATA_UNITTEST_LOCALE "/testsResult_proj4_xml.xml" );
  CPPUNIT_NS::XmlOutputter xml( &result, file2 );
  xml.setStyleSheet( "report.xsl" ); //it is found at c:\....\cppunit-1.12.1\contrib\xml-xsl
  xml.write();
  file2.close();

// Print formated testResult in a txt file
  CPPUNIT_NS::OFileStream file3( TE_DATA_UNITTEST_LOCALE "/testsResult_proj4_formated.txt" );
  CPPUNIT_NS::TextOutputter outputter3( &result, file3 );
  outputter3.write();
  file3.close();  

  bool resultStatus = result.wasSuccessful();

  return resultStatus ? EXIT_SUCCESS : EXIT_FAILURE;
}

