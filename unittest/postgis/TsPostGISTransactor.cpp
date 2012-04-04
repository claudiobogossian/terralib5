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
  \file TsPostGISTransactor.cpp
 
  \brief A test suit for the PostGIS Transactor class.
 */

// TerraLib
#include <terralib/common.h>
//#include <terralib/dataaccess.h>
#include <terralib/geometry.h>
#include <terralib/postgis.h>

// Unit-Test TerraLib
#include "TsManagerPostGIS.h"
#include "TsPostGISTransactor.h"

#include "../dataaccess/Config.h"

#ifdef TE_TEST_PGIS
CPPUNIT_TEST_SUITE_REGISTRATION( TsPostGISTransactor );
#endif

void TsPostGISTransactor::setUp()
{ 
  //get values from TsManagerPostGIS (statics)
  m_ds = TsManagerPostGIS::sm_datasource;
  m_capabilit = TsManagerPostGIS::sm_capabilit;
  m_connInfo = TsManagerPostGIS::sm_connInfo;
  m_dsType = TsManagerPostGIS::sm_dsType;
  m_connInfoNewDs = TsManagerPostGIS::sm_connInfoNewDs;
  m_nroDataSets = TsManagerPostGIS::sm_nroDataSets;

  m_vecDtNames = TsManagerPostGIS::sm_vecDtNames;
  m_vecEnvelops = TsManagerPostGIS::sm_vecEnvelops;
  m_vecDtNamesAndEnvelops = TsManagerPostGIS::sm_vecDtNamesAndEnvelops;
  m_vecNamesSizes = TsManagerPostGIS::sm_vecNamesSizes;
  m_vecNamesAndRecs  = TsManagerPostGIS::sm_vecNamesAndRecs;
  m_vecNamesSizesRec = TsManagerPostGIS::sm_vecNamesSizesRec;

  m_box = TsManagerPostGIS::sm_box;

  m_pt.setSRID(4291);
  m_pt.setX(TsManagerPostGIS::sm_pt.getX());
  m_pt.setY(TsManagerPostGIS::sm_pt.getY());

  m_linearRing = TsManagerPostGIS::sm_linearRing;
  m_pol = TsManagerPostGIS::sm_pol;

  m_geom = TsManagerPostGIS::sm_geom;
}

void TsPostGISTransactor::tearDown() 
{
  m_ds = 0;
  m_dsType = "";

}

void TsPostGISTransactor::tcGetDataBaseInfo()
{
//#ifdef TE_COMPILE_ALL
  CPPUNIT_ASSERT_NO_THROW(m_ds->open(m_connInfo));
  std::auto_ptr<te::da::DataSourceTransactor> t(0);
  CPPUNIT_ASSERT_NO_THROW(t.reset(m_ds->getTransactor()));
  CPPUNIT_ASSERT(t.get());

  std::auto_ptr<te::da::DataSourceCatalogLoader> cl(0);
  CPPUNIT_ASSERT_NO_THROW(cl.reset(t->getCatalogLoader()));
  CPPUNIT_ASSERT(cl.get());

  std::string currSch;
 
  static_cast<te::pgis::Transactor*>(t.get())->getDatabaseInfo(currSch);

  CPPUNIT_ASSERT_MESSAGE ("POSTGRES SCHEMA not PUBLIC", currSch == TsManagerPostGIS::sm_currentSchema);

//#endif
}

void TsPostGISTransactor::tcGetPGDataSource()
{
//#ifdef TE_COMPILE_ALL

  CPPUNIT_ASSERT(m_ds);
  CPPUNIT_ASSERT(m_ds->isOpened() == true);

  te::pgis::Transactor* transactor;
  CPPUNIT_ASSERT_NO_THROW(transactor = static_cast<te::pgis::Transactor*>(m_ds->getTransactor()));
  te::pgis::DataSource* dspgis;

  CPPUNIT_ASSERT(dspgis = transactor->getPGDataSource());
  CPPUNIT_ASSERT_NO_THROW(delete transactor);

//#endif
}

void TsPostGISTransactor::tcGetTableId()
{
//#ifdef TE_COMPILE_ALL
  CPPUNIT_ASSERT(m_ds->isOpened()== true);

// get a transactor to retrieve information about the data source 
  std::auto_ptr<te::da::DataSourceTransactor> t(0);
  CPPUNIT_ASSERT_NO_THROW(t.reset(m_ds->getTransactor()));
  CPPUNIT_ASSERT(t.get());

// get a catalogloader  
  std::auto_ptr<te::da::DataSourceCatalogLoader> cl(0);
  CPPUNIT_ASSERT_NO_THROW(cl.reset(t->getCatalogLoader()));
  CPPUNIT_ASSERT(cl.get());
  std::vector<std::string>::iterator it = m_vecDtNames.begin();

  unsigned int dtid;
  //dtid = static_cast<te::pgis::CatalogLoader*>(cl.get())->getTableId(*it);
  CPPUNIT_ASSERT_NO_THROW(dtid = static_cast<te::pgis::CatalogLoader*>(cl.get())->getTableId(*it));
//#endif
}



