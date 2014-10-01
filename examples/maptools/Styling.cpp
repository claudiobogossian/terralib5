#include "MapToolsExamples.h"

// TerraLib
#include <terralib/common.h>
#include <terralib/geometry.h>
#include <terralib/dataaccess.h>
#include <terralib/maptools.h>
#include <terralib/se.h>
#include <terralib/qt/widgets/canvas/Canvas.h>

// STL
#include <cassert>
#include <iostream>
#include <memory>
#include <vector>

// Qt
#include <QApplication>
#include <QDialog>
#include <QLabel>

/** @name Polygon Styles */
//@{
te::se::Style* SimplePolygonStyle()
{
  te::se::Fill* fill = te::se::CreateFill("#FF8C00", "1.0");

  te::se::PolygonSymbolizer* symbolizer = new te::se::PolygonSymbolizer;
  symbolizer->setFill(fill);

  te::se::Rule* rule = new te::se::Rule;
  rule->push_back(symbolizer);

  te::se::FeatureTypeStyle* style = new te::se::FeatureTypeStyle;
  style->push_back(rule);

  return style;
}

te::se::Style* SimplePolygonStyleWithStroke()
{
  te::se::Fill* fill = te::se::CreateFill("#FF8C00", "1.0");
  te::se::Stroke* stroke = te::se::CreateStroke("#FFFFFF", "2");

  te::se::PolygonSymbolizer* symbolizer = te::se::CreatePolygonSymbolizer(stroke, fill);

  te::se::Rule* rule = new te::se::Rule;
  rule->push_back(symbolizer);

  te::se::FeatureTypeStyle* style = new te::se::FeatureTypeStyle;
  style->push_back(rule);

  return style;
}

te::se::Style* TransparentPolygonStyle()
{
  te::se::Fill* fill = te::se::CreateFill("#FF8C00", "0.5");
  te::se::Stroke* stroke = te::se::CreateStroke("#FFFFFF", "2");

  te::se::PolygonSymbolizer* symbolizer = CreatePolygonSymbolizer(stroke, fill);

  te::se::Rule* rule = new te::se::Rule;
  rule->push_back(symbolizer);

  te::se::FeatureTypeStyle* style = new te::se::FeatureTypeStyle;
  style->push_back(rule);

  return style;
}

te::se::Style* GraphicFillPolygonStyle(const std::string& markName)
{
  te::se::Fill* markFill = te::se::CreateFill("#FF8C00", "1.0");
  te::se::Stroke* markStroke = te::se::CreateStroke("#000000", "1");

  te::se::Mark* mark = te::se::CreateMark(markName, markStroke, markFill);

  te::se::Graphic* graphic = te::se::CreateGraphic(mark, "16", "", "");

  te::se::Fill* fill = te::se::CreateFill(graphic);
  te::se::Stroke* stroke = te::se::CreateStroke("#000000", "2");

  te::se::PolygonSymbolizer* symbolizer = te::se::CreatePolygonSymbolizer(stroke, fill);

  te::se::Rule* rule = new te::se::Rule;
  rule->push_back(symbolizer);

  te::se::FeatureTypeStyle* style = new te::se::FeatureTypeStyle;
  style->push_back(rule);

  return style;
}

te::se::Style* TreePolygonStyle()
{
  te::se::Fill* markFill = te::se::CreateFill("#00FF00", "1.0");
  te::se::Stroke* markStroke = te::se::CreateStroke("#900020", "2");

  te::se::Mark* mark = te::se::CreateMark("triangle", markStroke, markFill);

  te::se::Graphic* graphic = te::se::CreateGraphic(mark, "24", "", "");

  te::se::Fill* fillOne = te::se::CreateFill(graphic);

  te::se::PolygonSymbolizer* symbolizerOne = te::se::CreatePolygonSymbolizer(0, fillOne);

  te::se::Fill* fillTwo = te::se::CreateFill("#00FF00", "0.5");
  te::se::Stroke* stroke = te::se::CreateStroke("#900020", "2");

  te::se::PolygonSymbolizer* symbolizerTwo = te::se::CreatePolygonSymbolizer(stroke, fillTwo);

  te::se::Rule* rule = new te::se::Rule;
  rule->push_back(symbolizerOne);
  rule->push_back(symbolizerTwo);

  te::se::FeatureTypeStyle* style = new te::se::FeatureTypeStyle;
  style->push_back(rule);

  return style;
}

te::se::Style* GraphicFillContourPolygonStyle(const std::string& markName)
{
  te::se::Fill* markFill = te::se::CreateFill("#FF0000", "1.0");
  te::se::Stroke* markStroke = te::se::CreateStroke("#000000", "1");

  te::se::Mark* mark = te::se::CreateMark(markName, markStroke, markFill);

  te::se::Graphic* graphic = te::se::CreateGraphic(mark, "16", "-45", "");

  te::se::Stroke* stroke = te::se::CreateStroke(graphic, "16", "", "", "", "");

  te::se::Fill* fill = te::se::CreateFill("#000000", "1.0");

  te::se::PolygonSymbolizer* symbolizer = te::se::CreatePolygonSymbolizer(stroke, fill);

  te::se::Rule* rule = new te::se::Rule;
  rule->push_back(symbolizer);

  te::se::FeatureTypeStyle* style = new te::se::FeatureTypeStyle;
  style->push_back(rule);

  return style;
}
//@}

/** @name Line Styles */
//@{
te::se::Style* SimpleLineStyle()
{
  te::se::Stroke* stroke = te::se::CreateStroke("#000000", "3.0");

  te::se::LineSymbolizer* symbolizer = te::se::CreateLineSymbolizer(stroke);

  te::se::Rule* rule = new te::se::Rule;
  rule->push_back(symbolizer);

  te::se::FeatureTypeStyle* style = new te::se::FeatureTypeStyle;
  style->push_back(rule);

  return style;
}

te::se::Style* DashedLineStyle()
{
  te::se::Stroke* stroke = te::se::CreateStroke("#0000FF", "3", "1.0", "5 2");
  te::se::LineSymbolizer* symbolizer = te::se::CreateLineSymbolizer(stroke);

  te::se::Rule* rule = new te::se::Rule;
  rule->push_back(symbolizer);

  te::se::FeatureTypeStyle* style = new te::se::FeatureTypeStyle;
  style->push_back(rule);

  return style;
}

te::se::Style* LineWithBorderStyle()
{
  te::se::Stroke* strokeOne = te::se::CreateStroke("#333333", "5");
  te::se::LineSymbolizer* symbolizerOne = te::se::CreateLineSymbolizer(strokeOne);

  te::se::Stroke* strokeTwo = te::se::CreateStroke("#6699FF", "3");
  te::se::LineSymbolizer* symbolizerTwo = te::se::CreateLineSymbolizer(strokeTwo);

  te::se::Rule* rule = new te::se::Rule;
  rule->push_back(symbolizerOne);
  rule->push_back(symbolizerTwo);

  te::se::FeatureTypeStyle* style = new te::se::FeatureTypeStyle;
  style->push_back(rule);

  return style;
}

te::se::Style* RoadStyleOne()
{
  te::se::Stroke* strokeOne = te::se::CreateStroke("#000000", "12");
  te::se::LineSymbolizer* symbolizerOne = te::se::CreateLineSymbolizer(strokeOne);

  te::se::Stroke* strokeTwo = te::se::CreateStroke("#888888", "8");
  te::se::LineSymbolizer* symbolizerTwo = te::se::CreateLineSymbolizer(strokeTwo);
  
  te::se::Stroke* strokeThree = te::se::CreateStroke("#FFFF00", "2", "1.0", "5 5");
  te::se::LineSymbolizer* symbolizerThree = te::se::CreateLineSymbolizer(strokeThree);

  te::se::Rule* rule = new te::se::Rule;
  rule->push_back(symbolizerOne);
  rule->push_back(symbolizerTwo);
  rule->push_back(symbolizerThree);

  te::se::FeatureTypeStyle* style = new te::se::FeatureTypeStyle;
  style->push_back(rule);

  return style;
}

te::se::Style* RoadStyleTwo()
{
  te::se::Stroke* strokeOne = te::se::CreateStroke("#000000", "16", "1.0", "", "butt", "miter");
  te::se::LineSymbolizer* symbolizerOne = te::se::CreateLineSymbolizer(strokeOne);

  te::se::Stroke* strokeTwo = te::se::CreateStroke("#666666", "10", "1.0", "", "butt", "miter");
  te::se::LineSymbolizer* symbolizerTwo = te::se::CreateLineSymbolizer(strokeTwo);
  
  te::se::Stroke* strokeThree = te::se::CreateStroke("#FFFFFF", "2", "1.0", "2 2", "butt", "miter");
  te::se::LineSymbolizer* symbolizerThree = te::se::CreateLineSymbolizer(strokeThree);

  te::se::Rule* rule = new te::se::Rule;
  rule->push_back(symbolizerOne);
  rule->push_back(symbolizerTwo);
  rule->push_back(symbolizerThree);

  te::se::FeatureTypeStyle* style = new te::se::FeatureTypeStyle;
  style->push_back(rule);

  return style;
}
//@}

/** @name Point Styles */
//@{
te::se::Style* MarkPointStyle(const std::string& markName)
{
  te::se::Fill* markFill = te::se::CreateFill("#009900", "1.0");
  te::se::Stroke* markStroke = te::se::CreateStroke("#000000", "1");
  te::se::Mark* mark = te::se::CreateMark(markName, markStroke, markFill);

  te::se::Graphic* graphic = te::se::CreateGraphic(mark, "16", "", "");

  te::se::PointSymbolizer* symbolizer = te::se::CreatePointSymbolizer(graphic);

  te::se::Rule* rule = new te::se::Rule;
  rule->push_back(symbolizer);

  te::se::FeatureTypeStyle* style = new te::se::FeatureTypeStyle;
  style->push_back(rule);

  return style;
}
//@}

te::map::DataSetLayer* CreateDataSetLayer(const std::string& path)
{
  // Connection string to a shape file
  std::map<std::string, std::string> connInfo;
  connInfo["path"] = path;

  // Creates and connects data source
  te::da::DataSourcePtr datasource = te::da::DataSourceManager::getInstance().open(te::common::Convert2String(G_ID++), "OGR", connInfo);

  // Get the number of data set types that belongs to the data source
  std::vector<std::string> datasets = datasource->getDataSetNames();
  assert(!datasets.empty());

  // Gets the first dataset
  std::string datasetName(datasets[0]);
  std::auto_ptr<te::da::DataSetType> dt(datasource->getDataSetType(datasetName));
  assert(dt->hasGeom());

  te::gm::GeometryProperty* geomProperty = te::da::GetFirstGeomProperty(dt.get());
  assert(geomProperty);

  // Box
  std::auto_ptr<te::gm::Envelope> extent(datasource->getExtent(datasetName, geomProperty->getName()));

  // Creates a DataSetLayer
  te::map::DataSetLayer* layer = new te::map::DataSetLayer(te::common::Convert2String(0), datasetName);
  layer->setDataSourceId(datasource->getId());
  layer->setDataSetName(datasetName);
  layer->setExtent(*extent);
  layer->setRendererType("DATASET_LAYER_RENDERER");

  return layer;
}

void Draw(te::map::DataSetLayer* layer)
{
  const te::gm::Envelope& extent = layer->getExtent();
  double llx = extent.m_llx;
  double lly = extent.m_lly;
  double urx = extent.m_urx;
  double ury = extent.m_ury;

  std::auto_ptr<te::qt::widgets::Canvas> canvas(new te::qt::widgets::Canvas(512, 512));
  canvas->calcAspectRatio(llx, lly, urx, ury);
  canvas->setWindow(llx, lly, urx, ury);
  canvas->setBackgroundColor(te::color::RGBAColor(255, 255, 255, TE_OPAQUE));

  layer->draw(canvas.get(), extent, 4326);

  QPixmap* pixmap = canvas->getPixmap();

  QDialog dialog;
  dialog.setFixedSize(pixmap->size());
  
  QLabel preview(&dialog);
  preview.setPixmap(*pixmap);
  
  dialog.exec();
}

void DrawStyledLayers()
{
  // If you want to use any Qt resource you must initialize a QApplication in some place!
  int argc = 0;
  QApplication app(argc, 0);

  try
  {
    // Gets supported marks
    std::vector<std::string> marks;
    //te::map::MarkRendererManager::getAllSupportedMarks(marks); // AbstractMarkFactory::SupportedMarks(marks);
    te::map::MarkRendererManager::getInstance().getAllSupportedMarks(marks);
    // Creates a layer of polygons
    te::map::DataSetLayer* polygons = CreateDataSetLayer("./shp/style/polygons.shp");

    // Polygon Styles
    std::vector<te::se::Style*> polygonStyles;
    polygonStyles.push_back(SimplePolygonStyle());
    polygonStyles.push_back(SimplePolygonStyleWithStroke());
    polygonStyles.push_back(TransparentPolygonStyle());
    for(std::size_t i = 0; i < marks.size(); ++i)
      polygonStyles.push_back(GraphicFillPolygonStyle(marks[i]));
    polygonStyles.push_back(TreePolygonStyle());
    polygonStyles.push_back(GraphicFillContourPolygonStyle("star"));

    // Draw!
    for(std::size_t i = 0; i < polygonStyles.size(); ++i)
    {
      polygons->setStyle(polygonStyles[i]);
      Draw(polygons);
    }

    // Creates a layer of lines
    te::map::DataSetLayer* lines = CreateDataSetLayer("./shp/style/lines.shp");

    // Line Styles
    std::vector<te::se::Style*> lineStyles;
    lineStyles.push_back(SimpleLineStyle());
    lineStyles.push_back(DashedLineStyle());
    lineStyles.push_back(LineWithBorderStyle());
    lineStyles.push_back(RoadStyleOne());
    lineStyles.push_back(RoadStyleTwo());

    // Draw!
    for(std::size_t i = 0; i < lineStyles.size(); ++i)
    {
      lines->setStyle(lineStyles[i]);
      Draw(lines);
    }

    // Creates a layer of points
    te::map::DataSetLayer* points = CreateDataSetLayer("./shp/style/points.shp");

    // Point Styles
    std::vector<te::se::Style*> pointStyles;
    for(std::size_t i = 0; i < marks.size(); ++i)
      pointStyles.push_back(MarkPointStyle(marks[i]));

    // Draw!
    for(std::size_t i = 0; i < pointStyles.size(); ++i)
    {
      points->setStyle(pointStyles[i]);
      Draw(points);
    }

    delete points;
    delete lines;
    delete polygons;
  }
  catch(const std::exception& e)
  {
    std::cout << std::endl << "An exception has occurred in Styling example: " << e.what() << std::endl;
  }
  catch(...)
  {
    std::cout << std::endl << "An unexpected exception has occurred in Styling example!" << std::endl;
  }
}
