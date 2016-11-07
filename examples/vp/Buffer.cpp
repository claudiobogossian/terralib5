
#include "../Config.h"
#include <terralib/common.h>
#include <terralib/dataaccess.h>
#include <terralib/dataaccess/datasource/DataSourceFactory.h>
#include <terralib/vp/BufferMemory.h>
#include <terralib/vp/BufferOp.h>
#include <terralib/vp/BufferQuery.h>

// STL
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>

//OGR to OGR
bool BufferOGRToOGR()
{
  std::string data_dir = TERRALIB_DATA_DIR;
  
  std::string filename = data_dir + "/shape/Buffer/SP_meso.shp";
  
  std::string srcInfo("file://" + filename);

  te::da::DataSourcePtr srcDs(te::da::DataSourceFactory::make("OGR", srcInfo));
  srcDs->open();

  std::string inDsetName = "SP_meso";
  if (!srcDs->dataSetExists(inDsetName))
  {
    std::cout << "Input dataset not found: " << inDsetName << std::endl;
    return false;
  }

  std::auto_ptr<te::da::DataSet> inDset = srcDs->getDataSet(inDsetName);
  std::auto_ptr<te::da::DataSetType> inDsetType = srcDs->getDataSetType(inDsetName);

  std::auto_ptr<te::da::DataSetTypeConverter> converter(new te::da::DataSetTypeConverter(inDsetType.get(), srcDs->getCapabilities(), srcDs->getEncoding()));

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

  std::string file_result = data_dir + "/shape/Buffer/file_result.shp";
  std::string tgrInfo("file://" + file_result);

  te::da::DataSourcePtr trgDs(te::da::DataSourceFactory::make("OGR", tgrInfo));
  trgDs->open();

  std::string outDS = "file_result";

  if (trgDs->dataSetExists(outDS))
  {
    std::cout << "A dataset with the same requested output dataset name already exists: " << outDS << std::endl;
    return false;
  }

  // sera feito por algum tipo de factory
  te::vp::BufferOp* bufferOp = new te::vp::BufferMemory();

  bufferOp->setInput(srcDs, inDsetName, converter);
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
  
  std::string filename(data_dir + "/shape/Buffer/SP_meso.shp");  
  std::string srcInfo("file://" + filename);

  te::da::DataSourcePtr srcDs(te::da::DataSourceFactory::make("OGR", filename));
  srcDs->open();
  
  std::string inDsetName = "SP_meso";
  if (!srcDs->dataSetExists(inDsetName))
  {
    std::cout << "Input dataset not found: " << inDsetName << std::endl;
    return false;
  }
  
  std::auto_ptr<te::da::DataSet> inDset = srcDs->getDataSet(inDsetName);
  std::auto_ptr<te::da::DataSetType> inDsetType = srcDs->getDataSetType(inDsetName);

  std::auto_ptr<te::da::DataSetTypeConverter> converter(new te::da::DataSetTypeConverter(inDsetType.get(), srcDs->getCapabilities(), srcDs->getEncoding()));

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

  std::string connInfo("ppgsql://postgres:postgres@atlas.dpi.inpe.br:5433/testPostGIS?PG_CONNECT_TIMEOUT=4&PG_CLIENT_ENCODING=UTF-8");

  te::da::DataSourcePtr trgDs(te::da::DataSourceFactory::make("POSTGIS", connInfo));
  trgDs->open();
  
  std::string outDS = "result";
  
  if (trgDs->dataSetExists(outDS))
  {
    std::cout << "A dataset with the same requested output dataset name already exists: " << outDS << std::endl;
    return false;
  }
  
  // sera feito por algum tipo de factory
  te::vp::BufferOp* bufferOp = new te::vp::BufferMemory();

  bufferOp->setInput(srcDs, inDsetName, converter);
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
  std::string connInfo("ppgsql://postgres:postgres@atlas.dpi.inpe.br:5433/testPostGIS?PG_CONNECT_TIMEOUT=4&PG_CLIENT_ENCODING=UTF-8");

  te::da::DataSourcePtr srcDs(te::da::DataSourceFactory::make("POSTGIS", connInfo));

  std::string inDsetName = "sp_meso";
  if (!srcDs->dataSetExists(inDsetName))
  {
    std::cout << "Input dataset not found: " << inDsetName << std::endl;
    return false;
  }
  
  std::auto_ptr<te::da::DataSet> inDset = srcDs->getDataSet(inDsetName);
  std::auto_ptr<te::da::DataSetType> inDsetType = srcDs->getDataSetType(inDsetName);

  std::auto_ptr<te::da::DataSetTypeConverter> converter(new te::da::DataSetTypeConverter(inDsetType.get(), srcDs->getCapabilities(), srcDs->getEncoding()));
  
  std::string outDSet = "bufferPGISToPGIS";

  double distance = 0.1;

  //options for Polygon Rule.
  int bufferPolygonRule = te::vp::INSIDE_OUTSIDE;

  //options for Boundaries Rule.
  int bufferBoundariesRule = te::vp::DISSOLVE;

  bool copyInputColumns = false;
  int levels = 2;

  te::da::DataSourcePtr outDsource(te::da::DataSourceFactory::make("POSTGIS", connInfo));
  outDsource->open();

  if (outDsource->dataSetExists(outDSet))
  {
    std::cout << "A dataset with the same requested output dataset name already exists: " << outDSet << std::endl;
    return false;
  }
  
  // sera feito por algum tipo de factory
  te::vp::BufferOp* bufferOp = new te::vp::BufferMemory();

  bufferOp->setInput(srcDs, inDsetName, converter);
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
  std::string connInfo("ppgsql://postgres:postgres@atlas.dpi.inpe.br:5433/testPostGIS?PG_CONNECT_TIMEOUT=4&PG_CLIENT_ENCODING=UTF-8");

  te::da::DataSourcePtr srcDs(te::da::DataSourceFactory::make("POSTGIS", connInfo));
  srcDs->open();
  
  std::string inDsetName = "distritos";
  if (!srcDs->dataSetExists(inDsetName))
  {
    std::cout << "Input dataset not found: " << inDsetName << std::endl;
    return false;
  }
  
  std::auto_ptr<te::da::DataSet> inDset = srcDs->getDataSet(inDsetName);
  std::auto_ptr<te::da::DataSetType> inDsetType = srcDs->getDataSetType(inDsetName);

  std::auto_ptr<te::da::DataSetTypeConverter> converter(new te::da::DataSetTypeConverter(inDsetType.get(), srcDs->getCapabilities(), srcDs->getEncoding()));

  double distance = 0.1;

  //options for Polygon Rule.
  int bufferPolygonRule = te::vp::INSIDE_OUTSIDE;

  //options for Boundaries Rule.
  int bufferBoundariesRule = te::vp::DISSOLVE;

  bool copyInputColumns = false;
  int levels = 1;
  
  std::string data_dir = TERRALIB_DATA_DIR;
  
  std::string uriResult(data_dir + "/shape/Buffer/bufferPGISToOGR_distritos.shp");

  std::string tgrInfo("file://" + uriResult);

  te::da::DataSourcePtr trgDs(te::da::DataSourceFactory::make("OGR", tgrInfo));
  trgDs->open();

  std::string outDSet = "bufferPGISToOGR_distritos";

  if (trgDs->dataSetExists(outDSet))
  {
    std::cout << "A dataset with the same requested output dataset name already exists and will be removed: " << outDSet << std::endl;
    trgDs->dropDataSet(outDSet);
	//return true;
  }
  
  // sera feito por algum tipo de factory
  te::vp::BufferOp* bufferOp = new te::vp::BufferQuery();

  bufferOp->setInput(srcDs, inDsetName, converter);
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