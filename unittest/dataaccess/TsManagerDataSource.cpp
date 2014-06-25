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
  \file TsManagerDataSource.cpp
 
  \brief A static class for managing the resources used by Test suit.
 */

// TerraLib
#include <terralib/common.h>
#include <terralib/dataaccess.h>
#include <terralib/datatype.h>
#include <terralib/geometry.h>
//#include <terralib/gdal2.h>
#include <terralib/raster.h>
// Unit-Test TerraLib
#include "TsManagerDataSource.h"
#include "../dataaccess/Config.h"
#include <terralib/dataaccess/datasource/DataSourceFactory.h>  //por algum motivo se nao colocar essa linha pega o antigo.

// Boost
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>

// cppUnit
#include <cppunit/extensions/HelperMacros.h>

// static members defined aand loaded from *.json file for each driver(postgis.json, gdal.josn, mysql.json, etc)

// Datasource type and connection parameters
//std::auto_ptr<te::da::DataSource> TsManagerDataSource::sm_datasource;
te::da::DataSource* TsManagerDataSource::sm_datasource;
std::string TsManagerDataSource::sm_dsType;
std::map<std::string, std::string> TsManagerDataSource::sm_connInfo;
std::map<std::string, std::string> TsManagerDataSource::sm_connInfoExist;

te::da::DataSourceCapabilities  TsManagerDataSource::sm_capabilit;
std::map<std::string, std::string> TsManagerDataSource::sm_connInfoNewDs;
std::map<std::string, std::string> TsManagerDataSource::sm_connInfoDropDs;
std::string TsManagerDataSource::sm_connStr;

// Vector of dataSetTypes names, envelops, rect, size, recsize
std::vector<std::string> TsManagerDataSource::sm_vecDtNames;
std::vector<te::gm::Envelope> TsManagerDataSource::sm_vecEnvelops;
std::vector<te::gm::Envelope> TsManagerDataSource::sm_recEnvelops;
std::vector<size_t>  TsManagerDataSource::sm_dtSize;
std::vector<size_t>  TsManagerDataSource::sm_dtRecSize;

//Pair of values based on vectors above
std::vector<std::pair<std::string, te::gm::Envelope> > TsManagerDataSource::sm_vecDtNamesAndEnvelops;
std::vector<std::pair<std::string, te::gm::Envelope> > TsManagerDataSource::sm_vecNamesAndRecs;
std::vector<std::pair<std::string, size_t> > TsManagerDataSource::sm_vecNamesSizes;
std::vector<std::pair<std::string, size_t> > TsManagerDataSource::sm_vecNamesSizesRec;

// DataSetType names
std::string TsManagerDataSource::sm_newDataSetType; //used in loadCatalog and Persistence
std::string TsManagerDataSource::sm_newCapabilitiesDt; //based on DataSource Capabilities
std::string TsManagerDataSource::sm_dtcloned; //cloned
std::string TsManagerDataSource::sm_newDataSetType2; //used to rename cloned one
std::string TsManagerDataSource::sm_newDataSetType3; //used to create dt with pk,uk,idx, etc.

size_t TsManagerDataSource::sm_nroDataSets;

// Names of primary key, Unique key, Index, Foreign Key, Sequences and Check Constraints
std::vector<std::string> TsManagerDataSource::sm_vecPkNames;
std::vector<std::string> TsManagerDataSource::sm_vecUkNames;
std::vector<std::string> TsManagerDataSource::sm_vecIdxNames;
std::vector<std::string> TsManagerDataSource::sm_vecFkNames;
std::vector<std::string> TsManagerDataSource::sm_vecSeqNames;
std::vector<std::string> TsManagerDataSource::sm_vecCcNames;

// Defining some geometries valid for the first dataSet in sm_vecDtNames (postgis: "br_munic_2001" and city "Ouro Preto")
//TODO:Agora s� tem dado para o primeiro dt=br_munic_2001 , mas pode ter mais e o teste varrer os datasets...
te::gm::Envelope    TsManagerDataSource::sm_box; //(-43.9329795837402, -20.6328010559082, -43.4036407470703, -20.1612071990967);
te::gm::Point      TsManagerDataSource::sm_pt; //(4291, 0, -43.6107606714293, -20.3913548070123);
te::gm::Polygon*    TsManagerDataSource::sm_pol = new te::gm::Polygon(0, te::gm::PolygonType);
te::gm::Geometry*   TsManagerDataSource::sm_geom;

// Used in DataSetTypePersistence
te::da::DataSetType* TsManagerDataSource::sm_newdt;
te::da::DataSetType* TsManagerDataSource::sm_newdt3;
te::da::ForeignKey* TsManagerDataSource::sm_fk;

std::string TsManagerDataSource::sm_ccName; //check constrain name
te::da::CheckConstraint* TsManagerDataSource::sm_cc;

// Defining specific variables to test DataSetTypePersistence... 

// Attribute names
std::string TsManagerDataSource::sm_NameAttr1;
std::string TsManagerDataSource::sm_Attr1;
std::string TsManagerDataSource::sm_NameAttr2; 
std::string TsManagerDataSource::sm_NameAttr3; 
std::string TsManagerDataSource::sm_NameAttr4; 
std::string TsManagerDataSource::sm_NameAttr5; 
std::string TsManagerDataSource::sm_NameAttr6; 
std::string TsManagerDataSource::sm_NameAttr7; 
std::string TsManagerDataSource::sm_NameAttr8;

std::string TsManagerDataSource::sm_Attr6Pk;
std::string TsManagerDataSource::sm_Attr6Fk;
std::string TsManagerDataSource::sm_Attr3e4Uk;
std::string TsManagerDataSource::sm_Attr5idx; 
std::string TsManagerDataSource::sm_Attr2idx; 
std::string TsManagerDataSource::sm_Attr7_seq;

std::string TsManagerDataSource::sm_oldNameProp; 
std::string TsManagerDataSource::sm_newNameProp;

// End of Specification of variables to be used in TsDataSetTypePersistence

void TsManagerDataSource::ConvertVectorToEnvelope(std::vector<std::vector<double> >& vectd, std::vector<te::gm::Envelope>& vecte)
{
    std::vector<std::vector<double> >::iterator it;
    te::gm::Envelope envi;
    std::vector<double> vecti;
    for(it = vectd.begin(); it < vectd.end(); it++)
    {
      vecti = (*it);
      te::gm::Envelope envi(vecti);
      vecte.push_back(envi);
    }
}

void TsManagerDataSource::ConvertVectorToPoints(std::vector<std::vector<double> >& vectd, std::vector<te::gm::Point>& vectp)
{
    std::vector<std::vector<double> >::iterator it;
    std::vector<double>::iterator it1;
    te::gm::Point pt;
    double  d[3]={0.0,0.0,0.0};int i=0;
    std::vector<double> vecti;
    for(it = vectd.begin(); it < vectd.end(); it++)
    {
      vecti = (*it);
      for(it1 = vecti.begin(); it1 < vecti.end(); it1++)
      {
         d[i] = (*it1);       
         i+= 1;
      }      
      pt.setSRID((int)d[0]);
      pt.setX(d[1]);
      pt.setY(d[2]);
      vectp.push_back(pt);
    }
}

void TsManagerDataSource::ConvertVectorToLinearRing(std::vector<std::vector<double> >& vectd, te::gm::LinearRing& lr)
{
    std::vector<std::vector<double> >::iterator it;
    std::vector<double>::iterator it1;
    te::gm::Point pt;
    double  d[3]={0.0,0.0,0.0};int i=0; int p =0;
    std::vector<double> vecti;
    for(it = vectd.begin(); it < vectd.end(); it++)
    {
      vecti = (*it); i=0;
      for(it1 = vecti.begin(); it1 < vecti.end(); it1++)
      {
         d[i] = (*it1);       
         i+= 1;
      }
      lr.setPoint(p,d[1],d[2]);
      p+= 1;
    }
    lr.setSRID((int)d[0]);
}

void TsManagerDataSource::ConvertToPropType(std::string s, te::dt::StringType &t)
{
  if (s == "te::dt::FIXED_STRING")
    t = te::dt::FIXED_STRING;
  else if (s == "te::dt::VAR_STRING")
    t = te::dt::VAR_STRING;
  else if (s == "te::dt::STRING")
    t = te::dt::STRING;
  else if (s == "te::dt::STRING")
    t = te::dt::STRING;
}
void TsManagerDataSource::ConvertToPropType(std::string s, int &t)
{
  if (s == "te::dt::INT16_TYPE")
    t = te::dt::INT16_TYPE;
  else if (s == "te::gm::PolygonMType")
    t = te::gm::PolygonMType; // te::gm::GeomType t
}
/* TODO:
  switch(s)
  {
    case "te::dt::CHAR_TYPE":
      t = te::dt::CHAR_TYPE;
    break;
    case "te::dt::UCHAR_TYPE":
      t = te::dt::UCHAR_TYPE;
    break;
    case "te::dt::INT16_TYPE":
      t = te::dt::INT16_TYPE;
    break;
    case "te::dt::INT32_TYPE":
      t = te::dt::INT32_TYPE;
    break;
    case "te::dt::INT64_TYPE":
      t = te::dt::INT64_TYPE;
     break; 
    case "te::dt::NUMERIC_TYPE":
      t = te::dt::NUMERIC_TYPE;
    break;
    case "te::dt::te::dt::DATE":
      t = te::dt::DATE;
    break;
   case "te::dt::TIME_DURATION":
      t = te::dt::TIME_DURATION;
    break;     
   case "te::dt::TIME_INSTANT":
      t = te::dt::TIME_INSTANT;
    break;     
    case "te::dt::TIME_INSTANT_TZ":
      t = te::dt::TIME_INSTANT_TZ;
    break;    
    case "te::dt::FLOAT_TYPE":
      t = te::dt::FLOAT_TYPE;
    break;
    case "te::dt::DOUBLE_TYPE":
      t = te::dt::DOUBLE_TYPE;
    break;
    case "te::dt::FIXED_STRING":
      t = te::dt::FIXED_STRING;
    break;
    case "te::dt::VAR_STRING":
      t = te::dt::VAR_STRING;
    break;
    case "te::dt::STRING_TYPE":
      t = te::dt::STRING_TYPE;
    break;
    case "te::dt::BOOLEAN_TYPE":
      t = te::dt::BOOLEAN_TYPE;
    break;
    case "te::dt::BYTE_ARRAY_TYPE":
      t = te::dt::BYTE_ARRAY_TYPE;
    break;
    case "te::dt::GEOMETRY_TYPE":
       t = te::dt::GEOMETRY_TYPE;
    break;
    case "te::dt::ARRAY_TYPE":
      t = te::dt::ARRAY_TYPE;
    break;
    default:
      t = te::dt::STRING_TYPE;
    break;
  } 
*/

void TsManagerDataSource::initialize(const std::string driver_name)
{
//*** Initializing datasource drivers - all available in drivers.json ***//

// the static variables sm_* for each datasource will be read from  a *.json file

  boost::property_tree::ptree driver;

  boost::property_tree::read_json(driver_name, driver);

  sm_dsType = driver.get_child("ds.ds_type").data();

  te::common::Convert(driver.get_child("ds.ds_connInfo"), sm_connInfo);
  te::common::Convert(driver.get_child("ds.ds_connInfoExist"), sm_connInfoExist);
  te::common::Convert(driver.get_child("ds.ds_connInfoNewDs"), sm_connInfoNewDs);
  te::common::Convert(driver.get_child("ds.ds_connInfoDropDs"), sm_connInfoDropDs);
  sm_connStr = driver.get_child("ds.ds_connStr").data();

  std::string sss = driver.get_value("ds.ds_connStr");

  // Reading dataSetType names that already exist in DataSource
  te::common::Convert(driver.get_child("dt.dt_Names"), sm_vecDtNames);

  //Reading dataSetType names that will be created for some Persistence test
  sm_newDataSetType = driver.get_child("dt_new").data();
  sm_newCapabilitiesDt = driver.get_child("dt_newCapabilities").data();
  sm_dtcloned = driver.get_child("dt_clonedName").data();
  sm_newDataSetType2 = driver.get_child("dt_newDt").data();
  sm_newDataSetType3 = driver.get_child("dt_newDtWithPk").data();
  
  sm_Attr1     = "43.78";
  sm_newdt = new te::da::DataSetType(sm_newDataSetType);
  sm_newdt3 = new te::da::DataSetType(sm_newDataSetType3);

  if (sm_dsType == "GDAL")
  {
    te::rst::Grid* grid = new te::rst::Grid(100,100);
    grid->setGeoreference(te::gm::Coord2D(100,100), 29183, 1,1);
    std::vector<te::rst::BandProperty*> bprops;
    bprops.push_back(new te::rst::BandProperty(0, te::dt::UCHAR_TYPE));
    std::map<std::string, std::string> rinfo1;

    te::rst::RasterProperty* rstp = new te::rst::RasterProperty(grid, bprops, rinfo1);
    sm_newdt->add(rstp);

    te::rst::Grid* grid3 = new te::rst::Grid(100,100);
    grid3->setGeoreference(te::gm::Coord2D(100,100), 29183, 1,1);
    std::vector<te::rst::BandProperty*> bprops3;
    bprops3.push_back(new te::rst::BandProperty(0, te::dt::UCHAR_TYPE));
    std::map<std::string, std::string> rinfo3;
    te::rst::RasterProperty* rstp3 = new te::rst::RasterProperty(grid3, bprops3, rinfo3);
    sm_newdt3->add(rstp3);
  }
  else
  { //to other drivers...

    //Reading-envelopes (rec) that will be used as FILTER when retrieving datasets(using intersects)
    std::vector<std::vector<double> > vect_res;
    te::common::Convert(driver.get_child("dt.dt_envelop"), vect_res);
    ConvertVectorToEnvelope(vect_res,sm_vecEnvelops);

    //teste marisa
    std::vector<std::vector<double> > vect_res3;
    std::vector<te::gm::Envelope> vecEnv3;
    te::common::Convert(driver.get_child("dt.dt_envelop3.br_munic_2001"), vect_res3);
    ConvertVectorToEnvelope(vect_res3,vecEnv3);
    // testefim

    std::vector<std::vector<double> > vect_rec; 
    te::common::Convert(driver.get_child("dt.dt_rect"), vect_rec);
    ConvertVectorToEnvelope(vect_rec,sm_recEnvelops);

    //Used in CatalogLoader of each driver defined in drivers.json
    te::common::Convert(driver.get_child("dt.dt_size_envelop"), sm_dtSize);
    te::common::Convert(driver.get_child("dt.dt_size_rec"), sm_dtRecSize);

    te::common::Convert(driver.get_child("dt.dt_pk_names"), sm_vecPkNames);
    te::common::Convert(driver.get_child("dt.dt_idx_names"), sm_vecIdxNames);
    te::common::Convert(driver.get_child("dt.dt_seq_names"), sm_vecSeqNames);
    te::common::Convert(driver.get_child("dt.dt_fk_names"), sm_vecFkNames);
    te::common::Convert(driver.get_child("dt.dt_uk_names"), sm_vecUkNames);

    te::common::Convert(driver.get_child("dt.dt_cconstrains_names"), sm_vecCcNames);
   
    setPairsValues(); //Setting some pairs of values based on related vetors above
    //Reading-envelopes (rec) - end 
   
    //Reading-geometries used as spatial filter 
    //Now it is only defined for the first dataSetType defined in the "dt.dt_Names" 
    //but the tests could change to recevive a vector of pts,env, pol, geom

    //reading srid of the geometries used as filters
    std::string srid_s = driver.get_child("geom.geom_srid").data();
    int geom_srid = boost::lexical_cast<int>(srid_s);
    //reading points
    std::vector<std::vector<double> > vect_pts;
    std::vector<te::gm::Point> vect_pts1;
    te::common::Convert(driver.get_child("geom.geom_pts"), vect_pts);

    std::vector<std::vector<double> > vect_envboxs;
    std::vector<te::gm::Envelope> vect_boxEnvel; //sm_box
    std::vector<std::vector<double> > vect_lines;
    std::vector<std::vector<double> > vect_pols;
    //TODO:Mudar para vetor os sm_pt, sm_box, sm_pol, sm_linearRing nos programas de teste...

    //reading-box
    te::common::Convert(driver.get_child("geom.geom_boxs"), vect_envboxs);
    ConvertVectorToEnvelope(vect_envboxs,vect_boxEnvel); // TODO sm_box mudar para vector

    if (vect_boxEnvel.size() > 0)
    {
    std::vector<te::gm::Envelope>::iterator it = vect_boxEnvel.begin();
    sm_box = (*it);
    ConvertVectorToPoints(vect_pts,vect_pts1);
    std::vector<te::gm::Point>::iterator it1 = vect_pts1.begin();
    sm_pt = (*it1);
    }
    sm_pt.setSRID(geom_srid);

    // reading-polgygon (points that will compose the sm_pol geometry (polygon) used in getDataSet)
    std::vector<std::vector<double> > vect_ptsx;
    te::common::Convert(driver.get_child("geom.geom_linearRings"), vect_ptsx);    
    if (vect_ptsx.size() >0)
    {
    te::gm::LinearRing *sm_linearRing1= new te::gm::LinearRing(vect_ptsx.size(),te::gm::LineStringType);
    ConvertVectorToLinearRing(vect_ptsx, *sm_linearRing1);
	  
    size_t lr_size = sm_linearRing1->size();
    std::size_t i;
    std::vector<te::gm::Point> vect_pis;
    te::gm::Point pi;

    for(i = 0; i < lr_size; i++)
    {
	    pi.setX(sm_linearRing1->getPointN(i)->getX())  ;
	    pi.setY(sm_linearRing1->getPointN(i)->getY())  ;   
	    vect_pis.push_back(pi);
    }

    sm_pol->push_back(sm_linearRing1);
    sm_pol->setSRID(geom_srid);
    }
	  te::common::Convert(driver.get_child("geom.geom_pols"), vect_pols);

	  //adding  numeric properties (it could be read from .json in a vector of numeric properties)
    sm_newdt->add(new te::dt::NumericProperty("attribute1_numeric", 10, 3, true, new std::string(sm_Attr1)));
    sm_newdt3->add(new te::dt::NumericProperty("attribute1_numeric", 10, 3, true, new std::string(sm_Attr1)));

    //reading string properties and adding to dataSetTypes
    std::vector<std::map<std::string, std::string> > vectProp;
    te::common::Convert(driver.get_child("dt_new_attr.dt_att_str"), vectProp);    

    std::vector<std::map<std::string, std::string> >::iterator itvectmap;
    std::map<std::string, std::string> atti;
    for(itvectmap = vectProp.begin(); itvectmap < vectProp.end(); itvectmap++)
    {
      atti = (*itvectmap);
      std::string propname= atti["name"];
      std::string proptypeM = atti["strtype"];
      te::dt::StringType propType;
      ConvertToPropType(proptypeM, propType);
      std::string prop_size = atti["strsize"];
      int propsizeI = boost::lexical_cast<int>(prop_size);
      sm_newdt->add(new te::dt::StringProperty(propname, propType,propsizeI));
      sm_newdt3->add(new te::dt::StringProperty(propname, propType,propsizeI));
    }

	  //reading some specific names used as pk,uk,Idx,check Constraints
    sm_NameAttr6 = driver.get_child("dt_propPkName").data();
    sm_Attr6Pk = driver.get_child("dt_PkName").data();

    sm_NameAttr3 = driver.get_child("dt_propUkName1").data();
    sm_NameAttr4 = driver.get_child("dt_propUkName2").data();
    sm_Attr3e4Uk = driver.get_child("dt_UkName").data();

    sm_NameAttr5 = driver.get_child("dt_propIdxGeom").data();
    sm_Attr5idx = driver.get_child("dt_IdxGeomName").data();

    sm_NameAttr2 = driver.get_child("dt_propIdxName").data();
    sm_Attr2idx = driver.get_child("dt_IdxName").data();

    sm_NameAttr1 = driver.get_child("dt_propCcName").data();
    sm_ccName = driver.get_child("dt_CheckCcName").data();

    //sm_NameAttr6 = driver.get_child("dt_propFkName").data();
    sm_Attr6Fk = driver.get_child("dt_FkName").data();
    sm_NameAttr7 = driver.get_child("dt_propNameToChange").data();

    sm_NameAttr8 = driver.get_child("dt_propOldName").data();
    sm_oldNameProp = driver.get_child("dt_propOldName").data();
    sm_newNameProp = driver.get_child("dt_propNewName").data();
 
    //initializing  something to be used in persistence tests
    sm_newdt->add(new te::gm::GeometryProperty(sm_NameAttr5, 4326, te::gm::PolygonMType, true));
    sm_newdt->add(new te::dt::SimpleProperty(sm_NameAttr6, te::dt::INT16_TYPE, false, 0));
    sm_newdt3->add(new te::gm::GeometryProperty(sm_NameAttr5, 4326, te::gm::PolygonMType, true));
    sm_newdt3->add(new te::dt::SimpleProperty(sm_NameAttr6, te::dt::INT16_TYPE, false, 0));

    //Defining checkConstraint over properties m_NameAttr1 (cc name will be recorded as lower case)
    sm_cc = new te::da::CheckConstraint(sm_ccName); // era  cc_datasettype2
    std::string e = sm_NameAttr1 + ">0" ;
    sm_cc->setExpression(e);

    //adding primaryKey
    te::dt::Property* prop = sm_newdt3->getProperty(sm_NameAttr6);
    te::da::PrimaryKey* pk = new te::da::PrimaryKey(sm_Attr6Pk+"3");
    pk->add(prop);
    sm_newdt3->add(pk);

    //adding uniqueKey
    te::dt::Property* x = sm_newdt3->getProperty(sm_NameAttr4);
    CPPUNIT_ASSERT(x);
    te::dt::Property* y = sm_newdt3->getProperty(sm_NameAttr3);
    CPPUNIT_ASSERT(y);
    te::da::UniqueKey* uk = new te::da::UniqueKey(sm_Attr3e4Uk+"3");
    uk->add(x);
    uk->add(y);
    sm_newdt3->add(uk);

    //adding index using geometry property m_NameAttr5
    te::dt::Property* g = sm_newdt3->getProperty(sm_NameAttr5);
    CPPUNIT_ASSERT(g);
    te::da::Index* idx = new te::da::Index(sm_Attr5idx+"3", te::da::R_TREE_TYPE);
    idx->add(g);
    sm_newdt3->add(idx);

    //adding checkConstraint over properties m_NameAttr1 
    te::da::CheckConstraint* cc = new te::da::CheckConstraint(sm_ccName+"3"); //"cc_datasetType3"
    std::string ee = sm_NameAttr1 + ">0" ;
    cc->setExpression(ee);
    sm_newdt3->add(cc);

    //Defining foreign key (based on sm_NameAttr6 from dtRef=sm_newdt) to be added in sm_newdt3 by tests
    sm_fk  = new te::da::ForeignKey(sm_Attr6Fk);
    te::dt::Property* propfk = sm_newdt->getProperty(sm_NameAttr6) ;
    sm_fk->add(propfk);
    //te::da::DataSetType* dtRef = cl->getDataSetType(m_newDataSetType,true);
    sm_fk->setReferencedDataSetType(sm_newdt);
    sm_fk->addRefProperty(sm_newdt->getProperty(sm_NameAttr6));
  }//postgis and other drivers

  // End of Specification of variables to be used in DataSetTypePersistence tests

    sm_datasource = (te::da::DataSourceFactory::make(sm_dsType)).release();
//  sm_datasource = te::da::DataSourceFactory::make(sm_dsType);
  //sm_datasource = te::da::DataSourceFactory::make(sm_dsType);
  sm_datasource->setConnectionInfo(sm_connInfo);

  sm_capabilit = sm_datasource->getCapabilities();
}

void TsManagerDataSource::finalize()
{
// delete all static pointers created at TsManagerDataSource::initialize
  //delete sm_datasource;
  sm_dsType = "";
  sm_connInfo.clear();
  sm_connInfoExist.clear();
  sm_connInfoNewDs.clear();
  sm_connInfoDropDs.clear();

  sm_vecDtNames.clear();
  sm_vecEnvelops.clear();
  sm_recEnvelops.clear();
  sm_dtSize.clear();
  sm_dtRecSize.clear();
  sm_recEnvelops.clear();

  sm_vecPkNames.clear();
  sm_vecUkNames.clear();
  sm_vecIdxNames.clear();
  sm_vecFkNames.clear();
  sm_vecSeqNames.clear();
  sm_vecCcNames.clear();

  sm_vecDtNamesAndEnvelops.clear();
  sm_vecNamesAndRecs.clear();
  sm_vecNamesSizes.clear();
  sm_vecNamesSizesRec.clear();

  sm_pol->clear(); 
  delete sm_geom;
}

void TsManagerDataSource::setPairsValues ()
{
// showing up DataSet names to be tested
  std::vector<std::string>::iterator it;
  for(it = sm_vecDtNames.begin(); it < sm_vecDtNames.end(); it++)
  {
    std::cout << "DataSets to be tested: " << *it << std::endl;
  }
/*  
//All of this are defined in posgtis.json or gdal.json, or other drive 

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
*/
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
}
