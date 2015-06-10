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
  \file Utils.h
   
  \brief Utility functions for ST module.  
 */

//TerraLib
#include "../geometry/Geometry.h"
#include "../geometry/GeometryProperty.h"
#include "../datatype/DateTimeProperty.h"
#include "../datatype/DateTime.h"
#include "../datatype/DateTimeInstant.h"
#include "../datatype/DateTimePeriod.h"

//ST
#include "Utils.h"
#include "core/observation/ObservationDataSetType.h"
#include "core/observation/ObservationDataSetInfo.h"

te::st::ObservationDataSetType te::st::GetType(const ObservationDataSetInfo& info)
{
  
  te::st::ObservationDataSetType result(info.getDataSetName());

  //Phenomenon time
  if(info.hasTimeProp())
  {
    te::dt::DateTimeProperty* prop1 = new te::dt::DateTimeProperty(*info.getBeginTimePropInfo()); 
    
    if(info.hasTwoTimeProp())
    {
      te::dt::DateTimeProperty* prop2 = new te::dt::DateTimeProperty(*info.getEndTimePropInfo()); 
      result.setTimePropInfo(prop1, prop2);
    }
    else
      result.setTimePropInfo(prop1);
  }
  
  if(info.hasTime())
  {
    te::dt::DateTime* t = dynamic_cast<te::dt::DateTime*>(info.getTime()->clone()); 
    result.setTime(t);
  }

  //Valid time
  if(info.hasVlTimeProp())
  {
    te::dt::DateTimeProperty* prop1 = new te::dt::DateTimeProperty(*info.getVlBeginTimePropInfo()); 
    if(info.hasTwoVlTimeProp())
    {
      te::dt::DateTimeProperty* prop2 = new te::dt::DateTimeProperty(*info.getVlEndTimePropInfo()); 
      result.setVlTimePropInfo(prop1, prop2);
    }
    else
      result.setVlTimePropInfo(prop1);
  }
  
  if(info.hasVlTime())
  {
    te::dt::DateTimePeriod* t = dynamic_cast<te::dt::DateTimePeriod*>(info.getVlTime()->clone()); 
    result.setVlTime(t);
  }  
  
  //Result time
  if(info.hasRsTimeProp())
  {
    te::dt::DateTimeProperty* prop = new te::dt::DateTimeProperty(*info.getRsTimePropInfo()); 
    result.setRsTimePropInfo(prop);
  }
  if(info.hasRsTime())
  {
    te::dt::DateTimeInstant* t = dynamic_cast<te::dt::DateTimeInstant*>(info.getRsTime()->clone()); 
    result.setRsTime(t);
  }  

  //observed properties
  result.setObsPropInfo(info.getObsPropIdxs());
  result.setObsPropInfo(info.getObsPropNames());

  //geometry
  if(info.hasGeomProp())
  {
    te::gm::GeometryProperty* prop = new te::gm::GeometryProperty(*info.getGeomPropInfo()); 
    result.setGeomPropInfo(prop);
  }
  if(info.hasGeometry())
  {
    te::gm::Geometry* g = dynamic_cast<te::gm::Geometry*>(info.getGeometry()->clone()); 
    result.setGeometry(g);
  }  

  //id properties
  result.setIdPropInfo(info.getIdPropIdx());
  result.setIdPropInfo(info.getIdPropName());
  result.setId(info.getObsId());

  //spatial extent
  if(info.hasSpatialExtent())
  {
    te::gm::Geometry* g = dynamic_cast<te::gm::Geometry*>(info.getSpatialExtent()->clone()); 
    result.setSpatialExtent(g);
  } 

  //temporal extent
  if(info.hasTemporalExtent())
  {
    te::dt::DateTimePeriod* t = dynamic_cast<te::dt::DateTimePeriod*>(info.getTemporalExtent()->clone()); 
    result.setTemporalExtent(t);
  }  

  return result;
}

