#include "MapToolsExamples.h"

// TerraLib
#include <terralib/common.h>
#include <terralib/geometry.h>
#include <terralib/dataaccess.h>
#include <terralib/maptools.h>
#include <terralib/qt/widgets/canvas/MapDisplay.h>
#include <terralib/qt/widgets/canvas/MultiThreadMapDisplay.h>

// STL
#include <cassert>
#include <iostream>
#include <list>
#include <memory>

// Qt
#include <QApplication>

void MapDisplay()
{
// If you want to use any Qt resource you must initialize a QApplication in some place!
  int argc = 0;
  QApplication app(argc, 0);

  try
  {
    std::map<std::string, std::string> connInfo;
    connInfo["URI"] = TERRALIB_EXAMPLES_DATA_DIR "/shp/munic_2001.shp";

    // Creates and connects data source
    te::da::DataSourcePtr datasource = te::da::DataSourceManager::getInstance().open(te::common::Convert2String(G_ID++), "OGR", connInfo);

    // Get the number of data set types that belongs to the data source
    std::vector<std::string> datasets = datasource->getDataSetNames();

    // Creates the MapDisplay
    std::auto_ptr<te::qt::widgets::MapDisplay> mapDisplay(new te::qt::widgets::MultiThreadMapDisplay(QSize(700, 500)));

    // MapDisplay box
    te::gm::Envelope env;

    // A map of GeomType -> Style
    std::map<te::gm::GeomType, te::se::Style*> styles;
    styles[te::gm::PolygonType] = SimplePolygonStyle();
    styles[te::gm::LineStringType] = SimpleLineStyle();
    styles[te::gm::PointType] = MarkPointStyle("circle");

    // Creates the Layer list
    int id = 0;
    std::list<te::map::AbstractLayerPtr> layerList;
    for(unsigned int i = 0; i < datasets.size(); ++i)
    {
      std::auto_ptr<te::da::DataSetType> dt(datasource->getDataSetType(datasets[i]));

      if(!dt->hasGeom())
        continue;

      te::gm::GeometryProperty* geomProperty = te::da::GetFirstGeomProperty(dt.get());
      assert(geomProperty);

      // To MapDisplay extent
      std::auto_ptr<te::gm::Envelope> e(datasource->getExtent(datasets[i], geomProperty->getName()));
      env.Union(*e);
      
      // Creates a Layer
      te::map::DataSetLayer* layer = new te::map::DataSetLayer(te::common::Convert2String(++id), datasets[i]);
      layer->setDataSourceId(datasource->getId());
      layer->setDataSetName(datasets[i]);
      layer->setVisibility(te::map::VISIBLE);
      layer->setExtent(*e);
      layer->setStyle(styles[geomProperty->getGeometryType()]);
      layer->setRendererType("ABSTRACT_LAYER_RENDERER");

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
