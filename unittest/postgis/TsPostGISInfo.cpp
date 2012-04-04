/*  Copyright (C) 2001-2009 National Institute For Space Research (INPE) - Brazil.

    This file is part of the TerraLib - a Framework for building GIS enabled applications.

    TerraLib is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License,
    or (at your option) any later version.

    TerraLib is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with TerraLib. See COPYING. If not, write to
    TerraLib Team at <terralib-team@terralib.org>.
 */

/*!
  \file TsPostGISInfo.cpp
 
  \brief A test suit for the PostGISInfo class.
 */

// TerraLib
#include <terralib/common.h>
#include <terralib/dataaccess.h>
#include <terralib/geometry.h>
#include <terralib/postgis.h>

// Unit-Test TerraLib
#include "TsManagerPostGIS.h"
#include "TsPostGISInfo.h"

#include "../dataaccess/Config.h"

#ifdef TE_TEST_PGIS
CPPUNIT_TEST_SUITE_REGISTRATION( TsPostGISInfo );
#endif

void TsPostGISInfo::setUp()
{  
  // used in TsDataSourceInfo
  m_dsInfoChk = TsManagerPostGIS::sm_dsInfoChk;
  m_dsInfo = TsManagerPostGIS::sm_dsInfo;
  m_desc = TsManagerPostGIS::sm_description;
  m_dispName = TsManagerPostGIS::sm_displayName; 

  // used in TsPostGisInfo to avoid too many static_cast 
  m_pgisInfoChk = static_cast<te::pgis::DataSourceInfo*>(m_dsInfoChk);
  m_pgisInfo = static_cast<te::pgis::DataSourceInfo*>(m_dsInfo);
}

void TsPostGISInfo::tearDown()
{
  m_dsInfoChk = 0;
  m_dsInfo = 0;

  m_pgisInfoChk = 0;
  m_pgisInfo = 0;
}

// Specific Test Cases for PostGIS
void TsPostGISInfo::tcGetDbName()
{  
//#ifdef TE_COMPILE_ALL
  CPPUNIT_ASSERT_MESSAGE("getDbName not equal ", static_cast<te::pgis::DataSourceInfo*>(m_dsInfo)->getDbName() == static_cast<te::pgis::DataSourceInfo*>(m_dsInfoChk)->getDbName());
  CPPUNIT_ASSERT_MESSAGE("getDbName not equal ", m_pgisInfo->getDbName() == static_cast<te::pgis::DataSourceInfo*>(m_dsInfoChk)->getDbName());
  CPPUNIT_ASSERT_MESSAGE("getDbName not equal ", m_pgisInfo->getDbName() == m_pgisInfoChk->getDbName());

//#endif
}

void TsPostGISInfo::tcToString()
{
//#ifdef TE_COMPILE_ALL
  const std::string strInfo = m_pgisInfo->toString().c_str();
  const std::string strInfoPGchk = m_pgisInfoChk->toString().c_str();
  CPPUNIT_ASSERT_MESSAGE ("Could not get info string 1", m_pgisInfo->toString()== m_pgisInfoChk->toString() );
  CPPUNIT_ASSERT_MESSAGE ("Could not get info string 2", strInfo == strInfoPGchk );
//#endif
}

void TsPostGISInfo::tcAllGetsRelatedWithInfo()
{
//#ifdef TE_COMPILE_ALL
  CPPUNIT_ASSERT(m_pgisInfo);
  CPPUNIT_ASSERT_MESSAGE("getClientEncoding not equal ", m_pgisInfo->getClientEncoding() == m_pgisInfoChk->getClientEncoding());
  CPPUNIT_ASSERT_MESSAGE("getConnectTimeout not equal ", m_pgisInfo->getConnectTimeout() == m_pgisInfoChk->getConnectTimeout());
  CPPUNIT_ASSERT_MESSAGE("getDbName not equal ", m_pgisInfo->getDbName() == m_pgisInfoChk->getDbName());
  CPPUNIT_ASSERT_MESSAGE("getDropDb not equal ", m_pgisInfo->getDropDb() == m_pgisInfoChk->getDropDb());
  CPPUNIT_ASSERT_MESSAGE("getGSSLib not equal ", m_pgisInfo->getGSSLib() == m_pgisInfoChk->getGSSLib()); 
  CPPUNIT_ASSERT_MESSAGE("getHost not equal ", m_pgisInfo->getHost() == m_pgisInfoChk->getHost());
  CPPUNIT_ASSERT_MESSAGE("getHostAddress not equal ", m_pgisInfo->getHostAddr() == m_pgisInfoChk->getHostAddr());
  CPPUNIT_ASSERT_MESSAGE("getInitialPoolSize not equal ", m_pgisInfo->getInitialPoolSize() == m_pgisInfoChk->getInitialPoolSize());
  CPPUNIT_ASSERT_MESSAGE("getKerberos not equal ", m_pgisInfo->getKerberos() == m_pgisInfoChk->getKerberos());
  CPPUNIT_ASSERT_MESSAGE("getMaxIdleTime not equal ", m_pgisInfo->getMaxIdleTime() == m_pgisInfoChk->getMaxIdleTime());
  CPPUNIT_ASSERT_MESSAGE("getMaxPoolSize not equal ", m_pgisInfo->getMaxPoolSize() == m_pgisInfoChk->getMaxPoolSize());
  CPPUNIT_ASSERT_MESSAGE("getMinPoolSize not equal ", m_pgisInfo->getMinPoolSize() == m_pgisInfoChk->getMinPoolSize());
  CPPUNIT_ASSERT_MESSAGE("getOptions not equal ", m_pgisInfo->getOptions() == m_pgisInfoChk->getOptions());
  CPPUNIT_ASSERT_MESSAGE("getPassWord not equal ", m_pgisInfo->getPassword() == m_pgisInfoChk->getPassword());
  CPPUNIT_ASSERT_MESSAGE("getPort not equal ", m_pgisInfo->getPort() == m_pgisInfoChk->getPort());
  CPPUNIT_ASSERT_MESSAGE("getType not equal ", m_pgisInfo->getType() == m_pgisInfoChk->getType());
  CPPUNIT_ASSERT_MESSAGE("getUser not equal ", m_pgisInfo->getUser() == m_pgisInfoChk->getUser());

  CPPUNIT_ASSERT_MESSAGE("getNewDbConnLimit equal ", m_pgisInfo->getNewDbConnLimit() == m_pgisInfoChk->getNewDbConnLimit());
  CPPUNIT_ASSERT_MESSAGE("getNewDbEncoding equal ", m_pgisInfo->getNewDbEncoding() == m_pgisInfoChk->getNewDbEncoding());
  CPPUNIT_ASSERT_MESSAGE("getNewDbName equal ", m_pgisInfo->getNewDbName() == m_pgisInfoChk->getNewDbName());
  CPPUNIT_ASSERT_MESSAGE("getNewDbOwner equal ", m_pgisInfo->getNewDbOwner() == m_pgisInfoChk->getNewDbOwner());
  CPPUNIT_ASSERT_MESSAGE("getNewDbTablespace equal ", m_pgisInfo->getNewDbTablespace() == m_pgisInfoChk->getNewDbTablespace());
  CPPUNIT_ASSERT_MESSAGE("getNewDbTemplate equal ", m_pgisInfo->getNewDbTemplate() == m_pgisInfoChk->getNewDbTemplate());
//#endif
}

void TsPostGISInfo::tcGetType()
{
//#ifdef TE_COMPILE_ALL
  CPPUNIT_ASSERT(m_dsInfo->getType().empty() == false);
  CPPUNIT_ASSERT_EQUAL_MESSAGE("DataSource Type not equal expected", m_dsInfo->getType(), TsManagerPostGIS::sm_dsType);
//#endif
}

void TsPostGISInfo::tcClone ()
{
}

void TsPostGISInfo::tcFromString ()
{
  CPPUNIT_ASSERT(m_pgisInfo);
  //CPPUNIT_ASSERT_MESSAGE("TsDataSource tested fromString method using the string <DataSource=PostGIS&host=atlas.dpi.inpe.br&port=5432&dbname=terralib4&user=postgres&password=sitim110&connect_timeout=20>", 0==1);
}

void TsPostGISInfo::tcToConnectionString ()
{
  CPPUNIT_ASSERT(m_pgisInfo);
  std::string connInfo = m_pgisInfo->toConnectionString();
  const std::string strInfoChk = " host = atlas.dpi.inpe.br port = 5432 dbname = terralib4 user = postgres password = sitim110 connect_timeout = 20";
  CPPUNIT_ASSERT_EQUAL(strInfoChk,connInfo);
}
