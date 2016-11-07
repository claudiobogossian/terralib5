
#include "../Config.h"
#include <terralib/core/filesystem/FileSystem.h>
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
  std::string dataDirRaster(TERRALIB_DATA_DIR  "/geotiff");
  std::string fileNameRaster = "cbers2b_rgb342_crop.tif";

  std::string connInfoRaster("file://" + dataDirRaster);

  te::da::DataSourcePtr dsGDAL(te::da::DataSourceFactory::make("GDAL", connInfoRaster).release());
  dsGDAL->open();

  std::auto_ptr<te::da::DataSet> dsRaster = dsGDAL->getDataSet(fileNameRaster);

  std::size_t rpos = te::da::GetFirstPropertyPos(dsRaster.get(), te::dt::RASTER_TYPE);

  std::auto_ptr<te::rst::Raster> inputRst = dsRaster->getRaster(rpos);


// Input Vector
  std::string dataDirVector(TERRALIB_DATA_DIR "/shape/shapeTeste.shp");
  std::string fileNameVector = "shapeTeste";

  std::string connInfoVector("file://" + dataDirVector);

  te::da::DataSourcePtr dsOGR(te::da::DataSourceFactory::make("OGR", connInfoVector).release());
  dsOGR->open();

  std::auto_ptr<te::da::DataSetType>dsTypeVector = dsOGR->getDataSetType(fileNameVector);

  std::auto_ptr<te::da::DataSetTypeConverter> converterVector(new te::da::DataSetTypeConverter(dsTypeVector.get(), dsOGR->getCapabilities(), dsOGR->getEncoding()));
  
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
  if (te::core::FileSystem::exists(uri.string()))
  {
    std::cout << "Output file already exists. Remove it or select a new name and try again.\n";
    return false;
  }
  
  std::string dsinfo("file://" + uri.string());

  te::da::DataSourcePtr outDataSource(te::da::DataSourceFactory::make("OGR", dsinfo).release());
  outDataSource->open();
  if (outDataSource->dataSetExists(outputdataset))
  {
    std::cout << "There is already a dataset with the requested name in the output data source. Remove it or select a new name and try again.\n";
    return false;
  }

// Processing
  te::attributefill::RasterToVector* rst2vec = new te::attributefill::RasterToVector();
  
  rst2vec->setInput(inputRst.get(),
                    dsOGR, fileNameVector, converterVector);
  rst2vec->setParams(vecBands, vecStat, false, false);
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
  std::string dataDirRaster(TERRALIB_DATA_DIR "/geotiff");
  std::string fileNameRaster = "cbers2b_rgb342_crop.tif";

  std::string connInfoRaster("file://" + dataDirRaster);
  te::da::DataSourcePtr dsGDAL(te::da::DataSourceFactory::make("GDAL", connInfoRaster).release());
  dsGDAL->open();

  std::auto_ptr<te::da::DataSet> dsRaster = dsGDAL->getDataSet(fileNameRaster);

  std::size_t rpos = te::da::GetFirstPropertyPos(dsRaster.get(), te::dt::RASTER_TYPE);

  std::auto_ptr<te::rst::Raster> inputRst = dsRaster->getRaster(rpos);


  // Input Vector
  std::string dataDirVector(TERRALIB_DATA_DIR "/shape/shapeTeste.shp");
  std::string fileNameVector = "shapeTeste";

  std::string connInfoVector("file://" + dataDirVector);

  te::da::DataSourcePtr dsOGR(te::da::DataSourceFactory::make("OGR", connInfoVector).release());
  dsOGR->open();

  std::auto_ptr<te::da::DataSetType>dsTypeVector = dsOGR->getDataSetType(fileNameVector);

  std::auto_ptr<te::da::DataSetTypeConverter> converterVector(new te::da::DataSetTypeConverter(dsTypeVector.get(), dsOGR->getCapabilities(), dsOGR->getEncoding()));


  // Params
  std::vector<unsigned int> vecBands;
  vecBands.push_back(0);
  vecBands.push_back(1);
  
  std::vector<te::stat::StatisticalSummary> vecStat;
  vecStat.push_back(te::stat::MIN_VALUE);
  vecStat.push_back(te::stat::MAX_VALUE);
  vecStat.push_back(te::stat::SUM);

  // Output Vector
  std::string connInfo("ppgsql://postgres:postgres@atlas.dpi.inpe.br:5433/testPostGIS");

  te::da::DataSourcePtr outDataSource(te::da::DataSourceFactory::make("POSTGIS", connInfo).release());
  outDataSource->open();

  std::string outputdataset = "raster2vector";

  if (outDataSource->dataSetExists(outputdataset))
  {
    std::cout << "There is already a dataset with the requested name in the output data source. Remove it or select a new name and try again.\n";
    return false;
  }

  // Processing
  te::attributefill::RasterToVector* rst2vec = new te::attributefill::RasterToVector();

  rst2vec->setInput(inputRst.get(),
                    dsOGR, fileNameVector, converterVector);
  rst2vec->setParams(vecBands, vecStat, false, false);
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
