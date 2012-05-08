#include "MapToolsExamples.h"

// TerraLib
#include <terralib/common.h>
#include <terralib/geometry.h>
#include <terralib/dataaccess.h>
#include <terralib/maptools.h>
#include <terralib/se.h>
#include <terralib/qt/widgets/canvas/Canvas.h>
#include <terralib/qt/se/MarkFactory.h>

// STL
#include <cassert>
#include <iostream>
#include <vector>

// Qt
#include <QtGui/QApplication>
#include <QtGui/QDialog>
#include <QtGui/QLabel>

te::se::Stroke* CreateStroke(const std::string& color     = "#000000",
                             const std::string& width     = "1",
                             const std::string& opacity   = "1.0",
                             const std::string& linecap   = "round",
                             const std::string& linejoin  = "round",
                             const std::string& dasharray = "")
{
  te::se::Stroke* stroke = new te::se::Stroke;
  stroke->setColor(color);
  stroke->setWidth(width);
  stroke->setOpacity(opacity);
  stroke->setLineCap(linecap);
  stroke->setLineJoin(linejoin);
  
  if(!dasharray.empty())
    stroke->setDashArray(dasharray);

  return stroke;
}

te::se::Stroke* CreateStroke(te::se::Graphic* graphic)
{
  te::se::Stroke* stroke = new te::se::Stroke;
  stroke->setGraphicFill(graphic);

  return stroke;
}

te::se::Fill* CreateFill(const std::string& color = "#888888", const std::string& opacity = "1.0")
{
  te::se::Fill* fill = new te::se::Fill;
  fill->setColor(color);
  fill->setOpacity(opacity);

  return fill;
}

te::se::Fill* CreateFill(te::se::Graphic* graphic)
{
  te::se::Fill* fill = new te::se::Fill;
  fill->setGraphicFill(graphic);

  return fill;
}

te::se::Mark* CreateMark(const std::string& name, te::se::Fill* fill,  te::se::Stroke* stroke)
{
  te::se::Mark* mark = new te::se::Mark;
  mark->setWellKnownName(new std::string(name));

  if(fill)
    mark->setFill(fill);

  if(stroke)
    mark->setStroke(stroke);

  return mark;
}

te::se::Mark* CreateMark(const std::string& name             = "square",
                         const std::string& color            = "#888888",
                         const std::string& opacity          = "1.0",
                         const std::string& contourColor     = "#000000",
                         const std::string& contourWidth     = "1",
                         const std::string& contourOpacity   = "1.0",
                         const std::string& countourLinecap  = "round",
                         const std::string& contourLinejoin  = "round",
                         const std::string& contourDasharray = "")
{
  te::se::Fill* fill = CreateFill(color, opacity); 
  te::se::Stroke* stroke = CreateStroke(contourColor, contourWidth, contourOpacity, countourLinecap, contourLinejoin, contourDasharray);

  return CreateMark(name, fill, stroke);
}

te::se::PolygonSymbolizer* CreatePolygonSymbolizer(te::se::Fill* fill, te::se::Stroke* stroke)
{
  te::se::PolygonSymbolizer* symbolizer = new te::se::PolygonSymbolizer;

  if(fill)
    symbolizer->setFill(fill);

  if(stroke)
    symbolizer->setStroke(stroke);

  return symbolizer;
}

te::se::PolygonSymbolizer* CreatePolygonSymbolizer(te::se::Fill* fill,
                                                   const std::string& contourColor     = "#000000",
                                                   const std::string& contourWidth     = "1",
                                                   const std::string& contourOpacity   = "1.0",
                                                   const std::string& countourLinecap  = "round",
                                                   const std::string& contourLinejoin  = "round",
                                                   const std::string& contourDasharray = "")
{
  te::se::Stroke* stroke = CreateStroke(contourColor, contourWidth, contourOpacity, countourLinecap, contourLinejoin, contourDasharray);
  return CreatePolygonSymbolizer(fill, stroke);
}

te::se::PolygonSymbolizer* CreatePolygonSymbolizer(const std::string& color            = "#888888",
                                                   const std::string& opacity          = "1.0",
                                                   const std::string& contourColor     = "#000000",
                                                   const std::string& contourWidth     = "1",
                                                   const std::string& contourOpacity   = "1.0",
                                                   const std::string& countourLinecap  = "round",
                                                   const std::string& contourLinejoin  = "round",
                                                   const std::string& contourDasharray = "")
{
  te::se::Fill* fill = CreateFill(color, opacity);
  return CreatePolygonSymbolizer(fill, contourColor, contourWidth, contourOpacity, countourLinecap, contourLinejoin, contourDasharray);
}

te::se::LineSymbolizer* CreateLineSymbolizer(const std::string& color     = "#000000",
                                             const std::string& width     = "1",
                                             const std::string& opacity   = "1.0",
                                             const std::string& linecap   = "round",
                                             const std::string& linejoin  = "round",
                                             const std::string& dasharray = "")
{
  te::se::Stroke* stroke = CreateStroke(color, width, opacity, linecap, linejoin, dasharray);

  te::se::LineSymbolizer* symbolizer = new te::se::LineSymbolizer;
  symbolizer->setStroke(stroke);

  return symbolizer;
}

/** @name Polygon Styles */
//@{
te::se::Style* SimplePolygonStyle()
{
  te::se::Fill* fill = CreateFill("#000080");

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
  te::se::PolygonSymbolizer* symbolizer = CreatePolygonSymbolizer("#000080", "1.0", "#FFFFFF", "2");

  te::se::Rule* rule = new te::se::Rule;
  rule->push_back(symbolizer);

  te::se::FeatureTypeStyle* style = new te::se::FeatureTypeStyle;
  style->push_back(rule);

  return style;
}

te::se::Style* TransparentPolygonStyle()
{
  te::se::PolygonSymbolizer* symbolizer = CreatePolygonSymbolizer("#000080", "0.5", "#FFFFFF", "2");

  te::se::Rule* rule = new te::se::Rule;
  rule->push_back(symbolizer);

  te::se::FeatureTypeStyle* style = new te::se::FeatureTypeStyle;
  style->push_back(rule);

  return style;
}

te::se::Style* TreePolygonStyle()
{
  te::se::Mark* mark = CreateMark("triangle", "#00FF00", "1.0", "#900020", "2");

  te::se::Graphic* graphic = new te::se::Graphic;
  graphic->add(mark);
  graphic->setSize(new te::se::ParameterValue("20"));

  te::se::Fill* fill = CreateFill(graphic);

  te::se::PolygonSymbolizer* symbolizerOne = CreatePolygonSymbolizer(fill, 0);
  te::se::PolygonSymbolizer* symbolizerTwo = CreatePolygonSymbolizer("#008000", "0.5", "#900020", "3");

  te::se::Rule* rule = new te::se::Rule;
  rule->push_back(symbolizerOne);
  rule->push_back(symbolizerTwo);

  te::se::FeatureTypeStyle* style = new te::se::FeatureTypeStyle;
  style->push_back(rule);

  return style;
}

te::se::Style* GraphicFillPolygonStyle(const std::string& markName)
{
  te::se::Mark* mark = CreateMark(markName, "#FF0000", "1.0", "#000000");

  te::se::Graphic* graphic = new te::se::Graphic;
  graphic->add(mark);
  graphic->setSize(new te::se::ParameterValue("16"));

  te::se::Fill* fill = CreateFill(graphic);

  te::se::PolygonSymbolizer* symbolizer = CreatePolygonSymbolizer(fill, "#000000", "2", "1.0");

  te::se::Rule* rule = new te::se::Rule;
  rule->push_back(symbolizer);

  te::se::FeatureTypeStyle* style = new te::se::FeatureTypeStyle;
  style->push_back(rule);

  return style;
}

te::se::Style* GraphicFillStrokePolygonStyle(const std::string& markName)
{
  te::se::Mark* mark = CreateMark(markName, "#FF0000", "1.0", "#000000");

  te::se::Graphic* graphic = new te::se::Graphic;
  graphic->add(mark);
  graphic->setSize(new te::se::ParameterValue("16"));
  graphic->setRotation(new te::se::ParameterValue("-45"));

  te::se::Stroke* stroke = CreateStroke(graphic);
  stroke->setWidth("32");

  te::se::Fill* fill = CreateFill("#000000");

  te::se::PolygonSymbolizer* symbolizer = CreatePolygonSymbolizer(fill, stroke);

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
  te::se::LineSymbolizer* symbolizer = CreateLineSymbolizer("#000000", "3.0");

  te::se::Rule* rule = new te::se::Rule;
  rule->push_back(symbolizer);

  te::se::FeatureTypeStyle* style = new te::se::FeatureTypeStyle;
  style->push_back(rule);

  return style;
}

te::se::Style* LineWithBorderStyle()
{
  te::se::LineSymbolizer* symbolizerOne = CreateLineSymbolizer("#333333", "5");
  te::se::LineSymbolizer* symbolizerTwo = CreateLineSymbolizer("#6699FF", "3");

  te::se::Rule* rule = new te::se::Rule;
  rule->push_back(symbolizerOne);
  rule->push_back(symbolizerTwo);

  te::se::FeatureTypeStyle* style = new te::se::FeatureTypeStyle;
  style->push_back(rule);

  return style;
}

te::se::Style* RoadStyleOne()
{
  te::se::LineSymbolizer* symbolizerOne = CreateLineSymbolizer("#000000", "12");
  te::se::LineSymbolizer* symbolizerTwo = CreateLineSymbolizer("#888888", "8");
  te::se::LineSymbolizer* symbolizerThree = CreateLineSymbolizer("#FFFF00", "2", "1.0", "round", "round", "5 5");

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
  te::se::LineSymbolizer* symbolizerOne = CreateLineSymbolizer("#000000", "16", "1.0", "butt", "miter");
  te::se::LineSymbolizer* symbolizerTwo = CreateLineSymbolizer("#666666", "10", "1.0", "butt", "miter");
  te::se::LineSymbolizer* symbolizerThree = CreateLineSymbolizer("#FFFFFF", "2", "1.0", "butt", "miter", "2 2");

  te::se::Rule* rule = new te::se::Rule;
  rule->push_back(symbolizerOne);
  rule->push_back(symbolizerTwo);
  rule->push_back(symbolizerThree);

  te::se::FeatureTypeStyle* style = new te::se::FeatureTypeStyle;
  style->push_back(rule);

  return style;
}

te::se::Style* DashedLineStyle()
{
  te::se::LineSymbolizer* symbolizer = CreateLineSymbolizer("#0000FF", "3", "1.0", "round", "round", "5 2");

  te::se::Rule* rule = new te::se::Rule;
  rule->push_back(symbolizer);

  te::se::FeatureTypeStyle* style = new te::se::FeatureTypeStyle;
  style->push_back(rule);

  return style;
}

te::se::Style* GraphicFillLineStyle(const std::string& markName)
{
  te::se::Mark* mark = CreateMark(markName, "#FFFF00", "1.0", "#000000", "3.0");

  te::se::Graphic* graphic = new te::se::Graphic;
  graphic->add(mark);
  graphic->setSize(new te::se::ParameterValue("16"));

  te::se::Stroke* stroke = CreateStroke(graphic);
  stroke->setWidth("8");

  te::se::LineSymbolizer* symbolizer = new te::se::LineSymbolizer;
  symbolizer->setStroke(stroke);

  te::se::Rule* rule = new te::se::Rule;
  rule->push_back(symbolizer);

  te::se::FeatureTypeStyle* style = new te::se::FeatureTypeStyle;
  style->push_back(rule);

  return style;
}
//@}

/** @name Point Styles */
//@{
te::se::Style* MarkPointStyle(const std::string& markName)
{
  te::se::Mark* mark = CreateMark(markName, "#009900", "1.0", "#000000");

  te::se::Graphic* graphic = new te::se::Graphic;
  graphic->add(mark);
  graphic->setSize(new te::se::ParameterValue("16"));
  graphic->setOpacity(new te::se::ParameterValue("0.5"));

  te::se::PointSymbolizer* symbolizer = new te::se::PointSymbolizer;
  symbolizer->setGraphic(graphic);

  te::se::Rule* rule = new te::se::Rule;
  rule->push_back(symbolizer);

  te::se::FeatureTypeStyle* style = new te::se::FeatureTypeStyle;
  style->push_back(rule);

  return style;
}

te::se::Style* MultiMarkPointStyle()
{
  // Square
  te::se::Mark* squareMark = CreateMark("square", "#00FFFF", "1.0", "#000000", "1", "1.0", "round", "round", "1 2");
  te::se::Graphic* graphicSquare = new te::se::Graphic;
  graphicSquare->add(squareMark);
  graphicSquare->setSize(new te::se::ParameterValue("30"));

  // Circle
  te::se::Mark* circleMark = CreateMark("circle", "#FFFF00", "1.0");
  te::se::Graphic* graphicCircle = new te::se::Graphic;
  graphicCircle->add(circleMark);
  graphicCircle->setSize(new te::se::ParameterValue("24"));

  // Star
  te::se::Mark* starMark = CreateMark("star", "#FF0000", "1.0", "#000000");
  te::se::Graphic* graphicStar = new te::se::Graphic;
  graphicStar->add(starMark);
  graphicStar->setSize(new te::se::ParameterValue("16"));

  te::se::PointSymbolizer* symbolizerOne = new te::se::PointSymbolizer;
  symbolizerOne->setGraphic(graphicSquare);

  te::se::PointSymbolizer* symbolizerTwo = new te::se::PointSymbolizer;
  symbolizerTwo->setGraphic(graphicCircle);
  
  te::se::PointSymbolizer* symbolizerThree = new te::se::PointSymbolizer;
  symbolizerThree->setGraphic(graphicStar);

  te::se::Rule* rule = new te::se::Rule;
  rule->push_back(symbolizerOne);
  rule->push_back(symbolizerTwo);
  rule->push_back(symbolizerThree);

  te::se::FeatureTypeStyle* style = new te::se::FeatureTypeStyle;
  style->push_back(rule);

  return style;
}
//@}

te::map::Layer* CreateLayer(const std::string& path)
{
  // Connection string to a shape file
  std::map<std::string, std::string> connInfo;
  connInfo["path"] = path;

  // Creates and connects data source
  te::da::DataSource* dataSource = te::da::DataSourceFactory::make("OGR");
  if(dataSource == 0)
    throw te::common::Exception("Sorry, I can not create the OGR driver!");
  dataSource->open(connInfo);

  // Transactor and catalog
  te::da::DataSourceTransactor* transactor = dataSource->getTransactor();
  te::da::DataSourceCatalogLoader* catalogLoader = transactor->getCatalogLoader();
  catalogLoader->loadCatalog();

  // Gets the number of data set types that belongs to the data source
  std::vector<std::string*> datasets;
  transactor->getCatalogLoader()->getDataSets(datasets);
  assert(!datasets.empty());

  // Gets the first dataset
  std::string dataSetName(*datasets[0]);
  te::da::DataSetType* dt = catalogLoader->getDataSetType(dataSetName);
  assert(dt->hasGeom());

  // Box
  te::gm::Envelope* extent = catalogLoader->getExtent(dt->getDefaultGeomProperty());

  // Creates a Layer
  te::map::Layer* layer = new te::map::Layer(te::common::Convert2String(0), dataSetName);
  layer->setDataSource(dataSource);
  layer->setDataSetName(dataSetName);
  layer->setExtent(extent);

  // Creates a Layer Renderer
  te::map::LayerRenderer* r = new te::map::LayerRenderer();
  layer->setRenderer(r);

  te::common::FreeContents(datasets);
  delete catalogLoader;
  delete transactor;

  return layer;
}

void Draw(te::map::Layer* layer)
{
  const te::gm::Envelope* extent = layer->getExtent();
  double llx = extent->m_llx;
  double lly = extent->m_lly;
  double urx = extent->m_urx;
  double ury = extent->m_ury;

  te::qt::widgets::Canvas* canvas = new te::qt::widgets::Canvas(512, 512);
  canvas->calcAspectRatio(llx, lly, urx, ury);
  canvas->setWindow(llx, lly, urx, ury);
  canvas->setBackgroundColor(te::color::RGBAColor(255, 255, 255, TE_OPAQUE));

  layer->draw(canvas, *extent, 4326);

  QPixmap* pixmap = canvas->getPixmap();

  QDialog dialog;
  dialog.setFixedSize(pixmap->size());
  
  QLabel preview(&dialog);
  preview.setPixmap(*pixmap);
  
  dialog.exec();

  delete canvas;
}

void DrawStyledLayers()
{
  // If you want to use any Qt resource you must initialize a QApplication in some place!
  int argc = 0;
  QApplication app(argc, 0);

  try
  {
    te::qt::MarkFactory::initialize();
    
    // Gets supported marks
    std::vector<std::string> marks;
    te::map::AbstractMarkFactory::SupportedMarks(marks);

    // Creates a layer of polygons
    te::map::Layer* polygons = CreateLayer("./data/shp/style/polygons.shp");

    // Polygon Styles
    std::vector<te::se::Style*> polygonStyles;
    polygonStyles.push_back(SimplePolygonStyle());
    polygonStyles.push_back(SimplePolygonStyleWithStroke());
    polygonStyles.push_back(TransparentPolygonStyle());
    for(std::size_t i = 0; i < marks.size(); ++i)
      polygonStyles.push_back(GraphicFillPolygonStyle(marks[i]));
    polygonStyles.push_back(TreePolygonStyle());
    polygonStyles.push_back(GraphicFillStrokePolygonStyle("triangle"));

    // Draw!
    for(std::size_t i = 0; i < polygonStyles.size(); ++i)
    {
      polygons->setStyle(polygonStyles[i]);
      Draw(polygons);
    }

    // Creates a layer of lines
    te::map::Layer* lines = CreateLayer("./data/shp/style/lines.shp");

    // Line Styles
    std::vector<te::se::Style*> lineStyles;
    lineStyles.push_back(GraphicFillLineStyle("triangle"));
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
    te::map::Layer* points = CreateLayer("./data/shp/style/points.shp");

    // Point Styles
    std::vector<te::se::Style*> pointStyles;
    for(std::size_t i = 0; i < marks.size(); ++i)
      pointStyles.push_back(MarkPointStyle(marks[i]));
    pointStyles.push_back(MultiMarkPointStyle());

    // Draw!
    for(std::size_t i = 0; i < pointStyles.size(); ++i)
    {
      points->setStyle(pointStyles[i]);
      Draw(points);
    }

    delete points;
    delete lines;
    delete polygons;

    te::qt::MarkFactory::finalize();
  }
  catch(const std::exception& e)
  {
    std::cout << std::endl << "An exception has occuried in Styling example: " << e.what() << std::endl;
  }
  catch(...)
  {
    std::cout << std::endl << "An unexpected exception has occuried in Styling example!" << std::endl;
  }
}
