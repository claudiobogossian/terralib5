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
#include "LoadModules.h"
#include "Config.h"

#include "TsManagerDataSource.h"

int main(int /*argc*/, char** /*argv*/)
{
  TerraLib::getInstance().initialize();    
  LoadModules();

  std::string report_dir = TERRALIB_REPORT_DIR;
  bool resultStatus;
 
  boost::property_tree::ptree drivers;

  boost::property_tree::read_json("../data/json_files/drivers.json", drivers);

  BOOST_FOREACH(const boost::property_tree::ptree::value_type& v, drivers.get_child("drivers").get_child(""))
  {
    try
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
  // The testResult_*.xml files will be saved at TERRALIB_REPORT_DIR directory.
  // The styleSheet 'report.xsl' should be at this directory (found originally at <third-party-lib>\cppunit-1.12.1\contrib\xml-xsl).
  // The "data.zip" (downloaded) containing the data used in unit tests should be at TERRALIB_DATA_DIR 

// Note on how to run the unittests: 
  // Debug - terralib_unittest_dataaccess_d.exe- Start up terralib_unittest_dataaccess
  // Release - terralib_unittest_dataaccess.exe

// Print only fail results in a txt file (the same containt you see in DOS window)
    std::string testResultDriver = TERRALIB_REPORT_DIR"/testResult_" + m_dsType ;
    std::ofstream file1( (testResultDriver + "_dos.txt" ).c_str() );

	CPPUNIT_NS::CompilerOutputter outputter1( &result, file1);
    outputter1.write();
    file1.close();

// Printing testResults in XML file 
    CPPUNIT_NS::OFileStream file2( (testResultDriver + ".xml").c_str() );
    CPPUNIT_NS::XmlOutputter xml( &result, file2 );
    xml.setStyleSheet( "report.xsl" );

    xml.write();
    file2.close();

// Print formated testResult in a txt 
    CPPUNIT_NS::OFileStream file3( (testResultDriver + ".txt" ).c_str() );
    CPPUNIT_NS::TextOutputter outputter3( &result, file3 );
    outputter3.write();
    file3.close();

    resultStatus = result.wasSuccessful();
    }
    catch (te::common::Exception  e)
    { 
      throw e;
    }

    TsManagerDataSource::finalize();
  }

// finalize TerraLib Plataform
  te::plugin::PluginManager::getInstance().unloadAll();
  TerraLib::getInstance().finalize();

  return resultStatus ? EXIT_SUCCESS : EXIT_FAILURE;
}
