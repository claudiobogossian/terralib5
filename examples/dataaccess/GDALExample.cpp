
#include "../Config.h"
#include "DataAccessExamples.h"

#include <iostream>

// This example shows how to instantiate a single raster data file as a GDAL data source.
void OpenFile()
{
  try
  {
    std::auto_ptr<te::da::DataSource> dsGDAL = te::da::DataSourceFactory::make("GDAL");
    std::cout << "Datasource is opened? " << std::boolalpha << dsGDAL->isOpened() << '\n' << '\n';
  
    std::string data_dir = TERRALIB_DATA_DIR;

    std::map<std::string, std::string> connInfo;
    connInfo["URI"] = data_dir + "/rasters/cbers2b_rgb342_crop.tif";
  
    dsGDAL->setConnectionInfo(connInfo);
    dsGDAL->open();
  
    std::cout << "Datasource is opened? " << std::boolalpha << dsGDAL->isOpened() << '\n' << '\n';
    std::cout << "Datasource is valid? " << std::boolalpha << dsGDAL->isValid() << '\n' << '\n';
    
    if (!dsGDAL->isOpened() || !dsGDAL->isValid())
      std::cout << "Datasource " << connInfo["URI"] << " can not be used!\n";

    PrintDataSets(dsGDAL.get());
  }
  catch(const std::exception& e)
  {
    std::cout << std::endl << "An exception has occurred in GDALExample OpenFile(): " << e.what() << std::endl;
  }
  catch(...)
  {
    std::cout << std::endl << "An unexpected exception has occurred in GDALExample OpenFile()!" << std::endl;
  }
}

// This example shows how to directory, with multiple raster data files, as a GDAL data source.
void OpenDirectory()
{
  try
  {
    std::string data_dir = TERRALIB_DATA_DIR;
  
    std::map<std::string, std::string> connInfo;
    connInfo["SOURCE"] = data_dir + "/rasters";
  
    std::auto_ptr<te::da::DataSource> ds = te::da::DataSourceFactory::make("GDAL");
    ds->setConnectionInfo(connInfo);
 
    bool res;
    res = ds->isOpened();  // expect false

    ds->open();
    
    std::cout << "Datasource is opened? " << std::boolalpha << ds->isOpened() << '\n' << '\n';
    std::cout << "Datasource is valid? " << std::boolalpha << ds->isValid() << '\n' << '\n';
    
    if (!ds->isOpened() || !ds->isValid())
      std::cout << "Datasource " << connInfo["SOURCE"] << " can not be used!\n";

    PrintDataSets(ds.get());


  }
  catch(const std::exception& e)
  {
    std::cout << std::endl << "An exception has occurred in GDALExample OpenDirectory(): " << e.what() << std::endl;
  }
  catch(...)
  {
    std::cout << std::endl << "An unexpected exception has occurred in GDALExample OpenDirectory()!" << std::endl;
  }

}

// This examples shows how to use a Data Source Transactor to retrieve information about a raster data file.
void DataSourceTransactor()
{
  try
  {
    std::string data_dir = TERRALIB_DATA_DIR;

    std::map<std::string, std::string> connInfo;
    connInfo["URI"] = data_dir + "/rasters";
  
    std::auto_ptr<te::da::DataSource> ds = te::da::DataSourceFactory::make("GDAL");

    ds->setConnectionInfo(connInfo);
   
    ds->open();

    std::auto_ptr<te::da::DataSourceTransactor> tr = ds->getTransactor();  // caller gets the pointer ownership, delete it later

    std::string dsName("cbers2b_rgb342_crop.tif");
    if (tr->dataSetExists(dsName))
    {
      std::auto_ptr<te::da::DataSet> dtset = tr->getDataSet("cbers2b_rgb342_crop.tif");  // caller gets the pointer ownership, delete it later
      PrintDataSet(dsName,dtset.get());
    }
  }
  catch(const std::exception& e)
  {
    std::cout << std::endl << "An exception has occurred in GDALExample DataSourceTransactor(): " << e.what() << std::endl;
  }
  catch(...)
  {
    std::cout << std::endl << "An unexpected exception has occurred in GDALExample DataSourceTransactor()!" << std::endl;
  }
}

void GDALExample()
{
  OpenFile();
  OpenDirectory();
  DataSourceTransactor();
}