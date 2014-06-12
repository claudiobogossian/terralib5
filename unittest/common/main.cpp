/*  Copyright (C) 2001-2009 National Institute For Space Research (INPE) - Brazil.

    This file is part of the TerraLib - a Framework for building GIS enabled applications.

    TerraLib is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License,
    or (at your option) any later version.

    TerraLib is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with TerraLib. See COPYING. If not, write to
    TerraLib Team at <terralib-team@terralib.org>.
 */

// TerraLib
#include <terralib/common.h>

// Unit-Test TerraLib includes by platform
#include "../Config.h"

// cppUnit
#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/XmlOutputter.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>

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

  // The testResult_*.xml files will be saved at TE_OUTPUT_REPORT_DIR directory.
  // The styleSheet 'report.xsl' should be at this directory (found originally at <third-party-lib>\cppunit-1.12.1\contrib\xml-xsl).
  // One level up TE_OUTPUT_REPORT_DIR should have a 'data' directory with all files used by unit test.
  
  //CPPUNIT_NS::OFileStream file2( TE_OUTPUT_REPORT_DIR"/testResults_common_xml.xml" );
  CPPUNIT_NS::OFileStream file2(TERRALIB_UNITTEST_REPORT_DIR"/testResults_common_xml.xml");
  CPPUNIT_NS::XmlOutputter xml(&result, file2);
  xml.setStyleSheet("report.xsl");
  xml.write();
  file2.close();

  bool resultStatus = result.wasSuccessful();

  TerraLib::getInstance().finalize();

  std::cout << "\nPress Enter to exit..." << std::endl;
  std::cin.get();

  return resultStatus ? EXIT_SUCCESS : EXIT_FAILURE;
}