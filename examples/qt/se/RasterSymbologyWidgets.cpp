#include "SymbologyWidgets.h"

// TerraLib
#include <terralib/qt/widgets/se/RasterSymbolizerDialog.h>

// Qt
#include <QtGui/QApplication>

void RasterSymbologyWidgets()
{
  int argc = 1;
  QApplication app(argc, 0);

  // Creates the rastersymbolizer dialog
  te::qt::widgets::RasterSymbolizerDialog dlg;

  dlg.exec();
}
