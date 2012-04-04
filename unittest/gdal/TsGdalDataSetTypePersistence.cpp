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
  \file TsDataSetTypePersistenceGDAL.cpp
 
  \brief Test suite for the GDAL DataSetTypePersistence implemention.
 */

// STL
#include <ctime>

// TerraLib
#include <terralib/common.h>
//#include <terralib/dataaccess.h>
#include <terralib/geometry.h>
#include <terralib/gdal.h>

// Unit-Test TerraLib
#include "TsManagerGdal.h"
#include "TsGdalDataSetTypePersistence.h"

#include "../dataaccess/Config.h"

#ifdef TE_TEST_PGIS
CPPUNIT_TEST_SUITE_REGISTRATION( TsGdalDataSetTypePersistence );
#endif

void TsGdalDataSetTypePersistence::setUp()
{ 
// get values from TsManagerGdal (statics)
  m_ds = TsManagerGdal::sm_datasource;
  m_capabilit = TsManagerGdal::sm_capabilit;
  m_connInfo = TsManagerGdal::sm_connInfo;
  m_dsType = TsManagerGdal::sm_dsType;
  m_connInfoNewDs = TsManagerGdal::sm_connInfoNewDs;
  m_nroDataSets = TsManagerGdal::sm_nroDataSets;
  m_newdt = TsManagerGdal::sm_newdt;
  m_newdt3 = TsManagerGdal::sm_newdt3;
  m_newDataSetType = TsManagerGdal::sm_newDataSetType;
  m_newCapabilitiesDt = TsManagerGdal::sm_newCapabilitiesDt;
  m_dtcloned = TsManagerGdal::sm_dtcloned;
  m_newDataSetType2 = TsManagerGdal::sm_newDataSetType2 ;
  m_newDataSetType3 = TsManagerGdal::sm_newDataSetType3 ;
  m_oldNameProp = TsManagerGdal::sm_oldNameProp;
  m_newNameProp = TsManagerGdal::sm_newNameProp;

}

void TsGdalDataSetTypePersistence::tearDown() 
{
  m_ds = 0;
  m_dsType = "";
}




