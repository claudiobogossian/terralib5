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
  \file TsGdalTransactor.cpp
 
  \brief A test suit for the Gdal Transactor class.
 */

// TerraLib
#include <terralib/common.h>
#include <terralib/geometry.h>
#include <terralib/gdal.h>

// Unit-Test TerraLib
#include "TsManagerGdal.h"
#include "TsGdalTransactor.h"

#include "../dataaccess/Config.h"

#ifdef TE_TEST_GDAL
CPPUNIT_TEST_SUITE_REGISTRATION( TsGdalTransactor );
#endif

void TsGdalTransactor::setUp()
{ 
  //get values from TsManagerGdal (statics)
  m_ds = TsManagerGdal::sm_datasource;
  m_capabilit = TsManagerGdal::sm_capabilit;
  m_connInfo = TsManagerGdal::sm_connInfo;
  m_dsType = TsManagerGdal::sm_dsType;
  //m_newDataSetType = TsManagerGdal::sm_newDataSetType;

  m_nroDataSets = TsManagerGdal::sm_nroDataSets;

  m_vecDtNames = TsManagerGdal::sm_vecDtNames;
  m_vecEnvelops = TsManagerGdal::sm_vecEnvelops;
  m_vecDtNamesAndEnvelops = TsManagerGdal::sm_vecDtNamesAndEnvelops;
  m_vecNamesSizes = TsManagerGdal::sm_vecNamesSizes;
  m_vecNamesAndRecs  = TsManagerGdal::sm_vecNamesAndRecs;
  m_vecNamesSizesRec = TsManagerGdal::sm_vecNamesSizesRec;

  m_box = TsManagerGdal::sm_box;

  m_pt.setSRID(4291);
  m_pt.setX(TsManagerGdal::sm_pt.getX());
  m_pt.setY(TsManagerGdal::sm_pt.getY());

  m_linearRing = TsManagerGdal::sm_linearRing;
  m_pol = TsManagerGdal::sm_pol;

  m_geom = TsManagerGdal::sm_geom;
}

void TsGdalTransactor::tearDown() 
{
  m_ds = 0;
  m_dsType = "";
}

