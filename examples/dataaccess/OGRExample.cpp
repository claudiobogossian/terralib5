// Examples
#include "../Config.h"
#include "DataAccessExamples.h"

// TerraLib
#include <terralib/dataaccess.h>
#include <terralib/datatype/Property.h>
#include <terralib/dataaccess/datasource/DataSourceFactory.h>

// STL
#include <iostream>

void OGRExampleRead()
{
  try
  {
    std::string data_dir = TERRALIB_DATA_DIR;
  
    std::string ogrInfo(data_dir + "/shp");
  
    std::map<std::string, std::string> connInfo;
  
    connInfo["URI"] = data_dir + "/shp";
  
    std::auto_ptr<te::da::DataSource> ds = te::da::DataSourceFactory::make("OGR");
    ds->setConnectionInfo(connInfo);
    ds->open();

// retrieve the data source capabilities and print it
    PrintDataSourceCapabilities(ds.get());

// retrieve the information about the data stored in the data source
    //PrintCatalog(ds);

// for each dataset in teh data source print all its data
    PrintDataSets(ds.get());

// release the data source: you are the owner
    //delete ds; //it is auto_ptr
  }
  catch(const std::exception& e)
  {
    std::cout << std::endl << "An exception has occurred in the OGR Example: " << e.what() << std::endl;
  }
  catch(...)
  {
    std::cout << std::endl << "An unexpected exception has occurred in the OGR Example!" << std::endl;
  }
}

void ORGExampleWrite()
{
  try
  {
    //te::da::DataSet* datas = CreatingDataSetInMemory("marisa_soilmeasures");

    //PrintDataSet("marisa_soilmeasures", datas);

    te::da::DataSetType* dt1 = CreateDataSetTypeInMemory("test_marisa");
  
    //Writing in OGR
    std::auto_ptr<te::da::DataSource> ds = te::da::DataSourceFactory::make("OGR");
  
    std::map<std::string, std::string> connInfo;
  
    std::string data_dir = TERRALIB_DATA_DIR;
  
    connInfo["URI"] = data_dir + "/shp";
  
    ds->setConnectionInfo(connInfo);
    ds->open();

    //std::auto_ptr<te::da::DataSetType> dt1 =  datas->getDataSetType("marisa_soilmeasures");
    std::map<std::string, std::string> options;
    //ds->createDataSet(dt1.get(),options);
    //te::da::DataSetType* dt1 = new te::da::DataSetType("marisa_soilmeasures");
    ds->createDataSet(dt1,options);
    te::da::DataSet* datas = CreatingDataSetInMemoryGivingDt(dt1);
    //std::vector<std::string> pnames;
    //std::vector<int> ptypes;
    //te::da::GetPropertyInfo(datas,pnames,ptypes);

    ////Como obter o dataSetType do dataset em memory para criar e criá-lo e depois  adicionar o conteudo...
    ds->add("test_marisa",datas,options);

    ////Checking ...
    //std::auto_ptr<te::da::DataSet> datas_ogr = ds->getDataSet("marisa_soilmeasures");
    //PrintDataSet("marisa_soilmeasures", datas_ogr.get());

    delete datas;
  }
  catch(const std::exception& e)
  {
    std::cout << std::endl << "An exception has occurred in the OGR Example: " << e.what() << std::endl;
  }
  catch(...)
  {
    std::cout << std::endl << "An unexpected exception has occurred in the OGR Example!" << std::endl;
  }
}
//------- Old code -----//
//  #if TE_EXAMPLE_USE_OGR
//  try
//  {
//    te::da::DataSet* datas = CreatingDataSetInMemory();
//
//    //PrintDataSet(datas);
//
//    te::da::DataSetType* newDt = new te::da::DataSetType(*datas->getType());
//    std::map<std::string, std::string> ogrInfo;
//    ogrInfo["connection_string"] = ""TE_DATA_EXAMPLE_DIR"/shp/soilmeasure.shp";
//    te::da::DataSource* ds = te::da::DataSource::create("OGR", ogrInfo);
//
//    te::da::DataSourceTransactor* trans = ds->getTransactor();
//    te::da::DataSetPersistence* dsPers = trans->getDataSetPersistence();
//    dsPers->create(newDt,datas);
//    delete dsPers;
//
//    delete trans;
//    delete datas;
//    delete ds;
//  }

