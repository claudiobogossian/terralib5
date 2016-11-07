
#include "../Config.h"
#include <terralib/common.h>
#include <terralib/dataaccess.h>
#include <terralib/dataaccess/datasource/DataSourceFactory.h>
#include <terralib/vp/AggregationMemory.h>
#include <terralib/vp/AggregationOp.h>
#include <terralib/vp/AggregationQuery.h>

// STL
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>

//Postgis to Postgis
bool SequenceOperations()
{
  std::vector<std::string> sequence;
  sequence.push_back("sp_cities");
  sequence.push_back("ST_Union");

  std::string connInfo("ppgsql://postgres:postgres@localhost:5432/TerraView5?PG_CONNECT_TIMEOUT=4&PG_CLIENT_ENCODING=UTF-8");

  te::da::DataSourcePtr srcDs(te::da::DataSourceFactory::make("POSTGIS", connInfo));
  srcDs->open();
  
  std::string inDsetName = "overlaps";
  if (!srcDs->dataSetExists(inDsetName))
  {
    std::cout << "Input dataset not found: " << inDsetName << std::endl;
    return false;
  }

  std::string outDSet = "result";

  te::da::DataSourcePtr outDsource(te::da::DataSourceFactory::make("POSTGIS", connInfo));
  outDsource->open();

  if (outDsource->dataSetExists(outDSet))
  {
    std::cout << "A dataset with the same requested output dataset name already exists: " << outDSet << std::endl;
    return false;
  }

  std::auto_ptr<te::da::DataSet> inDset = srcDs->getDataSet(inDsetName);
  std::auto_ptr<te::da::DataSetType> inDsetType = srcDs->getDataSetType(inDsetName);


  return true;
}