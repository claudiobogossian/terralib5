// STL
#include <cstdlib>
#include <string>
#include <iostream>

// TerraLib
#include "../../src/terralib/ws/ogc/wcs/client/WCSClient.h"
#include "../../src/terralib/qt/af/ApplicationController.h"
#include "../../src/terralib/common/TerraLib.h"

//QT
#include <QApplication>

int main(int argc, char *argv[])
{

  TerraLib::getInstance().initialize();

  QApplication app(argc, argv);

  te::qt::af::AppCtrlSingleton::getInstance().initialize();

  // Directory where temporary WCS data will be stored.
  std::string usrDataDir = te::qt::af::AppCtrlSingleton::getInstance().getUserDataDir().toStdString();

  // WCS server URL.
  std::string url = "http://demo.opengeo.org/geoserver/ows";

  // Version that will be used on WCS Requests.
  std::string version = "2.0.1";

  {
    std::cout << "GetCapabilities Example." << std::endl;

    te::ws::ogc::WCSClient client (usrDataDir, url, version);

    // Makes GetCapabilities Request.
    client.updateCapabilities();

    te::ws::ogc::wcs::Capabilities capabilities = client.getCapabilities();

    std::vector<std::string> coverages = capabilities.coverages;

    for (unsigned int i = 0; i < coverages.size(); ++i)
    {
      std::cout << "Coverage Name: " << coverages[i] << std::endl;
    }
  }

  {
    std::cout << "DescribeCoverage Example." << std::endl;

    te::ws::ogc::WCSClient client (usrDataDir, url, version);

    // Coverage name that could be retrieved on GetCapabilities.
    std::string coverageName = "nasa__bluemarble";

    te::ws::ogc::wcs::CoverageDescription description = client.describeCoverage(coverageName);

    std::cout << "CoverageId: " << description.coverageId << std::endl;

    te::ws::ogc::wcs::EnvelopeWithTimePeriod boundedBy = description.envelope;

    std::cout << "--- boundedBy attributes ---" << std::endl;
    std::cout << "Lower Corner X: " << boundedBy.lowerCorner_X << std::endl;
    std::cout << "Lower Corner Y: " << boundedBy.lowerCorner_Y << std::endl;
    std::cout << "Upper Corner X: " << boundedBy.upperCorner_X << std::endl;
    std::cout << "Upper Corner Y: " << boundedBy.upperCorner_Y << std::endl;

    std::cout << "SRS Name: " << boundedBy.srsName << std::endl;
    std::cout << "SRS Dimension: " << boundedBy.srsDimension << std::endl;
    std::cout << "--- boundedBy attributes ---" << std::endl;

    std::vector<std::string> rangeTypes = description.fieldNames;


    std::cout << "--- rangeTypes ---" << std::endl;
    for (unsigned int i = 0; i < rangeTypes.size(); ++i)
    {
      std::cout << "Field Name: " << rangeTypes[i] << std::endl;
    }
    std::cout << "--- rangeTypes ---" << std::endl;
  }

  {
    std::cout << "GetCoverage Example." << std::endl;

    te::ws::ogc::WCSClient client (usrDataDir, url, version);

    // Coverage ID that could be retrieved on DescribeCoverage.
    std::string coverageId = "nasa__bluemarble";

    // Simple image request by id.
    te::ws::ogc::wcs::CoverageRequest request;
    request.coverageID = coverageId;

    std::string coverageDiskPath = client.getCoverage(request);

    std::cout << "Path to Requested Image: " << coverageDiskPath << std::endl;
  }

  TerraLib::getInstance().finalize();

  return EXIT_SUCCESS;
}
