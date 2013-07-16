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
#include <memory>

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
    te::da::DataSourcePtr dataSource = te::da::DataSourceManager::getInstance().open(te::common::Convert2String(G_ID++), "OGR", connInfo);

    std::auto_ptr<te::da::DataSourceTransactor> transactor(dataSource->getTransactor());
    std::auto_ptr<te::da::DataSourceCatalogLoader> cl(transactor->getCatalogLoader());
    cl->loadCatalog();

    // Get the number of data set types that belongs to the data source
    boost::ptr_vector<std::string> datasets;
    transactor->getCatalogLoader()->getDataSets(datasets);

    std::auto_ptr<te::qt::widgets::MapDisplay> mapDisplay(new te::qt::widgets::MapDisplay(QSize(700, 500)));

    // MapDisplay box
    te::gm::Envelope env;

    std::map<te::gm::GeomType, te::se::Style*> styles;
    styles[te::gm::PolygonType] = SimplePolygonStyle();
    styles[te::gm::LineStringType] = SimpleLineStyle();
    styles[te::gm::PointType] = MarkPointStyle("circle");

    // Layer list
    std::vector<std::string*>::iterator it;
    std::list<te::map::AbstractLayerPtr> layerList;
    int id = 0;
    for(unsigned int i=0; i<datasets.size(); ++i)
    {
      te::da::DataSetType* dt = cl->getDataSetType(datasets[i]);
      if(!dt->hasGeom())
        continue;

      // To MapDisplay extent
      te::gm::Envelope* e = cl->getExtent(dt->findFirstPropertyOfType(te::dt::GEOMETRY_TYPE)); //getDefaultGeomProperty());

      env.Union(*e);
      delete e;
      
      // Creates a Layer
      te::map::DataSetLayer* layer = new te::map::DataSetLayer(te::common::Convert2String(++id), datasets[i]);
      layer->setDataSourceId(dataSource->getId());
      layer->setDataSetName(datasets[i]);
      layer->setVisibility(te::map::VISIBLE);
      layer->setStyle(styles[te::da::GetFirstGeomProperty(dt)->getGeometryType()]);
      layer->setRendererType("DATASET_LAYER_RENDERER");

      layerList.push_back(layer);
    }

    mapDisplay->setMinimumSize(QSize(60, 60));
    mapDisplay->setResizePolicy(te::qt::widgets::MapDisplay::Center);
    mapDisplay->setLayerList(layerList);
    mapDisplay->show();
    mapDisplay->setExtent(env);

    app.exec();

    layerList.clear();
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
