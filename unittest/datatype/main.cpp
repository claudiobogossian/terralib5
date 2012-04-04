/*  Copyright (C) 2001-2009 National Institute For Space Research (INPE) - Brazil.

    This file is part of the TerraLib - a Framework for building GIS enabled applications.

    TerraLib is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License,
    or (at your option) any later version.

    TerraLib is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with TerraLib. See COPYING. If not, write to
    TerraLib Team at <terralib-team@terralib.org>.
 */

// TerraLib
#include <terralib/common.h>

// cppUnit
#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/XmlOutputter.h>
#include <cppunit/TextOutputter.h>

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>

// Unit-Test TerraLib includes by platform
#include "../Config.h"


int main(int /*argc*/, char** /*argv*/)
{

// initialize Terralib support
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

// Print only fail results in a txt file (the same you can see in DOS window)  
  std::ofstream file1( TE_DATA_UNITTEST_LOCALE "/testsResult_datatype_dos.txt" );
  CPPUNIT_NS::CompilerOutputter outputter1( &result, file1); 
  outputter1.write();
  file1.close();  

 // Printing testResults in XML file 
  // NOTE: copy StyleSheet "report.xsl" from the cppunit library to the same folder of testResult*.xlm 
  // and then you can open the testResults using your web-browser.   
  CPPUNIT_NS::OFileStream file2( TE_DATA_UNITTEST_LOCALE "/testsResult_datatype_xml.xml" );
  CPPUNIT_NS::XmlOutputter xml( &result, file2 );
  xml.setStyleSheet( "report.xsl" ); //it is found at c:\....\cppunit-1.12.1\contrib\xml-xsl
  xml.write();
  file2.close();

// Print formated testResult in a txt 
  CPPUNIT_NS::OFileStream file3( TE_DATA_UNITTEST_LOCALE "/testsResult_datatype_formated.txt" );
  CPPUNIT_NS::TextOutputter outputter3( &result, file3 );
  outputter3.write();
  file3.close();


  bool resultStatus = result.wasSuccessful();

  TerraLib::getInstance().finalize();

  return resultStatus ? EXIT_SUCCESS : EXIT_FAILURE;
}

