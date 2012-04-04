#include "DataAccessExamples.h"

#include <terralib/dataaccess.h>

#include <iostream>

// This example shows how to instantiate a single raster data file as a GDAL data source.
void OpenFile()
{
  try
  {
    te::da::DataSource* dsGDAL = te::da::DataSourceFactory::make("GDAL");

    bool res = dsGDAL->isOpened();  // expect false;

    std::map<std::string, std::string> connInfo;
    connInfo["SOURCE"] = ""TE_DATA_EXAMPLE_LOCALE"\\data\\rasters\\cbers2b_rgb342_crop.tif" ;  
    dsGDAL->open(connInfo);
    res = dsGDAL->isOpened();  // expect true
    res = dsGDAL->isValid();   // expect true

    PrintDataSets(dsGDAL);

    delete dsGDAL;
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
    connInfo["URI"] = ""TE_DATA_EXAMPLE_LOCALE"\\data\\rasters" ;  
    te::da::DataSource* ds = te::da::DataSourceFactory::make("GDAL");

    bool res;
    res = ds->isOpened();  // expect false

    ds->open(connInfo);
    res = ds->isOpened();  // expect true
    res = ds->isValid();   // expect true

    PrintDataSets(ds);

    delete ds;
  }
  catch(const std::exception& e)
  {
    std::cout << std::endl << "An exception has occuried in GDALExample OpenDirectory(): " << e.what() << std::endl;
  }
  catch(...)
  {
    std::cout << std::endl << "An unexpected exception has occuried in GDALExample OpenDirectory()!" << std::endl;
  }

}

// This examples shows how to use a Data Source Transactor to retrieve information about a raster data file.
void DataSourceTransactor()
{
  try
  {
    std::map<std::string, std::string> connInfo;
    connInfo["URI"] = ""TE_DATA_EXAMPLE_LOCALE"\\data\\rasters";
    te::da::DataSource* ds = te::da::DataSourceFactory::make("GDAL");
    ds->open(connInfo);

    te::da::DataSourceTransactor* tr = ds->getTransactor();  // caller gets the pointer ownership, delete it later
    te::da::DataSourceCatalogLoader* cl = tr->getCatalogLoader(); // caller gets the pointer ownership, delete it later
    cl->loadCatalog();

    if (cl->datasetExists("cbers2b_rgb342_crop.tif"))
    {
      te::da::DataSet* dtset = tr->getDataSet("cbers2b_rgb342_crop.tif");  // caller gets the pointer ownership, delete it later
      PrintDataSet(dtset);
      delete dtset;
    }

    delete cl;
    delete tr;
    delete ds;
  }
  catch(const std::exception& e)
  {
    std::cout << std::endl << "An exception has occuried in GDALExample DataSourceTransactor(): " << e.what() << std::endl;
  }
  catch(...)
  {
    std::cout << std::endl << "An unexpected exception has occuried in GDALExample DataSourceTransactor()!" << std::endl;
  }
}

void GDALExample()
{
#if TE_EXAMPLE_USE_GDAL
  OpenFile();
  OpenDirectory();
  DataSourceTransactor();
#endif
}