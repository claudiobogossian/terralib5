/*  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

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
  \file TsManagerDataSource.h
 
  \brief A static class for managing the resources used by DataSource test suit.
 */

#ifndef __TERRALIB_UNITTEST_DATAACCESS_INTERNAL_MANAGERDATASOURCE_H
#define __TERRALIB_UNITTEST_DATAACCESS_INTERNAL_MANAGERDATASOURCE_H

// TerraLib forward declarations
#include <terralib/dataaccess/datasource_fw.h>
#include <terralib/dataaccess/dataset_fw.h>

#include <terralib/geometry_fw.h>
//#include <terralib/postgis_fw.h>

#include <terralib/datatype.h>

// STL
#include <string>
#include <vector>

/*!
  \class TsManagerDataSource

  \brief A static class for managing the resources used by DataSource test suit.
 */
class TsManagerDataSource
{
  public:
    /*!
      \brief It will initialize the resources used by Test Suit:
              TsDataSource, TsDataSet, TsDataSetTypePersistence, TsTransactor etc
      \param driver_name    A json file name from where to read values used by tests.

      This will avoid openning/closing database for each test case.
    */
    static void initialize(const std::string driver_name);

    /*!
      \brief It will finalize the resources used in Teste Suit:
             TsDataSource, TsDataSet, TsDataSetTypePersistence, TsTransactor...
     */
    static void finalize();

    /*!
      \brief Converts a vector of vector of doubles - representing the envelopes of layers
             into a  vector of te::gm::Envelope

      \param vectd    A vector of vector of doubles std::vector<std::vector<double>>.
      \param vecte    The vector of Envelope to output the values.
    */
    static void ConvertVectorToEnvelope(std::vector<std::vector<double> >& vectd, std::vector<te::gm::Envelope>& vecte);

    /*!
      \brief Converts a vector of vector of 3 doubles - representing points (SRID,X,Y) into a vector of te::gm::Envelope

      \param vectd    A vector of vector of doubles std::vector<std::vector<double>>.
      \param vectp    The vector of Points with SRID,X,Y to output the values.
    */
    static void ConvertVectorToPoints( std::vector< std::vector<double> >& vectd, std::vector<te::gm::Point>& vectp);

    /*!
      \brief Converts a vector of vector of 3 doubles - representing points (SRID,X,Y) into a vector of te::gm::LinearRings

      \param vectd    A vector of vector of values representing points std::vector<std::vector<double>>.
      \param vectp    The vector of Points with SRID,X,Y to output the values.
    */
    static void ConvertVectorToLinearRing(std::vector<std::vector<double> >& vectd, te::gm::LinearRing& lr);

    /*!
      \brief Converts a string into a string (Enum) property type

      \param s    String represententing a property type.
      \param t    A string property type.
    */
    static void ConvertToPropType(std::string s, te::dt::StringType &t);

    /*!
      \brief Converts a string into a number (Enum) representing a property type

      \param s    String represententing a property type.
      \param t    A number (int) representing a property type.
    */
    static void ConvertToPropType(std::string s, int &t);
 
    /*!
      \brief It will set vectors of pairs of values based in some vector already known.
             
    */
    static void setPairsValues();

// The sm_* will be initialized here and the setUp of PostGIS driver will use them to run all test suite
  static te::da::DataSource* sm_datasource ;
  //static std::auto_ptr<te::da::DataSource> sm_datasource;
  static te::da::DataSourceCapabilities  sm_capabilit;
  static std::string sm_dsType;
  static std::map<std::string, std::string> sm_connInfo;
  static std::map<std::string, std::string> sm_connInfoExist;

  static std::map<std::string, std::string> sm_connInfoNewDs;
  static std::map<std::string, std::string> sm_connInfoDropDs;
  static std::string sm_connStr;

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
  static std::vector<std::string> sm_vecIdxNames;
  static std::vector<std::string> sm_vecFkNames;
  static std::vector<std::string> sm_vecSeqNames;
  static std::vector<std::string> sm_vecCcNames;

// Geometries to be used in getDataSet API�s
  static te::gm::Envelope sm_box;
  static te::gm::Point sm_pt;
  static te::gm::LinearRing* sm_linearRing;
  static te::gm::Polygon* sm_pol;
  static te::gm::Geometry* sm_geom;

  static size_t sm_nroDataSets;

// Defining specific variables to test DataSetTypePersistence

  static te::da::DataSetType* sm_newdt ;
  static te::da::DataSetType* sm_newdt3 ;

  static std::string sm_ccName; 
  static te::da::CheckConstraint* sm_cc;
  static std::string sm_NameAttr1;
  static std::string sm_Attr1;
  static std::string sm_NameAttr2; 
  static std::string sm_NameAttr3; 
  static std::string sm_NameAttr4; 
  static std::string sm_NameAttr5; 
  static std::string sm_NameAttr6; 
  static std::string sm_NameAttr7; 
  static std::string sm_NameAttr8;

  static std::string sm_Attr6Pk;
  static std::string sm_Attr6Fk;

  static std::string sm_Attr3e4Uk;
  static std::string sm_Attr5idx; 
  static std::string sm_Attr2idx; 
  static std::string sm_Attr7_seq;

// DataSetType names used in persistence
  static std::string sm_newDataSetType;
  static std::string sm_newCapabilitiesDt;
  static std::string sm_newDataSetType2;
  static std::string sm_newDataSetType3;
  static std::string sm_dtcloned;

  static std::string sm_oldNameProp;
  static std::string sm_newNameProp;

  static te::da::ForeignKey* sm_fk ;
};
#endif  // __TERRALIB_UNITTEST_DATAACCESS_INTERNAL_MANAGERDATASOURCE_H

