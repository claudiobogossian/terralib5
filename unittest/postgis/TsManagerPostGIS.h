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
  \file TsManagerPostGIS.h
 
  \brief A static class for managing the resources used by PostGIS test suit.
 */

#ifndef __TERRALIB_UNITTEST_DATAACCESS_INTERNAL_MANAGERPOSTGIS_H
#define __TERRALIB_UNITTEST_DATAACCESS_INTERNAL_MANAGERPOSTGIS_H

// TerraLib forward declarations
#include <terralib/dataaccess/datasource_fw.h>
#include <terralib/dataaccess/dataset_fw.h>
#include <terralib/geometry_fw.h>
#include <terralib/postgis_fw.h>

// STL
#include <string>
#include <vector>

/*!
  \class TsManagerPostGIS

  \brief A static class for managing the resources used by PostGIS test suit.
 */
class TsManagerPostGIS
{
  public:

    /*!
      \brief It will initialize the resources used in the PostGIS Teste Suit:
             TsDataSource, TsDataSet, TsDataSetTypePersistence, TsTransactor etc

      This will avoid openning/closing database for each test case.
     */
    static void initialize();

    /*!
      \brief It will finalize the resources used in PostGIS Teste Suit:
             TsDataSource, TsDataSet, TsDataSetTypePersistence, TsTransactor...
     */
    static void finalize();

    /*!
      \brief It will print some static values initialized before and used by all test cases.
             
     */
    static void printValues();

// The sm_* will be initialized here and the setUp of PostGIS driver will use them to run all test suite
  static te::da::DataSource* sm_datasource ;
  static std::map<std::string, std::string> sm_capabilit;
  static te::da::DataSetType* sm_newdt ;
  static te::da::DataSetType* sm_newdt3 ;
  static te::da::ForeignKey* sm_fk ;
  static std::map<std::string, std::string> sm_connInfo;
  static std::map<std::string, std::string> sm_connInfoChk;
  static std::map<std::string, std::string> sm_connInfoNewDs;
  static std::map<std::string, std::string> sm_connInfoNewPostgresDb;
  static std::map<std::string, std::string> sm_connInfoDropDs;
  static std::string sm_dsType;
  static std::string sm_description;
  static std::string sm_displayName; 

// The next statics will be used to compose the vector of pairs below
  static std::vector<std::string> sm_vecDtNames;

  static std::vector<te::gm::Envelope>  sm_vecEnvelops;
  static std::vector<te::gm::Envelope>  sm_recEnvelops;
  static std::vector<size_t>  sm_dtSize;
  static std::vector<size_t>  sm_dtRecSize;

// Vector of pairs to be used in Test Cases
  static std::vector<std::pair<std::string, te::gm::Envelope> > sm_vecDtNamesAndEnvelops;
  static std::vector<std::pair<std::string, te::gm::Envelope> > sm_vecNamesAndRecs;
  static std::vector<std::pair<std::string, size_t> > sm_vecNamesSizes;
  static std::vector<std::pair<std::string, size_t> > sm_vecNamesSizesRec;

// The next vectors will be used to test the existence of pk, uk, fk, idx, Cc, seq
  static std::vector<std::string> sm_vecPkNames;
  static std::vector<std::string> sm_vecUkNames;
  static std::vector<std::string> sm_vecCcNames;
  static std::vector<std::string> sm_vecIdxNames;
  static std::vector<std::string> sm_vecFkNames;
  static std::vector<std::string> sm_vecSeqNames;

// Geometries to be used in getDataSet API´s
  static te::gm::Envelope sm_box;
  static te::gm::Point sm_pt;
  static te::gm::LinearRing* sm_linearRing;
  static te::gm::Polygon* sm_pol;
  static te::gm::Geometry* sm_geom;

// Defining  sm_* variables dependent on the driver  

// These variables can be used only by derived Test classes.
  static std::string sm_currentSchema;
 
// Defining sm_* variables to test catalog stufs after loadCatalog...
  static size_t sm_nroDataSets;

//Defining specific variables to test DataSetTypePersistence
  static te::da::CheckConstraint* sm_cc;
  static std::string sm_newDataSetType;
  static std::string sm_NameAttr1;
  static std::string sm_Attr1;
  static std::string sm_NameAttr2; 
  static std::string sm_NameAttr3; 
  static std::string sm_NameAttr4; 
  static std::string sm_NameAttr5; 
  static std::string sm_NameAttr6; 
  static std::string sm_NameAttr7; 
  static std::string sm_NameAttr8;

  //new capabities
  static std::string sm_attr_bit;
  static std::string sm_attr_char;  
  static std::string sm_attr_uchar;
  static std::string sm_attr_int16;
  static std::string sm_attr_uint16;
  static std::string sm_attr_int32;
  static std::string sm_attr_uint32;
  static std::string sm_attr_int64;
  static std::string sm_attr_uint64;

  static std::string sm_attr_bool;
  static std::string sm_attr_float;
  static std::string sm_attr_double ;
  static std::string sm_attr_numeric;
  static std::string sm_attr_string;
  static std::string sm_attr_byte_array;
  static std::string sm_attr_geometry;
  static std::string sm_attr_datetime;
  static std::string sm_attr_array;
  static std::string sm_attr_composite;
  static std::string sm_attr_raster;
  //

  static std::string sm_Attr6Pk;
  static std::string sm_Attr6Fk;

  static std::string sm_Attr3e4Uk;
  static std::string sm_Attr5idx; 
  static std::string sm_Attr2idx; 
  static std::string sm_Attr7_seq;

  static std::string sm_newCapabilitiesDt;
  static std::string sm_newDataSetType2;
  static std::string sm_newDataSetType3;
  static std::string sm_dtcloned;

  static std::string sm_oldNameProp;
  static std::string sm_newNameProp;

// End of Specification of  variables to use in DataSetTypePersistence tests
};

#endif  // __TERRALIB_UNITTEST_DATAACCESS_INTERNAL_MANAGERPOSTGIS_H

