
#include "../Config.h"
#include <terralib/common.h>
#include <terralib/dataaccess.h>
#include <terralib/dataaccess/datasource/DataSourceFactory.h>
#include <terralib/attributefill/VectorToRaster.h>

// STL
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>

// Boost
#include <boost/filesystem.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

// Adapt the source and target datasource information to your environment!
bool VectorToRaster()
{
  std::string filename(""TERRALIB_DATA_DIR"/shp/SP_cities.shp");
  
  std::map<std::string, std::string> srcInfo;
  srcInfo["URI"] = filename;
  srcInfo["DRIVER"] = "ESRI Shapefile";

  te::da::DataSourcePtr srcDs = te::da::DataSourceFactory::make("OGR");
  srcDs->setConnectionInfo(srcInfo);
  srcDs->open();

  std::string inDset = "SP_cities";
  if (!srcDs->dataSetExists(inDset))
  {
    std::cout << "input dataset not found: " << inDset << std::endl;
    return false;
  }

  std::auto_ptr<te::da::DataSetType> dsType(srcDs->getDataSetType(inDset));
  std::vector<te::dt::Property*> props = dsType->getProperties();
  
  std::vector<std::string> propName;
  

  for (std::size_t i = 0; i < props.size(); ++i)
  {
    if (props[i]->getName() == "FID" || props[i]->getName() == "POPULACA")
      propName.push_back(props[i]->getName());
  }

  double resolutionX = 0.0111898;
  double resolutionY = 0.00868972;
  int columns = 1000;
  int rows = 1000;
  
  boost::filesystem::path uri(""TERRALIB_DATA_DIR"/rasters/vector2raster.tif");
  std::string dsName = "vector2raster";

  if (boost::filesystem::exists(uri))
  {
    std::cout << "Output file already exists. Remove it or select a new name and try again.";
    return false;
  }

  std::map<std::string, std::string> dsinfo;
  dsinfo["URI"] = uri.string();

  te::da::DataSourcePtr dsOGR(te::da::DataSourceFactory::make("OGR").release());
  dsOGR->setConnectionInfo(dsinfo);
  dsOGR->open();
  if (dsOGR->dataSetExists(dsName))
  {
    std::cout << "There is already a dataset with the requested name in the output data source. Remove it or select a new name and try again.";
    return false;
  }

  te::attributefill::VectorToRaster* vec2rst = new te::attributefill::VectorToRaster();
  
  vec2rst->setInput(srcDs, inDset, dsType);
  vec2rst->setParams( propName, 
                      resolutionX, 
                      resolutionY, 
                      columns, 
                      rows, 
                      false);
  vec2rst->setOutput(dsOGR, dsName);
  
  bool res;

  if (!vec2rst->paramsAreValid())
    res = false;
  else
    res = vec2rst->run();
  
  if (!res)
  {
    dsOGR->close();
    std::cout << "Error: could not generate the operation.";
  }
  dsOGR->close();

  delete vec2rst;
  
  // let's include the new datasource in the managers
  boost::uuids::basic_random_generator<boost::mt19937> gen;
  boost::uuids::uuid u = gen();
  std::string id_ds = boost::uuids::to_string(u);

  te::da::DataSourceInfoPtr ds(new te::da::DataSourceInfo);
  ds->setConnInfo(dsinfo);
  ds->setTitle(uri.stem().string());
  ds->setAccessDriver("GDAL");
  ds->setType("GDAL");
  ds->setDescription(uri.string());
  ds->setId(id_ds);

  te::da::DataSourcePtr newds = te::da::DataSourceManager::getInstance().get(id_ds, "GDAL", ds->getConnInfo());
  newds->open();
  te::da::DataSourceInfoManager::getInstance().add(ds);

  return true;
}