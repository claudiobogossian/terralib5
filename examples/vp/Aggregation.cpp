#include <terralib/common.h>
#include <terralib/dataaccess.h>
#include <terralib/vp/Aggregation.h>

// STL
#include <iostream>

bool AggregPGISToOGR()
{
  // this refers to a PostGIS connection, use valid ones for your own environment
 
  std::map<std::string, std::string> connInfo;
  connInfo["PG_HOST"] = "atlas.dpi.inpe.br" ; 
  connInfo["PG_PORT"] = "5433" ;
  connInfo["PG_USER"] = "postgres";
  connInfo["PG_PASSWORD"] = "postgres";
  connInfo["PG_DB_NAME"] = "testPostGIS";
  connInfo["PG_CONNECT_TIMEOUT"] = "4"; 
  connInfo["PG_CLIENT_ENCODING"] = "WIN1252";  

  std::auto_ptr<te::da::DataSource> srcDs = te::da::DataSourceFactory::make("POSTGIS");
  srcDs->setConnectionInfo(connInfo);
  srcDs->open();
  
  std::string inDset = "sp_cities";
  
  if (!srcDs->dataSetExists(inDset))
  {
    std::cout << "Input dataset not found: " << inDset << std::endl;
    return false;
  }
  
  std::string attAgreg = "nomemeso";
  std::auto_ptr<te::dt::Property> aggregBy = srcDs->getProperty(inDset, attAgreg);
  
  std::vector<te::dt::Property*> groupingProperties;
  groupingProperties.push_back(aggregBy.release());
  
  std::string filename(""TE_DATA_EXAMPLE_DIR"/data/vpresult/sp_nomemesoPGIStoOGR.shp");
  
  std::map<std::string, std::string> tgrInfo;
  tgrInfo["URI"] = filename;
  tgrInfo["DRIVER"] = "ESRI Shapefile";
  
  std::auto_ptr<te::da::DataSource> trgDs = te::da::DataSourceFactory::make("OGR");
  trgDs->setConnectionInfo(tgrInfo);
  trgDs->open();
  
  std::string outDset = "sp_nomemeso";
  if (trgDs->dataSetExists(outDset))
  {
    std::cout << "A dataset with the same requested output dataset name already exists: " << outDset << std::endl;
    return false;
  }
  
  std::map<te::dt::Property*, std::vector<te::stat::StatisticalSummary> > stats;
  
  bool res = te::vp::Aggregation(inDset,srcDs.get(),groupingProperties,stats,outDset,trgDs.get());
  
  return res;
}

bool AggregOGRToPGIS()
{
  std::string filename(""TE_DATA_EXAMPLE_DIR"/data/shp/SP_cities.shp");
  
  std::map<std::string, std::string> srcInfo;
  srcInfo["URI"] = filename;
  srcInfo["DRIVER"] = "ESRI Shapefile";
  
  std::auto_ptr<te::da::DataSource> srcDs = te::da::DataSourceFactory::make("OGR");
  srcDs->setConnectionInfo(srcInfo);
  srcDs->open();
  
  std::string inDset = "sp_cities";
  if (!srcDs->dataSetExists(inDset))
  {
    std::cout << "Input dataset not found: " << inDset << std::endl;
    return false;
  }
  
  std::string attAgreg = "nomemeso";
  std::auto_ptr<te::dt::Property> aggregBy = srcDs->getProperty(inDset, attAgreg);
  std::vector<te::dt::Property*> groupingProperties;
  groupingProperties.push_back(aggregBy.release());
  
  std::map<te::dt::Property*, std::vector<te::stat::StatisticalSummary> > stats;
  
  std::map<std::string, std::string> connInfo;
  connInfo["PG_HOST"] = "atlas.dpi.inpe.br" ; 
  connInfo["PG_PORT"] = "5433" ;
  connInfo["PG_USER"] = "postgres";
  connInfo["PG_PASSWORD"] = "postgres";
  connInfo["PG_DB_NAME"] = "testPostGIS";
  connInfo["PG_CONNECT_TIMEOUT"] = "4"; 
  connInfo["PG_CLIENT_ENCODING"] = "WIN1252";  
  
  std::auto_ptr<te::da::DataSource> trgDs = te::da::DataSourceFactory::make("POSTGIS");
  trgDs->setConnectionInfo(connInfo);
  trgDs->open();
  
  std::string outDS = "sp_nomemeso";
  
  if (trgDs->dataSetExists(outDS))
  {
    std::cout << "A dataset with the same requested output dataset name already exists: " << outDS << std::endl;
    return false;
  }
  
  bool res = te::vp::Aggregation(inDset,srcDs.get(),groupingProperties,stats,outDS,trgDs.get());
  
  return res;
}

bool AggregOGRToOGR()
{
  std::string filename(""TE_DATA_EXAMPLE_DIR"/data/shp/SP_cities.shp");
  
  std::map<std::string, std::string> srcInfo;
  srcInfo["URI"] = filename;
  srcInfo["DRIVER"] = "ESRI Shapefile";
  
  std::auto_ptr<te::da::DataSource> srcDs = te::da::DataSourceFactory::make("OGR");
  srcDs->setConnectionInfo(srcInfo);
  srcDs->open();
  
  std::string inDset = "SP_cities";
  if (!srcDs->dataSetExists(inDset))
  {
    std::cout << "Input dataset not found: " << inDset << std::endl;
    return false;
  }
  
  std::string attAgreg = "nomemeso";
  std::auto_ptr<te::dt::Property> aggregBy = srcDs->getProperty(inDset, attAgreg);
  std::vector<te::dt::Property*> groupingProperties;
  groupingProperties.push_back(aggregBy.release());
  
  std::map<te::dt::Property*, std::vector<te::stat::StatisticalSummary> > stats;
  
  std::string filename2(""TE_DATA_EXAMPLE_DIR"/data/vpresult/sp_nomemesoOGRtoOGR.shp");
  std::map<std::string, std::string> tgrInfo;
  tgrInfo["URI"] = filename2;
  tgrInfo["DRIVER"] = "ESRI Shapefile";
  
  std::auto_ptr<te::da::DataSource> trgDs = te::da::DataSourceFactory::make("OGR");
  trgDs->setConnectionInfo(tgrInfo);
  trgDs->open();
  
  std::string outDS = "sp_nomemeso";
  
  if (trgDs->dataSetExists(outDS))
  {
    std::cout << "A dataset with the same requested output dataset name already exists: " << outDS << std::endl;
    return false;
  }
  
  bool res = te::vp::Aggregation(inDset,srcDs.get(),groupingProperties,stats,outDS,trgDs.get());
  
  return res;
}

bool AggregPGISToPGIS()
{
  std::map<std::string, std::string> connInfo;
  connInfo["PG_HOST"] = "atlas.dpi.inpe.br" ; 
  connInfo["PG_PORT"] = "5433" ;
  connInfo["PG_USER"] = "postgres";
  connInfo["PG_PASSWORD"] = "postgres";
  connInfo["PG_DB_NAME"] = "testPostGIS";
  connInfo["PG_CONNECT_TIMEOUT"] = "4"; 
  connInfo["PG_CLIENT_ENCODING"] = "WIN1252";  
  
  std::auto_ptr<te::da::DataSource> srcDs = te::da::DataSourceFactory::make("POSTGIS");
  srcDs->setConnectionInfo(connInfo);
  srcDs->open();
  
  std::string inDset = "sp_cities";
  if (!srcDs->dataSetExists(inDset))
  {
    std::cout << "Input dataset not found: " << inDset << std::endl;
    return false;
  }
  
  std::string attAgreg = "nomemeso";
  std::auto_ptr<te::dt::Property> aggregBy = srcDs->getProperty(inDset, attAgreg);
  std::vector<te::dt::Property*> groupingProperties;
  groupingProperties.push_back(aggregBy.release());
  
  std::map<te::dt::Property*, std::vector<te::stat::StatisticalSummary> > stats;
  std::string outDS = "sp_nomemeso_PGIStoPGIS";
  if (srcDs->dataSetExists(outDS))
  {
    std::cout << "A dataset with the same requested output dataset name already exists: " << outDS << std::endl;
    return false;
  }
  
  bool res = te::vp::Aggregation(inDset,srcDs.get(),groupingProperties,stats,outDS,srcDs.get());
  
  return res;
}

