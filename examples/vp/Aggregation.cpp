
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

//OGR to OGR
bool AggregOGRToOGR()
{
  std::string data_dir = TERRALIB_DATA_DIR;

  std::string filename(data_dir + "/Nulos/nulos2.shp");
  
  std::string srcInfo ("file://" + filename);
  
  te::da::DataSourcePtr srcDs(te::da::DataSourceFactory::make("OGR", srcInfo));
  srcDs->open();
  
  std::string inDsetName = "nulos2";
  if (!srcDs->dataSetExists(inDsetName))
  {
    std::cout << "Input dataset not found: " << inDsetName << std::endl;
    return false;
  }

  std::auto_ptr<te::da::DataSet> inDset = srcDs->getDataSet(inDsetName);
  std::auto_ptr<te::da::DataSetType> inDsetType = srcDs->getDataSetType(inDsetName);

  std::auto_ptr<te::da::DataSetTypeConverter> converter(new te::da::DataSetTypeConverter(inDsetType.get(), srcDs->getCapabilities(), srcDs->getEncoding()));
  
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
  std::string tgrInfo ("file://" + filename2);

  te::da::DataSourcePtr trgDs(te::da::DataSourceFactory::make("OGR", tgrInfo));
  trgDs->open();
  
  std::string outDS = "result";
  
  if (trgDs->dataSetExists(outDS))
  {
    std::cout << "A dataset with the same requested output dataset name already exists: " << outDS << std::endl;
    return false;
  }

  // sera feito por algum tipo de factory
  te::vp::AggregationOp* aggregOp = new te::vp::AggregationMemory();

  aggregOp->setInput(srcDs, inDsetName, converter);
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
  
  std::string filename(data_dir + "/Nulos/nulos2.shp");
  
  std::string srcInfo("file://" + filename);

  te::da::DataSourcePtr srcDs(te::da::DataSourceFactory::make("OGR", srcInfo));
  srcDs->open();
  
  std::string inDsetName = "nulos2";
  if (!srcDs->dataSetExists(inDsetName))
  {
    std::cout << "Input dataset not found: " << inDsetName << std::endl;
    return false;
  }
  
  std::auto_ptr<te::da::DataSet> inDset = srcDs->getDataSet(inDsetName);
  std::auto_ptr<te::da::DataSetType> inDsetType = srcDs->getDataSetType(inDsetName);

  std::auto_ptr<te::da::DataSetTypeConverter> converter(new te::da::DataSetTypeConverter(inDsetType.get(), srcDs->getCapabilities(), srcDs->getEncoding()));
  
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

  std::string connInfo("ppgsql://postgres:postgres@atlas.dpi.inpe.br:5433/testPostGIS");

  te::da::DataSourcePtr trgDs(te::da::DataSourceFactory::make("POSTGIS", connInfo));
  trgDs->open();
  
  std::string outDS = "result";
  
  if (trgDs->dataSetExists(outDS))
  {
    std::cout << "A dataset with the same requested output dataset name already exists: " << outDS << std::endl;
    return false;
  }
  
  // sera feito por algum tipo de factory
  te::vp::AggregationOp* aggregOp = new te::vp::AggregationMemory();

  aggregOp->setInput(srcDs, inDsetName, converter);
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
  std::string connInfo("ppgsql://postgres:postgres@atlas.dpi.inpe.br:5433/testPostGIS");

  te::da::DataSourcePtr srcDs(te::da::DataSourceFactory::make("POSTGIS", connInfo));
  srcDs->open();
  
  std::string inDsetName = "sp_cities";
  if (!srcDs->dataSetExists(inDsetName))
  {
    std::cout << "Input dataset not found: " << inDsetName << std::endl;
    return false;
  }
  
  std::auto_ptr<te::da::DataSet> inDset = srcDs->getDataSet(inDsetName);
  std::auto_ptr<te::da::DataSetType> inDsetType = srcDs->getDataSetType(inDsetName);

  std::auto_ptr<te::da::DataSetTypeConverter> converter(new te::da::DataSetTypeConverter(inDsetType.get(), srcDs->getCapabilities(), srcDs->getEncoding()));
  
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

  te::da::DataSourcePtr outDsource(te::da::DataSourceFactory::make("POSTGIS", connInfo));
  outDsource->open();

  if (outDsource->dataSetExists(outDSet))
  {
    std::cout << "A dataset with the same requested output dataset name already exists: " << outDSet << std::endl;
    return false;
  }
  
  // sera feito por algum tipo de factory
  te::vp::AggregationOp* aggregOp = new te::vp::AggregationQuery();

  aggregOp->setInput(srcDs, inDsetName, converter);
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
  std::string connInfo("ppgsql://postgres:postgres@atlas.dpi.inpe.br:5433/testPostGIS");

  te::da::DataSourcePtr srcDs(te::da::DataSourceFactory::make("POSTGIS", connInfo));
  srcDs->open();
  
  std::string inDsetName = "sp_cities";
  if (!srcDs->dataSetExists(inDsetName))
  {
    std::cout << "Input dataset not found: " << inDsetName << std::endl;
    return false;
  }
  
  std::auto_ptr<te::da::DataSet> inDset = srcDs->getDataSet(inDsetName);
  std::auto_ptr<te::da::DataSetType> inDsetType = srcDs->getDataSetType(inDsetName);

  std::auto_ptr<te::da::DataSetTypeConverter> converter(new te::da::DataSetTypeConverter(inDsetType.get(), srcDs->getCapabilities(), srcDs->getEncoding()));
  
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
  
  std::string tgrInfo("file://" + uriResult);

  te::da::DataSourcePtr trgDs(te::da::DataSourceFactory::make("OGR", tgrInfo));
  trgDs->open();

  std::string outDSet = "result";

  if (trgDs->dataSetExists(outDSet))
  {
    std::cout << "A dataset with the same requested output dataset name already exists: " << outDSet << std::endl;
    return false;
  }
  
  // sera feito por algum tipo de factory
  te::vp::AggregationOp* aggregOp = new te::vp::AggregationQuery();

  aggregOp->setInput(srcDs, inDsetName, converter);
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