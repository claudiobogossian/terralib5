// DSCopy
#include "DSCopyDialog.h"

// Terralib
#include <terralib/common.h>

// Qt
#include <QtGui/QApplication>

int main(int argc, char* argv[])
{

  TerraLib::getInstance().initialize();

  QApplication app(argc, argv);

  te::tools::dscopy::DSCopyDialog* p = new te::tools::dscopy::DSCopyDialog();
  p->show();

  app.exec();
  
  TerraLib::getInstance().finalize();
    
  return 0;
}
