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
  \file TsDataSetTypePersistencePostGIS.cpp
 
  \brief Test suite for the PostGIS DataSetTypePersistence implemention.
 */

// STL
#include <ctime>

// TerraLib
#include <terralib/common.h>
#include <terralib/dataaccess.h>
#include <terralib/geometry.h>
#include <terralib/postgis.h>

// Unit-Test TerraLib
#include "TsManagerPostGIS.h"
#include "TsDataSetTypePersistencePostGIS.h"

#include "../dataaccess/Config.h"

#ifdef TE_TEST_PGIS
CPPUNIT_TEST_SUITE_REGISTRATION( TsDataSetTypePersistencePostGIS );
#endif

void TsDataSetTypePersistencePostGIS::setUp()
{ 
// get values from TsManagerPostGIS (statics)
  m_ds = TsManagerPostGIS::sm_datasource;
  m_capabilit = TsManagerPostGIS::sm_capabilit;
  m_newdt = TsManagerPostGIS::sm_newdt;
  m_newdt3 = TsManagerPostGIS::sm_newdt3;
  m_fk = TsManagerPostGIS::sm_fk;
  m_connInfo = TsManagerPostGIS::sm_connInfo;
  m_dsType = TsManagerPostGIS::sm_dsType;
  m_connInfoNewDs = TsManagerPostGIS::sm_connInfoNewDs;
  m_nroDataSets = TsManagerPostGIS::sm_nroDataSets;
  m_newDataSetType = TsManagerPostGIS::sm_newDataSetType;

  m_cc = TsManagerPostGIS::sm_cc;
  m_NameAttr1 = TsManagerPostGIS::sm_NameAttr1 ;
  m_Attr1 = TsManagerPostGIS::sm_Attr1 ;
  m_NameAttr2 = TsManagerPostGIS::sm_NameAttr2 ; 
  m_NameAttr3 = TsManagerPostGIS::sm_NameAttr3; 
  m_NameAttr4 = TsManagerPostGIS::sm_NameAttr4; 
  m_NameAttr5 = TsManagerPostGIS::sm_NameAttr5;; 
  m_NameAttr6 = TsManagerPostGIS::sm_NameAttr6; 
  m_NameAttr7 = TsManagerPostGIS::sm_NameAttr7; 
  m_NameAttr8 = TsManagerPostGIS::sm_NameAttr8;

  m_Attr6Pk   = TsManagerPostGIS::sm_Attr6Pk;
  m_Attr6Fk   = TsManagerPostGIS::sm_Attr6Fk;
  m_Attr3e4Uk = TsManagerPostGIS::sm_Attr3e4Uk;
  m_Attr5idx  = TsManagerPostGIS::sm_Attr5idx; 
  m_Attr2idx  = TsManagerPostGIS::sm_Attr2idx; 
  m_Attr7_seq = TsManagerPostGIS::sm_Attr7_seq;

// to check the datatypes defined in postgis capabilities
  m_attr_bit = TsManagerPostGIS::sm_attr_bit;
  m_attr_char = TsManagerPostGIS::sm_attr_char;  
  m_attr_uchar = TsManagerPostGIS::sm_attr_uchar;
  m_attr_int16 = TsManagerPostGIS::sm_attr_int16;
  m_attr_uint16 = TsManagerPostGIS::sm_attr_uint16;
  m_attr_int32 = TsManagerPostGIS::sm_attr_int32;
  m_attr_uint32 = TsManagerPostGIS::sm_attr_uint32;
  m_attr_int64 = TsManagerPostGIS::sm_attr_int64;
  m_attr_uint64 = TsManagerPostGIS::sm_attr_uint64;
  m_attr_bool = TsManagerPostGIS::sm_attr_bool;
  m_attr_float = TsManagerPostGIS::sm_attr_float;
  m_attr_double = TsManagerPostGIS::sm_attr_double;
  m_attr_numeric = TsManagerPostGIS::sm_attr_numeric;
  m_attr_string = TsManagerPostGIS::sm_attr_string;
  m_attr_byte_array = TsManagerPostGIS::sm_attr_byte_array;
  m_attr_geometry = TsManagerPostGIS::sm_attr_geometry;
  m_attr_datetime = TsManagerPostGIS::sm_attr_datetime;
  m_attr_array = TsManagerPostGIS::sm_attr_array;
  m_attr_composite = TsManagerPostGIS::sm_attr_composite;
  m_attr_raster = TsManagerPostGIS::sm_attr_raster;
 
// DataSetTypes names
  m_newCapabilitiesDt = TsManagerPostGIS::sm_newCapabilitiesDt;
  m_dtcloned = TsManagerPostGIS::sm_dtcloned;
  m_newDataSetType2 = TsManagerPostGIS::sm_newDataSetType2 ;
  m_newDataSetType3 = TsManagerPostGIS::sm_newDataSetType3 ;

  m_oldNameProp = TsManagerPostGIS::sm_oldNameProp ;
  m_newNameProp = TsManagerPostGIS::sm_newNameProp ;
}

void TsDataSetTypePersistencePostGIS::tearDown() 
{
  m_ds = 0;
  m_dsType = "";
}




