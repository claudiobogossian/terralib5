// Raster Manager
#include "RasterManagerDialog.h"

// Terralib
#include <terralib/common.h>

// Qt
#include <QtGui/QApplication>

int main(int argc, char* argv[])
{

  TerraLib::getInstance().initialize();

  QApplication app(argc, argv);

  te::tools::rastermanager::RasterManagerDialog* p = new te::tools::rastermanager::RasterManagerDialog();
  p->show();

  app.exec();
  
  TerraLib::getInstance().finalize();
    
  return 0;
}
