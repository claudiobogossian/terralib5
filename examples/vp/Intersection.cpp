#include <terralib/common.h>
#include <terralib/dataaccess.h>
#include <terralib/vp/Intersection.h>

bool IntersectionPGISToOGR()
{
  // this refers to a PostGIS connection, use valid ones for your own environment
  std::map<std::string, std::string> connInfo;
  connInfo["PG_HOST"] = "atlas.dpi.inpe.br" ; 
  connInfo["PG_PORT"] = "5433" ;
  connInfo["PG_USER"] = "postgres";
  //connInfo["PG_PASSWORD"] = "postgres";
  connInfo["PG_DB_NAME"] = "testPostGIS";
  connInfo["PG_CONNECT_TIMEOUT"] = "4"; 
  connInfo["PG_CLIENT_ENCODING"] = "WIN1252";    
  std::auto_ptr<te::da::DataSource> srcDs = te::da::DataSourceFactory::make("POSTGIS");
  srcDs->setConnectionInfo(connInfo);
  srcDs->open();
  
  std::string inDset1 = "plano1"; //dataset name of the first layer.
  std::string inDset2 = "plano2"; //dataset name of the second layer.
  
  if (!srcDs->dataSetExists(inDset1))
  {
    std::cout << "Input dataset not found: " << inDset1 << std::endl;
    return false;
  }
  if (!srcDs->dataSetExists(inDset2))
  {
    std::cout << "Input dataset not found: " << inDset2 << std::endl;
    return false;
  }

  std::string filename(""TE_DATA_EXAMPLE_DIR"/data/vpresult/teste5.shp");
  
  std::map<std::string, std::string> tgrInfo;
  tgrInfo["URI"] = filename;
  tgrInfo["DRIVER"] = "ESRI Shapefile";
  
  std::auto_ptr<te::da::DataSource> trgDs = te::da::DataSourceFactory::make("OGR");
  trgDs->setConnectionInfo(tgrInfo);
  trgDs->open();
  
  std::string outDset = "teste5";
  if (trgDs->dataSetExists(outDset))
  {
    std::cout << "A dataset with the same requested output dataset name already exists: " << outDset << std::endl;
    return false;
  }
  
  bool res = te::vp::Intersection(inDset1, srcDs.get(), inDset2, srcDs.get(), false, outDset,trgDs.get());
  
  return res;
}

bool IntersectionPGISToPGIS()
{
  std::map<std::string, std::string> connInfo;
  connInfo["PG_HOST"] = "atlas.dpi.inpe.br" ; 
  connInfo["PG_PORT"] = "5433" ;
  connInfo["PG_USER"] = "postgres";
  //connInfo["PG_PASSWORD"] = "postgres";
  connInfo["PG_DB_NAME"] = "testPostGIS";
  connInfo["PG_CONNECT_TIMEOUT"] = "4"; 
  connInfo["PG_CLIENT_ENCODING"] = "WIN1252";  
  

  std::auto_ptr<te::da::DataSource> srcDs = te::da::DataSourceFactory::make("POSTGIS");
  srcDs->setConnectionInfo(connInfo);
  srcDs->open();
  
  std::string inDset1 = "plano1"; //dataset name of the first layer.
  std::string inDset2 = "plano2"; //dataset name of the second layer.

  if (!srcDs->dataSetExists(inDset1))
  {
    std::cout << "Input dataset not found: " << inDset1 << std::endl;
    return false;
  }
  if (!srcDs->dataSetExists(inDset2))
  {
    std::cout << "Input dataset not found: " << inDset2 << std::endl;
    return false;
  }
  
  std::string outDS = "resultadoPlano";
  if (srcDs->dataSetExists(outDS))
  {
    std::cout << "A dataset with the same requested output dataset name already exists: " << outDS << std::endl;
    return false;
  }
  
  bool res = te::vp::Intersection(inDset1, srcDs.get(), inDset2, srcDs.get(), false, outDS,srcDs.get());
  
  return res;
}

