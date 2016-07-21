
//TerraLib
#include "../../src/terralib/ws/ogc/wms/client/XMLParser.h"
#include "../../src/terralib/common/TerraLib.h"
#include "../../src/terralib/qt/af/ApplicationController.h"

#include <QApplication>

int main(int argc, char *argv[])
{
  TerraLib::getInstance().initialize();

  QApplication app(argc, argv);

  te::qt::af::AppCtrlSingleton::getInstance().initialize();

  te::ws::ogc::wms::XMLParser parser;

  parser.parseCapabilities("/home/emerson/workspace/test_data/wmscapabilities.xml");

  return 0;
}
