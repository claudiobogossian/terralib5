//TerraLib
#include <terralib/dataaccess/datasource/DataSource.h>
#include <terralib/dataaccess/datasource/DataSourceFactory.h>
#include <terralib/dataaccess/utils/Utils.h>
#include <terralib/raster/Grid.h>
#include <terralib/raster/RasterFactory.h>
#include "GraphExamples.h"



std::auto_ptr<te::rst::Raster> OpenRaster(const std::string& pathName, const int& srid)
{
  std::cout << std::endl << "Open Raster: " << pathName << std::endl;

  //set connection info
  std::map<std::string, std::string> rinfo;
  rinfo["URI"] = pathName;

  // open input raster
  te::rst::Raster* raster = te::rst::RasterFactory::open(rinfo);
  raster->getGrid()->setSRID(srid);

  std::auto_ptr<te::rst::Raster> rst(raster);

  return rst;
}

std::auto_ptr<te::da::DataSource> OpenDataSource(std::map<std::string, std::string> connInfo, std::string dsType)
{
  std::auto_ptr<te::da::DataSource> ds = te::da::DataSourceFactory::make(dsType);
  ds->setConnectionInfo(connInfo);
  ds->open();

  return ds;
}

std::auto_ptr<te::da::DataSource> OpenOGRDataSource(const std::string& pathName)
{
  std::map<std::string, std::string> connInfo;
  connInfo["URI"] = pathName;

  return OpenDataSource(connInfo, "OGR");
}

std::auto_ptr<te::gm::Envelope> getDataSetExtent(te::da::DataSource* ds, std::string dataSetName)
{
  std::auto_ptr<te::da::DataSet> dataSet = ds->getDataSet(dataSetName);

  std::size_t geomPos = te::da::GetFirstSpatialPropertyPos(dataSet.get());

  return dataSet->getExtent(geomPos);
}
