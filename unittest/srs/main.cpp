#include "../Config.h"
#include <terralib/common.h>

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

#include <stdlib.h>

int main(int /*argc*/, char** /*argv*/)
{
  TerraLib::getInstance().initialize();
  
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
  
// Testing  different outputs

// Print testResults in a txt file 
  std::ofstream file1( TERRALIB_REPORT_DIR "/testsResult_srs_dos.txt" );
  CPPUNIT_NS::CompilerOutputter outputter1( &result, file1);
  outputter1.write();
  file1.close();

// Printing testResult in XML file 
  // The testResult_*.xml files will be saved at TE_OUTPUT_REPORT_DIR directory.
  // The styleSheet 'report.xsl' should be at this directory (found originally at <third-party-lib>\cppunit-1.12.1\contrib\xml-xsl).
  // One level up TE_OUTPUT_REPORT_DIR should have a 'data' directory with all files used by unit test.
  
  CPPUNIT_NS::OFileStream file2( TERRALIB_REPORT_DIR "/testsResult_srs_xml.xml" );
  CPPUNIT_NS::XmlOutputter xml( &result, file2 );
  xml.setStyleSheet( "report.xsl" );
  xml.write();
  file2.close();

// Print testResult in a formated file 
  CPPUNIT_NS::OFileStream file3( TERRALIB_REPORT_DIR "/testsResult_srs_formated.txt" );
  CPPUNIT_NS::TextOutputter outputter3( &result, file3 );
  outputter3.write();
  file3.close();  

  bool resultStatus = result.wasSuccessful();
  
  TerraLib::getInstance().finalize();

  return resultStatus ? EXIT_SUCCESS : EXIT_FAILURE;
}

