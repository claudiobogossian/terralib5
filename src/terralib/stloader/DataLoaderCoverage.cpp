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
  \file DataLoaderCoverage.cpp

  \brief This file contains a function that loads a coverage from a set of raster files. 
*/
//Terralib
#include "../dataaccess/dataset/DataSet.h"
#include "../dataaccess/datasource/DataSource.h"
#include "../dataaccess/datasource/DataSourceTransactor.h"
#include "../raster/Raster.h"
#include "../datatype/TimeInstant.h"

//ST
#include "DataLoader.h"
#include "Utils.h"
#include "../st/coverage/Coverage.h"
#include "../st/coverage/RasterCoverage.h"

//XML
#include "../xml/Reader.h"
#include "../xml/ReaderFactory.h"

//BOOST
#include <boost/filesystem/path.hpp>

struct CoverageObsInfo
{
  std::string m_filePath;
  std::string m_fileName;
  std::string m_timeInstant; 
};

void te::stloader::DataLoader::loadCoverage(te::da::DataSource* ds, 
                        const std::string& metadata, te::st::Coverage** output)
{
  //====== Read XML metadata files
  te::xml::Reader* xmlReader = te::xml::ReaderFactory::make();      
  xmlReader->setValidationScheme(false);  
  xmlReader->read(metadata);

  if(!xmlReader->next())
  {
    delete xmlReader;
    return;
  }
    
  if(!GetNextStartNode(xmlReader, "CoverageInfo"))
  {
    delete xmlReader;
    return;
  }  
    
  //get each coverage observation
  std::vector<CoverageObsInfo> cvInfo;
  while(GetNextStartNode(xmlReader, "CoverageObservation"))
  {
    CoverageObsInfo info; 
      
    //get the file path  
    if(!GetNextValueNode(xmlReader, "FilePath", info.m_filePath))
    {
      delete xmlReader;
      return;
    }

    if(info.m_filePath==".")
    {
      boost::filesystem::path aux(metadata);
      info.m_filePath = aux.remove_filename().string();
    } 

    //get the file name 
    if(!GetNextValueNode(xmlReader, "FileName", info.m_fileName))
    {
      delete xmlReader;
      return;
    }

    //get the time instant 
    if(!GetNextValueNode(xmlReader, "TimeInstant", info.m_timeInstant))
    {
      delete xmlReader;
      return;
    }    

    cvInfo.push_back(info);
  }
  delete xmlReader;
   
  //====== Read raster files, using GDAL driver
  *output = new te::st::RasterCoverage();
  for(unsigned int i = 0; i <  cvInfo.size(); ++i)
  {
    std::map<std::string, std::string> connInfo;
    connInfo["URI"] = cvInfo[i].m_filePath;    
    
    ds->open(connInfo);
    te::da::DataSourceTransactor* t = ds->getTransactor();
    
    //it returns only a data set
    te::da::DataSet* cvDataSet = t->getDataSet(cvInfo[i].m_fileName, te::common::FORWARDONLY, te::common::RWAccess);
    if(cvDataSet == 0)
    {
      delete t;
      return;
    } 

    //in this case, the data set has only a column of raster data type
    //TO DO:  the structure CoverageDataSetMetadata in order to get coverage observations
    //        from raster files as well as DBMS
    //TO DO:  no caso de coverage a partir de raster files:
    //        (1) o tempo não está no dataset (como no moving object), ele vem do metadado
    //        (2) uma coverage é formado por n data sets.
    int rasterIdx = 0;    

    cvDataSet->loadTypeInfo();
    
    //raster data
    te::rst::Raster* raster = cvDataSet->getRaster(rasterIdx);
    
    //time data
    te::dt::DateTime* timeInstant = new te::dt::TimeInstant(cvInfo[i].m_timeInstant);

    dynamic_cast<te::st::RasterCoverage*>(*output)->addObservation(timeInstant, raster);
    
    delete cvDataSet;
    delete t;
    ds->close();
  }
  
  return;
}



