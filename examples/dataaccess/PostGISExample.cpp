
// Examples
#include "DataAccessExamples.h"

// TerraLib
#include <terralib/dataaccess/datasource/DataSourceFactory.h>
#include <terralib/datatype.h>

// STL
#include <iostream>
#include <exception>

std::auto_ptr<te::da::DataSource> GetPostGISConnection()
{
  // let's give the minimal server connection information needed to connect to the database server
  std::string aux, user, password, host, port, path, query;
  std::string strURI = "pgsql://"; // The base of the URI

  std::cout << "Inform the Host for your postGIS server (ENTER to accept default \'atlas.dpi.inpe.br\'): ";
  std::getline (std::cin, aux);
  host = aux.empty() ? "atlas.dpi.inpe.br" : aux;
  
  std::cout << "Inform the Port number to access your postGIS server (ENTER to accept default \'5433\'): ";
  std::getline (std::cin, aux);
  port = aux.empty() ? "5433" : aux;

  std::cout << "Inform the User to access your postGIS server (ENTER to accept default \'postgres\'): ";
  std::getline (std::cin, aux);
  user = aux.empty() ? "postgres" : aux;

  std::cout << "Inform the Password to access your postGIS server (ENTER to accept default \'postgres\'): ";
  std::getline (std::cin, aux);
  password = aux.empty() ? "postgres" : aux;

  std::cout << "Inform the Database name to connect to your postGIS server (ENTER to accept default \'terralib4\'): ";
  std::getline (std::cin, aux);
  path = aux.empty() ? "terralib4" : aux;

  std::cout << "Inform the Client enconding to connect to your postGIS server (ENTER to accept default \'UTF-8\'): ";
  std::getline(std::cin, aux);
  query = aux.empty() ?  "&PG_CLIENT_ENCODING=" + te::core::CharEncoding::getEncodingName(te::core::EncodingType::UTF8) : aux;

  std::cout << "Inform the Connection Time Out to connect to your postGIS server (ENTER to accept default \'4\'): ";
  std::getline (std::cin, aux);
  query += aux.empty() ? "&PG_CONNECT_TIMEOUT=4" : "&PG_CONNECT_TIMEOUT=" + aux;

  strURI += user + ":";
  strURI += password + "@";
  strURI += host + ":";
  strURI += port + "/";
  strURI += path + "?";
  strURI += query;

  // create a data source using the data source factory
  std::auto_ptr<te::da::DataSource> ds = te::da::DataSourceFactory::make("POSTGIS", strURI);

  try
  {
      ds->open();
  }
  catch(const std::exception& e)
  {
    std::cout << "Datasource " << host << "/" <<  path << " can not be used!\nMake sure to have the correct connection parameters\n";
    std::cout << "Error: " << e.what() << std::endl;
    ds.reset();
    return ds;
  }
  catch(...)
  {
    std::cout << "Datasource " << host << "/" << path << " can not be used!\nMake sure to have the correct connection parameters\n";
    ds.reset();
    return ds;
  }
  
  std::cout << "Using datasource " << host << "/" <<  path << std::endl;
  ds->close();
  return ds;
}

void PostGISExample()
{
  try
  {
    std::auto_ptr<te::da::DataSource> ds = GetPostGISConnection();
    if (!ds.get())
      return;
    
    ds->open();
    
    std::cout << "Datasource is opened? " << std::boolalpha << ds->isOpened() << '\n';
    std::cout << "Datasource is valid? " << std::boolalpha << ds->isValid() << '\n';

// retrieve the data source capabilities and print it
    std::cout << std::endl;
    PrintDataSourceCapabilities(ds.get());
    
// printing some datasets from the datasource
    std::vector<std::string> dsets = ds->getDataSetNames();
    
    if (dsets.empty())
    {
      std::cout << "Datasource has no datasets.\n";
      return;
    }
    
// let's check one of them (or all)
    std::cout << "\nThere is(are) " << dsets.size() << " dataset(s) in the datasource: \n";
    for (size_t i=0; i<dsets.size(); ++i)
      std::cout << '\t' << i+1 << ':' << dsets[i] << std::endl;
    
    // check point: retrieving the data from a dataset of the datasource
    while (true)
    {
      std::cout << "\nSelect a dataset from 1 to " << ds->getNumberOfDataSets() << " to see its data (0 to none): ";
      int n;
      std::cin >> n;
      if (n<1 || n>ds->getNumberOfDataSets())
        break;
      PrintDataSet(dsets[n-1], ds->getDataSet(dsets[n-1]).get());
    }

// shows how to use a spatial filter
    std::vector<std::string>::iterator it;
    it = std::find(dsets.begin(),dsets.end(),"public.munic_2001");
    if (it != dsets.end())
    {
      std::cout << "\nHow to do spatial filtering using the dataset \'public.munic_2001\'\n";
      RetrieveUsingSpatialFilter(ds.get());
    }
    
// it creates a DataSetType called 'our_country' using the schema 'public' in the given data source
    it = std::find(dsets.begin(),dsets.end(),"public.our_country");
    if (it != dsets.end())
      return;
    
    std::cout << "\nHow to create a new dataset the dataset: ex 'public.our_country'\n";
    std::auto_ptr<te::da::DataSourceTransactor> transactor = ds->getTransactor();
    std::string dt_name = "our_country";
    std::cout << std::endl << "Creating dataSet= " << dt_name  << std::endl;
    te::da::DataSetType* dtype = new te::da::DataSetType(dt_name);
    dtype->add(new te::dt::SimpleProperty("gid", te::dt::INT32_TYPE, true));
    dtype->add(new te::dt::StringProperty("country_name", te::dt::STRING));
    dtype->add(new te::dt::StringProperty("city_name", te::dt::VAR_STRING, 50, true));
    dtype->add(new te::gm::GeometryProperty("spatial_data", 4326, te::gm::GeometryType, true));

    te::da::DataSetType* datasetType = CreateDataSetType(dt_name,dtype,transactor.get());

// it adds a primary key to the given dataset type
    std::cout << std::endl << "Adding Primary Key to " << dt_name << std::endl;
    te::da::PrimaryKey* pk = AddPrimaryKey(datasetType->getName(), transactor.get());
    assert(pk);

// it adds an Unique Key to the given dataset type
    std::cout << std::endl << "Adding Unique Key to " << dt_name << std::endl;
    te::da::UniqueKey* uk = AddUniqueKey(datasetType->getName(), transactor.get());
    assert(uk);

// it adds a spatial index to the given dataset type
    std::cout << std::endl << "Adding spatial index to " << dt_name << std::endl;
    te::da::Index* idx = AddSpatialIndex(datasetType->getName(), transactor.get());
    assert(idx);

// it adds an integer property called 'population' to the given dataset type
    std::cout << std::endl << "Adding new Property population to " << dt_name << std::endl;
    AddProperty(datasetType->getName(), transactor.get());

// Now, lets  remove things from the data source using transactor or function DroppingDataSetTypeProperty
// first, drop the recently added property
    std::cout << std::endl << "Droping Property population of " << dt_name <<" using transactor or ds"<< std::endl;
    (transactor.get())->dropProperty(datasetType->getName(), "population");

// Now, lets it add again an integer property called 'population' to the given dataset type and drop it using ds
    std::cout << std::endl << "Adding new Property population to " << dt_name << std::endl;
    AddProperty(datasetType->getName(), transactor.get());
    
// Dropping using ds api   
    ds->dropProperty(dt_name, "population");

// Now, lets drop a geom column
    ds->dropProperty(dt_name, "spatial_data"); //check the view geometry_columns

// finally, drop the dataset we have created above via ds or via transactor
    std::cout << std::endl << "Droping dataSet " << dt_name << std::endl;
    ds->dropDataSet(dt_name);

// Create again the dataset and drop it using transactor
    datasetType = CreateDataSetType(dt_name,dtype,transactor.get());

    DroppingDataSetType(datasetType->getName(),transactor.get());
   
    if (transactor->isInTransaction())
      std::cout << std::endl << "Transactor in transaction! "<< std::endl;
    delete transactor.release(); 
    ds->close();
  }
  catch(const std::exception& e)
  {
    std::cout << std::endl << "An exception has occurred in the PostGIS Example: " << e.what() << std::endl;
  }
  catch(...)
  {
    std::cout << std::endl << "An unexpected exception has occurred in the PostGIS Example!" << std::endl;
  }
}

