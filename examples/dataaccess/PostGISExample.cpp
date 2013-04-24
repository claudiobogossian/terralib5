// Examples
#include "DataAccessExamples.h"

// TerraLib
#include <terralib/dataaccess.h>
//#include <../terralib/dataaccess_h.h>
 
// STL
#include <iostream>
#include <exception>

void PostGISExample()
{
#if TE_EXAMPLE_USE_PGIS
  
  try
  {
// let's give the minimal server connection information needed to connect to the database server
    std::map<std::string, std::string> connInfo;
    connInfo["PG_HOST"] = "atlas.dpi.inpe.br" ;   // or "localhost";
    connInfo["PG_USER"] = "postgres";
    connInfo["PG_PASSWORD"] = "sitim110";
    connInfo["PG_DB_NAME"] = "terralib4";
    connInfo["PG_CONNECT_TIMEOUT"] = "4"; 
    
//    connInfo["PG_HOST"] = "localhost" ;   // or "localhost";
//    connInfo["PG_USER"] = "postgres";
//    connInfo["PG_PASSWORD"] = "";
//    connInfo["PG_DB_NAME"] = "bdimages";
//    connInfo["PG_CONNECT_TIMEOUT"] = "4";     

// create a data source using the data source factory
    te::da::DataSource* ds = te::da::DataSourceFactory::make("POSTGIS");

// as we are going to use the data source, let's open it using the connection info above!
    ds->open(connInfo);

// shows how to print information about datasets stored in the data source
    PrintCatalog(ds);

// shows hot to retrieve and then print the datasets stored in the data source
//    PrintDataSets(ds);

// get a transactor to interact to the data source in the next examples
    te::da::DataSourceTransactor* transactor = ds->getTransactor();

// shows how to use a spatial filter
    RetrieveUsingSpatialFilter(transactor);

// shows several examples on how to retrieve the dataset extent
    DataSetGetExtent(transactor);

// it creates a DataSetType called 'our_country' using the schema 'public' in the given data source
    te::da::DataSetType* datasetType = CreateDataSetType(transactor);

// it adds a primary key to the given dataset type
    te::da::PrimaryKey* pk = AddPrimaryKey(datasetType, transactor);
    assert(pk);

// it adds an Unique Key to the given dataset type
    te::da::UniqueKey* uk = AddUniqueKey(datasetType, transactor);
    assert(uk);

// it adds a spatial index to the given dataset type
    te::da::Index* idx = AddSpatialIndex(datasetType, transactor);
    assert(idx);

// it adds an integer property called 'population' to the given dataset type
    te::dt::SimpleProperty* p = AddProperty(datasetType, transactor);

// Now, let's use the persistence API in order to remove things from the data source.

// first, drop the recently added property
    te::dt::Property* pp = datasetType->getProperty("population");
    te::da::DataSetTypePersistence* dtp = transactor->getDataSetTypePersistence();
    dtp->drop(pp);  

// finally, drop the dataset we have create above
    dtp->drop(datasetType);

// remember to release the persistence: you are the owner
    delete dtp;

// at the end, let's release the transactor and data source instances
    delete transactor;
    delete ds;
  }
  catch(const std::exception& e)
  {
    std::cout << std::endl << "An exception has occurried in PostGIS Example: " << e.what() << std::endl;
  }
  catch(...)
  {
    std::cout << std::endl << "An unexpected exception has occurried in PostGIS Example!" << std::endl;
  }
#endif
}

