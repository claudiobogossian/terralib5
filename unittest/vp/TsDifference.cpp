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
#include "TsDifference.h"

// TerraLib
#include <terralib/dataaccess/datasource/DataSourceFactory.h>
#include <terralib/dataaccess/datasource/DataSourceInfo.h>
#include <terralib/dataaccess/datasource/DataSourceManager.h>

#include <terralib/vp/Utils.h>

// BOOST
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

CPPUNIT_TEST_SUITE_REGISTRATION(TsDifference);

void TsDifference::setUp()
{
  m_params = new te::vp::AlgorithmParams();
}

void TsDifference::tearDown()
{
  delete m_params;
}

void TsDifference::tcDifferenceQuery1()
{
  boost::uuids::basic_random_generator<boost::mt19937> gen;
  boost::uuids::uuid u = gen();
  std::string id = boost::uuids::to_string(u);

  std::map<std::string, std::string> connInfo;
  connInfo["PG_HOST"] = "localhost";
  connInfo["PG_PORT"] = "5432";
  connInfo["PG_USER"] = "postgres";
  connInfo["PG_DB_NAME"] = "TerraLib5";
  connInfo["PG_CONNECT_TIMEOUT"] = "4";
  connInfo["PG_CLIENT_ENCODING"] = "UTF-8";

  te::da::DataSourceInfoPtr dsInfoPtr(new te::da::DataSourceInfo);
  dsInfoPtr->setConnInfo(connInfo);
  dsInfoPtr->setTitle("TsDifference");
  dsInfoPtr->setAccessDriver("POSTGIS");
  dsInfoPtr->setType("POSTGIS");
  dsInfoPtr->setDescription("Teste Difference");

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

  std::string dataSetName1 = "floresta2013_22668_pol";
  CPPUNIT_ASSERT_MESSAGE("Input dataset not found: " + dataSetName1, m_inputDataSource->dataSetExists(dataSetName1));

  std::string dataSetName2 = "detex_2013_pol";
  CPPUNIT_ASSERT_MESSAGE("Input dataset not found: " + dataSetName2, m_inputDataSource->dataSetExists(dataSetName2));

  te::vp::InputParams structInputParams1;
  structInputParams1.m_inputDataSource = m_inputDataSource;
  structInputParams1.m_inputDataSetName = dataSetName1;
  m_inputParams.push_back(structInputParams1);

  te::vp::InputParams structInputParams2;
  structInputParams2.m_inputDataSource = m_inputDataSource;
  structInputParams2.m_inputDataSetName = dataSetName2;
  m_inputParams.push_back(structInputParams2);

  // Output DataSource
  boost::uuids::basic_random_generator<boost::mt19937> out_gen;
  boost::uuids::uuid out_u = out_gen();
  std::string out_id = boost::uuids::to_string(out_u);

  std::map<std::string, std::string> out_connInfo;
  out_connInfo["PG_HOST"] = "localhost";
  out_connInfo["PG_PORT"] = "5432";
  out_connInfo["PG_USER"] = "postgres";
  out_connInfo["PG_DB_NAME"] = "Other";
  out_connInfo["PG_CONNECT_TIMEOUT"] = "4";
  out_connInfo["PG_CLIENT_ENCODING"] = "UTF-8";

  te::da::DataSourceInfoPtr out_dsInfoPtr(new te::da::DataSourceInfo);
  out_dsInfoPtr->setConnInfo(out_connInfo);
  out_dsInfoPtr->setTitle("TsDiffOutput");
  out_dsInfoPtr->setAccessDriver("POSTGIS");
  out_dsInfoPtr->setType("POSTGIS");
  out_dsInfoPtr->setDescription("Teste Difference Output");
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
  std::string outputDataSetName = "diffresult1";
  CPPUNIT_ASSERT_MESSAGE("A dataset with the same requested output dataset name already exists: " + outputDataSetName, !m_outputDataSource->dataSetExists(outputDataSetName));

  m_params->setInputParams(m_inputParams);
  m_params->setOutputDataSource(m_outputDataSource.get());
  m_params->setOutputDataSetName(outputDataSetName);

  te::vp::Difference difference;
  bool success = difference.executeQuery(m_params);
}