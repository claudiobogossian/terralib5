#include "RasterExamples.h"

#include <terralib/dataaccess.h>
#include <terralib/raster.h>
#include <terralib/geometry.h>

#include <iostream>
#include <map>
#include <string>

// This example shows how to use a Data Set Type Persistence to create a raster data file.
void DataSetTypePersistence()
{
  try
  {
    std::cout << "=== Example DataSetTypePersistence === " << std::endl;
    // describes the raster that you want
    te::rst::Grid* grid = new te::rst::Grid(100, 100);
    grid->setGeoreference(te::gm::Coord2D(100, 100), 29183, 1,1);

    std::vector<te::rst::BandProperty*> bprops;
    bprops.push_back(new te::rst::BandProperty(0, te::dt::UCHAR_TYPE));

    std::map<std::string, std::string> rinfo;

    te::rst::RasterProperty* rstp = new te::rst::RasterProperty(grid, bprops, rinfo);

    te::da::DataSetType* dstp = new te::da::DataSetType("test.tif");
    dstp->add(rstp);
   
    // access a datasource to persist it
    std::map<std::string, std::string> connInfo;
    connInfo["URI"] = ""TE_DATA_EXAMPLE_DIR"/data/rasters";

    te::da::DataSource* ds = te::da::DataSourceFactory::make("GDAL");
    ds->open(connInfo);

    te::da::DataSourceTransactor* tr = ds->getTransactor(); 
    te::da::DataSetTypePersistence* pers = tr->getDataSetTypePersistence(); 

    // manipulate it using data set type persistence;
    pers->create(dstp);

    pers->rename(dstp, "test2.tif");

    pers->drop(dstp);

    delete pers;
    delete tr;
    delete ds;
  }
  catch(const std::exception& e)
  {
    std::cout << std::endl << "An exception has occuried in DataAccess DataSetTypePersistence(): " << e.what() << std::endl;
  }
  catch(...)
  {
    std::cout << std::endl << "An unexpected exception has occuried in DataAccess DataSetTypePersistence()!" << std::endl;
  }
  std::cout << "=== End example DataSetTypePersistence === " << std::endl;
}

// This example shows how to access a raster data file using the Data Set interface.
void DataSet()
{
  try
  {
    std::cout << "=== Example Data Set === " << std::endl;

// describes the raster that you want
    te::rst::Grid* grid = new te::rst::Grid(100,100);
    grid->setGeoreference(te::gm::Coord2D(100,100), 29183, 1,1);

    std::vector<te::rst::BandProperty*> bprops;
    bprops.push_back(new te::rst::BandProperty(0, te::dt::UCHAR_TYPE));

    std::map<std::string, std::string> rinfo;

    te::rst::RasterProperty* rstp = new te::rst::RasterProperty(grid, bprops, rinfo);

    te::da::DataSetType* dstp = new te::da::DataSetType("test.tif");
    dstp->add(rstp);

// access a datasource to persist it
    std::map<std::string, std::string> connInfo;
    connInfo["URI"] = ""TE_DATA_EXAMPLE_DIR"/data/rasters";

    te::da::DataSource* ds = te::da::DataSourceFactory::make("GDAL");
    ds->open(connInfo);

    te::da::DataSourceTransactor* tr = ds->getTransactor(); 
    te::da::DataSetTypePersistence* pers = tr->getDataSetTypePersistence();
    pers->create(dstp);

// access the data set
    te::da::DataSet* dset = tr->getDataSet("test.tif",te::common::FORWARDONLY, te::common::RWAccess);

// access the raster
    std::size_t rpos = te::da::GetFirstPropertyPos(dset, te::dt::RASTER_TYPE);
    te::rst::Raster* rst = dset->getRaster(rpos);
    for (unsigned int b = 0; b < rst->getNumberOfBands(); ++b)
    {
      for (unsigned int l = 0; l < rst->getNumberOfRows(); ++l)
      {
        for (unsigned int c = 0; c < rst->getNumberOfColumns(); ++c)
        {
          rst->setValue(c,l,l*2,b);
        }
      }
    }
    delete rst;
    delete dset;
    delete pers;
    delete tr;
    delete ds;
    std::cout << std::endl << "Raster test.tif was successfully created, check your data source directory." << std::endl;
  }
  catch(const std::exception& e)
  {
    std::cout << std::endl << "An exception has occuried in DataAccess DataSet(): " << e.what() << std::endl;
  }
  catch(...)
  {
    std::cout << std::endl << "An unexpected exception has occuried in DataAccess DataSet()!" << std::endl;
  }
  std::cout << "=== End example Data Set === " << std::endl;
}

// This examples shows how to use the Data Set Persistence interface to create and manipulate a raster data file.
void DataSetPersistence()
{
  try
  {
    std::cout << "=== Example DataSetPersistence === " << std::endl;
    std::map<std::string, std::string> connInfo;
    connInfo["URI"] = ""TE_DATA_EXAMPLE_DIR"/data/rasters";

    te::da::DataSource* ds = te::da::DataSourceFactory::make("GDAL");
    ds->open(connInfo);

    te::da::DataSourceTransactor* tr = ds->getTransactor();
    te::da::DataSetType* dt_tiff = tr->getCatalogLoader()->getDataSetType("test.tif");
    te::da::DataSet* ds_tiff = tr->getDataSet("test.tif");

    te::da::DataSetType* dt_tiff2 = new te::da::DataSetType("test2.tif");
    te::rst::RasterProperty* rstp = static_cast<te::rst::RasterProperty*>(dt_tiff->getProperties()[0]->clone());
    rstp->setName("test2.tif");
    //dt_tiff2->add(rstp);

    te::da::DataSetTypePersistence* datasetTypePersistence = tr->getDataSetTypePersistence();
    datasetTypePersistence->add(dt_tiff2, rstp);

    te::da::DataSetPersistence* pers = tr->getDataSetPersistence();
    pers->add("test2.tif", ds_tiff);
    //pers->create(dt_tiff2,ds_tiff);

    //pers->remove(dt_tiff2);
    pers->remove("test2.tif");

    delete pers;
    delete tr;
    delete ds;
  }
  catch(const std::exception& e)
  {
    std::cout << std::endl << "An exception has occuried in DataAccess DataSetPersistence(): " << e.what() << std::endl;
  }
  catch(...)
  {
    std::cout << std::endl << "An unexpected exception has occuried in DataAccess DataSetPersistence()!" << std::endl;
  }
  std::cout << "=== End example DataSetPersistence === " << std::endl;
}

void DataSource()
{
  try
  {
    std::cout << "=== Example Data Source === " << std::endl;
    std::map<std::string, std::string> connInfo;
    connInfo["URI"] = ""TE_DATA_EXAMPLE_DIR"/data/rasters";

    te::da::DataSource* ds = te::da::DataSourceFactory::make("GDAL");
    ds->open(connInfo);

    te::da::DataSourceTransactor* transactor = ds->getTransactor();

    te::da::DataSourceCatalogLoader* cloader = transactor->getCatalogLoader();

    boost::ptr_vector<std::string> datasets;

    cloader->getDataSets(datasets);

    std::cout << "List of raster datasets in " << connInfo["URI"] << ":";
    for(size_t i=0; i<datasets.size(); ++i)
    {
      te::da::DataSet* ds = transactor->getDataSet(datasets[i]);
      te::rst::Raster* rst = ds->getRaster(0);
      std::cout << std::endl;
      std::cout << rst->toString();
      std::cout << std::endl;
      delete rst;
      delete ds;
    }
    std::cout << "=== End example Data Source === " << std::endl;
  }
  catch(const std::exception& e)
  {
    std::cout << std::endl << "An exception has occuried in RasterDataAccessExamples DataSetPersistence(): " << e.what() << std::endl;
  }
  catch(...)
  {
    std::cout << std::endl << "An unexpected exception has occuried in RasterDataAccessExamples DataSetPersistence()!" << std::endl;
  }
}

void ExemplifyDataAccess()
{
#if TE_EXAMPLE_USE_GDAL
  DataSource();
  DataSet();
  DataSetPersistence();
  DataSetTypePersistence();
#endif
}