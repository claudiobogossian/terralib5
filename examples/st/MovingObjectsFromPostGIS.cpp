// TerraLib
#include <terralib/dataaccess.h>
#include <terralib/st.h>
#include <terralib/stloader.h>

// Examples
#include "STExamples.h"

// STL
#include <iostream>

void MovingObjectsFromPostGIS(std::vector<te::st::MovingObject*>& output)
{
  try
  { 
    std::string pgInfo = "DataSource=POSTGIS&PG_HOST=localhost&PG_PORT=5432&PG_DB_NAME=dygeo_database&PG_USER=postgres&PG_CONNECT_TIMEOUT=5";
    std::map<std::string, std::string> connInfo;
    //connInfo["PG_HOST"] = "atlas.dpi.inpe.br" ;
    connInfo["PG_HOST"] = "localhost" ;
    connInfo["PG_PORT"] = "5432" ;
    connInfo["PG_USER"] = "postgres";
//    connInfo["PG_PASSWORD"] = "postgres";
    connInfo["PG_DB_NAME"] = "dygeo_database";  
    connInfo["PG_CONNECT_TIMEOUT"] = "5";

// create a data source using the data source factory
    std::auto_ptr<te::da::DataSource> ds = te::da::DataSourceFactory::make("POSTGIS");
// as we are going to use the data source, let´s set the connection info
    ds->setConnectionInfo(connInfo);
// let's open it with the connection info above!
    ds->open();

    //TO DO: carregar metadato do banco
    std::string name = "car_trajectories";
    int idIdx = 5;      /* property name: car_id */
    int phTimeIdx = 4;  /* property name: date_obs */
    int geomIdx = 3;    /* property name: geom */
   
    std::auto_ptr<te::da::DataSourceTransactor> t = ds->getTransactor();

    std::auto_ptr<te::da::DataSet> dataSet = t->getDataSet(name);   

    te::stloader::MovingObjectDataSetMetadata metadata(phTimeIdx, geomIdx, idIdx); 
    
    te::stloader::DataLoader::loadMovingObjects(dataSet.get(), metadata, output);
   
  }
  catch(const std::exception& e)
  {
    std::cout << std::endl << "An exception has occurried in MovingObjectsFromPostGIS: " << e.what() << std::endl;
  }
  catch(...)
  {
    std::cout << std::endl << "An unexpected exception has occurried MovingObjectsFromPostGIS!" << std::endl;
  }
}