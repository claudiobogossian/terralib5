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
  \file TsDataSource.cpp
 
  \brief A test suit for the DataSource interface.
 */

// TerraLib
#include <terralib/common.h>
#include <terralib/dataaccess.h>
#include <terralib/geometry.h>

// Unit-Test TerraLib
#include "TsDataSource.h"
#include "TsManagerDataSource.h"


CPPUNIT_TEST_SUITE_REGISTRATION( TsDataSource );

void TsDataSource::setUp()
{  
  m_ds = TsManagerDataSource::sm_datasource;
  m_connInfo = TsManagerDataSource::sm_connInfo;
  m_dsType = TsManagerDataSource::sm_dsType;
  m_connInfoNewDs  = TsManagerDataSource::sm_connInfoNewDs;
  m_connInfoDropDs = TsManagerDataSource::sm_connInfoDropDs;
  m_connStr = TsManagerDataSource::sm_connStr;
  //m_capabilit =  m_ds->getCapabilities(); //TsManagerDataSource::sm_capabilit;
  m_capabilit = TsManagerDataSource::sm_capabilit;
  int i=0;
}

void TsDataSource::tearDown()
{
  m_ds = 0;
}

void TsDataSource::tcGetType()
{
//#ifdef TE_COMPILE_ALL
  CPPUNIT_ASSERT(m_ds);
  CPPUNIT_ASSERT(m_ds->getType()== m_dsType);
//#endif
}

void TsDataSource::tcOpenWithMap()
{
//#ifdef TE_COMPILE_ALL
  CPPUNIT_ASSERT(m_ds);
  CPPUNIT_ASSERT_NO_THROW(m_ds->open(m_connInfo));
//#endif
}

void TsDataSource::tcOpenWithStr()
{
//#ifdef TE_COMPILE_ALL
  CPPUNIT_ASSERT(m_ds);
  CPPUNIT_ASSERT_NO_THROW(m_ds->open(m_connStr));
//#endif
}
void TsDataSource::tcClose()
{
//#ifdef TE_COMPILE_ALL
  CPPUNIT_ASSERT(m_ds);
  CPPUNIT_ASSERT_NO_THROW(m_ds->close());
  CPPUNIT_ASSERT(m_ds->isOpened()== false);
//#endif
}


void TsDataSource::tcIsOpened()
{
//#ifdef TE_COMPILE_ALL
  CPPUNIT_ASSERT(m_ds->isOpened()== true);
//#endif
}

void TsDataSource::tcIsValid()
{
//#ifdef TE_COMPILE_ALL
  CPPUNIT_ASSERT(m_ds->isValid()== true);
//#endif
}

void TsDataSource::tcGetCatalog()
{
//#ifdef TE_COMPILE_ALL
  CPPUNIT_ASSERT(m_ds->getCatalog() != 0);
  CPPUNIT_ASSERT(m_ds->getCatalog()->getNumberOfDataSets() == 0);

// get a transactor to retrieve the datasource catalog
  std::auto_ptr<te::da::DataSourceTransactor> t(0);
  CPPUNIT_ASSERT_NO_THROW(t.reset(m_ds->getTransactor()));
  CPPUNIT_ASSERT(t.get());

// get a catalog loader and load the datasource catalog before calling getCatalog
  std::auto_ptr<te::da::DataSourceCatalogLoader> cl(0);
  CPPUNIT_ASSERT_NO_THROW(cl.reset(t->getCatalogLoader()));
  CPPUNIT_ASSERT(cl.get());
  CPPUNIT_ASSERT_NO_THROW(cl->loadCatalog(false)); //false: minimum information (properties)

// now you can get the datasource catalog
  te::da::DataSourceCatalog* ct = m_ds->getCatalog();
  CPPUNIT_ASSERT(ct);
  CPPUNIT_ASSERT(ct->getNumberOfDataSets() != 0);

//#endif
}

void TsDataSource::tcGetCatalogFull()
{
//#ifdef TE_COMPILE_ALL
  CPPUNIT_ASSERT(m_ds->getCatalog() != 0);
// This time the catalog is not completetly empty (m_ds->close() does not clear it).

// get a transactor to retrieve the data source catalog
  std::auto_ptr<te::da::DataSourceTransactor> t(0);
  CPPUNIT_ASSERT_NO_THROW(t.reset(m_ds->getTransactor()));
  CPPUNIT_ASSERT(t.get());

// get a catalog loader and load the data source catalog before calling getCatalog
  std::auto_ptr<te::da::DataSourceCatalogLoader> cl(0);
  CPPUNIT_ASSERT_NO_THROW(cl.reset(t->getCatalogLoader()));
  CPPUNIT_ASSERT(cl.get());
  CPPUNIT_ASSERT_NO_THROW(cl->loadCatalog(true)); //true: Load maximum information(fk,pk,uk,idx,etc)

  te::da::DataSourceCatalog* ct = m_ds->getCatalog();
  CPPUNIT_ASSERT(ct);
  CPPUNIT_ASSERT(ct->getNumberOfDataSets() != 0);

//#endif
}

void TsDataSource::tcGetCapabilities()
{
//#ifdef TE_COMPILE_ALL
  const te::da::DataSourceCapabilities& capabilities = m_ds->getCapabilities();

  CPPUNIT_ASSERT_NO_THROW(m_ds->getCapabilities());
//#endif
}


void TsDataSource::tcGetTransactor()
{
//#ifdef TE_COMPILE_ALL
  // get a transactor to retrieve information about the data source 
  std::auto_ptr<te::da::DataSourceTransactor> t(m_ds->getTransactor());
  CPPUNIT_ASSERT(t.get());

//#endif
}

void TsDataSource::tcGetConnectionInfo()
{
//#ifdef TE_COMPILE_ALL
  std::map<std::string, std::string>  connInfo;
  connInfo = m_ds->getConnectionInfo();
  size_t size_connInfo = connInfo.size();

  CPPUNIT_ASSERT(size_connInfo > 0);

//#endif
}

void TsDataSource::tcExist()
{
//#ifdef TE_COMPILE_ALL
  CPPUNIT_ASSERT(m_ds->exists(m_dsType, m_connInfo)== true);
//#endif

}
void TsDataSource::tcCreate()
{
//#ifdef TE_COMPILE_ALL

// creating a postgis database 
  try 
  {
    te::da::DataSource * newDb = te::da::DataSource::create(m_dsType, m_connInfoNewDs);
    CPPUNIT_ASSERT(newDb);
    delete newDb;
  }
  catch (te::common::Exception  e)
  {
    throw e;
  }

//#endif
}

void TsDataSource::tcDrop()
{
//#ifdef TE_COMPILE_ALL

// dropping the datasource (Postgis, Gdal, Ogr) created before 
  te::da::DataSource::drop(m_dsType, m_connInfoDropDs);

//#endif
}

void TsDataSource::tcGetConnectionStr()
{
//#ifdef TE_COMPILE_ALL
  std::string conn;
  CPPUNIT_ASSERT(m_ds);
  conn = m_ds->getConnectionStr();
  CPPUNIT_ASSERT(conn.size() != 0);
//#endif
}

void TsDataSource::tcSetConnectionInfo()
{
//#ifdef TE_COMPILE_ALL
  std::map<std::string, std::string>  connInfo;

  CPPUNIT_ASSERT(m_ds);
  m_ds->setConnectionInfo(m_connInfo);
  connInfo = m_ds->getConnectionInfo();
  CPPUNIT_ASSERT(connInfo == m_connInfo);

//#endif
}

void TsDataSource::tcSetConnectionStr()
{
//#ifdef TE_COMPILE_ALL
  std::string  connStr;
  CPPUNIT_ASSERT(m_ds);
  m_ds->setConnectionStr(m_connStr);
  connStr = m_ds->getConnectionStr();
  CPPUNIT_ASSERT(connStr == m_connStr);

//#endif
}

void TsDataSource::tcSQLDialect()
{
//#ifdef TE_COMPILE_ALL
  CPPUNIT_ASSERT(m_ds);
  const te::da::SQLDialect* dialect = m_ds->getDialect();
  CPPUNIT_ASSERT(dialect != 0);
//#endif
}

void TsDataSource::tcGetDataSources()
{
//#ifdef TE_COMPILE_ALL
  CPPUNIT_ASSERT(m_ds);
  std::vector<std::string> dbs = m_ds->getDataSources(m_dsType,m_connInfo);
 
  CPPUNIT_ASSERT(dbs.size() >= 0);
//#endif
}

void TsDataSource::tcGetEncodings()
{
//#ifdef TE_COMPILE_ALL
  CPPUNIT_ASSERT(m_ds);
  std::vector<std::string> encodings = m_ds->getEncodings(m_dsType,m_connInfo);
 
  CPPUNIT_ASSERT(encodings.size() >= 0);
//#endif
}
