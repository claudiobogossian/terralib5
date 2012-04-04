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
  \file DataLoaderMovingObjects.cpp

  \brief This file contains a function that loads moving objects from KML file. 
*/
//Terralib
#include "../dataaccess/dataset/DataSet.h"
#include "../dataaccess/datasource/DataSource.h"
#include "../dataaccess/datasource/DataSourceTransactor.h"

//ST
#include "DataLoader.h"
#include "Utils.h"
#include "../st/movingobject/MovingObject.h"
#include "MovingObjectDataSetMetadata.h"

//XML
#include "../xml/Reader.h"
#include "../xml/ReaderFactory.h"

//BOOST
#include <boost/filesystem/path.hpp>

struct MovingObjectInfo
{
  std::string m_id;
  std::string m_type;
  std::string m_containerType;
  std::string m_containerName; 
};

void te::stloader::DataLoader::loadMovingObjects(te::da::DataSource* ds, 
                        const std::string& metadata, 
                        std::vector<te::st::MovingObject*>& output)
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
    
  if(!GetNextStartNode(xmlReader, "KMLMovingObjectInfo"))
  {
    delete xmlReader;
    return;
  }

  //Get the KML file path
  std::string kmlFilePath; 
  if(!GetNextValueNode(xmlReader, "KMLFilePath", kmlFilePath))
  {
    delete xmlReader;
    return;
  }

  if(kmlFilePath==".")
  {
    boost::filesystem::path aux(metadata);
    kmlFilePath = aux.remove_filename().string();
  } 

  //Get the KML file name
  std::string kmlFileName; 
  if(!GetNextValueNode(xmlReader, "KMLFileName", kmlFileName))
  {
    delete xmlReader;
    return;
  }
    
  //get each moving object information
  std::vector<MovingObjectInfo> moInfo;
  while(GetNextStartNode(xmlReader, "MovingObject"))
  {
    MovingObjectInfo info; 
      
    //get the moving object name 
    if(!GetNextValueNode(xmlReader, "Name", info.m_id))
    {
      delete xmlReader;
      return;
    }

    //get the moving object type 
    if(!GetNextValueNode(xmlReader, "Type", info.m_type))
    {
      delete xmlReader;
      return;
    }

    //get the moving object observation container 
    if(!GetNextStartNode(xmlReader, "ObservationContainer"))
    {
      delete xmlReader;
      return;
    }

    //get the moving object observation container type 
    if(!GetNextValueNode(xmlReader, "Type", info.m_containerType))
    {
      delete xmlReader;
      return;
    }

    //get the moving object observation container name 
    if(!GetNextValueNode(xmlReader, "Name", info.m_containerName))
    {
      delete xmlReader;
      return;
    }

    moInfo.push_back(info);
  }
  delete xmlReader;
   
  //====== Read KML files, using OGR driver
  std::string ogrInfo("connection_string="+ kmlFilePath +"\\"+ kmlFileName);
  ds->setConnectionStr(ogrInfo);
  ds->open();
  te::da::DataSourceTransactor* t = ds->getTransactor();

  for(unsigned int i = 0; i <  moInfo.size(); ++i)
  {
    //OGR supports only kml:FolderType and kml:DocumentType as DataSet 
    if(moInfo[i].m_containerType != "kml:FolderType" && moInfo[i].m_containerType != "kml:DocumentType") 
    {
      delete t;
      return;
    }

    te::da::DataSet* moDataSet = t->getDataSet(moInfo[i].m_containerName);
    if(moDataSet == 0)
    {
      delete t;
      return;
    } 

    //This structure is fixed for OGR driver
    int phTimeIdx = 2;  /* property name: timestamp*/
    int geomIdx = 9;    /* property name: geom*/

    te::stloader::MovingObjectDataSetMetadata MOMetadata(phTimeIdx, geomIdx); 

    te::stloader::DataLoader::loadMovingObjects(moDataSet, MOMetadata, output);

    output[i]->setId(moInfo[i].m_id);

    delete moDataSet;
  }

  delete t;
}



