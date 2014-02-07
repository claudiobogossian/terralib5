#include "SymbologyWidgets.h"

// TerraLib
#include <terralib/common.h>
#include <terralib/se.h>
#include <terralib/serialization/qt/widgets/Symbol.h>
#include <terralib/qt/widgets/se/BasicFillDialog.h>
#include <terralib/qt/widgets/se/BasicStrokeDialog.h>
#include <terralib/qt/widgets/se/GlyphMarkDialog.h>
#include <terralib/qt/widgets/se/GraphicDialog.h>
#include <terralib/qt/widgets/se/LineSymbolizerWidget.h>
#include <terralib/qt/widgets/se/PointSymbolizerWidget.h>
#include <terralib/qt/widgets/se/PolygonSymbolizerWidget.h>
#include <terralib/qt/widgets/se/Symbol.h>
#include <terralib/qt/widgets/se/SymbolSelectorDialog.h>
#include <terralib/qt/widgets/se/SymbolEditorWidget.h>
#include <terralib/qt/widgets/se/SymbolInfoDialog.h>
#include <terralib/qt/widgets/se/SymbolPreviewWidget.h>
#include <terralib/qt/widgets/se/SymbolTableWidget.h>
#include <terralib/qt/widgets/se/WellKnownMarkDialog.h>

// Qt
#include <QtGui/QApplication>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QMdiArea>
#include <QtGui/QMdiSubWindow>
#include <QtGui/QToolBox>

// STL
#include <cassert>
#include <iostream>

void SymbolSelector()
{
  // Loads the Symbol Library from XML file
  te::serialize::ReadSymbolLibrary("E:/TerraLib/terralib5_develop/schemas/terralib/qt/se/symbol.xml");

  // Selects a symbol
  te::qt::widgets::Symbol* symbol =  te::qt::widgets::SymbolSelectorDialog::getSymbol(0);
  if(symbol == 0)
    return;

  // Shows the selected symbol information
  te::qt::widgets::SymbolInfo info = symbol->getInfo();
  std::cout << ":: Selected Symbol ::" << std::endl;
  std::cout << "- ID: " << info.m_id << std::endl;
  std::cout << "- Name: " << info.m_name << std::endl;
  std::cout << "- Author: " << info.m_author << std::endl;
  std::cout << "- Tags: " << info.m_tags << std::endl;
  std::cout << "- Description: " << info.m_description << std::endl;
  std::cout << "- Number of Symbolizers: " << symbol->getSymbolizersCount() << std::endl;

  delete symbol;
}

void SymbolEditor()
{
  // Widgets for symbol edition...
  te::qt::widgets::SymbolEditorWidget* editPointSymbol = new te::qt::widgets::SymbolEditorWidget(te::qt::widgets::PointSymbol);
  te::qt::widgets::SymbolEditorWidget* editLineSymbol = new te::qt::widgets::SymbolEditorWidget(te::qt::widgets::LineSymbol);
  te::qt::widgets::SymbolEditorWidget* editPolygonSymbol = new te::qt::widgets::SymbolEditorWidget(te::qt::widgets::PolygonSymbol);

  // Symbol Information
  te::qt::widgets::SymbolInfo info;
  info.m_id = "179C25E4-AABC-4F63-8C10-AAD00CCE74F9";
  info.m_name = "Brazilian Highway";
  info.m_author = "TerraLib Team";
  info.m_tags = "highway; road; Brazil";
  info.m_description = "This symbol is used to represent highways.";
  te::qt::widgets::SymbolInfoDialog* symbolInfo = new te::qt::widgets::SymbolInfoDialog; 
  symbolInfo->setSymbolInfo(info);

  // Grouping to show!
  QMdiArea mainWidget;
  mainWidget.setWindowTitle("Symbol Editors");
  mainWidget.addSubWindow(editPointSymbol)->setWindowTitle("Point Symbol");
  mainWidget.addSubWindow(editLineSymbol)->setWindowTitle("Line Symbol");
  mainWidget.addSubWindow(editPolygonSymbol)->setWindowTitle("Polygon Symbol");
  mainWidget.addSubWindow(symbolInfo);
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
  QDialog dlg;
  dlg.setWindowTitle("Symbolizer Preview Example");

  // Creates the symbol
  te::qt::widgets::Symbol* symbol = new te::qt::widgets::Symbol;
  symbol->addSymbolizer(te::se::CreateLineSymbolizer(te::se::CreateStroke("#000000", "9.0")));
  symbol->addSymbolizer(te::se::CreateLineSymbolizer(te::se::CreateStroke("#808080", "5.0")));
  symbol->addSymbolizer(te::se::CreateLineSymbolizer(te::se::CreateStroke("#FFFF00", "2.0", "1.0", "2 2")));

  // Preview
  te::qt::widgets::SymbolPreviewWidget* preview = new te::qt::widgets::SymbolPreviewWidget(QSize(150, 150), &dlg);
  preview->updatePreview(symbol);

  // Table preview
  te::qt::widgets::SymbolTableWidget* table = new te::qt::widgets::SymbolTableWidget(QSize(150, 32), &dlg);
  table->updatePreview(symbol);
  table->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);

  // Adjusting...
  QGridLayout* layout = new QGridLayout(&dlg);
  layout->setSizeConstraint(QLayout::SetFixedSize);
  layout->addWidget(preview, 0, 0);
  layout->addWidget(table, 1, 0);

  dlg.exec();

  delete symbol;
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

  // Adjusting icons theme
  QString spaths = std::string(ICON_THEME_PATH).c_str();
  QStringList paths = spaths.split(";");
  QIcon::setThemeName(ICON_THEME);
  QIcon::setThemeSearchPaths(paths);

  SymbolSelector();

  SymbolEditor();

  SymbolizerWidgets();

  PreviewWidgets();

  BasicWidgets();
}
