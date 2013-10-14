#include <terralib/common.h>
#include <terralib/dataaccess.h>
#include <terralib/vp/Intersection.h>

bool IntersectionPGISToOGR()
{
  // this refers to a PostGIS connection, use valid ones for your own environment
  std::map<std::string, std::string> srcInfo;
  srcInfo["PG_HOST"] = "localhost" ;
  srcInfo["PG_USER"] = "postgres";
  srcInfo["PG_PASSWORD"] = "root";
  srcInfo["PG_DB_NAME"] = "testPostGIS";
  srcInfo["PG_CONNECT_TIMEOUT"] = "4";
  srcInfo["PG_PORT"]="5432";
  
  std::auto_ptr<te::da::DataSource> srcDs = te::da::DataSourceFactory::make("POSTGIS");
  srcDs->setConnectionInfo(srcInfo);
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
    std::cout << "Input dataset not found: " << inDset1 << std::endl;
    return false;
  }

  std::string filename(""TE_DATA_EXAMPLE_DIR"/data/vpresult/teste.gml");
  
  std::map<std::string, std::string> tgrInfo;
  tgrInfo["URI"] = filename;
  tgrInfo["DRIVER"] = "GML";
  
  std::auto_ptr<te::da::DataSource> trgDs = te::da::DataSourceFactory::make("OGR");
  trgDs->setConnectionInfo(tgrInfo);
  trgDs->open();
  
  std::string outDset = "regioes";
  if (trgDs->dataSetExists(outDset))
  {
    std::cout << "A dataset with the same requested output dataset name already exists: " << outDset << std::endl;
    return false;
  }
  
  bool res = te::vp::Intersection(inDset1, srcDs.get(), inDset2, srcDs.get(), false, outDset,trgDs.get());
  
  return res;
}

//bool IntersectionOGRToPGIS()
//{
//  std::string filename1(""TE_DATA_EXAMPLE_DIR"/data/Shape/PLANO_plano1_pol.shp");
//  
//  std::map<std::string, std::string> srcInfo;
//  srcInfo["URI"] = filename1;
//  srcInfo["DRIVER"] = "ESRI Shapefile";
//  
//  std::auto_ptr<te::da::DataSource> srcDs = te::da::DataSourceFactory::make("OGR");
//  srcDs->setConnectionInfo(srcInfo);
//  srcDs->open();
//  
//  std::string inDset = "regioes";
//  if (!srcDs->dataSetExists(inDset))
//  {
//    std::cout << "Input dataset not found: " << inDset << std::endl;
//    return false;
//  }
//  
//  std::string attAgreg = "regiao";
//  std::auto_ptr<te::dt::Property> aggregBy = srcDs->getProperty(inDset, attAgreg);
//  std::vector<te::dt::Property*> groupingProperties;
//  groupingProperties.push_back(aggregBy.release());
//  
//  std::map<te::dt::Property*, std::vector<te::stat::StatisticalSummary> > stats;
//  
//  std::map<std::string, std::string> tgrInfo;
//  tgrInfo["PG_HOST"] = "localhost" ;
//  tgrInfo["PG_USER"] = "postgres";
//  tgrInfo["PG_PASSWORD"] = "root";
//  tgrInfo["PG_DB_NAME"] = "testPostGIS";
//  tgrInfo["PG_CONNECT_TIMEOUT"] = "4";
//  tgrInfo["PG_PORT"]="5432";
//  
//  std::auto_ptr<te::da::DataSource> trgDs = te::da::DataSourceFactory::make("POSTGIS");
//  trgDs->setConnectionInfo(tgrInfo);
//  trgDs->open();
//  
//  std::string outDS = "regioes";
//  
//  if (trgDs->dataSetExists(outDS))
//  {
//    std::cout << "A dataset with the same requested output dataset name already exists: " << outDS << std::endl;
//    return false;
//  }
//  
//  bool res = te::vp::Aggregation(inDset,srcDs.get(),groupingProperties,stats,outDS,trgDs.get());
//  
//  return res;
//}

//bool OGRToOGR()
//{
//  std::string filename(""TE_DATA_EXAMPLE_DIR"/data/ogr/cidades_sp.shp");
//  
//  std::map<std::string, std::string> srcInfo;
//  srcInfo["URI"] = filename;
//  srcInfo["DRIVER"] = "ESRI Shapefile";
//  
//  std::auto_ptr<te::da::DataSource> srcDs = te::da::DataSourceFactory::make("OGR");
//  srcDs->setConnectionInfo(srcInfo);
//  srcDs->open();
//  
//  std::string inDset = "cidades_sp";
//  if (!srcDs->dataSetExists(inDset))
//  {
//    std::cout << "Input dataset not found: " << inDset << std::endl;
//    return false;
//  }
//  
//  std::string attAgreg = "codmeso";
//  std::auto_ptr<te::dt::Property> aggregBy = srcDs->getProperty(inDset, attAgreg);
//  std::vector<te::dt::Property*> groupingProperties;
//  groupingProperties.push_back(aggregBy.release());
//  
//  std::map<te::dt::Property*, std::vector<te::stat::StatisticalSummary> > stats;
//  
//  std::string filename2(""TE_DATA_EXAMPLE_DIR"/data/vpresult/regioes_sp.shp");
//  std::map<std::string, std::string> tgrInfo;
//  tgrInfo["URI"] = filename2;
//  tgrInfo["DRIVER"] = "ESRI Shapefile";
//  
//  std::auto_ptr<te::da::DataSource> trgDs = te::da::DataSourceFactory::make("OGR");
//  trgDs->setConnectionInfo(tgrInfo);
//  trgDs->open();
//  
//  std::string outDS = "regioes_sp_res";
//  
//  if (trgDs->dataSetExists(outDS))
//  {
//    std::cout << "A dataset with the same requested output dataset name already exists: " << outDS << std::endl;
//    return false;
//  }
//  
//  bool res = te::vp::Aggregation(inDset,srcDs.get(),groupingProperties,stats,outDS,trgDs.get());
//  
//  return res;
//}
//
//bool PGISToPGIS()
//{
//  std::map<std::string, std::string> srcInfo;
//  srcInfo["PG_HOST"] = "localhost" ;
//  srcInfo["PG_USER"] = "postgres";
//  srcInfo["PG_PASSWORD"] = "root";
//  srcInfo["PG_DB_NAME"] = "testPostGIS";
//  srcInfo["PG_CONNECT_TIMEOUT"] = "4";
//  srcInfo["PG_PORT"]="5432";
//  
//  std::auto_ptr<te::da::DataSource> srcDs = te::da::DataSourceFactory::make("POSTGIS");
//  srcDs->setConnectionInfo(srcInfo);
//  srcDs->open();
//  
//  std::string inDset = "cidades_sp";
//  if (!srcDs->dataSetExists(inDset))
//  {
//    std::cout << "Input dataset not found: " << inDset << std::endl;
//    return false;
//  }
//  
//  std::string attAgreg = "codmeso";
//  std::auto_ptr<te::dt::Property> aggregBy = srcDs->getProperty(inDset, attAgreg);
//  std::vector<te::dt::Property*> groupingProperties;
//  groupingProperties.push_back(aggregBy.release());
//  
//  std::map<te::dt::Property*, std::vector<te::stat::StatisticalSummary> > stats;
//  std::string outDS = "regioes_sp2";
//  if (srcDs->dataSetExists(outDS))
//  {
//    std::cout << "A dataset with the same requested output dataset name already exists: " << outDS << std::endl;
//    return false;
//  }
//  
//  bool res = te::vp::Aggregation(inDset,srcDs.get(),groupingProperties,stats,outDS,srcDs.get());
//  
//  return res;
//}

