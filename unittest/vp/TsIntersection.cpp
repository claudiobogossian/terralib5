/*  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

    This file is part of the TerraLib - a Framework for building GIS enabled applications.

    TerraLib is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License,
    or (at your option) any later version.

    TerraLib is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with TerraLib. See COPYING. If not, write to
    TerraLib Team at <terralib-team@terralib.org>.
 */

// Unit-Test TerraLib
#include "TsIntersection.h"

// TerraLib
#include <terralib/dataaccess/datasource/DataSourceFactory.h>
#include <terralib/dataaccess/datasource/DataSourceInfo.h>
#include <terralib/dataaccess/datasource/DataSourceManager.h>

#include <terralib/vp/Utils.h>

// BOOST
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

CPPUNIT_TEST_SUITE_REGISTRATION(TsIntersection);

void TsIntersection::setUp()
{
  m_params = new te::vp::AlgorithmParams();
}

void TsIntersection::tearDown()
{
  delete m_params;
}

void TsIntersection::tcIntersectionQuery1()
{
  boost::uuids::basic_random_generator<boost::mt19937> gen;
  boost::uuids::uuid u = gen();
  std::string id = boost::uuids::to_string(u);

  std::map<std::string, std::string> connInfo;
  connInfo["PG_HOST"] = "localhost";
  connInfo["PG_PORT"] = "5432";
  connInfo["PG_USER"] = "postgres";
  connInfo["PG_DB_NAME"] = "TerraView5";
  connInfo["PG_CONNECT_TIMEOUT"] = "4";
  connInfo["PG_CLIENT_ENCODING"] = "UTF-8";

  te::da::DataSourceInfoPtr dsInfoPtr(new te::da::DataSourceInfo);
  dsInfoPtr->setConnInfo(connInfo);
  dsInfoPtr->setTitle("TsIntersection");
  dsInfoPtr->setAccessDriver("POSTGIS");
  dsInfoPtr->setType("POSTGIS");
  dsInfoPtr->setDescription("Teste Intersection");

  std::map<std::string, te::da::DataSourceInfoPtr>::const_iterator dsInfoIt = te::da::DataSourceInfoManager::getInstance().begin();
  bool dataSourceFound = false;

  while (dsInfoIt != te::da::DataSourceInfoManager::getInstance().end())
  {
    if (dsInfoPtr->getConnInfoAsString() == dsInfoIt->second->getConnInfoAsString())
    {
      id = dsInfoIt->second->getId();
      dataSourceFound = true;
    }
    ++dsInfoIt;
  }

  if (!dataSourceFound)
  {
    dsInfoPtr->setId(id);
    te::da::DataSourceInfoManager::getInstance().add(dsInfoPtr);
  }

  te::da::DataSourcePtr m_inputDataSource = te::da::DataSourceManager::getInstance().get(id, "POSTGIS", dsInfoPtr->getConnInfo());

  std::vector<std::string> vecDataSetName;

  std::string dataSetName1 = "geominter4";
  CPPUNIT_ASSERT_MESSAGE("Input dataset not found: " + dataSetName1, m_inputDataSource->dataSetExists(dataSetName1));

  std::string dataSetName2 = "geominter5";
  CPPUNIT_ASSERT_MESSAGE("Input dataset not found: " + dataSetName2, m_inputDataSource->dataSetExists(dataSetName2));

  std::string dataSetName3 = "geominter6";
  CPPUNIT_ASSERT_MESSAGE("Input dataset not found: " + dataSetName3, m_inputDataSource->dataSetExists(dataSetName3));

  te::vp::InputParams structInputParams1;
  structInputParams1.m_inputDataSource = m_inputDataSource;
  structInputParams1.m_inputDataSetName = dataSetName1;
  m_inputParams.push_back(structInputParams1);

  te::vp::InputParams structInputParams2;
  structInputParams2.m_inputDataSource = m_inputDataSource;
  structInputParams2.m_inputDataSetName = dataSetName2;
  m_inputParams.push_back(structInputParams2);

  te::vp::InputParams structInputParams3;
  structInputParams3.m_inputDataSource = m_inputDataSource;
  structInputParams3.m_inputDataSetName = dataSetName3;
  m_inputParams.push_back(structInputParams3);


  // Output DataSource
  boost::uuids::basic_random_generator<boost::mt19937> out_gen;
  boost::uuids::uuid out_u = out_gen();
  std::string out_id = boost::uuids::to_string(out_u);

  std::map<std::string, std::string> out_connInfo;
  out_connInfo["PG_HOST"] = "localhost";
  out_connInfo["PG_PORT"] = "5432";
  out_connInfo["PG_USER"] = "postgres";
  out_connInfo["PG_DB_NAME"] = "TerraView";
  out_connInfo["PG_CONNECT_TIMEOUT"] = "4";
  out_connInfo["PG_CLIENT_ENCODING"] = "UTF-8";

  te::da::DataSourceInfoPtr out_dsInfoPtr(new te::da::DataSourceInfo);
  out_dsInfoPtr->setConnInfo(out_connInfo);
  out_dsInfoPtr->setTitle("TsIntersectionOutput");
  out_dsInfoPtr->setAccessDriver("POSTGIS");
  out_dsInfoPtr->setType("POSTGIS");
  out_dsInfoPtr->setDescription("Teste Intersection Output");
  out_dsInfoPtr->setId(out_id);

  dsInfoIt = te::da::DataSourceInfoManager::getInstance().begin();
  dataSourceFound = false;

  while (dsInfoIt != te::da::DataSourceInfoManager::getInstance().end())
  {
    if (out_dsInfoPtr->getConnInfoAsString() == dsInfoIt->second->getConnInfoAsString())
    {
      out_id = dsInfoIt->second->getId();
      dataSourceFound = true;
    }
    ++dsInfoIt;
  }
  
  if (!dataSourceFound)
  {
    out_dsInfoPtr->setId(out_id);
    te::da::DataSourceInfoManager::getInstance().add(out_dsInfoPtr);
  }

  m_outputDataSource = te::da::DataSourceManager::getInstance().get(out_id, "POSTGIS", out_dsInfoPtr->getConnInfo());

  // Output DataSetName
  std::string outputDataSetName = "intersectionresult1";
  CPPUNIT_ASSERT_MESSAGE("A dataset with the same requested output dataset name already exists: " + outputDataSetName, !m_outputDataSource->dataSetExists(outputDataSetName));

  double copy = 105060;
  std::map<std::string, te::dt::AbstractData*> specificParams;
  specificParams.insert(std::pair<std::string, te::dt::AbstractData*>("copyInputAttributes", new te::dt::SimpleData<double, te::dt::DOUBLE_TYPE>(copy)));

  m_params->setInputParams(m_inputParams);
  m_params->setOutputDataSource(m_inputDataSource.get());
  m_params->setOutputDataSetName(outputDataSetName);

  m_params->setSpecificParams(specificParams);


  te::vp::Intersection intersection;
  bool success = intersection.executeQuery(m_params);
}

void TsIntersection::tcIntersectionQuery2()
{
  boost::uuids::basic_random_generator<boost::mt19937> gen;
  boost::uuids::uuid u = gen();
  std::string id = boost::uuids::to_string(u);

  std::map<std::string, std::string> connInfo;
  connInfo["PG_HOST"] = "localhost";
  connInfo["PG_PORT"] = "5432";
  connInfo["PG_USER"] = "postgres";
  connInfo["PG_DB_NAME"] = "TerraView5";
  connInfo["PG_CONNECT_TIMEOUT"] = "4";
  connInfo["PG_CLIENT_ENCODING"] = "UTF-8";

  te::da::DataSourceInfoPtr dsInfoPtr(new te::da::DataSourceInfo);
  dsInfoPtr->setConnInfo(connInfo);
  dsInfoPtr->setTitle("TsIntersection");
  dsInfoPtr->setAccessDriver("POSTGIS");
  dsInfoPtr->setType("POSTGIS");
  dsInfoPtr->setDescription("Teste Intersection");

  std::map<std::string, te::da::DataSourceInfoPtr>::const_iterator dsInfoIt = te::da::DataSourceInfoManager::getInstance().begin();
  bool dataSourceFound = false;

  while (dsInfoIt != te::da::DataSourceInfoManager::getInstance().end())
  {
    std::string conn1 = dsInfoPtr->getConnInfoAsString();
    std::string conn2 = dsInfoIt->second->getConnInfoAsString();
    if (dsInfoPtr->getConnInfoAsString() == dsInfoIt->second->getConnInfoAsString())
    {
      id = dsInfoIt->first;
      dataSourceFound = true;
    }
    ++dsInfoIt;
  }

  if (!dataSourceFound)
  {
    dsInfoPtr->setId(id);
    te::da::DataSourceInfoManager::getInstance().add(dsInfoPtr);
  }

  te::da::DataSourcePtr m_inputDataSource = te::da::DataSourceManager::getInstance().get(id, "POSTGIS", dsInfoPtr->getConnInfo());

  std::vector<std::string> vecDataSetName;

  std::string dataSetName1 = "geominter4";
  CPPUNIT_ASSERT_MESSAGE("Input dataset not found: " + dataSetName1, m_inputDataSource->dataSetExists(dataSetName1));

  std::string dataSetName2 = "geominter5";
  CPPUNIT_ASSERT_MESSAGE("Input dataset not found: " + dataSetName2, m_inputDataSource->dataSetExists(dataSetName2));

  std::string dataSetName3 = "geominter6";
  CPPUNIT_ASSERT_MESSAGE("Input dataset not found: " + dataSetName3, m_inputDataSource->dataSetExists(dataSetName3));

  te::vp::InputParams structInputParams1;
  structInputParams1.m_inputDataSource = m_inputDataSource;
  structInputParams1.m_inputDataSetName = dataSetName1;
  m_inputParams.push_back(structInputParams1);

  te::vp::InputParams structInputParams2;
  structInputParams2.m_inputDataSource = m_inputDataSource;
  structInputParams2.m_inputDataSetName = dataSetName2;
  m_inputParams.push_back(structInputParams2);

  te::vp::InputParams structInputParams3;
  structInputParams3.m_inputDataSource = m_inputDataSource;
  structInputParams3.m_inputDataSetName = dataSetName3;
  m_inputParams.push_back(structInputParams3);


  // Output DataSource
  boost::uuids::basic_random_generator<boost::mt19937> out_gen;
  boost::uuids::uuid out_u = out_gen();
  std::string out_id = boost::uuids::to_string(out_u);

  std::map<std::string, std::string> out_connInfo;
  out_connInfo["PG_HOST"] = "localhost";
  out_connInfo["PG_PORT"] = "5432";
  out_connInfo["PG_USER"] = "postgres";
  out_connInfo["PG_DB_NAME"] = "TerraView";
  out_connInfo["PG_CONNECT_TIMEOUT"] = "4";
  out_connInfo["PG_CLIENT_ENCODING"] = "UTF-8";

  te::da::DataSourceInfoPtr out_dsInfoPtr(new te::da::DataSourceInfo);
  out_dsInfoPtr->setConnInfo(out_connInfo);
  out_dsInfoPtr->setTitle("TsIntersectionOutput");
  out_dsInfoPtr->setAccessDriver("POSTGIS");
  out_dsInfoPtr->setType("POSTGIS");
  out_dsInfoPtr->setDescription("Teste Intersection Output");

  dsInfoIt = te::da::DataSourceInfoManager::getInstance().begin();
  dataSourceFound = false;

  while (dsInfoIt != te::da::DataSourceInfoManager::getInstance().end())
  {
    if (out_dsInfoPtr->getConnInfoAsString() == dsInfoIt->second->getConnInfoAsString())
    {
      out_id = dsInfoIt->second->getId();
      dataSourceFound = true;
    }
    ++dsInfoIt;
  }

  if (!dataSourceFound)
  {
    out_dsInfoPtr->setId(out_id);
    te::da::DataSourceInfoManager::getInstance().add(out_dsInfoPtr);
  }

  m_outputDataSource = te::da::DataSourceManager::getInstance().get(out_id, "POSTGIS", out_dsInfoPtr->getConnInfo());

  // Output DataSetName
  std::string outputDataSetName = "intersectionresult2";
  CPPUNIT_ASSERT_MESSAGE("A dataset with the same requested output dataset name already exists: " + outputDataSetName, !m_outputDataSource->dataSetExists(outputDataSetName));

  double copy = 105060;
  std::map<std::string, te::dt::AbstractData*> specificParams;
  specificParams.insert(std::pair<std::string, te::dt::AbstractData*>("copyInputAttributes", new te::dt::SimpleData<double, te::dt::DOUBLE_TYPE>(copy)));

  m_params->setInputParams(m_inputParams);
  m_params->setOutputDataSource(m_outputDataSource.get());
  m_params->setOutputDataSetName(outputDataSetName);

  m_params->setSpecificParams(specificParams);


  te::vp::Intersection intersection;
  bool success = intersection.executeQuery(m_params);
}

void TsIntersection::tcIntersectionMemory1()
{
//// PARAMETER LAYER 1
//  std::string data_dir = TERRALIB_DATA_DIR;
//  //std::string filename1(data_dir + "/shp/rodovias_sp.shp");
//  std::string filename1(data_dir + "/shp/Intersection/result.shp");
//
//  std::map<std::string, std::string> srcInfo1;
//  srcInfo1["URI"] = filename1;
//  srcInfo1["DRIVER"] = "ESRI Shapefile";
//
//  te::da::DataSourcePtr dSource1(te::da::DataSourceFactory::make("OGR"));
//  dSource1->setConnectionInfo(srcInfo1);
//  dSource1->open();
//  
//  //std::string inDSetName1 = "rodovias_sp";
//  std::string inDSetName1 = "result";
//  CPPUNIT_ASSERT_MESSAGE("Input dataset not found: " + inDSetName1, dSource1->dataSetExists(inDSetName1));
//
//  std::auto_ptr<te::gm::Envelope> env = dSource1->getDataSet(inDSetName1)->getExtent(1);
//
//// PARAMETER LAYER 2
//  std::string filename2(data_dir + "/shp/pol_sp.shp");
//
//  std::map<std::string, std::string> srcInfo2;
//  srcInfo2["URI"] = filename2;
//  srcInfo2["DRIVER"] = "ESRI Shapefile";
//
//  te::da::DataSourcePtr dSource2(te::da::DataSourceFactory::make("OGR"));
//  dSource2->setConnectionInfo(srcInfo2);
//  dSource2->open();
//
//  std::string inDSetName2 = "pol_sp";
//  CPPUNIT_ASSERT_MESSAGE("Input dataset not found: " + inDSetName2, dSource2->dataSetExists(inDSetName2));
//
//// OUTPUT PARAM
//  std::string filenameResult(data_dir + "/shp/Intersection/result1.shp");
//
//  std::map<std::string, std::string> tgrInfo;
//  tgrInfo["URI"] = filenameResult;
//  tgrInfo["DRIVER"] = "ESRI Shapefile";
//
//  //std::auto_ptr<te::da::DataSource> trgDs = te::da::DataSourceFactory::make("OGR");
//  te::da::DataSourcePtr trgDs(te::da::DataSourceFactory::make("OGR"));
//  trgDs->setConnectionInfo(tgrInfo);
//  trgDs->open();
//
//  std::string outDataSet = "result1";
//  CPPUNIT_ASSERT_MESSAGE("A dataset with the same requested output dataset name already exists: " + outDataSet, !trgDs->dataSetExists(outDataSet));
//
//
////INPUT/OUTPUT PARAMS
//  te::vp::InputParams structInputParams1;
//  structInputParams1.m_inputDataSource = dSource1;
//  structInputParams1.m_inputDataSetName = inDSetName1;
//  m_inputParams.push_back(structInputParams1);
//
//  te::vp::InputParams structInputParams2;
//  structInputParams2.m_inputDataSource = dSource2;
//  structInputParams2.m_inputDataSetName = inDSetName2;
//  m_inputParams.push_back(structInputParams2);
//
//  m_params->setInputParams(m_inputParams);
//  m_params->setOutputDataSource(trgDs.get());
//  m_params->setOutputDataSetName(outDataSet);
//
//
//// TILES
//  std::vector<const te::vp::InputTileParams> inputTileParamsVec;
//
//  std::vector<std::string> attributes = { "DISTRITO" };
//
//  te::vp::InputTileParams structInputTileParams1;
//  structInputTileParams1.m_inputDataSource = dSource1;
//  structInputTileParams1.m_inputDataSetName = inDSetName1;
//  structInputTileParams1.m_attributes = attributes;
//  inputTileParamsVec.push_back(structInputTileParams1);
//
//  te::vp::InputTileParams structInputTileParams2;
//  structInputTileParams2.m_inputDataSource = dSource2;
//  structInputTileParams2.m_inputDataSetName = inDSetName2;
//  structInputTileParams2.m_attributes = attributes;
//  inputTileParamsVec.push_back(structInputTileParams2);
//
//  te::vp::TileParams tileParams = te::vp::TileParams(inputTileParamsVec);
//  te::vp::Tiles* tiles = new te::vp::TilesByAttributes();
//
//  tiles->setTileParams(tileParams);
//  std::size_t size = tiles->getSize();
//
//  for (int i = 0; i < size; ++i)
//  {
//    std::vector<te::da::DataSet*> dSetTile = tiles->getTiles(i);
//
//    te::vp::Intersection intersection;
//    bool result = intersection.executeMemory(m_params, dSetTile[0]);
//  }
}