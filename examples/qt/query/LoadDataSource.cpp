#include "QueryExamples.h"

// TerraLib
#include <terralib/common.h>
#include <terralib/dataaccess.h>
#include <terralib/maptools.h>

// STL
#include <cassert>

te::da::DataSourcePtr LoadShapeDataSource(const std::string& fileName, const std::string& dsId)
{
  //shape
  std::map<std::string, std::string> connInfo;
  connInfo["path"] = fileName;
  
  te::da::DataSource* dsShape = te::da::DataSourceFactory::make("OGR");
  dsShape->open(connInfo);
  dsShape->setId(dsId);

  te::da::DataSourcePtr dsPtrShape(dsShape);

  te::da::DataSourceManager::getInstance().insert(dsPtrShape);

  return dsPtrShape;
}

te::da::DataSourcePtr LoadPGISDataSource(const std::string& dsId)
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

  return dsPtrPGIS;
}

te::da::DataSet* GetDataSet(te::da::Select& s, te::da::DataSourcePtr& ds)
{
  te::da::DataSourceTransactor* transactor = ds->getTransactor();

  te::da::DataSet* dataset = transactor->query(s);

  delete transactor;

  return dataset;
}

