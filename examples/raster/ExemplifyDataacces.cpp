#include "RasterExamples.h"

#include <terralib/dataaccess.h>
#include <terralib/dataaccess/datasource/DataSourceFactory.h>
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
    connInfo["URI"] = TERRALIB_DATA_DIR "/rasters";

    std::auto_ptr<te::da::DataSource> ds = te::da::DataSourceFactory::make("GDAL");
    ds->setConnectionInfo(connInfo);
    ds->open();

    std::auto_ptr<te::da::DataSourceTransactor> tr = ds->getTransactor();

    // manipulate it using data set type persistence;
    tr->createDataSet(dstp, std::map<std::string, std::string> ());
    tr->renameDataSet(dstp->getName(), "test2.tif");
    tr->dropDataSet(dstp->getName());
  }
  catch(const std::exception& e)
  {
    std::cout << std::endl << "An exception has occurred in DataAccess DataSetTypePersistence(): " << e.what() << std::endl;
  }
  catch(...)
  {
    std::cout << std::endl << "An unexpected exception has occurred in DataAccess DataSetTypePersistence()!" << std::endl;
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
    connInfo["URI"] = TERRALIB_DATA_DIR "/rasters";

    std::auto_ptr<te::da::DataSource> ds = te::da::DataSourceFactory::make("GDAL");
    ds->setConnectionInfo(connInfo);
    ds->open();

    std::auto_ptr<te::da::DataSourceTransactor> tr = ds->getTransactor();
    tr->createDataSet(dstp, std::map<std::string, std::string> ());

// access the data set
    //std::auto_ptr<te::da::DataSet> dset = tr->getDataSet("test.tif", te::common::FORWARDONLY, te::common::RWAccess);
    std::auto_ptr<te::da::DataSet> dset = tr->getDataSet("test.tif", te::common::FORWARDONLY);

// access the raster
    std::size_t rpos = te::da::GetFirstPropertyPos(dset.get(), te::dt::RASTER_TYPE);
    std::auto_ptr<te::rst::Raster> rst = dset->getRaster(rpos);
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
    std::cout << std::endl << "Raster test.tif was successfully created, check your data source directory." << std::endl;
  }
  catch(const std::exception& e)
  {
    std::cout << std::endl << "An exception has occurred in DataAccess DataSet(): " << e.what() << std::endl;
  }
  catch(...)
  {
    std::cout << std::endl << "An unexpected exception has occurred in DataAccess DataSet()!" << std::endl;
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
    connInfo["URI"] = TERRALIB_DATA_DIR "/rasters";

    std::auto_ptr<te::da::DataSource> ds = te::da::DataSourceFactory::make("GDAL");
    ds->setConnectionInfo(connInfo);
    ds->open();

    std::auto_ptr<te::da::DataSourceTransactor> tr = ds->getTransactor();
    std::auto_ptr<te::da::DataSetType> dt_tiff = tr->getDataSetType("test.tif");
    std::auto_ptr<te::da::DataSet> ds_tiff = tr->getDataSet("test.tif");

    //te::da::DataSetType* dt_tiff2 = new te::da::DataSetType("test2.tif");
    te::rst::RasterProperty* rstp = static_cast<te::rst::RasterProperty*>(dt_tiff->getProperties()[0]->clone());
    rstp->setName("test2.tif");
    //dt_tiff2->add(rstp);

    //tr->add(dt_tiff2, rstp);
//    te::da::DataSetTypePersistence* datasetTypePersistence = tr->getDataSetTypePersistence();
//    datasetTypePersistence->add(dt_tiff2, rstp);

//    te::da::DataSetPersistence* pers = tr->getDataSetPersistence();
    //tr->add("test2.tif", ds_tiff);
    //tr->remove("test2.tif");
  }
  catch(const std::exception& e)
  {
    std::cout << std::endl << "An exception has occurred in DataAccess DataSetPersistence(): " << e.what() << std::endl;
  }
  catch(...)
  {
    std::cout << std::endl << "An unexpected exception has occurred in DataAccess DataSetPersistence()!" << std::endl;
  }
  std::cout << "=== End example DataSetPersistence === " << std::endl;
}

void DataSource()
{
  try
  {
    std::cout << "=== Example Data Source === " << std::endl;
    std::map<std::string, std::string> connInfo;
    connInfo["URI"] = TERRALIB_DATA_DIR "/rasters";

    std::auto_ptr<te::da::DataSource> ds = te::da::DataSourceFactory::make("GDAL");
    ds->setConnectionInfo(connInfo);
    ds->open();

    std::auto_ptr<te::da::DataSourceTransactor> transactor = ds->getTransactor();
    std::vector<std::string> datasets = transactor->getDataSetNames();

    std::cout << "List of raster datasets in " << connInfo["URI"] << ":";
    for(size_t i = 0; i < datasets.size(); ++i)
    {
      std::auto_ptr<te::da::DataSet> ds = transactor->getDataSet(datasets[i]);
      std::auto_ptr<te::rst::Raster> rst = ds->getRaster(0);
      std::cout << std::endl;
      std::cout << rst->toString();
      std::cout << std::endl;
    }
    std::cout << "=== End example Data Source === " << std::endl;
  }
  catch(const std::exception& e)
  {
    std::cout << std::endl << "An exception has occurred in RasterDataAccessExamples DataSetPersistence(): " << e.what() << std::endl;
  }
  catch(...)
  {
    std::cout << std::endl << "An unexpected exception has occurred in RasterDataAccessExamples DataSetPersistence()!" << std::endl;
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
