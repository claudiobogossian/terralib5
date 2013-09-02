// Examples
#include "DataAccessExamples.h"

// TerraLib
//#include <terralib/dataaccess2.h>
#include <terralib/dataaccess2/datasource/DataSourceFactory.h>

//#include <../terralib/dataaccess_h.h>
 
// STL
#include <iostream>
#include <exception>

void PostGISExample()
{ 
  try
  {
// let's give the minimal server connection information needed to connect to the database server
    std::map<std::string, std::string> connInfo;
    connInfo["PG_HOST"] = "atlas.dpi.inpe.br" ;   // or "localhost";
    connInfo["PG_PORT"] = "5433" ;
    connInfo["PG_USER"] = "postgres";
    connInfo["PG_PASSWORD"] = "postgres";
    connInfo["PG_DB_NAME"] = "terralib4";
    connInfo["PG_CONNECT_TIMEOUT"] = "4"; 
    
// create a data source using the data source factory
    std::auto_ptr<te::da::DataSource> ds = te::da::DataSourceFactory::make("POSTGIS");
// as we are going to use the data source, let´s set the connection info
    ds->setConnectionInfo(connInfo);
// let's open it with the connection info above!
    ds->open();

// shows how to print information about datasets stored in the data source
    //PrintCatalog(ds);

// shows how to retrieve and then print the datasets stored in the data source
//    PrintDataSets(ds);

// get a transactor to interact to the data source in the next examples
    std::auto_ptr<te::da::DataSourceTransactor> transactor = ds->getTransactor();
    //te::da::DataSourceTransactor* transactor = transactor_auto.release();
// shows how to use a spatial filter
    RetrieveUsingSpatialFilter(ds.get());

// shows several examples on how to retrieve the dataset extent
    DataSetGetExtent(transactor.get());

// it creates a DataSetType called 'our_country' using the schema 'public' in the given data source
    std::cout << std::endl << "Creating dataSet 'our_country'  "<< std::endl;
    te::da::DataSetType* datasetType = CreateDataSetType(transactor.get());

// it adds a primary key to the given dataset type
    std::cout << std::endl << "Adding Primary Key to 'our_country'"<< std::endl;
    te::da::PrimaryKey* pk = AddPrimaryKey(datasetType->getName(), transactor.get());
    assert(pk);

// it adds an Unique Key to the given dataset type
    std::cout << std::endl << "Adding Unique Key to 'our_country'"<< std::endl;
    te::da::UniqueKey* uk = AddUniqueKey(datasetType->getName(), transactor.get());
    assert(uk);

// it adds a spatial index to the given dataset type
    std::cout << std::endl << "Adding spatial index to 'our_country' "<< std::endl;
    te::da::Index* idx = AddSpatialIndex(datasetType->getName(), transactor.get());
    assert(idx);

// it adds an integer property called 'population' to the given dataset type
    std::cout << std::endl << "Adding new Property population to 'our_country' "<< std::endl;
    te::dt::SimpleProperty* p = AddProperty(datasetType->getName(), transactor.get());

// Now, let's  remove things from the data source using transactor
    // first, drop the recently added property
    std::cout << std::endl << "Droping Property population of 'our_country' "<< std::endl;
//CAI    (transactor.get())->dropProperty(datasetType->getName(), "population");

    // finally, drop the dataset we have created above
    std::cout << std::endl << "Droping dataSet 'our_country' "<< std::endl;
    (transactor.get())->dropDataSet( datasetType->getName());

    ds->close();

    //delete ds.release();

  }
  catch(const std::exception& e)
  {
    std::cout << std::endl << "An exception has occurried in PostGIS Example: " << e.what() << std::endl;
  }
  catch(...)
  {
    std::cout << std::endl << "An unexpected exception has occurried in PostGIS Example!" << std::endl;
  }
}

