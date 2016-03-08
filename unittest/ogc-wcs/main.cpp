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
//  te::plugin::PluginInfo pinfo;

//  pinfo.m_name = "TERRALIB_XERCES";
//  pinfo.m_category = "XML";
//  pinfo.m_engine = TE_CPPPLUGINENGINE_CODE;
//  pinfo.m_resources.push_back(te::plugin::PluginInfo::Resource("SharedLibraryName", "terralib_mod_xerces"));

//  //te::plugin::PluginManager::getInstance().load(pinfo, true);
//  te::plugin::PluginManager::getInstance().loadAll();

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
