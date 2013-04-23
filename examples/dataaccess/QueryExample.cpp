// Examples
#include "DataAccessExamples.h"

// TerraLib
#include <terralib/dataaccess.h>
#include <terralib/geometry.h>

// STL
#include <iostream>
#include <exception>

void QueryExample()
{
#if TE_EXAMPLE_USE_PGIS
// let's give the minimal server connection information needed to connect to the database server
  std::map<std::string, std::string> connInfo;
  connInfo["PG_HOST"] = "atlas.dpi.inpe.br" ;   // or "localhost";
  connInfo["PG_USER"] = "postgres";
  connInfo["PG_PASSWORD"] = "sitim110";
  connInfo["PG_DB_NAME"] = "terralib4";
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

// quering a table called public.geometry_test using native interface
    {
      std::string sql("SELECT st_intersects(g1.spatial_data, g2.spatial_data) "
                      "FROM geometry_test AS g1, geometry_test AS g2 "
                      "WHERE g1.gid = 1 "
                      "  AND g2.gid = 2 ");

      te::da::DataSet* dataset = transactor->query(sql);

      while(dataset->moveNext())
      {
        std::string value = dataset->getAsString(0);
        std::cout << "st_intersects(g1.spatial_data, g2.spatial_data) = " << value << std::endl;
      }

      delete dataset;
    }
 
// quering a table called public.geometry_test using query object
//    {
//      te::da::Field* f = new te::da::Field(new te::da::Function("st_intersects", new te::da::PropertyName("g1.spatial_data"), new te::da::PropertyName("g2.spatial_data")));
//      te::da::Fields* fields = new te::da::Fields;
//      fields->push_back(f);
//
//      te::da::FromItem* t1 = new te::da::DataSetName("geometry_test", "g1");
//      te::da::FromItem* t2 = new te::da::DataSetName("geometry_test", "g2");
//      te::da::From* from = new te::da::From;
//      from->push_back(t1);
//      from->push_back(t2);
//
//      te::da::EqualTo* e1 = new te::da::EqualTo(new te::da::PropertyName("g1.gid"), new te::da::LiteralInt(1));
//      te::da::EqualTo* e2 = new te::da::EqualTo(new te::da::PropertyName("g2.gid"), new te::da::LiteralInt(2));
//      te::da::And* andd = new te::da::And(e1, e2);
//      te::da::Where* filter = andd;
//
//      te::da::Select select(fields, from, filter, 0);
//
//      te::da::DataSet* dataset = transactor->query(select);
//
//      while(dataset->moveNext())
//      {
//        std::string value = dataset->getAsString(0);
//        std::cout << "st_intersects(g1.spatial_data, g2.spatial_data) = " << value << std::endl;
//      }
//
//      delete dataset;
//    }
//
//// what city contains the given point?
//    {
//      te::da::Field* f = new te::da::Field(new te::da::PropertyName("*"));
//      te::da::Fields* fields = new te::da::Fields;
//      fields->push_back(f);
//
//      te::da::FromItem* t = new te::da::DataSetName("public.br_munic_2001");
//      te::da::From* from = new te::da::From;
//      from->push_back(t);
//
//      te::da::LiteralGeom* lgeom = new te::da::LiteralGeom(new te::gm::Point(-43.6107606714293, -20.3913548070123, 4291, 0));
//
//      te::da::Where* filter = new te::da::Function("st_contains", new te::da::PropertyName("geom"), lgeom);
//
//      te::da::Select select(fields, from, filter, 0);
//
//      te::da::DataSet* dataset = transactor->query(select);
//
//      while(dataset->moveNext())
//      {
//        std::string value = dataset->getAsString("nome");
//        std::cout << "city: " << value << std::endl;
//      }
//
//      delete dataset;
//    }

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

