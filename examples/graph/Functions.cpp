//TerraLib
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
