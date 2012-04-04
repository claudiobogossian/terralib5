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
  \file TsPostGIS.cpp
 
  \brief A test suit for the PostGIS class.
 */

// TerraLib
#include <terralib/common.h>
#include <terralib/geometry.h>
#include <terralib/postgis.h>

// Unit-Test TerraLib
#include "TsManagerPostGIS.h"
#include "TsPostGIS.h"

#include "../dataaccess/Config.h"

#ifdef TE_TEST_PGIS
CPPUNIT_TEST_SUITE_REGISTRATION( TsPostGIS );
#endif

void TsPostGIS::setUp()
{ 
  //get values from TsManagerPostGIS (statics)
  m_ds = TsManagerPostGIS::sm_datasource;
  m_capabilit =  TsManagerPostGIS::sm_capabilit;
  m_connInfo = TsManagerPostGIS::sm_connInfo;
  m_dsType = TsManagerPostGIS::sm_dsType;
  m_connInfoNewDs = TsManagerPostGIS::sm_connInfoNewDs;
  m_connInfoDropDs = TsManagerPostGIS::sm_connInfoDropDs;
  m_connInfoNewPostgresDb = TsManagerPostGIS::sm_connInfoNewPostgresDb;
  m_nroDataSets = TsManagerPostGIS::sm_nroDataSets;
}

void TsPostGIS::tearDown() 
{
  m_ds = 0;
  m_dsType = "";
}

void TsPostGIS::tcGetConnPool()
{
//#ifdef TE_COMPILE_ALL
  te::pgis::ConnectionPool* getcpool = static_cast<te::pgis::DataSource*>(m_ds)->getConnPool();
  CPPUNIT_ASSERT (getcpool);
//#endif
}

void TsPostGIS::tcGetCurrentSchema()
{
//#ifdef TE_COMPILE_ALL
  std::string curSch = TsManagerPostGIS::sm_currentSchema;
  std::string getCurSch = (static_cast<te::pgis::DataSource*>(m_ds)->getCurrentSchema())->c_str();
  CPPUNIT_ASSERT_MESSAGE ("Current Schema not public", curSch == getCurSch);
//#endif
}

void TsPostGIS::tcGetDialect()
{
//#ifdef TE_COMPILE_ALL
  const te::da::SQLDialect* dialect = static_cast<te::pgis::DataSource*>(m_ds)->getDialect();
  CPPUNIT_ASSERT (dialect);
//#endif
}

void TsPostGIS::tcGetGeomTypeId()
{
//#ifdef TE_COMPILE_ALL
  unsigned int gTypeId = static_cast<te::pgis::DataSource*>(m_ds)->getGeomTypeId(); //16414;
  CPPUNIT_ASSERT_MESSAGE ("geomTypeID not equal 16414 ???", static_cast<te::pgis::DataSource*>(m_ds)->getGeomTypeId() == gTypeId);
//#endif
}

void TsPostGIS::tcPGTransactor()
{
//#ifdef TE_COMPILE_ALL
  //std::auto_ptr<te::pgis::Transactor> pgtrans(static_cast<te::pgis::DataSource*>(m_ds)->getPGTransactor());
  //CPPUNIT_ASSERT(pgtrans.get());
  te::pgis::Transactor* pgtrans;
  CPPUNIT_ASSERT_NO_THROW(pgtrans = static_cast<te::pgis::DataSource*>(m_ds)->getPGTransactor());
  CPPUNIT_ASSERT_NO_THROW(delete pgtrans);
//#endif
}

void TsPostGIS::tcSetDialect()
{
//#ifdef TE_COMPILE_ALL
//#endif
}

void TsPostGIS::tcSetCapabilities()
{
//#ifdef TE_COMPILE_ALL
//#endif
}

void TsPostGIS::tcCreatePostgres()
{
//#ifdef TE_COMPILE_ALL
// creating a postgres database without postgis extension
  try 
  {
    te::da::DataSource * newDb1 = te::da::DataSource::create(m_dsType, m_connInfoNewPostgresDb);
    CPPUNIT_ASSERT(newDb1);
  }
  catch (te::common::Exception  e)
  {
    throw e;
  }
//#endif
}

void TsPostGIS::tcDropPostgresDb()
{
// getting the right dbname to be dropped from m_connInfoNewPostgresDb["newdb_name"]
  std::map<std::string, std::string> connInfo_drop = m_connInfo;
  connInfo_drop["db_todrop"] = m_connInfoNewPostgresDb["newdb_name"];
  
  te::da::DataSource::drop(m_dsType, connInfo_drop);
}

