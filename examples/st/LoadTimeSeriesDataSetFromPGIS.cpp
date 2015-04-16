// TerraLib
#include <terralib/dataaccess.h>
#include <terralib/stmemory.h>
#include <terralib/st.h>
#include <terralib/datatype.h>

// Examples
#include "STExamples.h"

// STL
#include <iostream>

// Boost
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

void LoadTimeSeriesDataSetFromPGIS(boost::ptr_vector<te::st::TimeSeries>& output)
{
  try
  {
    //Container used to store the resulting timeseries before outputting them ack
    std::vector<te::st::TimeSeries*> series;

    // let's give the minimal server connection information needed to connect to the database server
    std::map<std::string, std::string> connInfo;
    connInfo["PG_CLIENT_ENCODING"] = "CP1252";
    connInfo["PG_CONNECT_TIMEOUT"] = "5";
    connInfo["PG_DB_NAME"] = "timeseries";
    connInfo["PG_HOST"] = "localhost" ;
    connInfo["PG_PORT"] = "5432" ;
    connInfo["PG_USER"] = "postgres";
    connInfo["PG_PASSWORD"] = "postgres";

    //Indicates the data source
    te::da::DataSourceInfo dsInfo;
    dsInfo.setConnInfo(connInfo);
    dsInfo.setTitle("TimeSeries");
    dsInfo.setAccessDriver("POSTGIS");
    dsInfo.setType("POSTGIS");

    //It creates a new Data Source and put it into the manager
    CreateDataSourceAndUpdateManager(dsInfo);

    te::st::TimeSeriesDataSetInfo tsInfo(dsInfo, "ordered_cells_view", "time", "value", "spatial_data", "id_cell", "");

    std::auto_ptr<te::st::TimeSeriesDataSet> timeSeriesDS = te::st::STDataLoader::getDataSet(tsInfo);
    timeSeriesDS->moveBeforeFirst();
    timeSeriesDS->getTimeSeriesSet(&te::st::NearestValueAtTimeInterp::getInstance(), tsInfo.getObservationDataSetInfo().getIdPropName(), series);
    for(size_t i = 0; i < 100; i++)
    {
      output.push_back(series[i]);
    }
  }
  catch(const std::exception& e)
  {
  std::cout << std::endl << "An exception has occurred in TimeSeriesExamplesFromKML: " << e.what() << std::endl;
  }
  catch(...)
  {
    std::cout << std::endl << "An unexpected exception has occurred in TimeSeriesExamplesFromKML!" << std::endl;
  }
}

