
//TerraLib
#include "../../src/terralib/ws/ogc/wms/client/XMLParser.h"
#include "../../src/terralib/common/TerraLib.h"
#include "../../src/terralib/qt/af/ApplicationController.h"
#include "../../src/terralib/ws/ogc/wms/client/WMSClient.h"

#include <QApplication>

int main(int argc, char *argv[])
{
  TerraLib::getInstance().initialize();

  QApplication app(argc, argv);

  te::qt::af::AppCtrlSingleton::getInstance().initialize();

  std::string usrDataDir = te::qt::af::AppCtrlSingleton::getInstance().getUserDataDir().toUtf8().data();

  std::string url = "http://mapas.mma.gov.br/i3geo/ogc.php?tema=transporte";

  te::ws::ogc::WMSClient client (usrDataDir, url);

  client.updateCapabilities();

  te::ws::ogc::wms::WMSCapabilities capabilities = client.getCapabilities();

  return 0;
}
