#include "SymbologyWidgets.h"

// TerraLib
#include <terralib/common.h>
#include <terralib/se.h>
#include <terralib/qt/widgets/se/BasicFillDialog.h>
#include <terralib/qt/widgets/se/BasicStrokeDialog.h>
#include <terralib/qt/widgets/se/GlyphMarkDialog.h>
#include <terralib/qt/widgets/se/GraphicDialog.h>
#include <terralib/qt/widgets/se/LineSymbolizerWidget.h>
#include <terralib/qt/widgets/se/PointSymbolizerWidget.h>
#include <terralib/qt/widgets/se/PolygonSymbolizerWidget.h>
#include <terralib/qt/widgets/se/SymbolEditorWidget.h>
#include <terralib/qt/widgets/se/SymbolizerPreviewWidget.h>
#include <terralib/qt/widgets/se/SymbolizerTableWidget.h>
#include <terralib/qt/widgets/se/WellKnownMarkDialog.h>

// Qt
#include <QtGui/QApplication>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QMdiArea>
#include <QtGui/QMdiSubWindow>
#include <QtGui/QToolBox>

void SymbolEditor()
{
  // Widgets for symbol edition...
  te::qt::widgets::SymbolEditorWidget* editPointSymbolizer = new te::qt::widgets::SymbolEditorWidget(te::se::POINT_SYMBOLIZER);
  te::qt::widgets::SymbolEditorWidget* editLineSymbolizer = new te::qt::widgets::SymbolEditorWidget(te::se::LINE_SYMBOLIZER);
  te::qt::widgets::SymbolEditorWidget* editPolygonSymbolizer = new te::qt::widgets::SymbolEditorWidget(te::se::POLYGON_SYMBOLIZER);

  // Grouping to show!
  QMdiArea mainWidget;
  mainWidget.setWindowTitle("Symbol Editors");
  mainWidget.addSubWindow(editPointSymbolizer)->setWindowTitle("Point Symbol");
  mainWidget.addSubWindow(editLineSymbolizer)->setWindowTitle("Line Symbol");
  mainWidget.addSubWindow(editPolygonSymbolizer)->setWindowTitle("Polygon Symbol");
  mainWidget.show();

  qApp->exec();
}

void SymbolizerWidgets()
{
  QDialog dlg;
  dlg.setWindowTitle("Symbolizer Widgets Example");

  // Polygon Symbolizer Widget
  te::qt::widgets::PolygonSymbolizerWidget* ps = new te::qt::widgets::PolygonSymbolizerWidget(&dlg);

  // Line Symbolizer Widget
  te::qt::widgets::LineSymbolizerWidget* ls = new te::qt::widgets::LineSymbolizerWidget(&dlg);

  // Point Symbolizer Widget
  te::qt::widgets::PointSymbolizerWidget* pts = new te::qt::widgets::PointSymbolizerWidget(&dlg);

  // Grouping...
  QToolBox* box = new QToolBox(&dlg);
  box->addItem(ps, "&Polygon Symbolyzer");
  box->addItem(ls, "&Line Symbolyzer");
  box->addItem(pts, "P&oint Symbolyzer");

  // Adjusting...
  QGridLayout* layout = new QGridLayout(&dlg);
  layout->setSizeConstraint(QLayout::SetFixedSize);
  layout->addWidget(box);

  dlg.exec();
}

void PreviewWidgets()
{
  std::vector<te::se::Symbolizer*> symbs;
  symbs.push_back(te::se::CreateLineSymbolizer(te::se::CreateStroke("#000000", "9.0")));
  symbs.push_back(te::se::CreateLineSymbolizer(te::se::CreateStroke("#808080", "5.0")));
  symbs.push_back(te::se::CreateLineSymbolizer(te::se::CreateStroke("#FFFF00", "2.0", "1.0", "2 2")));

  QDialog dlg;
  dlg.setWindowTitle("Symbolizer Preview Example");

  // Preview
  te::qt::widgets::SymbolizerPreviewWidget* preview = new te::qt::widgets::SymbolizerPreviewWidget(QSize(150, 150), te::se::LINE_SYMBOLIZER, &dlg);
  preview->updatePreview(symbs);

  // Table preview
  te::qt::widgets::SymbolizerTableWidget* table = new te::qt::widgets::SymbolizerTableWidget(QSize(150, 32), te::se::LINE_SYMBOLIZER, &dlg);
  table->updatePreview(symbs);
  table->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);

  // Adjusting...
  QGridLayout* layout = new QGridLayout(&dlg);
  layout->setSizeConstraint(QLayout::SetFixedSize);
  layout->addWidget(preview, 0, 0);
  layout->addWidget(table, 1, 0);

  dlg.exec();

  te::common::FreeContents(symbs);
}

void BasicWidgets()
{
  QString title("Testing Basic Symbology Widgets");

  // Creates a new stroke
  te::se::Stroke* stroke = te::qt::widgets::BasicStrokeDialog::getStroke(0, 0, title);
  delete stroke;

  // Creates a new stroke based on pre-existed
  te::se::Stroke* initialStroke = te::se::CreateStroke("#FF0000", "2", "0.5", "5 5", "round", "round");
  stroke = te::qt::widgets::BasicStrokeDialog::getStroke(initialStroke, 0, title);
  delete stroke;
  delete initialStroke;

  // Create a new Fill
  te::se::Fill* fill = te::qt::widgets::BasicFillDialog::getFill(0, 0, title);
  delete fill;

  // Creates a new fill based on pre-existed
  te::se::Fill* initialFill = te::se::CreateFill("#00FF00", "0.5");
  fill = te::qt::widgets::BasicFillDialog::getFill(initialFill, 0, title);
  delete fill;
  delete initialFill;

  // Creates a new well known mark
  te::se::Mark* mark = te::qt::widgets::WellKnownMarkDialog::getMark(0, 0, title);
  delete mark;

  // Creates a new mark based on pre-existed
  te::se::Stroke* markStroke = te::se::CreateStroke("#000000", "3", "0.5");
  te::se::Fill* markFill = te::se::CreateFill("#FFFF00", "1.0");
  te::se::Mark* initialMark = te::se::CreateMark("star", markStroke, markFill);
  mark = te::qt::widgets::WellKnownMarkDialog::getMark(initialMark, 0, title);
  delete mark;
  delete initialMark;

  // Create a new glyph mark
  te::se::Mark* glyphMark = te::qt::widgets::GlyphMarkDialog::getMark(0, 0, title);
  delete glyphMark;

  // Creates a new graphic
  te::se::Graphic* graphic = te::qt::widgets::GraphicDialog::getGraphic(0, 0, "");
  delete graphic;
 }

 void SymbologyWidgets()
{
  int argc = 1;
  QApplication app(argc, 0);

  SymbolEditor();

  SymbolizerWidgets();

  PreviewWidgets();

  BasicWidgets();
}
