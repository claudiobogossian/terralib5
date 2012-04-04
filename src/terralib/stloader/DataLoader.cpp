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
  \file DataLoader.cpp

  \brief This file contains a class called DataLoader
*/
//Terralib
#include "../dataaccess/dataset/DataSet.h"
#include "../dataaccess/dataset/DataSetType.h"
#include "../datatype/Property.h"
#include "../geometry/GeometryProperty.h"
#include "../datatype/AbstractData.h"
#include "../datatype/DateTime.h"
#include "../datatype/DateTimeInstant.h"
#include "../datatype/DateTimePeriod.h"
#include "../datatype/DateTimeUtils.h"

//XML
#include "../xml/Reader.h"
#include "../xml/ReaderFactory.h"

//ST
#include "DataLoader.h"
#include "../st/observation/ObservationSet.h"
#include "../st/observation/Observation.h"
#include "../st/movingobject/MovingObject.h"
#include "../st/timeseries/TimeSeries.h"
#include "../st/timeseries/LocatedTimeSeries.h"
#include "ObservationDataSetMetadata.h"
#include "MovingObjectDataSetMetadata.h"
#include "LocatedTimeSeriesDataSetMetadata.h"

void te::stloader::DataLoader::loadObservationSet(te::da::DataSet* ds, 
                        ObservationDataSetMetadata& metadata, 
                        te::st::ObservationSet*& output)
{
    std::vector<te::dt::Property*>    props;
    output = new te::st::ObservationSet();

    //------ Get the properties
    std::vector<int> idxs = metadata.getObservedPropertiesIdx();
    std::vector<int> phTimeIdxs = metadata.getPhenomenonTimeIdx();
   
    te::da::DataSetType* dsType =  ds->getType();
    for(unsigned int i=0; i<idxs.size(); ++i)
    {
      te::dt::Property* pp = dsType->getProperty(idxs[i])->clone();
      props.push_back(pp);
    }
    output->setProperties(props);
        
    //------ Get the observed values, time, etc;
    while(ds->moveNext())
    {
      std::vector<te::dt::AbstractData*> obsData;
      te::dt::DateTime* phTime = 0;
     
      for(unsigned int i=0; i<idxs.size(); ++i)
      {
        te::dt::AbstractData* d = ds->getValue(idxs[i]);
        obsData.push_back(d);
      }

      if(phTimeIdxs.size()==1)
        phTime = ds->getDateTime(phTimeIdxs[0]); 
      else if (phTimeIdxs.size()==2)
      {
        te::dt::DateTimeInstant* initTime = dynamic_cast<te::dt::DateTimeInstant*>(ds->getDateTime(phTimeIdxs[0])); 
        te::dt::DateTimeInstant* finalTime = dynamic_cast<te::dt::DateTimeInstant*>(ds->getDateTime(phTimeIdxs[1])); 
        phTime = te::dt::GetTimePeriod(initTime,finalTime);  
        delete initTime;
        delete finalTime;
      }

      te::st::Observation* ob = new te::st::Observation(phTime, obsData, output);
      output->addObservation(ob);
    }    
}

void te::stloader::DataLoader::loadMovingObjects(te::da::DataSet* ds, 
                        MovingObjectDataSetMetadata& metadata, 
                        std::vector<te::st::MovingObject*>& output)
{
    std::map<std::string, te::st::MovingObject*> mObjs;
    
    //------ Get the properties
    std::vector<int> phTimeIdxs = metadata.getPhenomenonTimeIdx();
    int geomPropIdx = metadata.getGeometryPropertyIdx();
    int idPropIdx = metadata.getIdPropertyIdx();
   
    te::da::DataSetType* dsType =  ds->getType();
    
    //------ Get observations
    while(ds->moveNext())
    {
      //get id
      std::string id = "-1";
      if(idPropIdx>=0)
        std::string id = ds->getAsString(idPropIdx);

      //get geometry 
      te::gm::Geometry* g = ds->getGeometry(geomPropIdx);
     
      //get phenomenon time
      te::dt::DateTime* phTime = 0;
      if(phTimeIdxs.size()==1)
        phTime = ds->getDateTime(phTimeIdxs[0]); 
      else if (phTimeIdxs.size()==2)
      {
        te::dt::DateTimeInstant* initTime = dynamic_cast<te::dt::DateTimeInstant*>(ds->getDateTime(phTimeIdxs[0])); 
        te::dt::DateTimeInstant* finalTime = dynamic_cast<te::dt::DateTimeInstant*>(ds->getDateTime(phTimeIdxs[1])); 
        phTime = te::dt::GetTimePeriod(initTime,finalTime);  
        delete initTime;
        delete finalTime;
      }

      std::map<std::string, te::st::MovingObject*>::iterator it = mObjs.find(id);
      if( it == mObjs.end())
      {
        //----------- gerar um novo Moving Object e inseri-lo no map
        te::gm::GeometryProperty* gp = dynamic_cast<te::gm::GeometryProperty*>(dsType->getProperty(geomPropIdx)->clone());
             
        te::st::MovingObject* mo = new te::st::MovingObject(gp, id); 
        mo->addObservation(phTime, g);
        mObjs.insert(std::pair<std::string,te::st::MovingObject*>(id, mo));
      }
      else
      {
        //adicionar a observacao no moving object existente
        it->second->addObservation(phTime, g);
      }
   }
   
   std::map<std::string, te::st::MovingObject*>::iterator it = mObjs.begin();
   while(it != mObjs.end())
   {
      output.push_back(it->second);
      ++it;
   }   
}

void te::stloader::DataLoader::loadLocatedTimeSeries(te::da::DataSet* ds, 
                        LocatedTimeSeriesDataSetMetadata& metadata, 
                        std::vector<te::st::LocatedTimeSeries*>& output)
{
  std::map<std::string, te::st::LocatedTimeSeries*> gTSs;
    
  //------ Get the properties
  std::vector<int> idxs = metadata.getObservedPropertiesIdx();
  std::vector<int> phTimeIdxs = metadata.getPhenomenonTimeIdx();
  int spPropIdx = metadata.getSpatialLocationPropertyIdx();
  int idPropIdx = metadata.getIdPropertyIdx();
   
  te::da::DataSetType* dsType =  ds->getType();
    
  //------ Get observations
  while(ds->moveNext())
  {
    //get id
    std::string id = ds->getAsString(idPropIdx);

    //get phenomenon time  
    te::dt::DateTime* phTime = 0;
    if(phTimeIdxs.size()==1)
      phTime = ds->getDateTime(phTimeIdxs[0]); 
    else if (phTimeIdxs.size()==2)
    {
      te::dt::DateTimeInstant* initTime = dynamic_cast<te::dt::DateTimeInstant*>(ds->getDateTime(phTimeIdxs[0])); 
      te::dt::DateTimeInstant* finalTime = dynamic_cast<te::dt::DateTimeInstant*>(ds->getDateTime(phTimeIdxs[1])); 
      phTime = te::dt::GetTimePeriod(initTime,finalTime);   
      delete initTime;
      delete finalTime;
    }

    std::map<std::string, te::st::LocatedTimeSeries*>::iterator it = gTSs.find(id);
    if( it == gTSs.end())
    {
      //----------- gerar um novo GeosensorTimeSeries e inseri-lo no map
      std::vector<te::st::TimeSeries*>  tsSet;

      for(unsigned int i=0; i<idxs.size(); ++i)
      {
        te::dt::Property* prop = dsType->getProperty(idxs[i])->clone();
        te::st::TimeSeries* ts = new te::st::TimeSeries(prop);
        
        te::dt::AbstractData* d = ds->getValue(idxs[i]);
        ts->addObservation(phTime, d);
        
        tsSet.push_back(ts);
      }
        
      te::gm::Geometry* sp = ds->getGeometry(spPropIdx);
      te::st::LocatedTimeSeries* gsTS = new te::st::LocatedTimeSeries(tsSet, sp, atoi(id.c_str())); 
      gTSs.insert(std::pair<std::string,te::st::LocatedTimeSeries*>(id, gsTS));
    }
    else
    {
      //adicionar a observacao no moving object existente
      for(unsigned int i=0; i<idxs.size(); ++i)
      {
        std::string name = dsType->getProperty(idxs[i])->getName();
        te::dt::AbstractData* d = ds->getValue(idxs[i]);
        it->second->addObservation(name, phTime, d);        
      }      
    }
  }
   
  std::map<std::string, te::st::LocatedTimeSeries*>::iterator it = gTSs.begin();
  while(it != gTSs.end())
  {
    output.push_back(it->second);
    ++it;
  }     
}



