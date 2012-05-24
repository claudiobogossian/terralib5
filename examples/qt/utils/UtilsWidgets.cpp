#include "UtilsWidgets.h"

// TerraLib
#include <terralib/se.h>
#include <terralib/qt/widgets/utils/DoubleListWidget.h>
#include <terralib/qt/widgets/utils/ListWidget.h>
#include <terralib/qt/widgets/utils/ParameterTableWidget.h>

// Qt
#include <QtGui/QApplication>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>

void UtilsWidgets()
{
  int argc = 1;
  
  //crate application
  QApplication app(argc, 0);
  QString title("Testing Utils Widgets");
  

  //create dummy dialog
  QDialog d(NULL);
  d.setWindowTitle(title);
  
  QGridLayout* layout = new QGridLayout(&d);

  //add DoubleListWidget
  te::qt::widgets::DoubleListWidget* dlW = new te::qt::widgets::DoubleListWidget(&d);
  layout->addWidget(dlW, 0, 0);

  //add ListWidget
  te::qt::widgets::ListWidget* lW = new te::qt::widgets::ListWidget(&d);
  layout->addWidget(lW, 0, 1);

  //add ListWidget
  te::qt::widgets::ParameterTableWidget* ptW = new te::qt::widgets::ParameterTableWidget(&d);
  layout->addWidget(ptW, 1, 0, 1, 2);

  d.exec();
}
