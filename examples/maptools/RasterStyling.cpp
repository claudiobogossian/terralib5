#include "MapToolsExamples.h"

// TerraLib
#include <terralib/common.h>
#include <terralib/geometry.h>
#include <terralib/dataaccess.h>
#include <terralib/maptools.h>
#include <terralib/raster.h>
#include <terralib/se.h>
#include <terralib/qt/widgets/canvas/Canvas.h>

// STL
#include <cassert>
#include <iostream>
#include <memory>
#include <vector>
#include <string>

// Qt
#include <QApplication>
#include <QDialog>
#include <QLabel>

bool generatePNG = true;

te::map::DataSetLayer* CreateRasterLayer(const std::string& path)
{
  // Connection string to a raster file
  std::map<std::string, std::string> connInfo;
  connInfo["URI"] = path;

  // Creates and connects data source
  te::da::DataSourcePtr datasource = te::da::DataSourceManager::getInstance().open(te::common::Convert2String(G_ID++), "GDAL", connInfo);

  // Get the number of data set types that belongs to the data source
  std::vector<std::string> datasets = datasource->getDataSetNames();
  assert(!datasets.empty());

  // Gets the first dataset
  std::string dataSetName(datasets[0]);
  std::auto_ptr<te::da::DataSet> ds(datasource->getDataSet(dataSetName));

  std::size_t rpos = te::da::GetFirstPropertyPos(ds.get(), te::dt::RASTER_TYPE);
  std::auto_ptr<te::rst::Raster> raster(ds->getRaster(rpos));

  te::gm::Envelope extent(*raster->getExtent());

  // Creates a DataSetLayer
  te::map::DataSetLayer* layer = new te::map::DataSetLayer(te::common::Convert2String(G_ID++), dataSetName);
  layer->setDataSourceId(datasource->getId());
  layer->setDataSetName(dataSetName);
  layer->setExtent(extent);
  layer->setRendererType("ABSTRACT_LAYER_RENDERER");
  layer->setSRID(raster->getSRID());

  return layer;
}

 te::qt::widgets::Canvas* CreateCanvas(te::map::DataSetLayer* layer, te::gm::Envelope* e, int srid)
{
  te::gm::Envelope extent(layer->getExtent());

  std::auto_ptr<te::rst::Raster> raster(te::map::GetRaster(layer));
  
  if(srid != raster->getSRID())
    extent.transform(raster->getSRID(), srid);

  double llx = extent.m_llx;
  double lly = extent.m_lly;
  double urx = extent.m_urx;
  double ury = extent.m_ury;

  te::qt::widgets::Canvas* canvas = new te::qt::widgets::Canvas(800, 600);
  canvas->calcAspectRatio(llx, lly, urx, ury);
  canvas->setWindow(llx, lly, urx, ury);
  canvas->setBackgroundColor(te::color::RGBAColor(255, 255, 255, TE_OPAQUE));

  return canvas;
}

void showPixmap(te::qt::widgets::Canvas* c, std::string fileName)
{
  QPixmap* pixmap = c->getPixmap();

  QDialog dialog;
  dialog.setWindowTitle(fileName.c_str());
  dialog.setFixedSize(pixmap->size());
  
  QLabel preview(&dialog);
  preview.setPixmap(*pixmap);
  
  dialog.exec();
}

void paint(te::qt::widgets::Canvas* c, bool generatePNG, std::string fileName)
{
  showPixmap(c, fileName);

  if(generatePNG)
  {
    fileName += ".tif";
    c->save(fileName.c_str(), te::map::PNG);
  }

  c->clear();
}

void RGB_012_Style(te::qt::widgets::Canvas* c, te::map::DataSetLayer* l, te::gm::Envelope* e, int srid)
 {
  //create default raster symbolizer
  te::se::RasterSymbolizer* rs = new te::se::RasterSymbolizer();

  //set transparency
  rs->setOpacity(new te::se::ParameterValue("1.0"));

  //set channel selection
  te::se::ChannelSelection* cs = new te::se::ChannelSelection();

  //channel R
  te::se::SelectedChannel* scR = new te::se::SelectedChannel();
  scR->setSourceChannelName("0");
  cs->setRedChannel(scR);

  //channel G
  te::se::SelectedChannel* scG = new te::se::SelectedChannel();
  scG->setSourceChannelName("1");
  cs->setGreenChannel(scG);

  //channel B
  te::se::SelectedChannel* scB = new te::se::SelectedChannel();
  scB->setSourceChannelName("2");
  cs->setBlueChannel(scB);

  rs->setChannelSelection(cs);

  //add symbolizer to a layer style
  te::se::Rule* r = new te::se::Rule();
  r->push_back(rs);

  te::se::Style* s = new te::se::CoverageStyle();
  s->push_back(r);

  l->setStyle(s);

  bool cancel = false;

  l->draw(c, *e, srid, 0, &cancel);

  paint(c, generatePNG, "RGB_012_Style");
 }

void RGB_012_Transp_Style(te::qt::widgets::Canvas* c, te::map::DataSetLayer* l, te::gm::Envelope* e, int srid)
 {
  //create default raster symbolizer
  te::se::RasterSymbolizer* rs = new te::se::RasterSymbolizer();

  //set transparency
  rs->setOpacity(new te::se::ParameterValue("0.5"));

  //set channel selection
  te::se::ChannelSelection* cs = new te::se::ChannelSelection();

  //channel R
  te::se::SelectedChannel* scR = new te::se::SelectedChannel();
  scR->setSourceChannelName("0");
  cs->setRedChannel(scR);

  //channel G
  te::se::SelectedChannel* scG = new te::se::SelectedChannel();
  scG->setSourceChannelName("1");
  cs->setGreenChannel(scG);

  //channel B
  te::se::SelectedChannel* scB = new te::se::SelectedChannel();
  scB->setSourceChannelName("2");
  cs->setBlueChannel(scB);

  rs->setChannelSelection(cs);

  //add symbolizer to a layer style
  te::se::Rule* r = new te::se::Rule();
  r->push_back(rs);

  te::se::Style* s = new te::se::CoverageStyle();
  s->push_back(r);

  l->setStyle(s);

  bool cancel = false;

  l->draw(c, *e, srid, 0, &cancel);

  paint(c, generatePNG, "RGB_012_Transp_Style");
 }

 void RGB_102_Style(te::qt::widgets::Canvas* c, te::map::DataSetLayer* l, te::gm::Envelope* e, int srid)
 {
  //create default raster symbolizer
  te::se::RasterSymbolizer* rs = new te::se::RasterSymbolizer();

  //set transparency
  rs->setOpacity(new te::se::ParameterValue("1.0"));

  //set channel selection
  te::se::ChannelSelection* cs = new te::se::ChannelSelection();

  //channel R
  te::se::SelectedChannel* scR = new te::se::SelectedChannel();
  scR->setSourceChannelName("1");
  cs->setRedChannel(scR);

  //channel G
  te::se::SelectedChannel* scG = new te::se::SelectedChannel();
  scG->setSourceChannelName("0");
  cs->setGreenChannel(scG);

  //channel B
  te::se::SelectedChannel* scB = new te::se::SelectedChannel();
  scB->setSourceChannelName("2");
  cs->setBlueChannel(scB);

  rs->setChannelSelection(cs);

  //add symbolizer to a layer style
  te::se::Rule* r = new te::se::Rule();
  r->push_back(rs);

  te::se::Style* s = new te::se::CoverageStyle();
  s->push_back(r);

  l->setStyle(s);

  bool cancel = false;

  l->draw(c, *e, srid, 0, &cancel);

  paint(c, generatePNG, "RGB_102_Style");
 }

void RGB_012_G_Contrast_Style(te::qt::widgets::Canvas* c, te::map::DataSetLayer* l, te::gm::Envelope* e, int srid)
 {
  //create default raster symbolizer
  te::se::RasterSymbolizer* rs = new te::se::RasterSymbolizer();

  //set transparency
  rs->setOpacity(new te::se::ParameterValue("1.0"));

  //set channel selection
  te::se::ChannelSelection* cs = new te::se::ChannelSelection();

  //channel R
  te::se::SelectedChannel* scR = new te::se::SelectedChannel();
  scR->setSourceChannelName("0");
  cs->setRedChannel(scR);

  //channel G
  te::se::SelectedChannel* scG = new te::se::SelectedChannel();
  scG->setSourceChannelName("1");

  te::se::ContrastEnhancement* cG = new te::se::ContrastEnhancement();
  cG->setGammaValue(0.5);
  scG->setContrastEnhancement(cG);
  cs->setGreenChannel(scG);

  //channel B
  te::se::SelectedChannel* scB = new te::se::SelectedChannel();
  scB->setSourceChannelName("2");
  cs->setBlueChannel(scB);

  rs->setChannelSelection(cs);

  //add symbolizer to a layer style
  te::se::Rule* r = new te::se::Rule();
  r->push_back(rs);

  te::se::Style* s = new te::se::CoverageStyle();
  s->push_back(r);

  l->setStyle(s);

  bool cancel = false;

  l->draw(c, *e, srid, 0, &cancel);

  paint(c, generatePNG, "RGB_012_G_Contrast_Style");
 }

void RGB_012_RGB_Contrast_Style(te::qt::widgets::Canvas* c, te::map::DataSetLayer* l, te::gm::Envelope* e, int srid)
 {
  //create default raster symbolizer
  te::se::RasterSymbolizer* rs = new te::se::RasterSymbolizer();

  //set transparency
  rs->setOpacity(new te::se::ParameterValue("1.0"));

  //set channel selection
  te::se::ChannelSelection* cs = new te::se::ChannelSelection();

  //channel R
  te::se::SelectedChannel* scR = new te::se::SelectedChannel();
  scR->setSourceChannelName("0");

  te::se::ContrastEnhancement* cR = new te::se::ContrastEnhancement();
  cR->setGammaValue(0.5);
  scR->setContrastEnhancement(cR);
  cs->setRedChannel(scR);

  //channel G
  te::se::SelectedChannel* scG = new te::se::SelectedChannel();
  scG->setSourceChannelName("1");

  te::se::ContrastEnhancement* cG = new te::se::ContrastEnhancement();
  cG->setGammaValue(0.5);
  scG->setContrastEnhancement(cG);
  cs->setGreenChannel(scG);

  //channel B
  te::se::SelectedChannel* scB = new te::se::SelectedChannel();
  scB->setSourceChannelName("2");

  te::se::ContrastEnhancement* cB = new te::se::ContrastEnhancement();
  cB->setGammaValue(0.5);
  scB->setContrastEnhancement(cB);
  cs->setBlueChannel(scB);

  rs->setChannelSelection(cs);

  //add symbolizer to a layer style
  te::se::Rule* r = new te::se::Rule();
  r->push_back(rs);

  te::se::Style* s = new te::se::CoverageStyle();
  s->push_back(r);

  l->setStyle(s);

  bool cancel = false;

  l->draw(c, *e, srid, 0, &cancel);

  paint(c, generatePNG, "RGB_012_RGB_Contrast_Style");
 }

void MONO_0_Style(te::qt::widgets::Canvas* c, te::map::DataSetLayer* l, te::gm::Envelope* e, int srid)
 {
  //create default raster symbolizer
  te::se::RasterSymbolizer* rs = new te::se::RasterSymbolizer();

  //set transparency
  rs->setOpacity(new te::se::ParameterValue("1.0"));

  //set channel selection
  te::se::ChannelSelection* cs = new te::se::ChannelSelection();

  //channel M
  te::se::SelectedChannel* scM = new te::se::SelectedChannel();
  scM->setSourceChannelName("0");
  cs->setGrayChannel(scM);

  rs->setChannelSelection(cs);

  //add symbolizer to a layer style
  te::se::Rule* r = new te::se::Rule();
  r->push_back(rs);

  te::se::Style* s = new te::se::CoverageStyle();
  s->push_back(r);

  l->setStyle(s);

  bool cancel = false;

  l->draw(c, *e, srid, 0, &cancel);

  paint(c, generatePNG, "MONO_0_Style");
 }

void MONO_2_Style(te::qt::widgets::Canvas* c, te::map::DataSetLayer* l, te::gm::Envelope* e, int srid)
 {
  //create default raster symbolizer
  te::se::RasterSymbolizer* rs = new te::se::RasterSymbolizer();

  //set transparency
  rs->setOpacity(new te::se::ParameterValue("1.0"));

  //set channel selection
  te::se::ChannelSelection* cs = new te::se::ChannelSelection();

  //channel M
  te::se::SelectedChannel* scM = new te::se::SelectedChannel();
  scM->setSourceChannelName("2");
  cs->setGrayChannel(scM);

  rs->setChannelSelection(cs);

  //add symbolizer to a layer style
  te::se::Rule* r = new te::se::Rule();
  r->push_back(rs);

  te::se::Style* s = new te::se::CoverageStyle();
  s->push_back(r);

  l->setStyle(s);

  bool cancel = false;

  l->draw(c, *e, srid, 0, &cancel);

  paint(c, generatePNG, "MONO_2_Style");
 }

void RED_Style(te::qt::widgets::Canvas* c, te::map::DataSetLayer* l, te::gm::Envelope* e, int srid)
 {
  //create default raster symbolizer
  te::se::RasterSymbolizer* rs = new te::se::RasterSymbolizer();

  //set transparency
  rs->setOpacity(new te::se::ParameterValue("1.0"));

  //set channel selection
  te::se::ChannelSelection* cs = new te::se::ChannelSelection();

  //channel R
  te::se::SelectedChannel* scR = new te::se::SelectedChannel();
  scR->setSourceChannelName("0");
  cs->setRedChannel(scR);

  rs->setChannelSelection(cs);

  //add symbolizer to a layer style
  te::se::Rule* r = new te::se::Rule();
  r->push_back(rs);

  te::se::Style* s = new te::se::CoverageStyle();
  s->push_back(r);

  l->setStyle(s);

  bool cancel = false;

  l->draw(c, *e, srid, 0, &cancel);

  paint(c, generatePNG, "RED_Style");
 }

void DrawRasterStyledLayers()
{
  // If you want to use any Qt resource you must initialize a QApplication in some place!
  int argc = 0;
  QApplication app(argc, 0);

  try
  {
    // Creates a layer of raster
    std::auto_ptr<te::map::DataSetLayer> rasterLayer(CreateRasterLayer(TERRALIB_DATA_DIR "/rasters/cbers2b_rgb342_crop.tif"));

    // Get the box to be painted
    te::gm::Envelope* extent = new te::gm::Envelope(rasterLayer->getExtent());

    // Get the projection used to be paint the raster
    std::auto_ptr<te::rst::Raster> raster(te::map::GetRaster(rasterLayer.get()));
    int srid = raster->getSRID();
    //int srid = 4618; // LL SAD69

    // Creates a canvas
    te::qt::widgets::Canvas* canvas = CreateCanvas(rasterLayer.get(), extent, srid);

    // RGB 012 Style
    //RGB_012_Style(canvas, rasterLayer.get(), extent, srid);

    // RGB 012 with transparency Style
    RGB_012_Transp_Style(canvas, rasterLayer.get(), extent, srid);

    // RGB 012 Style
    RGB_102_Style(canvas, rasterLayer.get(), extent, srid);

    // RGB 012 with contrast in green band Style
    RGB_012_G_Contrast_Style(canvas, rasterLayer.get(), extent, srid);

    // RGB 012 with contrast in RGB bands Style
    RGB_012_RGB_Contrast_Style(canvas, rasterLayer.get(), extent, srid);

    // Mono band 0 Style
    MONO_0_Style(canvas, rasterLayer.get(), extent, srid);

    // Mono band 2 Style
    MONO_2_Style(canvas, rasterLayer.get(), extent, srid);

    delete extent;
    delete canvas;
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
