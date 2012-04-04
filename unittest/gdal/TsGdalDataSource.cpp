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
  \file TsGdalDataSource.cpp
 
  \brief A test suit for the Gdal class.
 */

// TerraLib
#include <terralib/common.h>
#include <terralib/geometry.h>
#include <terralib/gdal.h>

// Unit-Test TerraLib
#include "TsManagerGdal.h"
#include "TsGdalDataSource.h"

#include "../dataaccess/Config.h"

#ifdef TE_TEST_GDAL
CPPUNIT_TEST_SUITE_REGISTRATION( TsGdal );
#endif

void TsGdal::setUp()
{ 
// get values from TsManagerGdal (statics)
  m_ds = TsManagerGdal::sm_datasource;
  m_capabilit = TsManagerGdal::sm_capabilit;
  m_dsType = TsManagerGdal::sm_dsType;
  m_connInfo = TsManagerGdal::sm_connInfo;
  m_connInfoStr = TsManagerGdal::sm_connInfoStr;
  m_connInfoNewDs = TsManagerGdal::sm_connInfoNewDs;
  m_connInfoDropDs = TsManagerGdal::sm_connInfoDropDs;

  m_nroDataSets = TsManagerGdal::sm_nroDataSets;
  //m_vecDtNames = TsManagerGdal::sm_vecDtNames;
  //m_vecEnvelops = TsManagerGdal::sm_vecEnvelops;
  //m_vecDtNamesAndEnvelops = TsManagerGdal::sm_vecDtNamesAndEnvelops;
}


void TsGdal::tearDown() 
{
  m_ds = 0;
  m_dsType = "";

}
