#include "SymbologyWidgets.h"

// TerraLib
#include <terralib/se.h>
#include <terralib/qt/widgets/se/BasicStrokeDialog.h>
#include <terralib/qt/widgets/se/MarkDialog.h>

// Qt
#include <QtGui/QApplication>

void SymbologyWidgets()
{
  int argc = 1;
  QApplication app(argc, 0);

  QString title("Testing Symbology Widgets");

  // Creates a new stroke
  te::se::Stroke* stroke = te::qt::widgets::BasicStrokeDialog::getStroke(0, 0, title);
  delete stroke;

  // Creates a new stroke based on pre-existed
  te::se::Stroke* initialStroke = te::se::CreateStroke("#FF0000", "2", "0.5", "5 5", "round", "round");
  stroke = te::qt::widgets::BasicStrokeDialog::getStroke(initialStroke, 0, title);
  delete stroke;
  delete initialStroke;

  // Creates a new mark
  te::se::Mark* mark = te::qt::widgets::MarkDialog::getMark(0, 0, title);
  delete mark;

  // Creates a new mark based on pre-existed
  te::se::Stroke* markStroke = te::se::CreateStroke("#000000", "3", "0.5");
  te::se::Fill* markFill = te::se::CreateFill("#FFFF00", "1.0");
  te::se::Mark* initialMark = te::se::CreateMark("star", markStroke, markFill);
  mark = te::qt::widgets::MarkDialog::getMark(initialMark, 0, title);
  delete mark;
  delete initialMark;
}
