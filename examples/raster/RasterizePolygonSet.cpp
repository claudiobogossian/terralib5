#include "RasterExamples.h"

// TerraLib
#include <terralib/gdal/Utils.h>
#include <terralib/dataaccess.h>
#include <terralib/dataaccess/datasource/DataSourceFactory.h>
#include <terralib/geometry.h>
#include <terralib/raster.h>

// STL
#include <iostream>
#include <string>

void RasterizePolygonSet()
{
  try
  {
    std::cout << "This is a test to open a shapefile and save some polygons in a raster file." << std::endl << std::endl;

    std::map<std::string, std::string> ogrInfo;
    ogrInfo["SOURCE"] = TERRALIB_EXAMPLES_DATA_DIR"/shp/munic_2001.shp";
    std::auto_ptr<te::da::DataSource> ds_pols = te::da::DataSourceFactory::make("OGR");
    ds_pols->setConnectionInfo(ogrInfo);
    ds_pols->open();

// get a transactor to interact to the data source
    std::auto_ptr<te::da::DataSourceTransactor> transactor = ds_pols->getTransactor();
    std::vector<std::string> datasets = transactor->getDataSetNames();

    for(unsigned int i=0; i<datasets.size(); ++i)
    {
// retrieve the dataset by its name
      std::auto_ptr<te::da::DataSet> dataset = transactor->getDataSet(datasets[i]);

      while(dataset->moveNext())
      {
// select arbitrary places using attribute "nome" from database
        if((dataset->getString("nome")=="Curitiba")||
           (dataset->getString("nome")=="Porto Alegre"))
        {
          std::cout << "  Drawing city " << dataset->getString("nome") << std::endl;

// retrieve polygon's geometry
          std::size_t geomPos = te::da::GetFirstPropertyPos(dataset.get(), te::dt::GEOMETRY_TYPE);
          std::auto_ptr<te::gm::Geometry> geometry = dataset->getGeometry(geomPos);
          te::gm::Coord2D ll = geometry->getMBR()->getLowerLeft();
          te::gm::Coord2D ur = geometry->getMBR()->getUpperRight();
          te::gm::Envelope* envelope = new te::gm::Envelope(ll.x, ll.y, ur.x, ur.y);

// define raster properties
          int srid = 29183;
          double factor = 0.0005;
          double zerox = envelope->getLowerLeftX();
          double geot[6] = {zerox, factor, 0.0, ur.y, 0.0, -factor};

// describes the raster that you want
          std::string rname = dataset->getString("nome")+".tif";
          te::rst::Grid* grid = new te::rst::Grid((unsigned)(envelope->getWidth()/factor + 1),
                                                  (unsigned)(envelope->getHeight()/factor + 1));
          grid->setGeoreference(geot, srid);
          std::vector<te::rst::BandProperty*> bprops;
          bprops.push_back(new te::rst::BandProperty(0, te::dt::UCHAR_TYPE));

// create raster and rasterize with the selected polygon
          std::map<std::string, std::string> orinfo;
          orinfo["URI"] = TERRALIB_EXAMPLES_DATA_DIR"/rasters/" + rname;
          te::rst::Raster* rout = te::rst::RasterFactory::make(grid, bprops, orinfo);
          std::vector<te::gm::Geometry*> vgeometry;
          vgeometry.push_back(geometry.get());
          std::vector<double> colors;
          colors.push_back(127.0);
          rout->rasterize(vgeometry, colors, 0);
          delete rout;
        }
      }
    }
    std::cout << "Done!" << std::endl << std::endl;
  }
  catch(const std::exception& e)
  {
    std::cout << std::endl << "An exception has occurried in RasterizePolygonSet(): " << e.what() << std::endl;
  }
  catch(...)
  {
    std::cout << std::endl << "An unexpected exception has occurried in RasterizePolygonSet()!" << std::endl;
  }
}
