/*  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

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

/*!
  \file terralib/unittest/ogc-wcs/main.cpp

  \brief Main file to test suit for the WS OGC WCS.

  \author Vinicius campanha
 */


// STL
#include <cstdlib>

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

// TerraLib
#include "../Config.h"
#include "TsWCS.h"
#include <terralib/common/TerraLib.h>
#include <terralib/plugin.h>
#include <terralib/common/PlatformUtils.h>
#include "LoadModules.h"


int main(int argc, char *argv[])
{
  TerraLib::getInstance().initialize();

  LoadModules();

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
    std::ofstream file1( TERRALIB_REPORT_DIR "/testsResult_ws_ogc_wcs_dos.txt" );
    CPPUNIT_NS::CompilerOutputter outputter1( &result, file1);
    outputter1.write();
    file1.close();

  // Printing testResults in XML file
  // Testing  different outputs
    // The testResult_*.xml files will be saved at TERRALIB_REPORT_DIR directory.
    // The styleSheet 'report.xsl' should be at this directory (found originally at <third-party-lib>\cppunit-1.12.1\contrib\xml-xsl).
    // The "data.zip" (downloaded) containing the data used in unit tests should be at TERRALIB_DATA_DIR

    CPPUNIT_NS::OFileStream file2( TERRALIB_REPORT_DIR "/testsResult_ws_ogc_wcs_xml.xml" );
    CPPUNIT_NS::XmlOutputter xml( &result, file2 );
    xml.setStyleSheet( "report.xsl" );
    xml.write();
    file2.close();

  // Print formated testResult in a txt
    CPPUNIT_NS::OFileStream file3( TERRALIB_REPORT_DIR "/testsResult_ws_ogc_wcs_formated.txt" );
    CPPUNIT_NS::TextOutputter outputter3( &result, file3 );
    outputter3.write();
    file3.close();

    bool resultStatus = result.wasSuccessful();

    TerraLib::getInstance().finalize();

    return resultStatus ? EXIT_SUCCESS : EXIT_FAILURE;
}
