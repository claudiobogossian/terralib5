#include "RasterExamples.h"

// TerraLib
#include <terralib/gdal/Utils.h>
#include <terralib/dataaccess.h>
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

    std::string ogrInfo = "connection_string="TE_DATA_EXAMPLE_LOCALE"/data/shp/munic_2001.shp";
    te::da::DataSource* ds_pols = te::da::DataSourceFactory::make("OGR");
    ds_pols->open(ogrInfo);

// get a transactor to interact to the data source
    te::da::DataSourceTransactor* transactor = ds_pols->getTransactor();

// from transactor, take a catalog loader to find out the datasets stored in the data source
    te::da::DataSourceCatalogLoader* cloader = transactor->getCatalogLoader();

// now retrieve the name of the datasets
    std::vector<std::string*> datasets;
    cloader->getDataSets(datasets);

    for(std::vector<std::string*>::const_iterator it = datasets.begin(); it < datasets.end(); ++it)
    {
// retrieve the dataset by its name
      const std::string* datasetName = *it;
      te::da::DataSet* dataset = transactor->getDataSet(*datasetName);

      while(dataset->moveNext())
      {
// select arbitrary places using attribute "nome" from database
        if((dataset->getString("nome")=="Curitiba")||
           (dataset->getString("nome")=="Porto Alegre"))
        {
          std::cout << "  Drawing city " << dataset->getString("nome") << std::endl;

// retrieve polygon's geometry
          te::gm::Geometry* geometry = dataset->getGeometry();
          te::gm::Coord2D ll = geometry->getMBR()->getLowerLeft();
          te::gm::Coord2D ur = geometry->getMBR()->getUpperRight();
          te::gm::Envelope* envelope = new te::gm::Envelope(ll.x, ll.y, ur.x, ur.y);

// define raster properties
          int srid = 29183;
          double factor = 0.0005;
          double zerox = envelope->getLowerLeftX();
          double geot[6] = {zerox, factor, 0.0, ur.y, 0.0, -factor};

// describes the raster that you want
          std::string rname= dataset->getString("nome")+".tif";
          te::rst::Grid* grid = new te::rst::Grid((unsigned)(envelope->getWidth()/factor + 1),
                                                  (unsigned)(envelope->getHeight()/factor + 1));
          grid->setGeoreference(geot, srid);

          std::vector<te::rst::BandProperty*> bprops;
          bprops.push_back(new te::rst::BandProperty(0, te::dt::UCHAR_TYPE));

          std::map<std::string, std::string> rinfo;
          te::rst::RasterProperty* rstp = new te::rst::RasterProperty(grid, bprops, rinfo);

          te::da::DataSetType* dstp = new te::da::DataSetType(rname);
          dstp->add(rstp);

// access a datasource to persist it
          std::map<std::string, std::string> connInfoRaster;
          connInfoRaster["URI"] = ""TE_DATA_EXAMPLE_LOCALE"/data/rasters";
          te::da::DataSource* ds = te::da::DataSourceFactory::make("GDAL");
          ds->open(connInfoRaster);

          te::da::DataSourceTransactor* tr = ds->getTransactor();
          te::da::DataSetTypePersistence* pers = tr->getDataSetTypePersistence();

// manipulate it using data set type persistence;
          pers->create(dstp);

// access the data set
          te::da::DataSet* dset = tr->getDataSet(rname, te::common::FORWARDONLY, te::common::RWAccess);
          te::rst::Raster* rst = dset->getRaster();

// fill Raster with PolygonSet information
          std::vector<te::gm::Geometry*> vgeometry;
          vgeometry.push_back(geometry);
          te::gdal::Rasterize(vgeometry, (static_cast<te::gdal::Raster*>(rst))->getGDALDataset());

          delete pers;
          delete tr;
          delete ds;
          delete rst;
          delete dset;
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
