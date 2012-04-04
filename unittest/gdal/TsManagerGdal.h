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
  \file TsManagerGdal.h
 
  \brief A static class for managing the resources used by Gdal test suit.
 */

#ifndef __TERRALIB_UNITTEST_DATAACCESS_INTERNAL_MANAGERGDAL_H
#define __TERRALIB_UNITTEST_DATAACCESS_INTERNAL_MANAGERGDAL_H

// TerraLib forward declarations
#include <terralib/dataaccess/datasource_fw.h>
#include <terralib/dataaccess/dataset_fw.h>

#include <terralib/geometry_fw.h>
#include <terralib/gdal_fw.h>

// STL
#include <string>
#include <vector>

/*!
  \class TsManagerGdal

  \brief A static class for managing the resources used by Gdal test suit.
 */
class TsManagerGdal
{
  public:

    /*!
      \brief It will initialize the resources used in the Gdal Teste Suit:
             TsDataSource, TsDataSet, TsDataSetTypePersistence, TsTransactor etc

      This will avoid openning/closing datasource for each test case.
     */
    static void initialize();

    /*!
      \brief It will finalize the resources used in Gdal Teste Suit:
             TsDataSource, TsDataSet, TsDataSetTypePersistence, TsTransactor...
     */
    static void finalize();

    /*!
      \brief It will print some static values initialized before and used by all test cases.
             
     */
    static void printValues();

// The sm_* will be initialized here and the setUp of Gdal driver will use them to run all test suite
  static te::da::DataSource* sm_datasource;
  static std::map<std::string, std::string> sm_capabilit;
  static te::da::DataSetType* sm_newdt;
  static te::da::DataSetType* sm_newdt3;

  static std::string sm_newCapabilitiesDt;
  static std::string sm_dtcloned;
  static std::string sm_newDataSetType2;

  static std::string sm_oldNameProp;
  static std::string sm_newNameProp;
  static std::string sm_newDataSetType3;

  static std::map<std::string, std::string> sm_connInfo;
  static std::map<std::string, std::string> sm_connInfoNewDs;
  static std::map<std::string, std::string> sm_connInfoDropDs;
  static std::string sm_newDataSetType;
  static std::string sm_dsType;
  static std::string sm_description;
  static std::string sm_displayName; 

  static std::string sm_connInfoStr;
  static std::string sm_newDSourceStr;

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

// Geometries to be used in getDataSet API´s
  static te::gm::Envelope sm_box;
  static te::gm::Point sm_pt;
  static te::gm::LinearRing* sm_linearRing;
  static te::gm::Polygon* sm_pol;
  static te::gm::Geometry* sm_geom;

// Defining  sm_* variables dependent on the driver GDAL =====================
  // These variables  can be used by base and derived Test classes.
  //???? LUBIA --- Ver o equivalente para o GDAL se houver...

// These variables that can be used only by derived Test classes.
  
// End of GDAL dependent sm_* variables  =================================================

// Defining sm_* variables to test catalog stufs after loadCatalog...
  static size_t sm_nroDataSets;


};

#endif  // __TERRALIB_UNITTEST_DATAACCESS_INTERNAL_MANAGERGDAL_H

