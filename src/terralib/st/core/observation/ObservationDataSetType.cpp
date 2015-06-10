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
  \file ObservationDataSetType.cpp

  \brief This file contains a class called ObservationDataSetType
*/

//TerraLib
#include "../../../common/StringUtils.h"
#include "../../../geometry/Geometry.h"
#include "../../../geometry/GeometryProperty.h"
#include "../../../datatype/DateTimeProperty.h"
#include "../../../datatype/DateTime.h"
#include "../../../datatype/DateTimeInstant.h"
#include "../../../datatype/DateTimePeriod.h"

//ST
#include "ObservationDataSetType.h"

te::st::ObservationDataSetType::ObservationDataSetType(const std::string& dsName)
  : m_dsName(dsName),
    m_phTimePropInfo(),
    m_phTime(),
    m_validTimePropInfo(),
    m_validTime(),
    m_resultTimePropInfo(),
    m_resultTime(),
    m_observedPropIdx(),
    m_observedPropName(),
    m_geomPropInfo(),
    m_geom(),
    m_idPropIndex(-1),
    m_idPropName(""),
    m_id(""),
    m_spatialExtent(),
    m_temporalExtent()
{  
}

te::st::ObservationDataSetType::ObservationDataSetType(const ObservationDataSetType& obs)
{
  *this = obs;
}

const te::st::ObservationDataSetType& 
te::st::ObservationDataSetType::operator=(const ObservationDataSetType& rhs)
{
  if(this == &rhs) 
    return *this; 
    
  m_dsName = rhs.m_dsName;

  m_phTimePropInfo.clear();
  for(unsigned int i = 0; i<rhs.m_phTimePropInfo.size(); ++i)
  {
    te::dt::DateTimeProperty* prop = new te::dt::DateTimeProperty(rhs.m_phTimePropInfo[i]); 
    m_phTimePropInfo.push_back(prop);
  }
  
  m_phTime.reset();
  if(rhs.m_phTime.get()!=0)
    m_phTime.reset(dynamic_cast<te::dt::DateTime*>(rhs.m_phTime->clone()));

  m_validTimePropInfo.clear();
  for(unsigned int i = 0; i<rhs.m_validTimePropInfo.size(); ++i)
  {
    te::dt::DateTimeProperty* prop = new te::dt::DateTimeProperty(rhs.m_validTimePropInfo[i]); 
    m_validTimePropInfo.push_back(prop);
  }

  m_validTime.reset();
  if(rhs.m_validTime.get()!=0)
    m_validTime.reset(dynamic_cast<te::dt::DateTimePeriod*>(rhs.m_validTime->clone()));

  m_resultTimePropInfo.reset();
  if(rhs.m_resultTimePropInfo.get()!=0)
  {
    te::dt::DateTimeProperty* propAux = new te::dt::DateTimeProperty(*(rhs.m_resultTimePropInfo.get())); 
    m_resultTimePropInfo.reset(propAux);
  }

  m_resultTime.reset();
  if(rhs.m_resultTime.get()!=0)
    m_resultTime.reset(dynamic_cast<te::dt::DateTimeInstant*>(rhs.m_resultTime->clone()));

  m_observedPropIdx = rhs.m_observedPropIdx;
  m_observedPropName = rhs.m_observedPropName;
  
  m_geomPropInfo.reset();
  if(rhs.m_geomPropInfo.get()!=0)
  {
    te::gm::GeometryProperty* prop = new te::gm::GeometryProperty(*(rhs.m_geomPropInfo.get())); 
    m_geomPropInfo.reset(prop);
  }

  m_geom.reset();
  if(rhs.m_geom.get()!=0)
    m_geom.reset(dynamic_cast<te::gm::Geometry*>(rhs.m_geom.get()->clone()));
  
  m_idPropIndex = rhs.m_idPropIndex; 
  m_idPropName = rhs.m_idPropName;
  m_id = rhs.m_id;

  m_spatialExtent.reset();
  if(rhs.m_spatialExtent.get()!=0)
    m_spatialExtent.reset(dynamic_cast<te::gm::Geometry*>(rhs.m_spatialExtent.get()->clone()));

  m_temporalExtent.reset();
  if(rhs.m_temporalExtent.get()!=0)
    m_temporalExtent.reset(dynamic_cast<te::dt::DateTimePeriod*>(rhs.m_temporalExtent.get()->clone()));

  return *this; 
}

void te::st::ObservationDataSetType::setDataSetName(const std::string& dsName)
{
  m_dsName = dsName;
}

void te::st::ObservationDataSetType::setTimePropInfo(te::dt::DateTimeProperty* tpInfo)
{
  m_phTimePropInfo.clear();
  m_phTimePropInfo.push_back(std::auto_ptr<te::dt::DateTimeProperty>(tpInfo));
}

void te::st::ObservationDataSetType::setTimePropInfo(te::dt::DateTimeProperty* tpInfo1, 
   te::dt::DateTimeProperty* tpInfo2)
{
  m_phTimePropInfo.clear();
  m_phTimePropInfo.push_back(std::auto_ptr<te::dt::DateTimeProperty>(tpInfo1));
  m_phTimePropInfo.push_back(std::auto_ptr<te::dt::DateTimeProperty>(tpInfo2));
}

void te::st::ObservationDataSetType::setTime(te::dt::DateTime* t)
{
  m_phTime.reset(t);
}

void te::st::ObservationDataSetType::setVlTimePropInfo(te::dt::DateTimeProperty* tpInfo)
{
  m_validTimePropInfo.clear();
  m_validTimePropInfo.push_back(std::auto_ptr<te::dt::DateTimeProperty>(tpInfo));
}

void te::st::ObservationDataSetType::setVlTimePropInfo(te::dt::DateTimeProperty* tpInfo1, 
   te::dt::DateTimeProperty* tpInfo2)
{
  m_validTimePropInfo.clear();
  m_validTimePropInfo.push_back(std::auto_ptr<te::dt::DateTimeProperty>(tpInfo1));
  m_validTimePropInfo.push_back(std::auto_ptr<te::dt::DateTimeProperty>(tpInfo2));
}

void te::st::ObservationDataSetType::setVlTime( te::dt::DateTimePeriod* t)
{
  m_validTime.reset(t);
}

void te::st::ObservationDataSetType::setRsTimePropInfo( te::dt::DateTimeProperty* tpInfo)
{
  m_resultTimePropInfo.reset(tpInfo);
}

void te::st::ObservationDataSetType::setRsTime( te::dt::DateTimeInstant* t)
{
  m_resultTime.reset(t);
}

void te::st::ObservationDataSetType::setObsPropInfo(const std::vector<int>& props)
{
  m_observedPropIdx = props;
}

void te::st::ObservationDataSetType::setObsPropInfo(const std::vector<std::string>& props)
{
  m_observedPropName = props;
}

void te::st::ObservationDataSetType::setGeomPropInfo( te::gm::GeometryProperty* prop)
{
  m_geomPropInfo.reset(prop);
}

void te::st::ObservationDataSetType::setGeometry( te::gm::Geometry* geom)
{
  m_geom.reset(geom);
}

void te::st::ObservationDataSetType::setIdPropInfo(int idProp)
{
  m_idPropIndex = idProp;
}

void te::st::ObservationDataSetType::setIdPropInfo(const std::string& idProp)
{
  m_idPropName = idProp;
}

void te::st::ObservationDataSetType::setId(const std::string& id)
{
  m_id = id;
}

void te::st::ObservationDataSetType::setSpatialExtent(te::gm::Geometry* se)
{
  m_spatialExtent.reset(se);
}

void te::st::ObservationDataSetType::setTemporalExtent(te::dt::DateTimePeriod* te)
{
  m_temporalExtent.reset(te);
}

bool te::st::ObservationDataSetType::hasTimeProp() const
{
  return m_phTimePropInfo.size()>0;
}

bool te::st::ObservationDataSetType::hasTwoTimeProp() const
{
  return m_phTimePropInfo.size()==2;
}

bool te::st::ObservationDataSetType::hasTime() const
{
  return m_phTime.get()!=0;
}

bool te::st::ObservationDataSetType::hasVlTimeProp() const
{
  return m_validTimePropInfo.size()>0;
}

bool te::st::ObservationDataSetType::hasTwoVlTimeProp() const
{
  return m_validTimePropInfo.size()==2;
}

bool te::st::ObservationDataSetType::hasVlTime() const
{
  return m_validTime.get()!=0;
}
        
bool te::st::ObservationDataSetType::hasRsTimeProp() const
{
  return m_resultTimePropInfo.get()!=0;
}
        
bool te::st::ObservationDataSetType::hasRsTime() const
{
  return m_resultTime.get()!=0;
}
        
bool te::st::ObservationDataSetType::hasGeomProp() const
{
  return m_geomPropInfo.get()!=0;
}
        
bool te::st::ObservationDataSetType::hasGeometry() const
{
  return m_geom.get()!=0;
}
        
bool te::st::ObservationDataSetType::hasIdProp() const
{
  if(m_idPropIndex >= 0 || !m_idPropName.empty())
    return true;
  return false;
}
        
bool te::st::ObservationDataSetType::hasId() const
{
  return !m_id.empty();
}

bool te::st::ObservationDataSetType::hasSpatialExtent() const
{
  return (m_spatialExtent.get()!=0);
}

bool te::st::ObservationDataSetType::hasTemporalExtent() const
{
  return (m_temporalExtent.get()!=0);
}

std::string 
te::st::ObservationDataSetType::getDataSetName() const
{
  return m_dsName;
}

const te::dt::DateTimeProperty* 
te::st::ObservationDataSetType::getBeginTimePropInfo() const
{
  if(hasTimeProp())
    return &m_phTimePropInfo[0];
  return 0;
}

const te::dt::DateTimeProperty* 
te::st::ObservationDataSetType::getEndTimePropInfo() const
{
  if(hasTwoTimeProp())
    return &m_phTimePropInfo[1];
  return 0;
}

const te::dt::DateTime* 
te::st::ObservationDataSetType::getTime() const
{
  return m_phTime.get();
}

const te::dt::DateTimeProperty* 
te::st::ObservationDataSetType::getVlBeginTimePropInfo() const
{
  if(hasVlTimeProp())
    return &m_validTimePropInfo[0];
  return 0;
}

const te::dt::DateTimeProperty* 
te::st::ObservationDataSetType::getVlEndTimePropInfo() const
{
  if(hasTwoVlTimeProp())
    return &m_validTimePropInfo[1];
  return 0;
}

const te::dt::DateTimePeriod* 
te::st::ObservationDataSetType::getVlTime() const
{
  return m_validTime.get();
}

const te::dt::DateTimeProperty* 
te::st::ObservationDataSetType::getRsTimePropInfo() const
{
  return m_resultTimePropInfo.get();
}
        
const te::dt::DateTimeInstant* 
te::st::ObservationDataSetType::getRsTime() const
{
  return m_resultTime.get();
}

const std::vector<int>& 
te::st::ObservationDataSetType::getObsPropIdxs() const
{
  return m_observedPropIdx;
}

const std::vector<std::string>& 
te::st::ObservationDataSetType::getObsPropNames() const
{
  return m_observedPropName;
}

const te::gm::GeometryProperty* 
te::st::ObservationDataSetType::getGeomPropInfo() const
{
  return m_geomPropInfo.get();
}
        
const te::gm::Geometry* 
te::st::ObservationDataSetType::getGeometry() const
{
  return m_geom.get();
}

int te::st::ObservationDataSetType::getIdPropIdx() const
{
  return m_idPropIndex;
}

std::string te::st::ObservationDataSetType::getIdPropName() const
{
  return m_idPropName;
}

std::string te::st::ObservationDataSetType::getObsId() const
{
  return m_id;
}

 const te::dt::DateTimePeriod* 
 te::st::ObservationDataSetType::getTemporalExtent() const
 {
   if(hasTemporalExtent())
     return m_temporalExtent.get();
   return 0;
 }

 const te::gm::Geometry* 
 te::st::ObservationDataSetType::getSpatialExtent() const
 {
   if(hasSpatialExtent())
     return m_spatialExtent.get();
   return 0;
 }

std::string
te::st::ObservationDataSetType::getBeginTimePropName() const
{
  if(hasTimeProp())
    return m_phTimePropInfo[0].getName();
  return "";
}

std::string
te::st::ObservationDataSetType::getEndTimePropName() const
{
  if(hasTwoTimeProp())
    return m_phTimePropInfo[1].getName();
  return "";
}

std::string te::st::ObservationDataSetType::getGeomPropName() const
{
  if(hasGeomProp())
    return m_geomPropInfo->getName();
  return "";
}


te::st::ObservationDataSetType::~ObservationDataSetType()
{
}

