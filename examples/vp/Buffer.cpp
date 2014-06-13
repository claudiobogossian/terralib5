
#include "../Config.h"
#include <terralib/common.h>
#include <terralib/dataaccess.h>
#include <terralib/dataaccess/datasource/DataSourceFactory.h>
//#include <terralib/vp/Buffer.h>
#include <terralib/vp/BufferMemory.h>
#include <terralib/vp/BufferOp.h>
#include <terralib/vp/BufferQuery.h>

// STL
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>

//New examples

//OGR to OGR
bool BufferOGRToOGR()
{
  std::string data_dir = TERRALIB_DATA_DIR;
  
  std::string filename = data_dir + "/shp/Buffer/SP_meso.shp";
  
  std::map<std::string, std::string> srcInfo;
  srcInfo["URI"] = filename;
  srcInfo["DRIVER"] = "ESRI Shapefile";

  //std::auto_ptr<te::da::DataSource> srcDs = te::da::DataSourceFactory::make("OGR");
  te::da::DataSourcePtr srcDs(te::da::DataSourceFactory::make("OGR"));
  srcDs->setConnectionInfo(srcInfo);
  srcDs->open();

  std::string inDsetName = "SP_meso";
  if (!srcDs->dataSetExists(inDsetName))
  {
    std::cout << "Input dataset not found: " << inDsetName << std::endl;
    return false;
  }

  std::auto_ptr<te::da::DataSet> inDset = srcDs->getDataSet(inDsetName);
  std::auto_ptr<te::da::DataSetType> inDsetType = srcDs->getDataSetType(inDsetName);

  double distance = 0.1;

  //options for Polygon Rule.
  int bufferPolygonRule = te::vp::INSIDE_OUTSIDE;
  //int bufferPolygonRule = te::vp::ONLY_INSIDE;
  //int bufferPolygonRule = te::vp::ONLY_OUTSIDE;

  //options for Boundaries Rule.
  //int bufferBoundariesRule = te::vp::DISSOLVE;
  int bufferBoundariesRule = te::vp::NOT_DISSOLVE;

  bool copyInputColumns = false;
  int levels = 1;

  std::string file_result = data_dir + "/shp/Buffer/file_result.shp";

  std::map<std::string, std::string> tgrInfo;
  tgrInfo["URI"] = file_result;
  tgrInfo["DRIVER"] = "ESRI Shapefile";

  //std::auto_ptr<te::da::DataSource> trgDs = te::da::DataSourceFactory::make("OGR");
  te::da::DataSourcePtr trgDs(te::da::DataSourceFactory::make("OGR"));
  trgDs->setConnectionInfo(tgrInfo);
  trgDs->open();

  std::string outDS = "file_result";

  if (trgDs->dataSetExists(outDS))
  {
    std::cout << "A dataset with the same requested output dataset name already exists: " << outDS << std::endl;
    return false;
  }

  // sera feito por algum tipo de factory
  te::vp::BufferOp* bufferOp = new te::vp::BufferMemory();

  bufferOp->setInput(srcDs, inDset, inDsetType);
  bufferOp->setOutput(trgDs, outDS);
  bufferOp->setParams(distance,
                      bufferPolygonRule,
                      bufferBoundariesRule,
                      copyInputColumns,
                      levels);
  
  bool result;

  if (!bufferOp->paramsAreValid())
    result = false;
  else
    result = bufferOp->run();

  delete bufferOp;
  
  return result;
}

//OGR to Postgis
bool BufferOGRToPGIS()
{
  std::string data_dir = TERRALIB_DATA_DIR;
  
  std::string filename(data_dir + "/shp/Buffer/SP_meso.shp");
  
  std::map<std::string, std::string> srcInfo;
  srcInfo["URI"] = filename;
  srcInfo["DRIVER"] = "ESRI Shapefile";
  
  //std::auto_ptr<te::da::DataSource> srcDs = te::da::DataSourceFactory::make("OGR");
  te::da::DataSourcePtr srcDs(te::da::DataSourceFactory::make("OGR"));
  srcDs->setConnectionInfo(srcInfo);
  srcDs->open();
  
  std::string inDsetName = "SP_meso";
  if (!srcDs->dataSetExists(inDsetName))
  {
    std::cout << "Input dataset not found: " << inDsetName << std::endl;
    return false;
  }
  
  std::auto_ptr<te::da::DataSet> inDset = srcDs->getDataSet(inDsetName);
  std::auto_ptr<te::da::DataSetType> inDsetType = srcDs->getDataSetType(inDsetName);

  double distance = 0.1;

  //options for Polygon Rule.
  int bufferPolygonRule = te::vp::INSIDE_OUTSIDE;
  //int bufferPolygonRule = te::vp::ONLY_INSIDE;
  //int bufferPolygonRule = te::vp::ONLY_OUTSIDE;

  //options for Boundaries Rule.
  int bufferBoundariesRule = te::vp::DISSOLVE;
  //int bufferBoundariesRule = te::vp::NOT_DISSOLVE;

  bool copyInputColumns = false;
  int levels = 1;

  std::map<std::string, std::string> connInfo;
  //connInfo["PG_HOST"] = "atlas.dpi.inpe.br" ; 
  //connInfo["PG_PORT"] = "5433" ;
  //connInfo["PG_USER"] = "postgres";
  //connInfo["PG_PASSWORD"] = "postgres";
  //connInfo["PG_DB_NAME"] = "testPostGIS";
  //connInfo["PG_CONNECT_TIMEOUT"] = "4"; 
  //connInfo["PG_CLIENT_ENCODING"] = "WIN1252";

  connInfo["PG_HOST"] = "localhost" ; 
  connInfo["PG_PORT"] = "5432" ;
  connInfo["PG_USER"] = "postgres";
  connInfo["PG_PASSWORD"] = "root";
  connInfo["PG_DB_NAME"] = "testPostGIS";
  connInfo["PG_CONNECT_TIMEOUT"] = "4"; 
  connInfo["PG_CLIENT_ENCODING"] = "WIN1252";
  
  //std::auto_ptr<te::da::DataSource> trgDs = te::da::DataSourceFactory::make("POSTGIS");
  te::da::DataSourcePtr trgDs(te::da::DataSourceFactory::make("POSTGIS"));
  trgDs->setConnectionInfo(connInfo);
  trgDs->open();
  
  std::string outDS = "result";
  
  if (trgDs->dataSetExists(outDS))
  {
    std::cout << "A dataset with the same requested output dataset name already exists: " << outDS << std::endl;
    return false;
  }
  
  // sera feito por algum tipo de factory
  te::vp::BufferOp* bufferOp = new te::vp::BufferMemory();

  bufferOp->setInput(srcDs, inDset, inDsetType);
  bufferOp->setOutput(trgDs, outDS);
  bufferOp->setParams(distance,
                      bufferPolygonRule,
                      bufferBoundariesRule,
                      copyInputColumns,
                      levels);
  
  bool result = false;

  if (!bufferOp->paramsAreValid())
    result = false;
  else
    result = bufferOp->run();

  delete bufferOp;
  
  return result;
}

//Postgis to Postgis
bool BufferPGISToPGIS()
{
  std::map<std::string, std::string> connInfo;
  connInfo["PG_HOST"] = "localhost" ; 
  connInfo["PG_PORT"] = "5432" ;
  connInfo["PG_USER"] = "postgres";
  connInfo["PG_PASSWORD"] = "root";
  connInfo["PG_DB_NAME"] = "testPostGIS";
  connInfo["PG_CONNECT_TIMEOUT"] = "4"; 
  connInfo["PG_CLIENT_ENCODING"] = "WIN1252"; 

  //std::auto_ptr<te::da::DataSource> srcDs = te::da::DataSourceFactory::make("POSTGIS");
  te::da::DataSourcePtr srcDs(te::da::DataSourceFactory::make("POSTGIS"));
  srcDs->setConnectionInfo(connInfo);
  srcDs->open();
  
  std::string inDsetName = "sp_meso";
  if (!srcDs->dataSetExists(inDsetName))
  {
    std::cout << "Input dataset not found: " << inDsetName << std::endl;
    return false;
  }
  
  std::auto_ptr<te::da::DataSet> inDset = srcDs->getDataSet(inDsetName);
  std::auto_ptr<te::da::DataSetType> inDsetType = srcDs->getDataSetType(inDsetName);
  
  std::string outDSet = "bufferPGISToPGIS";

  double distance = 0.1;

  //options for Polygon Rule.
  int bufferPolygonRule = te::vp::INSIDE_OUTSIDE;
  //int bufferPolygonRule = te::vp::ONLY_INSIDE;
  //int bufferPolygonRule = te::vp::ONLY_OUTSIDE;

  //options for Boundaries Rule.
  int bufferBoundariesRule = te::vp::DISSOLVE;
  //int bufferBoundariesRule = te::vp::NOT_DISSOLVE;

  bool copyInputColumns = false;
  int levels = 2;

  //std::auto_ptr<te::da::DataSource> outDsource = te::da::DataSourceFactory::make("POSTGIS");
  te::da::DataSourcePtr outDsource(te::da::DataSourceFactory::make("POSTGIS"));
  outDsource->setConnectionInfo(connInfo);
  outDsource->open();

  if (outDsource->dataSetExists(outDSet))
  {
    std::cout << "A dataset with the same requested output dataset name already exists: " << outDSet << std::endl;
    return false;
  }
  
  // sera feito por algum tipo de factory
  te::vp::BufferOp* bufferOp = new te::vp::BufferMemory();

  bufferOp->setInput(srcDs, inDset, inDsetType);
  bufferOp->setOutput(outDsource, outDSet);
  bufferOp->setParams(distance,
                      bufferPolygonRule,
                      bufferBoundariesRule,
                      copyInputColumns,
                      levels);
  
  bool result = false;

  if (!bufferOp->paramsAreValid())
    result = false;
  else
    result = bufferOp->run();

  delete bufferOp;
  
  return result;
}

//Postgis to OGR
bool BufferPGISToOGR()
{
  std::map<std::string, std::string> connInfo;
  connInfo["PG_HOST"] = "localhost" ; 
  connInfo["PG_PORT"] = "5432" ;
  connInfo["PG_USER"] = "postgres";
  connInfo["PG_PASSWORD"] = "root";
  connInfo["PG_DB_NAME"] = "testPostGIS";
  connInfo["PG_CONNECT_TIMEOUT"] = "4"; 
  connInfo["PG_CLIENT_ENCODING"] = "WIN1252";

  //std::auto_ptr<te::da::DataSource> srcDs = te::da::DataSourceFactory::make("POSTGIS");
  te::da::DataSourcePtr srcDs(te::da::DataSourceFactory::make("POSTGIS"));
  srcDs->setConnectionInfo(connInfo);
  srcDs->open();
  
  std::string inDsetName = "sp_meso";
  if (!srcDs->dataSetExists(inDsetName))
  {
    std::cout << "Input dataset not found: " << inDsetName << std::endl;
    return false;
  }
  
  std::auto_ptr<te::da::DataSet> inDset = srcDs->getDataSet(inDsetName);
  std::auto_ptr<te::da::DataSetType> inDsetType = srcDs->getDataSetType(inDsetName);

  double distance = 0.1;

  //options for Polygon Rule.
  int bufferPolygonRule = te::vp::INSIDE_OUTSIDE;
  //int bufferPolygonRule = te::vp::ONLY_INSIDE;
  //int bufferPolygonRule = te::vp::ONLY_OUTSIDE;

  //options for Boundaries Rule.
  int bufferBoundariesRule = te::vp::DISSOLVE;
  //int bufferBoundariesRule = te::vp::NOT_DISSOLVE;

  bool copyInputColumns = false;
  int levels = 1;
  
  std::string data_dir = TERRALIB_DATA_DIR;
  
  std::string uriResult(data_dir + "/shp/Buffer/bufferPGISToOGR.shp");

  std::map<std::string, std::string> tgrInfo;
  tgrInfo["URI"] = uriResult;
  tgrInfo["DRIVER"] = "ESRI Shapefile";

  //std::auto_ptr<te::da::DataSource> trgDs = te::da::DataSourceFactory::make("OGR");
  te::da::DataSourcePtr trgDs(te::da::DataSourceFactory::make("OGR"));
  trgDs->setConnectionInfo(tgrInfo);
  trgDs->open();

  std::string outDSet = "bufferPGISToOGR";

  if (trgDs->dataSetExists(outDSet))
  {
    std::cout << "A dataset with the same requested output dataset name already exists: " << outDSet << std::endl;
    return false;
  }
  
  // sera feito por algum tipo de factory
  te::vp::BufferOp* bufferOp = new te::vp::BufferQuery();

  bufferOp->setInput(srcDs, inDset, inDsetType);
  bufferOp->setOutput(trgDs, outDSet);
  bufferOp->setParams(distance,
                      bufferPolygonRule,
                      bufferBoundariesRule,
                      copyInputColumns,
                      levels);
  
  bool result = false;

  if (!bufferOp->paramsAreValid())
    result = false;
  else
    result = bufferOp->run();

  delete bufferOp;
  
  return result;
}