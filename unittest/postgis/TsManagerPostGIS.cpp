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
  \file TsManagerPostGIS.cpp
 
  \brief A static class for managing the resources used by PostGIS test suit.
 */

// TerraLib
#include <terralib/common.h>
#include <terralib/dataaccess.h>
#include <terralib/geometry.h>
#include <terralib/postgis.h>

// Unit-Test TerraLib
#include "TsManagerPostGIS.h"
#include "../dataaccess/Config.h"


// cppUnit
#include <cppunit/extensions/HelperMacros.h>

// static members defined in TsManagerPostGIS
te::da::DataSource* TsManagerPostGIS::sm_datasource;
std::map<std::string, std::string> TsManagerPostGIS::sm_capabilit;
te::da::DataSetType* TsManagerPostGIS::sm_newdt;
te::da::DataSetType* TsManagerPostGIS::sm_newdt3;
te::da::ForeignKey* TsManagerPostGIS::sm_fk;

te::da::CheckConstraint* TsManagerPostGIS::sm_cc; 
std::map<std::string, std::string> TsManagerPostGIS::sm_connInfo;
std::map<std::string, std::string> TsManagerPostGIS::sm_connInfoNewDs;
std::map<std::string, std::string> TsManagerPostGIS::sm_connInfoNewPostgresDb;
std::map<std::string, std::string> TsManagerPostGIS::sm_connInfoDropDs;
std::string TsManagerPostGIS::sm_dsType;
std::string TsManagerPostGIS::sm_description;
std::string TsManagerPostGIS::sm_displayName; 

std::vector<std::string> TsManagerPostGIS::sm_vecDtNames;
std::vector<te::gm::Envelope> TsManagerPostGIS::sm_vecEnvelops;
std::vector<te::gm::Envelope> TsManagerPostGIS::sm_recEnvelops;
std::vector<size_t>  TsManagerPostGIS::sm_dtSize;
std::vector<size_t>  TsManagerPostGIS::sm_dtRecSize;

std::vector<std::string> TsManagerPostGIS::sm_vecPkNames;
std::vector<std::string> TsManagerPostGIS::sm_vecUkNames;
std::vector<std::string> TsManagerPostGIS::sm_vecCcNames;
std::vector<std::string> TsManagerPostGIS::sm_vecIdxNames;
std::vector<std::string> TsManagerPostGIS::sm_vecFkNames;
std::vector<std::string> TsManagerPostGIS::sm_vecSeqNames;

std::vector<std::pair<std::string, te::gm::Envelope> > TsManagerPostGIS::sm_vecDtNamesAndEnvelops;
std::vector<std::pair<std::string, te::gm::Envelope> > TsManagerPostGIS::sm_vecNamesAndRecs;
std::vector<std::pair<std::string, size_t> > TsManagerPostGIS::sm_vecNamesSizes;
std::vector<std::pair<std::string, size_t> > TsManagerPostGIS::sm_vecNamesSizesRec;

// Defining some geometries valid for dataSet "br_munic_2001" and city "Ouro Preto" 
te::gm::Envelope    TsManagerPostGIS::sm_box; //(-43.9329795837402, -20.6328010559082, -43.4036407470703, -20.1612071990967);
te::gm::Point      TsManagerPostGIS::sm_pt; //(4291, 0, -43.6107606714293, -20.3913548070123);
te::gm::LinearRing* TsManagerPostGIS::sm_linearRing = new te::gm::LinearRing(6, te::gm::LineStringType);
te::gm::Polygon*    TsManagerPostGIS::sm_pol = new te::gm::Polygon(0, te::gm::PolygonType);
te::gm::Geometry*   TsManagerPostGIS::sm_geom;

std::string TsManagerPostGIS::sm_currentSchema;

size_t TsManagerPostGIS::sm_nroDataSets;

// Defining specific variables to test DataSetTypePersistence 
std::string TsManagerPostGIS::sm_newDataSetType;
std::string TsManagerPostGIS::sm_NameAttr1;
std::string TsManagerPostGIS::sm_Attr1;
std::string TsManagerPostGIS::sm_NameAttr2; 
std::string TsManagerPostGIS::sm_NameAttr3; 
std::string TsManagerPostGIS::sm_NameAttr4; 
std::string TsManagerPostGIS::sm_NameAttr5; 
std::string TsManagerPostGIS::sm_NameAttr6; 
std::string TsManagerPostGIS::sm_NameAttr7; 
std::string TsManagerPostGIS::sm_NameAttr8;

std::string TsManagerPostGIS::sm_Attr6Pk;
std::string TsManagerPostGIS::sm_Attr6Fk;
std::string TsManagerPostGIS::sm_Attr3e4Uk;
std::string TsManagerPostGIS::sm_Attr5idx; 
std::string TsManagerPostGIS::sm_Attr2idx; 
std::string TsManagerPostGIS::sm_Attr7_seq;

// to check the datatypes defined in postgis capabilities
std::string TsManagerPostGIS::sm_attr_bit;
std::string TsManagerPostGIS::sm_attr_char;  
std::string TsManagerPostGIS::sm_attr_uchar;
std::string TsManagerPostGIS::sm_attr_int16;
std::string TsManagerPostGIS::sm_attr_uint16;
std::string TsManagerPostGIS::sm_attr_int32;
std::string TsManagerPostGIS::sm_attr_uint32;
std::string TsManagerPostGIS::sm_attr_int64;
std::string TsManagerPostGIS::sm_attr_uint64;

std::string TsManagerPostGIS::sm_attr_bool;
std::string TsManagerPostGIS::sm_attr_float;
std::string TsManagerPostGIS::sm_attr_double;
std::string TsManagerPostGIS::sm_attr_numeric;
std::string TsManagerPostGIS::sm_attr_string;
std::string TsManagerPostGIS::sm_attr_byte_array;
std::string TsManagerPostGIS::sm_attr_geometry;
std::string TsManagerPostGIS::sm_attr_datetime;
std::string TsManagerPostGIS::sm_attr_array;
std::string TsManagerPostGIS::sm_attr_composite;
std::string TsManagerPostGIS::sm_attr_raster;

// DataSetType names
std::string TsManagerPostGIS::sm_newCapabilitiesDt; //based on DataSource Capabilities
std::string TsManagerPostGIS::sm_dtcloned; //cloned
std::string TsManagerPostGIS::sm_newDataSetType2; //used to rename cloned one
std::string TsManagerPostGIS::sm_newDataSetType3; //used to create dt with pk,uk,idx, etc.

std::string TsManagerPostGIS::sm_oldNameProp; 
std::string TsManagerPostGIS::sm_newNameProp;

// End of Specification of variables to be used in TsDataSetTypePersistence


//end of static members

void TsManagerPostGIS::initialize()
{
//************ Initializing datasource driver - PostGIS **********//

// the static variables sm_* can be read from a file (in the future)

// let's get PostGIS data source instance using the DataSourceFactory
  sm_datasource = te::da::DataSourceFactory::make("POSTGIS");
  sm_datasource->getCapabilities(sm_capabilit);

// let´s give the minimal server connection information used to connect
  sm_connInfo["PG_HOST"] = "atlas.dpi.inpe.br" ;   // or "localhost";
  sm_connInfo["PG_USER"] = "postgres";
  sm_connInfo["PG_PASSWORD"] = "sitim110";
  sm_connInfo["PG_DB_NAME"] = "terralib4";
  sm_connInfo["PG_CONNECT_TIMEOUT"] = "4"; 

// Other static memory variables used to check datasource recovered data
  sm_dsType = te::pgis::Platform::getDriverID();
  sm_currentSchema ="public";

// Defining sm_* to check DataSourceCatalog in test cases...
  sm_nroDataSets = 33;

  sm_description = "Test on Set Description";
  sm_displayName = "TerraLib5"; 

// vector of DataSet Names
  sm_vecDtNames.push_back("public.br_munic_2001");
  sm_vecDtNames.push_back("public.batimetria");
  sm_vecDtNames.push_back("public.br_focos2003");
  sm_vecDtNames.push_back("public.tstcatalog2");
  sm_vecDtNames.push_back("public.tstcatalog3");
  sm_vecDtNames.push_back("public.tstcatalog1");

// Defining some valid geometries to be used as spatial filter...
// It must be defined for the first dataSet defined in sm_vecDtNames...
// In this case it is "public.br_munic_2001" and city "Ouro Preto" 
  sm_box = te::gm::Envelope(-43.9329795837402, -20.6328010559082, -43.4036407470703, -20.1612071990967);

  //point
  sm_pt.setSRID(4291);
  sm_pt.setX(-43.6107606714293);
  sm_pt.setY(-20.3913548070123);
  
  //polygon
  sm_linearRing->setPoint(0, -43.932979522347395, -20.632799968306863);  
  sm_linearRing->setPoint(1, -43.932979522347395, -20.161208868097958);
  sm_linearRing->setPoint(2, -43.600000000000000, -19.500000000000000); //inserted point
  sm_linearRing->setPoint(3, -43.403643659752738, -20.161208868097958); 
  sm_linearRing->setPoint(4, -43.403643659752738, -20.632799968306863);
  sm_linearRing->setPoint(5, -43.932979522347395, -20.632799968306863); // closing
  sm_pol->push_back(sm_linearRing);
  sm_pol->setSRID(4291);

  //geometry
  //...

// End of definition of geometries

// Minimal Server connection information to create a newDB POSTGIS
  sm_connInfoNewDs["PG_HOST"] = "atlas.dpi.inpe.br" ;   // or "localhost";
  sm_connInfoNewDs["PG_USER"] = "postgres";
  sm_connInfoNewDs["PG_PASSWORD"] = "sitim110";
  sm_connInfoNewDs["PG_DB_NAME"] = "postgres";
  sm_connInfoNewDs["PG_CONNECT_TIMEOUT"] = "4";
  sm_connInfoNewDs["template"] = "template_postgis";

  //newdb parameters added to connection info
  sm_connInfoNewDs["newdb_name"] = "terralib5_newdb";
  sm_connInfoNewDs["newdb_connect_timeout"] = "4";
  sm_connInfoNewDs["newdb_template"] = "template_postgis";
  sm_connInfoNewDs["newdb_tablespace"] = "pg_default";
  sm_connInfoNewDs["newdb_owner"] = "postgres";
  sm_connInfoNewDs["newdb_encoding"] = "'LATIN1'";
  sm_connInfoNewDs["newdb_connection_timeout"] = "4";

// Minimal Server connection information to create a new postgres DB (without postgis extension)
  sm_connInfoNewPostgresDb["PG_HOST"] = "atlas.dpi.inpe.br" ;   // or "localhost";
  sm_connInfoNewPostgresDb["PG_USER"] = "postgres";
  sm_connInfoNewPostgresDb["PG_PASSWORD"] = "sitim110";
  sm_connInfoNewPostgresDb["PG_DB_NAME"] = "postgres";
  sm_connInfoNewPostgresDb["PG_CONNECT_TIMEOUT"] = "4";
 
  //newdb parameters added to connection info
  sm_connInfoNewPostgresDb["newdb_name"] = "terralib5_postgresdb";
  sm_connInfoNewPostgresDb["newdb_tablespace"] = "pg_default";
  sm_connInfoNewPostgresDb["newdb_owner"] = "postgres";
  sm_connInfoNewPostgresDb["newdb_encoding"] = "'LATIN1'";
  sm_connInfoNewPostgresDb["newdb_conn_limit"] = "16";

// Minimal Server connection information to drop the postgis created before
  sm_connInfoDropDs["PG_HOST"] = "atlas.dpi.inpe.br" ;   // or "localhost";
  sm_connInfoDropDs["PG_USER"] = "postgres";
  sm_connInfoDropDs["PG_PASSWORD"] = "sitim110";
  sm_connInfoDropDs["PG_DB_NAME"] = "terralib4";
  sm_connInfoDropDs["PG_CONNECT_TIMEOUT"] = "4";
  sm_connInfoDropDs["db_todrop"] = "terralib5_newdb";
 
// Specific variables to use in DataSetTypePersistence tests
  sm_newDataSetType = "public.dataset_type"; //define always 8 attributes

  sm_NameAttr1 = "attribute1_numeric" ; 
  sm_Attr1     = "43.78";
  sm_NameAttr2 = "attribute2_fixed_string" ;
  sm_NameAttr3 = "attribute3_var_string" ;
  sm_NameAttr4 = "attribute4_var_unlimited_string" ;
  sm_NameAttr5 = "attribute5_geom_polm" ;
  sm_NameAttr6 = "attribute6_int" ;
  sm_NameAttr7 = "attribute7_int_serial" ;
  sm_NameAttr8 = "attribute8_array_of_int";

  sm_Attr6Pk = "attribute6_int_pk";
  sm_Attr6Fk = "attribute6_as_fk_datasetType"; //used in datasettype3
  sm_Attr3e4Uk = "attribute4_and_attribute3_uk";
  sm_Attr5idx = "attribute5_geom_polm_spidx" ;
  sm_Attr2idx = "attribute2_fixed_string_idx" ;
  sm_Attr7_seq = "dataset_type_attribute7_int_serial_seq";

// used to create properties of all types suported by postgis (defined in capabilities)
  sm_attr_bit = "attr_bit";
  sm_attr_char = "attr_char";  
  sm_attr_uchar = "attr_uchar";
  sm_attr_int16 = "attr_int16";
  sm_attr_uint16 = "attr_uint16";
  sm_attr_int32 = "attr_int32";
  sm_attr_uint32 = "attr_uint32";
  sm_attr_int64 = "attr_int64";
  sm_attr_uint64 = "attr_uint64";
  sm_attr_bool  = "attr_bool";
  sm_attr_float = "attr_float";
  sm_attr_double = "attr_double";
  sm_attr_numeric = "attr_numeric";
  sm_attr_string= "attr_string";
  sm_attr_byte_array = "attr_byte_array";
  sm_attr_geometry = "attr_geometry";
  sm_attr_datetime = "attr_datetime";
  sm_attr_array = "attr_array";
  sm_attr_composite = "attr_composite";
  sm_attr_raster = "attr_raster";

// DataSetType names
  sm_newCapabilitiesDt = "public.dt_capabilities";
  sm_dtcloned = "public.dt_cloned";
  sm_newDataSetType2 = "public.dataset_type2";
  sm_newDataSetType3 = "public.dataset_type3";

  sm_oldNameProp = sm_NameAttr8;
  sm_newNameProp = "attribute8_array_of_int_renamed";

// vector of primaryKey Names related with sm_vecDtNames
  sm_vecPkNames.push_back("br_munic_2001_pkey");
  sm_vecPkNames.push_back("batimetria_pkey");
  sm_vecPkNames.push_back("br_focos2003_pkey");
  sm_vecPkNames.push_back("tstcatalog2_pkey");
  sm_vecPkNames.push_back("tstcatalog3_pkey");
  sm_vecPkNames.push_back("attr6_int_pk");

// vector of uniqueKey Names
  sm_vecUkNames.push_back("attr4_and_attr3_uk");

// vector of checkConstraints Names
  sm_vecCcNames.push_back("tstcatalog4_cc");
 
// vector of index Names related with sm_vecDtNames
  sm_vecIdxNames.push_back("br_munic_2001_geom_gist");
  sm_vecIdxNames.push_back("batimetria_geom_gist");
  sm_vecIdxNames.push_back("br_focos2003_geom_gist");
  sm_vecIdxNames.push_back("attr2_fixed_string_idx"); // tstcatalog4 table
  sm_vecIdxNames.push_back("attr6_int_idx");
  sm_vecIdxNames.push_back("attr5_geom_polm_spidx");

// vector of foreignKey Names 
  sm_vecFkNames.push_back("tstcatalog1_fk1");
  sm_vecFkNames.push_back("tstcatalog1_fk2");

// vector of Sequence Names related with sm_vecDtNames
  sm_vecSeqNames.push_back("br_munic_2001_gid_seq");
  sm_vecSeqNames.push_back("batimetria_gid_seq");
  sm_vecSeqNames.push_back("br_focos2003_gid_seq");

// Defining a new dataSetType "sm_newDataSetType" with some properties
  sm_newdt = new te::da::DataSetType(sm_newDataSetType);
  sm_newdt->add(new te::dt::NumericProperty(sm_NameAttr1, 10, 3, true, new std::string(sm_Attr1)));
  sm_newdt->add(new te::dt::StringProperty(sm_NameAttr2, te::dt::FIXED_STRING));
  sm_newdt->add(new te::dt::StringProperty(sm_NameAttr3, te::dt::VAR_STRING, 50, false));
  sm_newdt->add(new te::dt::StringProperty(sm_NameAttr4, te::dt::STRING, 0, true));
  sm_newdt->add(new te::gm::GeometryProperty(sm_NameAttr5, 4326, te::gm::PolygonMType, true));
  sm_newdt->add(new te::dt::SimpleProperty(sm_NameAttr6, te::dt::INT16_TYPE, false, 0));

// Defining checkConstraint over properties m_NameAttr1 (cc name will be recorded as lower case)
  sm_cc = new te::da::CheckConstraint("cc_datasettype2");
  std::string e = sm_NameAttr1 + ">0" ;
  sm_cc->setExpression(e);
 
// Defining a new dataSetType "sm_newDataSetType3" with properties and pk,uk,idx,cc
  sm_newdt3 = new te::da::DataSetType(sm_newDataSetType3);
  sm_newdt3->add(new te::dt::NumericProperty(sm_NameAttr1, 10, 3, true, new std::string(sm_Attr1)));
  sm_newdt3->add(new te::dt::StringProperty(sm_NameAttr2, te::dt::FIXED_STRING));
  sm_newdt3->add(new te::dt::StringProperty(sm_NameAttr3, te::dt::VAR_STRING, 50, false));
  sm_newdt3->add(new te::dt::StringProperty(sm_NameAttr4, te::dt::STRING, 0, true));
  sm_newdt3->add(new te::gm::GeometryProperty(sm_NameAttr5, 4326, te::gm::PolygonMType, true));
  sm_newdt3->add(new te::dt::SimpleProperty(sm_NameAttr6, te::dt::INT16_TYPE, false, 0));

// adding primaryKey
  te::dt::Property* prop = sm_newdt3->getProperty(sm_NameAttr6);
  te::da::PrimaryKey* pk = new te::da::PrimaryKey(sm_Attr6Pk+"3");
  pk->add(prop);
  sm_newdt3->add(pk);

// adding uniqueKey
  te::dt::Property* x = sm_newdt3->getProperty(sm_NameAttr4);
  CPPUNIT_ASSERT(x);
  te::dt::Property* y = sm_newdt3->getProperty(sm_NameAttr3);
  CPPUNIT_ASSERT(y);
  te::da::UniqueKey* uk = new te::da::UniqueKey(sm_Attr3e4Uk+"3");
  uk->add(x);
  uk->add(y);
  sm_newdt3->add(uk);

// adding index using geometry property m_NameAttr5
  te::dt::Property* g = sm_newdt3->getProperty(sm_NameAttr5);
  CPPUNIT_ASSERT(g);
  te::da::Index* idx = new te::da::Index(sm_Attr5idx+"3", te::da::R_TREE_TYPE);
  idx->add(g);
  sm_newdt3->add(idx);

// adding checkConstraint over properties m_NameAttr1 
  te::da::CheckConstraint* cc = new te::da::CheckConstraint("cc_datasetType3");
  std::string ee = sm_NameAttr1 + ">0" ;
  cc->setExpression(ee);
  sm_newdt3->add(cc);

// Defining foreign key (based on sm_NameAttr6 from dtRef=sm_newdt) to be added in sm_newdt3 by tests
  sm_fk  = new te::da::ForeignKey(sm_Attr6Fk);
  te::dt::Property* propfk = sm_newdt->getProperty(sm_NameAttr6) ;
  sm_fk->add(propfk);
  //te::da::DataSetType* dtRef = cl->getDataSetType(m_newDataSetType,true);
  sm_fk->setReferencedDataSetType(sm_newdt);
  sm_fk->addRefProperty(sm_newdt->getProperty(sm_NameAttr6));

// End of Specification of  variables to use in DataSetTypePersistence tests
  printValues(); 
}

void TsManagerPostGIS::finalize()
{
// delete all static pointers created at initialize
  delete sm_datasource;
  delete sm_cc;
  delete sm_newdt;
  delete sm_newdt3;
  delete sm_pol; 
  delete sm_geom;
}

void TsManagerPostGIS::printValues()
{
// showing up DataSet names to be tested
  std::vector<std::string>::iterator it;
  for(it = sm_vecDtNames.begin(); it < sm_vecDtNames.end(); it++)
  {
    std::cout << "DataSets to be tested: " << *it << std::endl;
  }

// vector of DataSets Envelops
  te::gm::Envelope *envBrMunic2001 = new te::gm::Envelope(-73.994430541992202,-33.752063751220703,-28.835882186889599,5.2718071937561000);
  te::gm::Envelope *envBatimetria  = new te::gm::Envelope(622925.37500000000,9744215.0000000000,715161.12500000000,9780345.0000000000);
  te::gm::Envelope *envBrFocos2003 = new te::gm::Envelope(-73.066673278808594,-33.566669464111300,-34.834999084472699,5.0933332443237296);

// sm_vecEnvelops.push_back(*envBrMunic2001);
  sm_vecEnvelops.push_back(*(new te::gm::Envelope(-73.994430541992202,-33.752063751220703,-28.835882186889599,5.2718071937561000)));
  sm_vecEnvelops.push_back(*envBatimetria);
  sm_vecEnvelops.push_back(*envBrFocos2003);


// vector of rectangles to be used spatial filter when retrieving datasets (using intersects or ...)
  te::gm::Envelope *recBrMunic2001 = new te::gm::Envelope(-60.0,-20.0,-35.0,2.0);
  te::gm::Envelope *recBatimetria = new te::gm::Envelope(680000.0,9750000.0,700000.0,9778000.0000000000);
  te::gm::Envelope *recBrFocos2003 = new te::gm::Envelope(-60.0,-20.0,-35.0,2.0);

  sm_recEnvelops.push_back(*recBrMunic2001);
  sm_recEnvelops.push_back(*recBatimetria);
  sm_recEnvelops.push_back(*recBrFocos2003);

// vector of DataSet Size (same order of sm_vecDtNames)
  sm_dtSize.push_back (5562);
  sm_dtSize.push_back (562932);
  sm_dtSize.push_back (46109);

// vector of DataSetRec Size (same order of sm_vecDtNames)
  sm_dtRecSize.push_back (3048);
  sm_dtRecSize.push_back (66991);
  sm_dtRecSize.push_back (31878);

  size_t ss = sm_vecEnvelops.size();

  std::vector<te::gm::Envelope>::iterator itEnv;
  std::vector<size_t>::iterator itSize;

  unsigned int i = 0;
  std::pair<std::string, te::gm::Envelope> dtNameEnvelPair;
  std::pair<std::string, te::gm::Envelope> dtNameRecPair;
  std::pair<std::string, size_t> dtNameSize;
  std::pair<std::string, size_t> dtNameRecSize;

  i = 0;
  it = sm_vecDtNames.begin();
  itEnv = sm_vecEnvelops.begin();
  while ( ( i < ss ) && ( it < sm_vecDtNames.end() ) && ( itEnv < sm_vecEnvelops.end() ) )
  {
    dtNameEnvelPair.first = *it;
    dtNameEnvelPair.second = *itEnv;
    sm_vecDtNamesAndEnvelops.push_back(dtNameEnvelPair);
    
    i++;
    it++;
    itEnv++;
  }

  i = 0;
  it = sm_vecDtNames.begin();
  itEnv = sm_recEnvelops.begin();
  while( ( i < ss ) && ( it < sm_vecDtNames.end() ) && ( itEnv < sm_recEnvelops.end() ) )
  {
    dtNameRecPair.first = *it;
    dtNameRecPair.second = *itEnv;
    sm_vecNamesAndRecs.push_back(dtNameRecPair);
    
    i++;
    it++;
    itEnv++;
  }

  i = 0;
  it = sm_vecDtNames.begin();
  itSize = sm_dtSize.begin();
  while( ( i < ss ) && ( it < sm_vecDtNames.end() ) && ( itSize < sm_dtSize.end() ) )
  {
    dtNameSize.first = *it;
    dtNameSize.second = *itSize;
    sm_vecNamesSizes.push_back(dtNameSize);
    
    i++;
    it++;
    itSize++;
  }

  i = 0;
  it = sm_vecDtNames.begin();
  itSize = sm_dtRecSize.begin();
  while( ( i < ss ) && ( it < sm_vecDtNames.end() ) && ( itSize < sm_dtRecSize.end() ) )
  {
    dtNameRecSize.first = *it;
    dtNameRecSize.second = *itSize;
    sm_vecNamesSizesRec.push_back(dtNameRecSize);
    
    i++;
    it++;
    itSize++;
  }

// showing up DataSet names and their envelops(bounding box)
  std::vector<std::pair<std::string, te::gm::Envelope> >::iterator itpair;  
  for(itpair = sm_vecDtNamesAndEnvelops.begin(); itpair < sm_vecDtNamesAndEnvelops.end(); itpair++)
  {
    std::cout << (*itpair).first << std::endl;
    std::cout << (*itpair).second.m_llx << "," << (*itpair).second.m_lly << "," << (*itpair).second.m_urx << "," << (*itpair).second.m_ury << std::endl;
  }

// showing up DataSet names and the rectangles to be used as a spatial filter when retrieving datasets
  for(itpair = sm_vecNamesAndRecs.begin(); itpair < sm_vecNamesAndRecs.end(); itpair++)
  {
    std::cout << (*itpair).first << std::endl;
    std::cout << (*itpair).second.m_llx << "," << (*itpair).second.m_lly << "," << (*itpair).second.m_urx << "," << (*itpair).second.m_ury << std::endl;
  }

  delete envBrMunic2001;
  delete envBatimetria;
  delete envBrFocos2003;
  delete recBrMunic2001;
  delete recBatimetria;
  delete recBrFocos2003;
}
