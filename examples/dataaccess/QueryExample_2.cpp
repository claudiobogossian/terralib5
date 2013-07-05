// Examples
#include "DataAccessExamples.h"

// TerraLib
#include <terralib/dataaccess.h>
#include <terralib/geometry.h>

// STL
#include <iostream>
#include <exception>

void QueryExample_2()
{
#if TE_EXAMPLE_USE_PGIS
// let's give the minimal server connection information needed to connect to the database server
  std::map<std::string, std::string> connInfo;
  connInfo["PG_HOST"] = "localhost" ;   // or "atlas.dpi.inpe.br";
  connInfo["PG_USER"] = "postgres";
  connInfo["PG_PASSWORD"] = "root";
  connInfo["PG_DB_NAME"] = "testPostGIS";
  connInfo["PG_CONNECT_TIMEOUT"] = "4";

  try
  {
// if you are not using the data source manager, create one instance of the data source called ds an other dsOGR!
    te::da::DataSource* ds = te::da::DataSourceFactory::make("POSTGIS");
  
// as we are going to use the data source, let's open it!
    ds->open(connInfo);

// let's find out the information in the data source!
    //ds->loadCatalog();

// get a transactor to interact to the data source
    te::da::DataSourceTransactor* transactor = ds->getTransactor();

// quering a table called public.munic_2001 using query object
    {
      te::da::Fields* fields = new te::da::Fields;
      te::da::PropertyName* pName = new te::da::PropertyName("area_tot_g");

      te::da::Expression* e_min = new te::da::Min(pName);
      te::da::Field* f_min = new te::da::Field(*e_min, "MIN");

      te::da::Expression* e_max = new te::da::Max(pName);
      te::da::Field* f_max = new te::da::Field(*e_max, "MAX");

      te::da::Expression* e_count = new te::da::Count(pName);
      te::da::Field* f_count = new te::da::Field(*e_count, "COUNT");

      te::da::Expression* e_sum = new te::da::Sum(pName);
      te::da::Field* f_sum = new te::da::Field(*e_sum, "SUM");

      te::da::Expression* e_mean = new te::da::Avg(pName);
      te::da::Field* f_mean = new te::da::Field(*e_mean, "MEAN");

      te::da::Expression* e_stddev = new te::da::StdDev(pName);
      te::da::Field* f_stddev = new te::da::Field(*e_stddev, "STD_DEV");

      te::da::Expression* e_variance = new te::da::Variance(pName);
      te::da::Field* f_variance = new te::da::Field(*e_variance, "VARIANCE");

      te::da::Expression* e_amplitude = new te::da::Sub(*e_max, *e_min);
      te::da::Field* f_amplitude = new te::da::Field(*e_amplitude, "AMPLITUDE");

      fields->push_back(f_min);
      fields->push_back(f_max);
      fields->push_back(f_count);
      fields->push_back(f_sum);
      fields->push_back(f_mean);
      fields->push_back(f_stddev);
      fields->push_back(f_variance);
      fields->push_back(f_amplitude);

      te::da::FromItem* t1 = new te::da::DataSetName("munic_2001", "t1");
      te::da::From* from = new te::da::From;
      from->push_back(t1);

      te::da::Select select(fields, from);

      te::da::DataSet* dataset = transactor->query(select);
      dataset->moveBeforeFirst();
      int i = 0;

      while(dataset->moveNext())
      {
        std::string min = dataset->getAsString(0);
        std::cout << "Min: " << min <<std::endl;

        std::string max = dataset->getAsString(1);
        std::cout << "Max: " << max <<std::endl;

        std::string count = dataset->getAsString(2);
        std::cout << "Count: " << count <<std::endl;

        std::string sum = dataset->getAsString(3);
        std::cout << "Sum: " << sum <<std::endl;

        std::string mean = dataset->getAsString(4);
        std::cout << "Mean: " << mean <<std::endl;
        
        std::string stddev = dataset->getAsString(5);
        std::cout << "Std_dev: " << stddev <<std::endl;

        std::string variance = dataset->getAsString(6);
        std::cout << "Variance: " << variance <<std::endl;

        std::string amplitude = dataset->getAsString(7);
        std::cout << "Amplitude: " << amplitude <<std::endl;
        ++i;
      }

      delete dataset;
    }
 
// At the end, let's release the transactor and data source instances
    delete transactor;
    ds->close();
    delete ds;
  }
  catch(const std::exception& e)
  {
    std::cout << std::endl << "An exception has occurried in QueryExample: " << e.what() << std::endl;
  }
  catch(...)
  {
    std::cout << std::endl << "An unexpected exception has occurried in QueryExample!" << std::endl;
  }
#endif
}

