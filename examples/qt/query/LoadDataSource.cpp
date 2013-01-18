#include "QueryExamples.h"

// TerraLib
#include <terralib/common.h>
#include <terralib/dataaccess.h>
#include <terralib/maptools.h>

// STL
#include <cassert>

void LoadShapeDataSource(const std::string& fileName, const std::string& dsId)
{
  //shape
  std::map<std::string, std::string> connInfo;
  connInfo["path"] = fileName;
  
  te::da::DataSource* dsShape = te::da::DataSourceFactory::make("OGR");
  dsShape->open(connInfo);
  dsShape->setId(dsId);

  te::da::DataSourcePtr dsPtrShape(dsShape);

  te::da::DataSourceManager::getInstance().insert(dsPtrShape);
}

void LoadPGISDataSource(const std::string& dsId)
{
  //postgis
  std::map<std::string, std::string> connInfo;
  connInfo["PG_HOST"] = "localhost";
  connInfo["PG_USER"] = "postgres";
  connInfo["PG_PASSWORD"] = "tdk696";
  connInfo["PG_DB_NAME"] = "t5graph";
  connInfo["PG_CONNECT_TIMEOUT"] = "4"; 

  te::da::DataSource* dsPGIS = te::da::DataSourceFactory::make("POSTGIS");
  dsPGIS->open(connInfo);
  dsPGIS->setId(dsId);

  te::da::DataSourcePtr dsPtrPGIS(dsPGIS);

  te::da::DataSourceManager::getInstance().insert(dsPtrPGIS);
}

