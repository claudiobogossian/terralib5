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
#include <terralib/dataaccess.h>
#include <terralib/plugin.h>

// STL
#include <cstdlib>

// Boost
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>

// cppUnit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>
#include <cppunit/TextOutputter.h>
#include <cppunit/XmlOutputter.h>

// Unit-Test TerraLib includes by platform
#include "Config.h"

#include "TsManagerDataSource.h"

#if TE_TEST_GDAL
#include <terralib/gdal/Platform.h>
#endif

#if TE_TEST_PGIS
#include <terralib/postgis/Platform.h>
#endif

//static te::da::DataSource* sm_ds;

int main(int /*argc*/, char** /*argv*/)
{

#if TE_TEST_PGIS
// initialize PostGIS platform
  te::pgis::Platform::initialize();
#endif

#if TE_TEST_GDAL
// initialize GDAL platform
  te::gdal::Platform::initialize();  
#endif

 // TsManagerDataSource::initialize();
  bool resultStatus;
  
  boost::property_tree::ptree drivers;

  boost::property_tree::read_json("../data/json_files/drivers.json", drivers);

  BOOST_FOREACH(const boost::property_tree::ptree::value_type& v, drivers.get_child("drivers").get_child(""))
  {

    const std::string& driver_name = v.second.data();

    TsManagerDataSource::initialize(driver_name);

    boost::property_tree::ptree driver;

    boost::property_tree::read_json(driver_name, driver);

    std::string m_dsType = driver.get_child("ds.ds_type").data();

// it creates the event manager and test controller
    CPPUNIT_NS::TestResult controller;

// it adds a listener that collects test result 
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
  // In a DEBUG version (Visual Net) testResults will be saved at Project directory (vcxproj)
  //      1) Look at Configuration Properties:  'Debug | Working Directory ' to see where it will be saved. 
  //         A post Build Event will copy the report.xsl to the folder defined by a macro $(ProjectDir)
  //      2) If you have 'data' to be read in your test, unzip de 'data.zip' file from the wiki-pages in the above dir.
  // In a Release Version...
  //      1) The TE_DATA_UNITTEST_LOCALE define the terralib folder where it will be saved
  //          and in this case the folder is  <terralib-working-dir>\bin32\msvc2010\ (the Output dir)
  //      2) If you have data to be read in your test, unzip the 'data.zip' file from the wiki-pages in the above path.
  //      3) Run your test from a command line <terrraliv-working-dir>\bin32\msvc2010\terralib_unittest_<module>_d.exe
  // NOTE: the Style Sheet "report.xsl" should be at the same folder of *.xml in order to open/see the .xml results in your web-browser
  //
  // If you want to save the testResults at the same folder in both Debug and Release version do:
  //      1) Change at Configuration Properties:  'Debug | Working Directory '  from $(ProjectDir) to $(OutDir)   
    
// Print only fail results in a txt file (the same containt you see in DOS window)
  //std::string driverTeste = TE_DATA_UNITTEST_LOCALE  + '/';
  //std::string testResultDriver = "/testResult_" + m_dsType + ".xml";
    std::string testResultDriver = "/testResult_" + m_dsType ;

    std::ofstream file1(TE_DATA_UNITTEST_LOCALE+ testResultDriver + "_dos.txt" );
    CPPUNIT_NS::CompilerOutputter outputter1( &result, file1);
    outputter1.write();
    file1.close();

// Printing testResults in XML file 
//  CPPUNIT_NS::OFileStream file2(TE_DATA_UNITTEST_LOCALE"/testsResult_dataaccess_xml.xml");
    CPPUNIT_NS::OFileStream file2(TE_DATA_UNITTEST_LOCALE + testResultDriver + ".xml");
    CPPUNIT_NS::XmlOutputter xml( &result, file2 );
    xml.setStyleSheet( TE_DATA_LOCALE"/data/report.xsl" ); //it is found at <third-party-lib>\cppunit-1.12.1\contrib\xml-xsl
    xml.write();
    file2.close();

// Print formated testResult in a txt 
    //CPPUNIT_NS::OFileStream file3(TE_DATA_UNITTEST_LOCALE"/testsResult_dataaccess_formated.txt" );
    CPPUNIT_NS::OFileStream file3(TE_DATA_UNITTEST_LOCALE + testResultDriver + ".txt" );
     CPPUNIT_NS::TextOutputter outputter3( &result, file3 );
    outputter3.write();
    file3.close();

    //bool resultStatus = result.wasSuccessful();
    resultStatus = result.wasSuccessful();

    TsManagerDataSource::finalize();
  }

// finalize defined drivers test suit

#if TE_TEST_GDAL
// finalize TerraLib Plataform
  te::gdal::Platform::finalize(); 
#endif

#if TE_TEST_PGIS
  // finalize TerraLib Plataform
  te::pgis::Platform::finalize();  
#endif

  return resultStatus ? EXIT_SUCCESS : EXIT_FAILURE;
}

