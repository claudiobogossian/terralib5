
#include "../Config.h"
#include <terralib/common.h>
#include <terralib/dataaccess.h>
#include <terralib/dataaccess/datasource/DataSourceFactory.h>
#include <terralib/attributefill/RasterToVector.h>

// STL
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>

// Boost
#include <boost/filesystem.hpp>

// Adapt the source and target datasource information to your environment!
bool RasterToVectorInSHP()
{
// Input Raster
  std::string dataDirRaster(""TERRALIB_DATA_DIR"/rasters");
  std::string fileNameRaster = "cbers2b_rgb342_crop.tif";

  std::map<std::string, std::string> connInfoRaster;
  connInfoRaster["URI"] = dataDirRaster + "/" + fileNameRaster;

  te::da::DataSourcePtr dsGDAL(te::da::DataSourceFactory::make("GDAL").release());

  dsGDAL->setConnectionInfo(connInfoRaster);
  dsGDAL->open();

  std::auto_ptr<te::da::DataSetType>dsTypeRaster = dsGDAL->getDataSetType(fileNameRaster);


// Input Vector
  std::string dataDirVector(""TERRALIB_DATA_DIR"/shp/shapeTeste.shp");
  std::string fileNameVector = "shapeTeste";

  std::map<std::string, std::string> connInfoVector;
  connInfoVector["URI"] = dataDirVector;
  connInfoVector["DRIVER"] = "ESRI Shapefile";

  te::da::DataSourcePtr dsOGR(te::da::DataSourceFactory::make("OGR").release());
  dsOGR->setConnectionInfo(connInfoVector);
  dsOGR->open();

  std::auto_ptr<te::da::DataSetType>dsTypeVector = dsOGR->getDataSetType(fileNameVector);
  
  // Params
  std::vector<unsigned int> vecBands;
  vecBands.push_back(0);
  vecBands.push_back(1);

  std::vector<te::stat::StatisticalSummary> vecStat;
  vecStat.push_back(te::stat::MIN_VALUE);
  vecStat.push_back(te::stat::MAX_VALUE);
  vecStat.push_back(te::stat::SUM);

// Output Vector
  std::string outputdataset = "raster2vector";
  std::string outputdatadir = "/shp/" + outputdataset + ".shp";

  boost::filesystem::path uri(TERRALIB_DATA_DIR + outputdatadir);
  if (boost::filesystem::exists(uri))
  {
    std::cout << "Output file already exists. Remove it or select a new name and try again.\n";
    return false;
  }
  
  std::map<std::string, std::string> dsinfo;
  dsinfo["URI"] = uri.string();
  dsinfo["DRIVER"] = "ESRI Shapefile";

  te::da::DataSourcePtr outDataSource(te::da::DataSourceFactory::make("OGR").release());
  outDataSource->setConnectionInfo(dsinfo);
  outDataSource->open();
  if (outDataSource->dataSetExists(outputdataset))
  {
    std::cout << "There is already a dataset with the requested name in the output data source. Remove it or select a new name and try again.\n";
    return false;
  }

// Processing
  te::attributefill::RasterToVector* rst2vec = new te::attributefill::RasterToVector();
  
  rst2vec->setInput(dsGDAL, fileNameRaster, dsTypeRaster, 
                    dsOGR, fileNameVector, dsTypeVector);
  rst2vec->setParams(vecBands, vecStat, false);
  rst2vec->setOutput(outDataSource, outputdataset);

  bool res;

  if (!rst2vec->paramsAreValid())
    res = false;
  else
    res = rst2vec->run();

  if (!res)
  {
    dsOGR->close();
    std::cout << "Error: could not generate the operation.";
  }
  dsOGR->close();

  delete rst2vec;
  
  return res;
}

// Adapt the source and target datasource information to your environment!
bool RasterToVectorInPGIS()
{
  // Input Raster
  std::string dataDirRaster(""TERRALIB_DATA_DIR"/rasters");
  std::string fileNameRaster = "cbers2b_rgb342_crop.tif";

  std::map<std::string, std::string> connInfoRaster;
  connInfoRaster["URI"] = dataDirRaster + "/" + fileNameRaster;

  te::da::DataSourcePtr dsGDAL(te::da::DataSourceFactory::make("GDAL").release());

  dsGDAL->setConnectionInfo(connInfoRaster);
  dsGDAL->open();

  std::auto_ptr<te::da::DataSetType>dsTypeRaster = dsGDAL->getDataSetType(fileNameRaster);


  // Input Vector
  std::string dataDirVector(""TERRALIB_DATA_DIR"/shp/shapeTeste.shp");
  std::string fileNameVector = "shapeTeste";

  std::map<std::string, std::string> connInfoVector;
  connInfoVector["URI"] = dataDirVector;
  connInfoVector["DRIVER"] = "ESRI Shapefile";

  te::da::DataSourcePtr dsOGR(te::da::DataSourceFactory::make("OGR").release());
  dsOGR->setConnectionInfo(connInfoVector);
  dsOGR->open();

  std::auto_ptr<te::da::DataSetType>dsTypeVector = dsOGR->getDataSetType(fileNameVector);


  // Params
  std::vector<unsigned int> vecBands;
  vecBands.push_back(0);
  vecBands.push_back(1);
  
  std::vector<te::stat::StatisticalSummary> vecStat;
  vecStat.push_back(te::stat::MIN_VALUE);
  vecStat.push_back(te::stat::MAX_VALUE);
  vecStat.push_back(te::stat::SUM);

  // Output Vector

  std::map<std::string, std::string> connInfo;
  connInfo["PG_HOST"] = "atlas.dpi.inpe.br";
  connInfo["PG_PORT"] = "5433";
  connInfo["PG_USER"] = "postgres";
  connInfo["PG_PASSWORD"] = "postgres";
  connInfo["PG_DB_NAME"] = "testPostGIS";
  connInfo["PG_CONNECT_TIMEOUT"] = "4";
  connInfo["PG_CLIENT_ENCODING"] = "CP1252";

  te::da::DataSourcePtr outDataSource(te::da::DataSourceFactory::make("POSTGIS").release());
  outDataSource->setConnectionInfo(connInfo);
  outDataSource->open();

  std::string outputdataset = "raster2vector";

  if (outDataSource->dataSetExists(outputdataset))
  {
    std::cout << "There is already a dataset with the requested name in the output data source. Remove it or select a new name and try again.\n";
    return false;
  }

  // Processing
  te::attributefill::RasterToVector* rst2vec = new te::attributefill::RasterToVector();

  rst2vec->setInput(dsGDAL, fileNameRaster, dsTypeRaster,
                    dsOGR, fileNameVector, dsTypeVector);
  rst2vec->setParams(vecBands, vecStat, false);
  rst2vec->setOutput(outDataSource, outputdataset);

  bool res;

  if (!rst2vec->paramsAreValid())
    res = false;
  else
    res = rst2vec->run();

  if (!res)
  {
    dsOGR->close();
    std::cout << "Error: could not generate the operation.";
  }
  dsOGR->close();

  delete rst2vec;

  return res;
}