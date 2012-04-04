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


void TsDataSource::tcGetType()
{
//#ifdef TE_COMPILE_ALL
  CPPUNIT_ASSERT(m_ds);
  CPPUNIT_ASSERT(m_ds->getType()== m_dsType);
//#endif
}

void TsDataSource::tcOpen()
{
//#ifdef TE_COMPILE_ALL
  CPPUNIT_ASSERT(m_ds);
  CPPUNIT_ASSERT_NO_THROW(m_ds->open());



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
  std::map<std::string, std::string> capabilities;
  m_ds->getCapabilities(capabilities); //  
  size_t size_capabilities = capabilities.size();

  CPPUNIT_ASSERT(size_capabilities != 0);
  CPPUNIT_ASSERT_NO_THROW(m_ds->getCapabilities(capabilities));
//#endif
}


void TsDataSource::tcGetTransactor()
{
//#ifdef TE_COMPILE_ALL
  te::da::DataSourceTransactor* transactor = 0;
  CPPUNIT_ASSERT_NO_THROW(transactor = m_ds->getTransactor());
  CPPUNIT_ASSERT_NO_THROW(delete transactor);
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

//===========New methods

void TsDataSource::tcOpenWithMap()
{
//#ifdef TE_COMPILE_ALL
  CPPUNIT_ASSERT(m_ds);
  CPPUNIT_ASSERT_NO_THROW(m_ds->open(m_connInfo));
//#endif
}

void TsDataSource::tcOpenWithString()
{
//#ifdef TE_COMPILE_ALL
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

void TsDataSource::tcGetConnectionStr()
{
//#ifdef TE_COMPILE_ALL
  std::string conn;
  CPPUNIT_ASSERT(m_ds);
  conn = m_ds->getConnectionStr();
  CPPUNIT_ASSERT(conn.size() != 0);
//#endif
}
void TsDataSource::tcSetConnectionStr()
{
//#ifdef TE_COMPILE_ALL
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
