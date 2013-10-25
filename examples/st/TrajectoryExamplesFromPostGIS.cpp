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
    te::da::DataSource* ds = te::da::DataSourceFactory::make("POSTGIS");
    ds->open(pgInfo);

    //TO DO: carregar metadato do banco
    std::string name = "car_trajectories";
    int idIdx = 5;      /* property name: car_id */
    int phTimeIdx = 4;  /* property name: date_obs */
    int geomIdx = 3;    /* property name: geom */
   
    te::da::DataSourceTransactor* t = ds->getTransactor();

    te::da::DataSet* dataSet = t->getDataSet(name);   

    te::stloader::MovingObjectDataSetMetadata metadata(phTimeIdx, geomIdx, idIdx); 
    
    te::stloader::DataLoader::loadMovingObjects(dataSet, metadata, output);

// release the data source: you are the owner
    delete dataSet;
    delete t;
    delete ds;    
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