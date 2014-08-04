
#include "../Config.h"
#include <terralib/common.h>
#include <terralib/dataaccess.h>
#include <terralib/dataaccess/datasource/DataSourceFactory.h>
#include <terralib/vp/AggregationMemory.h>
#include <terralib/vp/AggregationOp.h>
#include <terralib/vp/AggregationQuery.h>

// STL
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>


#pragma region
//bool AggregPGISToOGR()
//{
//  // this refers to a PostGIS connection, use valid ones for your own environment
// 
//  std::map<std::string, std::string> connInfo;
//  connInfo["PG_HOST"] = "atlas.dpi.inpe.br" ; 
//  connInfo["PG_PORT"] = "5433" ;
//  connInfo["PG_USER"] = "postgres";
//  connInfo["PG_PASSWORD"] = "postgres";
//  connInfo["PG_DB_NAME"] = "testPostGIS";
//  connInfo["PG_CONNECT_TIMEOUT"] = "4"; 
//  connInfo["PG_CLIENT_ENCODING"] = "CP1252";  
//
//  std::auto_ptr<te::da::DataSource> srcDs = te::da::DataSourceFactory::make("POSTGIS");
//  srcDs->setConnectionInfo(connInfo);
//  srcDs->open();
//  
//  std::string inDset = "sp_cities";
//  
//  if (!srcDs->dataSetExists(inDset))
//  {
//    std::cout << "Input dataset not found: " << inDset << std::endl;
//    return false;
//  }
//  
//  std::string attAgreg = "nomemeso";
//  std::auto_ptr<te::dt::Property> aggregBy = srcDs->getProperty(inDset, attAgreg);
//  
//  std::vector<te::dt::Property*> groupingProperties;
//  groupingProperties.push_back(aggregBy.release());
//  
//  std::string filename(""TE_DATA_EXAMPLE_DIR"/vpresult/sp_nomemesoPGIStoOGR.shp");
//  
//  std::map<std::string, std::string> tgrInfo;
//  tgrInfo["URI"] = filename;
//  tgrInfo["DRIVER"] = "ESRI Shapefile";
//  
//  std::auto_ptr<te::da::DataSource> trgDs = te::da::DataSourceFactory::make("OGR");
//  trgDs->setConnectionInfo(tgrInfo);
//  trgDs->open();
//  
//  std::string outDset = "sp_nomemeso";
//  if (trgDs->dataSetExists(outDset))
//  {
//    std::cout << "A dataset with the same requested output dataset name already exists: " << outDset << std::endl;
//    return false;
//  }
//  
//  std::map<te::dt::Property*, std::vector<te::stat::StatisticalSummary> > stats;
//  
//  bool res = te::vp::Aggregation(inDset,srcDs.get(),groupingProperties,stats,outDset,trgDs.get());
//  
//  return res;
//}

//bool AggregOGRToPGIS()
//{
//  std::string filename(""TE_DATA_EXAMPLE_DIR"/shp/SP_cities.shp");
//  
//  std::map<std::string, std::string> srcInfo;
//  srcInfo["URI"] = filename;
//  srcInfo["DRIVER"] = "ESRI Shapefile";
//  
//  std::auto_ptr<te::da::DataSource> srcDs = te::da::DataSourceFactory::make("OGR");
//  srcDs->setConnectionInfo(srcInfo);
//  srcDs->open();
//  
//  std::string inDset = "sp_cities";
//  if (!srcDs->dataSetExists(inDset))
//  {
//    std::cout << "Input dataset not found: " << inDset << std::endl;
//    return false;
//  }
//  
//  std::string attAgreg = "nomemeso";
//  std::auto_ptr<te::dt::Property> aggregBy = srcDs->getProperty(inDset, attAgreg);
//  std::vector<te::dt::Property*> groupingProperties;
//  groupingProperties.push_back(aggregBy.release());
//  
//  std::map<te::dt::Property*, std::vector<te::stat::StatisticalSummary> > stats;
//  
//  std::map<std::string, std::string> connInfo;
//  connInfo["PG_HOST"] = "atlas.dpi.inpe.br" ; 
//  connInfo["PG_PORT"] = "5433" ;
//  connInfo["PG_USER"] = "postgres";
//  connInfo["PG_PASSWORD"] = "postgres";
//  connInfo["PG_DB_NAME"] = "testPostGIS";
//  connInfo["PG_CONNECT_TIMEOUT"] = "4"; 
//  connInfo["PG_CLIENT_ENCODING"] = "CP1252";
//  
//  std::auto_ptr<te::da::DataSource> trgDs = te::da::DataSourceFactory::make("POSTGIS");
//  trgDs->setConnectionInfo(connInfo);
//  trgDs->open();
//  
//  std::string outDS = "sp_nomemeso";
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

//bool AggregOGRToOGR()
//{
//  std::string filename(""TE_DATA_EXAMPLE_DIR"/shp/SP_cities.shp");
//  
//  std::map<std::string, std::string> srcInfo;
//  srcInfo["URI"] = filename;
//  srcInfo["DRIVER"] = "ESRI Shapefile";
//  
//  std::auto_ptr<te::da::DataSource> srcDs = te::da::DataSourceFactory::make("OGR");
//  srcDs->setConnectionInfo(srcInfo);
//  srcDs->open();
//  
//  std::string inDset = "SP_cities";
//  if (!srcDs->dataSetExists(inDset))
//  {
//    std::cout << "Input dataset not found: " << inDset << std::endl;
//    return false;
//  }
//  
//  std::string attAgreg = "nomemeso";
//  std::auto_ptr<te::dt::Property> aggregBy = srcDs->getProperty(inDset, attAgreg);
//  std::vector<te::dt::Property*> groupingProperties;
//  groupingProperties.push_back(aggregBy.release());
//  
//  std::map<te::dt::Property*, std::vector<te::stat::StatisticalSummary> > stats;
//  
//  std::string filename2(""TE_DATA_EXAMPLE_DIR"/vpresult/sp_nomemesoOGRtoOGR.shp");
//  std::map<std::string, std::string> tgrInfo;
//  tgrInfo["URI"] = filename2;
//  tgrInfo["DRIVER"] = "ESRI Shapefile";
//  
//  std::auto_ptr<te::da::DataSource> trgDs = te::da::DataSourceFactory::make("OGR");
//  trgDs->setConnectionInfo(tgrInfo);
//  trgDs->open();
//  
//  std::string outDS = "sp_nomemeso";
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


//bool AggregPGISToPGIS()
//{
//  std::map<std::string, std::string> connInfo;
//  connInfo["PG_HOST"] = "atlas.dpi.inpe.br" ; 
//  connInfo["PG_PORT"] = "5433" ;
//  connInfo["PG_USER"] = "postgres";
//  connInfo["PG_PASSWORD"] = "postgres";
//  connInfo["PG_DB_NAME"] = "testPostGIS";
//  connInfo["PG_CONNECT_TIMEOUT"] = "4"; 
//  connInfo["PG_CLIENT_ENCODING"] = "CP1252";  
//  
//  std::auto_ptr<te::da::DataSource> srcDs = te::da::DataSourceFactory::make("POSTGIS");
//  srcDs->setConnectionInfo(connInfo);
//  srcDs->open();
//  
//  std::string inDset = "sp_cities";
//  if (!srcDs->dataSetExists(inDset))
//  {
//    std::cout << "Input dataset not found: " << inDset << std::endl;
//    return false;
//  }
//  
//  std::string attAgreg = "nomemeso";
//  std::auto_ptr<te::dt::Property> aggregBy = srcDs->getProperty(inDset, attAgreg);
//  std::vector<te::dt::Property*> groupingProperties;
//  groupingProperties.push_back(aggregBy.release());
//  
//  std::map<te::dt::Property*, std::vector<te::stat::StatisticalSummary> > stats;
//  std::string outDS = "sp_nomemeso_PGIStoPGIS";
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
#pragma endregion Old examples.


//New examples

//OGR to OGR
bool AggregOGRToOGR()
{
  std::string data_dir = TERRALIB_DATA_DIR;
  
  //std::string filename(data_dir + '/shp/SP_cities.shp");
  
  std::string filename(data_dir + "/Nulos/nulos2.shp");
  
  std::map<std::string, std::string> srcInfo;
  srcInfo["URI"] = filename;
  srcInfo["DRIVER"] = "ESRI Shapefile";
  
  //std::auto_ptr<te::da::DataSource> srcDs = te::da::DataSourceFactory::make("OGR");
  te::da::DataSourcePtr srcDs(te::da::DataSourceFactory::make("OGR"));
  
  srcDs->setConnectionInfo(srcInfo);
  srcDs->open();
  
  std::string inDsetName = "nulos2";
  if (!srcDs->dataSetExists(inDsetName))
  {
    std::cout << "Input dataset not found: " << inDsetName << std::endl;
    return false;
  }

  std::auto_ptr<te::da::DataSet> inDset = srcDs->getDataSet(inDsetName);
  std::auto_ptr<te::da::DataSetType> inDsetType = srcDs->getDataSetType(inDsetName);
  
  std::string attAgreg = "grupo";
  std::auto_ptr<te::dt::Property> aggregBy = srcDs->getProperty(inDsetName, attAgreg);
  std::vector<te::dt::Property*> groupingProperties;
  groupingProperties.push_back(aggregBy.release());
  
  std::map<te::dt::Property*, std::vector<te::stat::StatisticalSummary> > stats;
  
  std::auto_ptr<te::dt::Property> prop1 = srcDs->getProperty(inDsetName, "sensor");
  std::vector<te::stat::StatisticalSummary> stat1;
  stat1.push_back(te::stat::SUM);
  stat1.push_back(te::stat::MIN_VALUE);
  stat1.push_back(te::stat::MODE);
  stats.insert(std::make_pair(prop1.release(), stat1));
  
  std::string filename2(data_dir + "/Nulos/result.shp");
  std::map<std::string, std::string> tgrInfo;
  tgrInfo["URI"] = filename2;
  tgrInfo["DRIVER"] = "ESRI Shapefile";

  //std::auto_ptr<te::da::DataSource> trgDs = te::da::DataSourceFactory::make("OGR");
  te::da::DataSourcePtr trgDs(te::da::DataSourceFactory::make("OGR"));
  trgDs->setConnectionInfo(tgrInfo);
  trgDs->open();
  
  std::string outDS = "result";
  
  if (trgDs->dataSetExists(outDS))
  {
    std::cout << "A dataset with the same requested output dataset name already exists: " << outDS << std::endl;
    return false;
  }

  // sera feito por algum tipo de factory
  te::vp::AggregationOp* aggregOp = new te::vp::AggregationMemory();

  aggregOp->setInput(srcDs, inDsetName, inDsetType);
  aggregOp->setOutput(trgDs, outDS);
  aggregOp->setParams(groupingProperties, stats);
  
  bool result;

  if (!aggregOp->paramsAreValid())
    result = false;
  else
    result = aggregOp->run();

  delete aggregOp;
  
  return result;
}

//OGR to Postgis
bool AggregOGRToPGIS()
{
  std::string data_dir = TERRALIB_DATA_DIR;
  
  //std::string filename(data_dir + "/shp/SP_cities.shp");
  
  std::string filename(data_dir + "/Nulos/nulos2.shp");
  
  std::map<std::string, std::string> srcInfo;
  srcInfo["URI"] = filename;
  srcInfo["DRIVER"] = "ESRI Shapefile";
  
  //std::auto_ptr<te::da::DataSource> srcDs = te::da::DataSourceFactory::make("OGR");
  te::da::DataSourcePtr srcDs(te::da::DataSourceFactory::make("OGR"));
  srcDs->setConnectionInfo(srcInfo);
  srcDs->open();
  
  std::string inDsetName = "nulos2";
  if (!srcDs->dataSetExists(inDsetName))
  {
    std::cout << "Input dataset not found: " << inDsetName << std::endl;
    return false;
  }
  
  std::auto_ptr<te::da::DataSet> inDset = srcDs->getDataSet(inDsetName);
  std::auto_ptr<te::da::DataSetType> inDsetType = srcDs->getDataSetType(inDsetName);
  
  std::string attAgreg = "grupo";
  std::auto_ptr<te::dt::Property> aggregBy = srcDs->getProperty(inDsetName, attAgreg);
  std::vector<te::dt::Property*> groupingProperties;
  groupingProperties.push_back(aggregBy.release());
  
  std::map<te::dt::Property*, std::vector<te::stat::StatisticalSummary> > stats;
  
  std::auto_ptr<te::dt::Property> prop1 = srcDs->getProperty(inDsetName, "sensor");
  std::vector<te::stat::StatisticalSummary> stat1;
  stat1.push_back(te::stat::SUM);
  stat1.push_back(te::stat::MIN_VALUE);
  stat1.push_back(te::stat::MODE);
  stats.insert(std::make_pair(prop1.release(), stat1));

  std::map<std::string, std::string> connInfo;
  connInfo["PG_HOST"] = "atlas.dpi.inpe.br" ; 
  connInfo["PG_PORT"] = "5433" ;
  connInfo["PG_USER"] = "postgres";
  connInfo["PG_PASSWORD"] = "postgres";
  connInfo["PG_DB_NAME"] = "testPostGIS";
  connInfo["PG_CONNECT_TIMEOUT"] = "4"; 
  connInfo["PG_CLIENT_ENCODING"] = "CP1252";
  
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
  te::vp::AggregationOp* aggregOp = new te::vp::AggregationMemory();

  aggregOp->setInput(srcDs, inDsetName, inDsetType);
  aggregOp->setOutput(trgDs, outDS);
  aggregOp->setParams(groupingProperties, stats);
  
  bool result;

  if (!aggregOp->paramsAreValid())
    result = false;
  else
    result = aggregOp->run();

  delete aggregOp;
  
  return result;
}

//Postgis to Postgis
bool AggregPGISToPGIS()
{
  std::map<std::string, std::string> connInfo;
  connInfo["PG_HOST"] = "localhost" ; 
  connInfo["PG_PORT"] = "5432" ;
  connInfo["PG_USER"] = "postgres";
  connInfo["PG_PASSWORD"] = "root";
  connInfo["PG_DB_NAME"] = "testPostGIS";
  connInfo["PG_CONNECT_TIMEOUT"] = "4"; 
  connInfo["PG_CLIENT_ENCODING"] = "CP1252"; 

  //std::auto_ptr<te::da::DataSource> srcDs = te::da::DataSourceFactory::make("POSTGIS");
  te::da::DataSourcePtr srcDs(te::da::DataSourceFactory::make("POSTGIS"));
  srcDs->setConnectionInfo(connInfo);
  srcDs->open();
  
  std::string inDsetName = "sp_cities";
  if (!srcDs->dataSetExists(inDsetName))
  {
    std::cout << "Input dataset not found: " << inDsetName << std::endl;
    return false;
  }
  
  std::auto_ptr<te::da::DataSet> inDset = srcDs->getDataSet(inDsetName);
  std::auto_ptr<te::da::DataSetType> inDsetType = srcDs->getDataSetType(inDsetName);
  
  std::string attAgreg = "nomemeso";
  std::auto_ptr<te::dt::Property> aggregBy = srcDs->getProperty(inDsetName, attAgreg);
  std::vector<te::dt::Property*> groupingProperties;
  groupingProperties.push_back(aggregBy.release());
  
  std::map<te::dt::Property*, std::vector<te::stat::StatisticalSummary> > stats;
  
  std::auto_ptr<te::dt::Property> prop1 = srcDs->getProperty(inDsetName, "sprarea");
  std::vector<te::stat::StatisticalSummary> stat1;
  stat1.push_back(te::stat::SUM);
  stat1.push_back(te::stat::MIN_VALUE);
  stats.insert(std::make_pair(prop1.release(), stat1));
  
  std::string outDSet = "result";

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
  te::vp::AggregationOp* aggregOp = new te::vp::AggregationQuery();

  aggregOp->setInput(srcDs, inDsetName, inDsetType);
  aggregOp->setOutput(outDsource, outDSet);
  aggregOp->setParams(groupingProperties, stats);
  
  bool result;

  if (!aggregOp->paramsAreValid())
    result = false;
  else
    result = aggregOp->run();

  delete aggregOp;
  
  return result;
}

//Postgis to OGR
bool AggregPGISToOGR()
{
  std::map<std::string, std::string> connInfo;
  connInfo["PG_HOST"] = "localhost" ; 
  connInfo["PG_PORT"] = "5432" ;
  connInfo["PG_USER"] = "postgres";
  connInfo["PG_PASSWORD"] = "root";
  connInfo["PG_DB_NAME"] = "testPostGIS";
  connInfo["PG_CONNECT_TIMEOUT"] = "4"; 
  connInfo["PG_CLIENT_ENCODING"] = "CP1252"; 

  //std::auto_ptr<te::da::DataSource> srcDs = te::da::DataSourceFactory::make("POSTGIS");
  te::da::DataSourcePtr srcDs(te::da::DataSourceFactory::make("POSTGIS"));
  srcDs->setConnectionInfo(connInfo);
  srcDs->open();
  
  std::string inDsetName = "sp_cities";
  if (!srcDs->dataSetExists(inDsetName))
  {
    std::cout << "Input dataset not found: " << inDsetName << std::endl;
    return false;
  }
  
  std::auto_ptr<te::da::DataSet> inDset = srcDs->getDataSet(inDsetName);
  std::auto_ptr<te::da::DataSetType> inDsetType = srcDs->getDataSetType(inDsetName);
  
  std::string attAgreg = "nomemeso";
  std::auto_ptr<te::dt::Property> aggregBy = srcDs->getProperty(inDsetName, attAgreg);
  std::vector<te::dt::Property*> groupingProperties;
  groupingProperties.push_back(aggregBy.release());
  
  std::map<te::dt::Property*, std::vector<te::stat::StatisticalSummary> > stats;
  
  std::auto_ptr<te::dt::Property> prop1 = srcDs->getProperty(inDsetName, "sprarea");
  std::vector<te::stat::StatisticalSummary> stat1;
  stat1.push_back(te::stat::SUM);
  stat1.push_back(te::stat::MIN_VALUE);
  stats.insert(std::make_pair(prop1.release(), stat1));

  std::string data_dir = TERRALIB_DATA_DIR;
  
  std::string uriResult(data_dir + "/Nulos/result.shp");
  
  std::map<std::string, std::string> tgrInfo;
  tgrInfo["URI"] = uriResult;
  tgrInfo["DRIVER"] = "ESRI Shapefile";

  //std::auto_ptr<te::da::DataSource> trgDs = te::da::DataSourceFactory::make("OGR");
  te::da::DataSourcePtr trgDs(te::da::DataSourceFactory::make("OGR"));
  trgDs->setConnectionInfo(tgrInfo);
  trgDs->open();

  std::string outDSet = "result";

  if (trgDs->dataSetExists(outDSet))
  {
    std::cout << "A dataset with the same requested output dataset name already exists: " << outDSet << std::endl;
    return false;
  }
  
  // sera feito por algum tipo de factory
  te::vp::AggregationOp* aggregOp = new te::vp::AggregationQuery();

  aggregOp->setInput(srcDs, inDsetName, inDsetType);
  aggregOp->setOutput(trgDs, outDSet);
  aggregOp->setParams(groupingProperties, stats);
  
  bool result;

  if (!aggregOp->paramsAreValid())
    result = false;
  else
    result = aggregOp->run();

  delete aggregOp;
  
  return result;
}