#include "DataAccessExamples.h"

//#include <terralib/dataaccess/datasource/DataSourceFactory.h>
//#include <terralib/dataaccess.h>

#include <iostream>

// This example shows how to instantiate a single raster data file as a GDAL data source.
void OpenFile()
{
  try
  {
    std::auto_ptr<te::da::DataSource> dsGDAL = te::da::DataSourceFactory::make("GDAL");

    bool res = dsGDAL->isOpened();  // expect false;

    std::map<std::string, std::string> connInfo;
    connInfo["SOURCE"] = ""TE_DATA_EXAMPLE_DIR"/data/rasters/cbers2b_rgb342_crop.tif";
    dsGDAL->setConnectionInfo(connInfo);
    dsGDAL->open();
    res = dsGDAL->isOpened();  // expect true
    res = dsGDAL->isValid();   // expect true

    PrintDataSets(dsGDAL.get());

    //delete dsGDAL;
  }
  catch(const std::exception& e)
  {
    std::cout << std::endl << "An exception has occurried in GDALExample OpenFile(): " << e.what() << std::endl;
  }
  catch(...)
  {
    std::cout << std::endl << "An unexpected exception has occurried in GDALExample OpenFile()!" << std::endl;
  }
}

// This example shows how to directory, with multiple raster data files, as a GDAL data source.
void OpenDirectory()
{
  try
  {
    std::map<std::string, std::string> connInfo;
    connInfo["URI"] = ""TE_DATA_EXAMPLE_DIR"/data/rasters" ; 
    std::auto_ptr<te::da::DataSource> ds = te::da::DataSourceFactory::make("GDAL");
    ds->setConnectionInfo(connInfo);
 
    bool res;
    res = ds->isOpened();  // expect false

    ds->open();
    res = ds->isOpened();  // expect true
    res = ds->isValid();   // expect true

    PrintDataSets(ds.get());


  }
  catch(const std::exception& e)
  {
    std::cout << std::endl << "An exception has occurried in GDALExample OpenDirectory(): " << e.what() << std::endl;
  }
  catch(...)
  {
    std::cout << std::endl << "An unexpected exception has occurried in GDALExample OpenDirectory()!" << std::endl;
  }

}

// This examples shows how to use a Data Source Transactor to retrieve information about a raster data file.
void DataSourceTransactor()
{
  try
  {
    std::map<std::string, std::string> connInfo;
    connInfo["URI"] = ""TE_DATA_EXAMPLE_DIR"/data/rasters";
    std::auto_ptr<te::da::DataSource> ds = te::da::DataSourceFactory::make("GDAL");

    ds->setConnectionInfo(connInfo);
   
    ds->open();

    std::auto_ptr<te::da::DataSourceTransactor> tr = ds->getTransactor();  // caller gets the pointer ownership, delete it later
    //te::da::DataSourceCatalogLoader* cl = tr->getC  getCatalogLoader(); // caller gets the pointer ownership, delete it later
    //cl->loadCatalog();

    if (tr->dataSetExists("cbers2b_rgb342_crop.tif"))
    {
      std::auto_ptr<te::da::DataSet> dtset = tr->getDataSet("cbers2b_rgb342_crop.tif");  // caller gets the pointer ownership, delete it later
      PrintDataSet("cbers2b_rgb342_crop.tif", dtset.get());
    }
  }
  catch(const std::exception& e)
  {
    std::cout << std::endl << "An exception has occurried in GDALExample DataSourceTransactor(): " << e.what() << std::endl;
  }
  catch(...)
  {
    std::cout << std::endl << "An unexpected exception has occurried in GDALExample DataSourceTransactor()!" << std::endl;
  }
}

void GDALExample()
{
  OpenFile();
  OpenDirectory();
  DataSourceTransactor();
}