#include "MapToolsExamples.h"

// TerraLib
#include <terralib/common.h>
#include <terralib/geometry.h>
#include <terralib/dataaccess.h>
#include <terralib/maptools.h>
#include <terralib/qt/widgets/canvas/MapDisplay.h>

// STL
#include <iostream>
#include <list>

// Qt
#include <QtGui/QApplication>

void MapDisplay()
{
// If you want to use any Qt resource you must initialize a QApplication in some place!
  int argc = 0;
  QApplication app(argc, 0);

  try
  {
    std::map<std::string, std::string> connInfo;
    connInfo["path"] = "./data/shp" ;

    // Creates and connects data source
    te::da::DataSource* ds = te::da::DataSourceFactory::make("OGR");
    if(ds == 0)
      throw te::common::Exception("Sorry, I can not create the OGR driver!");
    ds->open(connInfo);
    
    te::da::DataSourceTransactor* transactor = ds->getTransactor();
    te::da::DataSourceCatalogLoader* cl = transactor->getCatalogLoader();
    cl->loadCatalog();

    // Get the number of data set types that belongs to the data source
    std::vector<std::string*> datasets;
    transactor->getCatalogLoader()->getDataSets(datasets);

    te::qt::widgets::MapDisplay* mapDisplay = new te::qt::widgets::MapDisplay(QSize(700, 500));

    // MapDisplay box
    te::gm::Envelope env;

    std::map<te::gm::GeomType, te::se::Style*> styles;
    styles[te::gm::PolygonType] = SimplePolygonStyle();
    styles[te::gm::LineStringType] = SimpleLineStyle();
    styles[te::gm::PointType] = MarkPointStyle("circle");

    // Layer list
    std::vector<std::string*>::iterator it;
    std::list<te::map::AbstractLayer*> layerList;
    int id = 0;
    for(it = datasets.begin(); it != datasets.end(); ++it)
    {
      const std::string* s = *it;
      te::da::DataSetType* dt = cl->getDataSetType(*s);
      if(!dt->hasGeom())
        continue;

      // To MapDisplay extent
      te::gm::Envelope* e = cl->getExtent(dt->getDefaultGeomProperty());
      env.Union(*e);
      delete e;
      
      // Creates a Layer
      te::map::Layer* layer = new te::map::Layer(te::common::Convert2String(++id), *s);
      layer->setDataSource(ds);
      layer->setDataSetName(*s);
      layer->setVisibility(te::map::VISIBLE);
      layer->setStyle(styles[dt->getDefaultGeomProperty()->getGeometryType()]);

      // Creates a Layer Renderer
      te::map::LayerRenderer* r = new te::map::LayerRenderer();
      layer->setRenderer(r);

      layerList.push_back(layer);
    }

    mapDisplay->setMinimumSize(QSize(60, 60));
    mapDisplay->setResizePolicy(te::qt::widgets::MapDisplay::Center);
    mapDisplay->setLayerList(layerList);
    mapDisplay->show();
    mapDisplay->setExtent(env);

    app.exec();

    delete mapDisplay;
    te::common::FreeContents(layerList);
    delete cl;
    delete transactor;
    delete ds;
  }
  catch(const std::exception& e)
  {
    std::cout << std::endl << "An exception has occuried in DrawLayer example: " << e.what() << std::endl;
  }
  catch(...)
  {
    std::cout << std::endl << "An unexpected exception has occuried in DrawLayer example!" << std::endl;
  }
}
