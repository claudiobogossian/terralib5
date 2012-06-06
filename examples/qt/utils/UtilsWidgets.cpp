#include "UtilsWidgets.h"

// TerraLib
#include <terralib/qt/widgets/utils/ColorPickerToolButton.h>
#include <terralib/qt/widgets/utils/DoubleListWidget.h>
#include <terralib/qt/widgets/utils/ListWidget.h>
#include <terralib/qt/widgets/utils/ParameterTableWidget.h>

// Qt
#include <QtGui/QApplication>
#include <QtGui/QDialog>
#include <QtGui/QFrame>
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

  dlW->setLeftLabel("Input Values");
  dlW->setRightLabel("Output Values");

  std::vector<std::string> inputValues;
  inputValues.push_back("TerraView");
  inputValues.push_back("TerraLib");
  inputValues.push_back("INPE");

  dlW->setInputValues(inputValues);

  //add line separator
  QFrame* line = new QFrame(&d);
  line->setFrameShape(QFrame::VLine);
  line->setFrameShadow(QFrame::Sunken);
  layout->addWidget(line, 0, 1);
  
  //add ListWidget
  te::qt::widgets::ListWidget* lW = new te::qt::widgets::ListWidget(&d);
  layout->addWidget(lW, 0, 2);

  lW->setLabel("Values");

  //add ListWidget
  te::qt::widgets::ParameterTableWidget* ptW = new te::qt::widgets::ParameterTableWidget(&d);
  layout->addWidget(ptW, 1, 0, 1, 3);

  //add ColorPickerToolButton
  te::qt::widgets::ColorPickerToolButton* cp = new te::qt::widgets::ColorPickerToolButton(&d);
  cp->setFixedSize(70, 24);
  layout->addWidget(cp, 2, 0, 1, 3);

  d.exec();
}
