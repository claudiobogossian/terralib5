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
  \file TsPostGISCatalogLoader.cpp
 
  \brief A test suit for the PostGIS CatalogLoader class.
 */

// TerraLib
#include <terralib/common.h>
#include <terralib/geometry.h>
#include <terralib/postgis.h>

// Unit-Test TerraLib
#include "TsManagerPostGIS.h"
#include "TsPostGISCatalogLoader.h"

#include "../dataaccess/Config.h"

#ifdef TE_TEST_PGIS
CPPUNIT_TEST_SUITE_REGISTRATION( TsPostGISCatalogLoader );
#endif

void TsPostGISCatalogLoader::setUp()
{ 
  //get values from TsManagerPostGIS (statics)
  m_ds = TsManagerPostGIS::sm_datasource;
  m_capabilit = TsManagerPostGIS::sm_capabilit;
  m_connInfo = TsManagerPostGIS::sm_connInfo;
  m_dsType = TsManagerPostGIS::sm_dsType;

  m_vecDtNames = TsManagerPostGIS::sm_vecDtNames;
  m_vecPkNames = TsManagerPostGIS::sm_vecPkNames;  
  m_vecUkNames = TsManagerPostGIS::sm_vecUkNames;
  m_vecIdxNames = TsManagerPostGIS::sm_vecIdxNames;
  m_vecFkNames = TsManagerPostGIS::sm_vecFkNames;
  m_vecCcNames = TsManagerPostGIS::sm_vecCcNames;
  m_vecSeqNames = TsManagerPostGIS::sm_vecSeqNames;
  m_vecDtNamesAndEnvelops = TsManagerPostGIS::sm_vecDtNamesAndEnvelops;

  m_nroDataSets = TsManagerPostGIS::sm_nroDataSets;
  m_newDataSetType = TsManagerPostGIS::sm_newDataSetType;
}

void TsPostGISCatalogLoader::tearDown() 
{
  m_ds = 0;
  m_dsType = "";
}



void TsPostGISCatalogLoader::tcPGTransactor()
{
//#ifdef TE_COMPILE_ALL
  //std::auto_ptr<te::pgis::Transactor> pgtrans(static_cast<te::pgis::DataSource*>(m_ds)->getPGTransactor());
  //CPPUNIT_ASSERT(pgtrans.get());
  te::pgis::Transactor* pgtrans;
  CPPUNIT_ASSERT_NO_THROW(pgtrans = static_cast<te::pgis::DataSource*>(m_ds)->getPGTransactor());
  CPPUNIT_ASSERT_NO_THROW(delete pgtrans);
//#endif
};

