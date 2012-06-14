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
  \file TsManagerGdal.cpp
 
  \brief A static class for managing the resources used by Gdal test suit.
 */

// TerraLib
#include <terralib/common.h>
#include <terralib/dataaccess.h>
#include <terralib/geometry.h>
#include <terralib/gdal.h>
#include <terralib/raster.h>

//#include "Config.h"

// Unit-Test TerraLib
#include "TsManagerGdal.h"
#include "../dataaccess/Config.h"


// cppUnit
#include <cppunit/extensions/HelperMacros.h>

// static members defined in TsManagerGdal
te::da::DataSource* TsManagerGdal::sm_datasource;
std::map<std::string, std::string> TsManagerGdal::sm_capabilit;
te::da::DataSetType* TsManagerGdal::sm_newdt;
te::da::DataSetType* TsManagerGdal::sm_newdt3;
std::map<std::string, std::string> TsManagerGdal::sm_connInfo;
std::map<std::string, std::string> TsManagerGdal::sm_connInfoNewDs;
std::map<std::string, std::string> TsManagerGdal::sm_connInfoDropDs;
std::string TsManagerGdal::sm_newDataSetType;
std::string TsManagerGdal::sm_dsType;
std::string TsManagerGdal::sm_description;
std::string TsManagerGdal::sm_displayName; 
std::string TsManagerGdal::sm_newDSourceStr;

std::string TsManagerGdal::sm_connInfoStr;

// DataSetType names used 
std::string TsManagerGdal::sm_newCapabilitiesDt;
std::string TsManagerGdal::sm_newDataSetType2;
std::string TsManagerGdal::sm_newDataSetType3;
std::string TsManagerGdal::sm_dtcloned;

std::string TsManagerGdal::sm_oldNameProp;
std::string TsManagerGdal::sm_newNameProp;

std::vector<std::string> TsManagerGdal::sm_vecDtNames;
std::vector<te::gm::Envelope> TsManagerGdal::sm_vecEnvelops;
std::vector<te::gm::Envelope> TsManagerGdal::sm_recEnvelops;
std::vector<size_t>  TsManagerGdal::sm_dtSize;
std::vector<size_t>  TsManagerGdal::sm_dtRecSize;

std::vector<std::pair<std::string, te::gm::Envelope> > TsManagerGdal::sm_vecDtNamesAndEnvelops;
std::vector<std::pair<std::string, te::gm::Envelope> > TsManagerGdal::sm_vecNamesAndRecs;
std::vector<std::pair<std::string, size_t> > TsManagerGdal::sm_vecNamesSizes;
std::vector<std::pair<std::string, size_t> > TsManagerGdal::sm_vecNamesSizesRec;

// Defining some geometries to be used by the queries 
te::gm::Envelope    TsManagerGdal::sm_box; 
te::gm::Point       TsManagerGdal::sm_pt; 
te::gm::LinearRing* TsManagerGdal::sm_linearRing = new te::gm::LinearRing(6, te::gm::LineStringType);
te::gm::Polygon*    TsManagerGdal::sm_pol = new te::gm::Polygon(0, te::gm::PolygonType);
te::gm::Geometry*   TsManagerGdal::sm_geom;

size_t TsManagerGdal::sm_nroDataSets;

//end of static members

void TsManagerGdal::initialize()
{
  //************ Initializing datasource driver - GDAL **********//

// let's get GDAL data source instance using the DataSourceFactory  
  sm_datasource = te::da::DataSourceFactory::make("GDAL");
  sm_datasource->getCapabilities(sm_capabilit);

// let´s give the minimal datasource connection information used to connect
  sm_connInfo["URI"] = ""TE_DATA_LOCALE"\\data\\rasters" ;
 
// Other static memory variables used to check datasource recovered data
  sm_dsType = te::gdal::Platform::sm_driverIdentifier;

// Minimal datasource connection information used to Create a new GDAL 
  sm_connInfoNewDs["URI"] = ""TE_DATA_LOCALE"\\data\\rasters\\dataNew" ;  

// Minimal datasource connection information used to Drop a GDAL 
  sm_connInfoDropDs["URI"] = ""TE_DATA_LOCALE"\\data\\rasters\\dataNew" ;  

// Specific variables to use in DataSetTypePersistence tests
  sm_newDataSetType = "teste_created.tif";
  sm_newdt = new te::da::DataSetType(sm_newDataSetType);

  te::rst::Grid* grid = new te::rst::Grid(100,100);
  grid->setGeoreference(te::gm::Coord2D(100,100), 29183, 1,1);
  std::vector<te::rst::BandProperty*> bprops;
  bprops.push_back(new te::rst::BandProperty(0, te::dt::UCHAR_TYPE));
  std::map<std::string, std::string> rinfo1;

  te::rst::RasterProperty* rstp = new te::rst::RasterProperty(grid, bprops, rinfo1);
  sm_newdt->add(rstp);


  sm_newCapabilitiesDt = "teste_capabilities.tif";
  sm_dtcloned = "teste_cloned.tif";
  sm_newDataSetType2 = "teste_cloned2.tif";

  sm_oldNameProp = "teste_created.tif"; //"teste_raster_prop.tif";
  sm_newNameProp = "teste_rename_prop.tif";

  // As there is no support for pk,uk,idx in GDAL, only create a new dataset with this name 
  sm_newDataSetType3 = "teste_created3.tif";
  sm_newdt3 = new te::da::DataSetType(sm_newDataSetType3);
  te::rst::Grid* grid3 = new te::rst::Grid(100,100);
  grid3->setGeoreference(te::gm::Coord2D(100,100), 29183, 1,1);
  std::vector<te::rst::BandProperty*> bprops3;
  bprops3.push_back(new te::rst::BandProperty(0, te::dt::UCHAR_TYPE));
  std::map<std::string, std::string> rinfo3;
  te::rst::RasterProperty* rstp3 = new te::rst::RasterProperty(grid3, bprops3, rinfo3);
  sm_newdt3->add(rstp3);

// Server connection information used to check get methods after connection
  std::string sm_dsInfoChkStr("SOURCE="TE_DATA_LOCALE"/data/rasters/cbers2b_rgb342_crop.tif");


// Defining sm_* to check DataSourceCatalog in test cases...
  sm_nroDataSets = 1;

// vector of DataSet Names
  sm_vecDtNames.push_back("cbers2b_rgb342_crop.tif");

// vector of DataSets Envelops
  //EPSG:29181
  te::gm::Envelope *envCbers2b = new te::gm::Envelope(770596.8125,7363093.0,777976.8125,7370113.0);
  sm_vecEnvelops.push_back(*envCbers2b);

// vector of rectangles to be used in spatial filter when retrieving datasets (using intersects or ...)
  te::gm::Envelope *recCbers2b = new te::gm::Envelope(770596.8125,7363093.0,777976.8125,7370113.0); 
  sm_recEnvelops.push_back(*recCbers2b);

// vector of DataSet Size (same order of sm_vecDtNames)
  sm_dtSize.push_back (1);

// vector of DataSetRec Size (same order of sm_vecDtNames)
  sm_dtRecSize.push_back (1);

  size_t ss = sm_vecEnvelops.size();

  std::vector<te::gm::Envelope>::iterator itEnv;
  std::vector<size_t>::iterator itSize;

  size_t i;
  std::pair<std::string, te::gm::Envelope> dtNameEnvelPair;
  std::pair<std::string, te::gm::Envelope> dtNameRecPair;
  std::pair<std::string, size_t> dtNameSize;
  std::pair<std::string, size_t> dtNameRecSize;

  std::vector<std::string>::iterator it;
  for (i = 0,it = sm_vecDtNames.begin(),itEnv = sm_vecEnvelops.begin(); i < ss ,it < sm_vecDtNames.end(), itEnv < sm_vecEnvelops.end()  ; i++, it++ , itEnv++ )
  {
    dtNameEnvelPair.first = *it;
    dtNameEnvelPair.second = *itEnv;
    sm_vecDtNamesAndEnvelops.push_back(dtNameEnvelPair);
  }

  for (i = 0,it = sm_vecDtNames.begin(),itEnv = sm_recEnvelops.begin(); i < ss ,it < sm_vecDtNames.end(), itEnv < sm_recEnvelops.end()  ; i++, it++ , itEnv++ )
  {
    dtNameRecPair.first = *it;
    dtNameRecPair.second = *itEnv;
    sm_vecNamesAndRecs.push_back(dtNameRecPair);
  }

  for (i = 0,it = sm_vecDtNames.begin(),itSize = sm_dtSize.begin(); i < ss ,it < sm_vecDtNames.end(), itSize < sm_dtSize.end()  ; i++, it++ , itSize++ )
  {
    dtNameSize.first = *it;
    dtNameSize.second = *itSize;
    sm_vecNamesSizes.push_back(dtNameSize);
  }

  for (i = 0,it = sm_vecDtNames.begin(),itSize = sm_dtRecSize.begin(); i < ss ,it < sm_vecDtNames.end(), itSize < sm_dtRecSize.end()  ; i++, it++ , itSize++ )
  {
    dtNameRecSize.first = *it;
    dtNameRecSize.second = *itSize;
    sm_vecNamesSizesRec.push_back(dtNameRecSize);
  }

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


// End of definition of geometries

  delete envCbers2b;
  delete recCbers2b;
}

void TsManagerGdal::finalize()
{
// deletar all static pointers created at initialize
  delete sm_datasource; 

  //delete sm_linearRing; it is deleted in sm_pol
  delete sm_pol; 
  delete sm_geom;
}

void TsManagerGdal::printValues()
{
// showing up DataSet names to be tested
  std::vector<std::string>::iterator it;
  for(it = sm_vecDtNames.begin(); it < sm_vecDtNames.end(); it++)
  {
    std::cout << std::endl << "DataSets to be tested " << *it << std::endl;
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