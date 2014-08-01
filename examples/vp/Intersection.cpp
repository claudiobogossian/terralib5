
#include "../Config.h"
#include <terralib/common.h>
#include <terralib/dataaccess.h>
//#include <terralib/vp/Intersection.h>
#include <terralib/vp/IntersectionMemory.h>
#include <terralib/vp/IntersectionOp.h>
#include <terralib/vp/IntersectionQuery.h>

// STL
#include <iostream>


#pragma region
//bool IntersectionPGISToOGR()
//{
//  // this refers to a PostGIS connection, use valid ones for your own environment
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
//  std::string inDset1 = "plano1"; //dataset name of the first layer.
//  std::string inDset2 = "plano2"; //dataset name of the second layer.
//  
//  if (!srcDs->dataSetExists(inDset1))
//  {
//    std::cout << "Input dataset not found: " << inDset1 << std::endl;
//    return false;
//  }
//  if (!srcDs->dataSetExists(inDset2))
//  {
//    std::cout << "Input dataset not found: " << inDset2 << std::endl;
//    return false;
//  }
//
//  std::string filename(""TE_DATA_EXAMPLE_DIR"/vpresult/intersectionResult.shp");
//  
//  std::map<std::string, std::string> tgrInfo;
//  tgrInfo["URI"] = filename;
//  tgrInfo["DRIVER"] = "ESRI Shapefile";
//  
//  std::auto_ptr<te::da::DataSource> trgDs = te::da::DataSourceFactory::make("OGR");
//  trgDs->setConnectionInfo(tgrInfo);
//  trgDs->open();
//  
//  std::string outDset = "intersectionResult";
//  if (trgDs->dataSetExists(outDset))
//  {
//    std::cout << "A dataset with the same requested output dataset name already exists: " << outDset << std::endl;
//    return false;
//  }
//  
//  bool res = te::vp::Intersection(inDset1, srcDs.get(), inDset2, srcDs.get(), false, outDset,trgDs.get());
//  
//  return res;
//}
//
//bool IntersectionOGRToPGIS()
//{
//  std::string filename1(""TE_DATA_EXAMPLE_DIR"/shp/PLANO_plano1_pol.shp");
//  std::string filename2(""TE_DATA_EXAMPLE_DIR"/shp/PLANO_plano2_pol.shp");
//  
//  std::map<std::string, std::string> srcInfo;
//  srcInfo["URI"] = filename1;
//  srcInfo["DRIVER"] = "ESRI Shapefile";
//  
//  std::auto_ptr<te::da::DataSource> srcDs1 = te::da::DataSourceFactory::make("OGR");
//  srcDs1->setConnectionInfo(srcInfo);
//  srcDs1->open();
//  
//  std::string inDset1 = "PLANO_plano1_pol";
//  if (!srcDs1->dataSetExists(inDset1))
//  {
//    std::cout << "Input dataset not found: " << inDset1 << std::endl;
//    return false;
//  }
//
//  srcInfo["URI"] = filename2;
//  srcInfo["DRIVER"] = "ESRI Shapefile";
//
//  std::auto_ptr<te::da::DataSource> srcDs2 = te::da::DataSourceFactory::make("OGR");
//  srcDs2->setConnectionInfo(srcInfo);
//  srcDs2->open();
//
//  std::string inDset2 = "PLANO_plano2_pol";
//  if (!srcDs2->dataSetExists(inDset2))
//  {
//    std::cout << "Input dataset not found: " << inDset2 << std::endl;
//    return false;
//  }
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
//  std::string outDS = "intersectionOGRtoPGIS";
//  
//  if (trgDs->dataSetExists(outDS))
//  {
//    std::cout << "A dataset with the same requested output dataset name already exists: " << outDS << std::endl;
//    return false;
//  }
//  
//  bool res = te::vp::Intersection(inDset1, srcDs1.get(), inDset2, srcDs2.get(), false, outDS,trgDs.get());
//  
//  return res;
//}
//
//bool IntersectionPGISToPGIS()
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
//
//  std::auto_ptr<te::da::DataSource> srcDs = te::da::DataSourceFactory::make("POSTGIS");
//  srcDs->setConnectionInfo(connInfo);
//  srcDs->open();
//  
//  std::string inDset1 = "plano1"; //dataset name of the first layer.
//  std::string inDset2 = "plano2"; //dataset name of the second layer.
//
//  if (!srcDs->dataSetExists(inDset1))
//  {
//    std::cout << "Input dataset not found: " << inDset1 << std::endl;
//    return false;
//  }
//  if (!srcDs->dataSetExists(inDset2))
//  {
//    std::cout << "Input dataset not found: " << inDset2 << std::endl;
//    return false;
//  }
//  
//  std::string outDS = "intersectionResult";
//  if (srcDs->dataSetExists(outDS))
//  {
//    std::cout << "A dataset with the same requested output dataset name already exists: " << outDS << std::endl;
//    return false;
//  }
//  
//  bool res = te::vp::Intersection(inDset1, srcDs.get(), inDset2, srcDs.get(), false, outDS,srcDs.get());
//  
//  return res;
//}
#pragma endregion Old examples

//New examples

//ORG to OGR
bool IntersectionOGRToOGR()
{
  std::string data_dir = TERRALIB_DATA_DIR;
  
  std::string filename1(data_dir + "shp/Intersection/rodovias.shp");
  
  std::map<std::string, std::string> srcInfo1;
  srcInfo1["URI"] = filename1;
  srcInfo1["DRIVER"] = "ESRI Shapefile";

  //std::auto_ptr<te::da::DataSource> srcDs1 = te::da::DataSourceFactory::make("OGR");
  te::da::DataSourcePtr srcDs1(te::da::DataSourceFactory::make("OGR"));
  srcDs1->setConnectionInfo(srcInfo1);
  srcDs1->open();

  std::string inDsetName1 = "rodovias";
  if (!srcDs1->dataSetExists(inDsetName1))
  {
    std::cout << "Input dataset not found: " << inDsetName1 << std::endl;
    return false;
  }

  std::auto_ptr<te::da::DataSet> inDset1 = srcDs1->getDataSet(inDsetName1);
  std::auto_ptr<te::da::DataSetType> inDsetType1 = srcDs1->getDataSetType(inDsetName1);


  std::string filename2(data_dir + "/shp/Intersection/SP_meso.shp");
  std::map<std::string, std::string> srcInfo2;
  srcInfo2["URI"] = filename2;
  srcInfo2["DRIVER"] = "ESRI Shapefile";
  
  //std::auto_ptr<te::da::DataSource> srcDs2 = te::da::DataSourceFactory::make("OGR");
  te::da::DataSourcePtr srcDs2(te::da::DataSourceFactory::make("OGR"));
  srcDs2->setConnectionInfo(srcInfo2);
  srcDs2->open();
  
  std::string inDsetName2 = "SP_meso";
  if (!srcDs2->dataSetExists(inDsetName2))
  {
    std::cout << "Input dataset not found: " << inDsetName2 << std::endl;
    return false;
  }

  std::auto_ptr<te::da::DataSet> inDset2 = srcDs2->getDataSet(inDsetName2);
  std::auto_ptr<te::da::DataSetType> inDsetType2 = srcDs2->getDataSetType(inDsetName2);

  bool copyInputColumns = true;
  std::size_t inSRID = 0;

  std::string filenameResult(data_dir + "/shp/Intersection/intersectionOGR2OGR.shp");

  std::map<std::string, std::string> tgrInfo;
  tgrInfo["URI"] = filenameResult;
  tgrInfo["DRIVER"] = "ESRI Shapefile";

  //std::auto_ptr<te::da::DataSource> trgDs = te::da::DataSourceFactory::make("OGR");
  te::da::DataSourcePtr trgDs(te::da::DataSourceFactory::make("OGR"));
  trgDs->setConnectionInfo(tgrInfo);
  trgDs->open();
  
  std::string outDS = "intersectionOGR2OGR";
  
  if (trgDs->dataSetExists(outDS))
  {
    std::cout << "A dataset with the same requested output dataset name already exists: " << outDS << std::endl;
    return false;
  }

  // sera feito por algum tipo de factory
  te::vp::IntersectionOp* intersectionOp = new te::vp::IntersectionMemory();

  intersectionOp->setInput(srcDs1, inDsetName1, inDsetType1, srcDs2, inDsetName2, inDsetType2);
  intersectionOp->setOutput(trgDs, outDS);
  intersectionOp->setParams(copyInputColumns, inSRID);
  
  bool result;

  if (!intersectionOp->paramsAreValid())
    result = false;
  else
    result = intersectionOp->run();

  delete intersectionOp;
  
  return result;
}

//OGR to Postgis
bool IntersectionOGRToPGIS()
{
  std::string data_dir = TERRALIB_DATA_DIR;
  
  std::string filename1(data_dir + "/shp/Intersection/rodovias.shp");
  
  std::map<std::string, std::string> srcInfo1;
  srcInfo1["URI"] = filename1;
  srcInfo1["DRIVER"] = "ESRI Shapefile";

  //std::auto_ptr<te::da::DataSource> srcDs1 = te::da::DataSourceFactory::make("OGR");
  te::da::DataSourcePtr srcDs1(te::da::DataSourceFactory::make("OGR"));
  srcDs1->setConnectionInfo(srcInfo1);
  srcDs1->open();

  std::string inDsetName1 = "rodovias";
  if (!srcDs1->dataSetExists(inDsetName1))
  {
    std::cout << "Input dataset not found: " << inDsetName1 << std::endl;
    return false;
  }

  std::auto_ptr<te::da::DataSet> inDset1 = srcDs1->getDataSet(inDsetName1);
  std::auto_ptr<te::da::DataSetType> inDsetType1 = srcDs1->getDataSetType(inDsetName1);


  std::string filename2(data_dir + "/shp/Intersection/SP_meso.shp");
  std::map<std::string, std::string> srcInfo2;
  srcInfo2["URI"] = filename2;
  srcInfo2["DRIVER"] = "ESRI Shapefile";
  
  //std::auto_ptr<te::da::DataSource> srcDs2 = te::da::DataSourceFactory::make("OGR");
  te::da::DataSourcePtr srcDs2(te::da::DataSourceFactory::make("OGR"));
  srcDs2->setConnectionInfo(srcInfo2);
  srcDs2->open();
  
  std::string inDsetName2 = "SP_meso";
  if (!srcDs2->dataSetExists(inDsetName2))
  {
    std::cout << "Input dataset not found: " << inDsetName2 << std::endl;
    return false;
  }

  std::auto_ptr<te::da::DataSet> inDset2 = srcDs2->getDataSet(inDsetName2);
  std::auto_ptr<te::da::DataSetType> inDsetType2 = srcDs2->getDataSetType(inDsetName2);

  bool copyInputColumns = true;
  std::size_t inSRID = 0;

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
  
  std::string outDS = "intersectionOGR2PGIS";
  
  if (trgDs->dataSetExists(outDS))
  {
    std::cout << "A dataset with the same requested output dataset name already exists: " << outDS << std::endl;
    return false;
  }

  // sera feito por algum tipo de factory
  te::vp::IntersectionOp* intersectionOp = new te::vp::IntersectionMemory();

  intersectionOp->setInput(srcDs1, inDsetName1, inDsetType1, srcDs2, inDsetName2, inDsetType2);
  intersectionOp->setOutput(trgDs, outDS);
  intersectionOp->setParams(copyInputColumns, inSRID);
  
  bool result;

  if (!intersectionOp->paramsAreValid())
    result = false;
  else
    result = intersectionOp->run();

  delete intersectionOp;
  
  return result;
}

//Postgis to Postgis
bool IntersectionPGISToPGIS()
{
  std::map<std::string, std::string> connInfo;
  connInfo["PG_HOST"] = "atlas.dpi.inpe.br" ; 
  connInfo["PG_PORT"] = "5433" ;
  connInfo["PG_USER"] = "postgres";
  connInfo["PG_PASSWORD"] = "postgres";
  connInfo["PG_DB_NAME"] = "testPostGIS";
  connInfo["PG_CONNECT_TIMEOUT"] = "4"; 
  connInfo["PG_CLIENT_ENCODING"] = "CP1252"; 

  //std::auto_ptr<te::da::DataSource> srcDs1 = te::da::DataSourceFactory::make("POSTGIS");
  te::da::DataSourcePtr srcDs1(te::da::DataSourceFactory::make("POSTGIS"));
  srcDs1->setConnectionInfo(connInfo);
  srcDs1->open();
  
  std::string inDsetName1 = "rodovias";
  if (!srcDs1->dataSetExists(inDsetName1))
  {
    std::cout << "Input dataset not found: " << inDsetName1 << std::endl;
    return false;
  }

  std::auto_ptr<te::da::DataSet> inDset1 = srcDs1->getDataSet(inDsetName1);
  std::auto_ptr<te::da::DataSetType> inDsetType1 = srcDs1->getDataSetType(inDsetName1);

  //std::auto_ptr<te::da::DataSource> srcDs2 = te::da::DataSourceFactory::make("POSTGIS");
  te::da::DataSourcePtr srcDs2(te::da::DataSourceFactory::make("POSTGIS"));
  srcDs2->setConnectionInfo(connInfo);
  srcDs2->open();
  
  std::string inDsetName2 = "sp_nomemeso";
  if (!srcDs2->dataSetExists(inDsetName2))
  {
    std::cout << "Input dataset not found: " << inDsetName2 << std::endl;
    return false;
  }

  std::auto_ptr<te::da::DataSet> inDset2 = srcDs2->getDataSet(inDsetName2);
  std::auto_ptr<te::da::DataSetType> inDsetType2 = srcDs2->getDataSetType(inDsetName2);

  bool copyInputColumns = true;
  std::size_t inSRID = 0;

  //std::auto_ptr<te::da::DataSource> trgDs = te::da::DataSourceFactory::make("POSTGIS");
  te::da::DataSourcePtr trgDs(te::da::DataSourceFactory::make("POSTGIS"));
  trgDs->setConnectionInfo(connInfo);
  trgDs->open();
  
  std::string outDS = "intersectionPGIS2PGIS";
  
  if (trgDs->dataSetExists(outDS))
  {
    std::cout << "A dataset with the same requested output dataset name already exists: " << outDS << std::endl;
    return false;
  }

  // sera feito por algum tipo de factory
  te::vp::IntersectionOp* intersectionOp = new te::vp::IntersectionMemory();

  intersectionOp->setInput(srcDs1, inDsetName1, inDsetType1, srcDs2, inDsetName2, inDsetType2);
  intersectionOp->setOutput(trgDs, outDS);
  intersectionOp->setParams(copyInputColumns, inSRID);
  
  bool result;

  if (!intersectionOp->paramsAreValid())
    result = false;
  else
    result = intersectionOp->run();

  delete intersectionOp;
  
  return result;
}

//Postgis to OGR
bool IntersectionPGISToOGR()
{
  std::map<std::string, std::string> connInfo;
  connInfo["PG_HOST"] = "localhost" ; 
  connInfo["PG_PORT"] = "5432" ;
  connInfo["PG_USER"] = "postgres";
  connInfo["PG_PASSWORD"] = "root";
  connInfo["PG_DB_NAME"] = "testPostGIS";
  connInfo["PG_CONNECT_TIMEOUT"] = "4"; 
  connInfo["PG_CLIENT_ENCODING"] = "CP1252"; 

  //std::auto_ptr<te::da::DataSource> srcDs1 = te::da::DataSourceFactory::make("POSTGIS");
  te::da::DataSourcePtr srcDs1(te::da::DataSourceFactory::make("POSTGIS"));
  srcDs1->setConnectionInfo(connInfo);
  srcDs1->open();
  
  std::string inDsetName1 = "rodovias";
  if (!srcDs1->dataSetExists(inDsetName1))
  {
    std::cout << "Input dataset not found: " << inDsetName1 << std::endl;
    return false;
  }

  std::auto_ptr<te::da::DataSet> inDset1 = srcDs1->getDataSet(inDsetName1);
  std::auto_ptr<te::da::DataSetType> inDsetType1 = srcDs1->getDataSetType(inDsetName1);

  //std::auto_ptr<te::da::DataSource> srcDs2 = te::da::DataSourceFactory::make("POSTGIS");
  te::da::DataSourcePtr srcDs2(te::da::DataSourceFactory::make("POSTGIS"));
  srcDs2->setConnectionInfo(connInfo);
  srcDs2->open();
  
  std::string inDsetName2 = "sp_meso";
  if (!srcDs2->dataSetExists(inDsetName2))
  {
    std::cout << "Input dataset not found: " << inDsetName2 << std::endl;
    return false;
  }

  std::auto_ptr<te::da::DataSet> inDset2 = srcDs2->getDataSet(inDsetName2);
  std::auto_ptr<te::da::DataSetType> inDsetType2 = srcDs2->getDataSetType(inDsetName2);


  bool copyInputColumns = true;
  std::size_t inSRID = 0;


  std::string outDSet = "intersectionPGIS2OGR";
  
  std::string data_dir = TERRALIB_DATA_DIR;

  std::string uriResult(data_dir + "/shp/Intersection/intersectionPGIS2OGR.shp");
  
  std::map<std::string, std::string> tgrInfo;
  tgrInfo["URI"] = uriResult;
  tgrInfo["DRIVER"] = "ESRI Shapefile";

  //std::auto_ptr<te::da::DataSource> trgDs = te::da::DataSourceFactory::make("OGR");
  te::da::DataSourcePtr trgDs(te::da::DataSourceFactory::make("OGR"));
  trgDs->setConnectionInfo(tgrInfo);
  trgDs->open();

  if (trgDs->dataSetExists(outDSet))
  {
    std::cout << "A dataset with the same requested output dataset name already exists: " << outDSet << std::endl;
    return false;
  }
  
  // sera feito por algum tipo de factory
  te::vp::IntersectionOp* intersectionOp = new te::vp::IntersectionQuery();

  intersectionOp->setInput(srcDs1, inDsetName1, inDsetType1, srcDs2, inDsetName2, inDsetType2);
  intersectionOp->setOutput(trgDs, outDSet);
  intersectionOp->setParams(copyInputColumns, inSRID);
  
  bool result;

  if (!intersectionOp->paramsAreValid())
    result = false;
  else
    result = intersectionOp->run();

  delete intersectionOp;
  
  return result;
}