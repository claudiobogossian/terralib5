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
  \file ObservationDataSetInfo.cpp

  \brief This file contains a class called ObservationDataSetInfo
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
#include "ObservationDataSetInfo.h"

te::st::ObservationDataSetInfo::ObservationDataSetInfo(const te::da::DataSourceInfo& dsInfo, 
                                                       const std::string& dsName)
  : m_dsInfo(dsInfo),
    m_dsName(dsName),
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

te::st::ObservationDataSetInfo::ObservationDataSetInfo(const ObservationDataSetInfo& obs)
{
  *this = obs;
}

const te::st::ObservationDataSetInfo& 
te::st::ObservationDataSetInfo::operator=(const ObservationDataSetInfo& rhs)
{
  if(this == &rhs) 
    return *this; 
    
  m_dsInfo = rhs.m_dsInfo;
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

void te::st::ObservationDataSetInfo::setDataSourceInfo( const te::da::DataSourceInfo& dsInfo, 
                                                        const std::string& dsName)
{
  m_dsInfo = dsInfo;
  m_dsName = dsName;
}

void te::st::ObservationDataSetInfo::setTimePropInfo(te::dt::DateTimeProperty* tpInfo)
{
  m_phTimePropInfo.clear();
  m_phTimePropInfo.push_back(std::auto_ptr<te::dt::DateTimeProperty>(tpInfo));
}

void te::st::ObservationDataSetInfo::setTimePropInfo(te::dt::DateTimeProperty* tpInfo1, 
   te::dt::DateTimeProperty* tpInfo2)
{
  m_phTimePropInfo.clear();
  m_phTimePropInfo.push_back(std::auto_ptr<te::dt::DateTimeProperty>(tpInfo1));
  m_phTimePropInfo.push_back(std::auto_ptr<te::dt::DateTimeProperty>(tpInfo2));
}

void te::st::ObservationDataSetInfo::setTime(te::dt::DateTime* t)
{
  m_phTime.reset(t);
}

void te::st::ObservationDataSetInfo::setVlTimePropInfo(te::dt::DateTimeProperty* tpInfo)
{
  m_validTimePropInfo.clear();
  m_validTimePropInfo.push_back(std::auto_ptr<te::dt::DateTimeProperty>(tpInfo));
}

void te::st::ObservationDataSetInfo::setVlTimePropInfo(te::dt::DateTimeProperty* tpInfo1, 
   te::dt::DateTimeProperty* tpInfo2)
{
  m_validTimePropInfo.clear();
  m_validTimePropInfo.push_back(std::auto_ptr<te::dt::DateTimeProperty>(tpInfo1));
  m_validTimePropInfo.push_back(std::auto_ptr<te::dt::DateTimeProperty>(tpInfo2));
}

void te::st::ObservationDataSetInfo::setVlTime( te::dt::DateTimePeriod* t)
{
  m_validTime.reset(t);
}

void te::st::ObservationDataSetInfo::setRsTimePropInfo( te::dt::DateTimeProperty* tpInfo)
{
  m_resultTimePropInfo.reset(tpInfo);
}

void te::st::ObservationDataSetInfo::setRsTime( te::dt::DateTimeInstant* t)
{
  m_resultTime.reset(t);
}

void te::st::ObservationDataSetInfo::setObsPropInfo(const std::vector<int>& props)
{
  m_observedPropIdx = props;
}

void te::st::ObservationDataSetInfo::setObsPropInfo(const std::vector<std::string>& props)
{
  m_observedPropName = props;
}

void te::st::ObservationDataSetInfo::setGeomPropInfo( te::gm::GeometryProperty* prop)
{
  m_geomPropInfo.reset(prop);
}

void te::st::ObservationDataSetInfo::setGeometry( te::gm::Geometry* geom)
{
  m_geom.reset(geom);
}

void te::st::ObservationDataSetInfo::setIdPropInfo(int idProp)
{
  m_idPropIndex = idProp;
}

void te::st::ObservationDataSetInfo::setIdPropInfo(const std::string& idProp)
{
  m_idPropName = idProp;
}

void te::st::ObservationDataSetInfo::setId(const std::string& id)
{
  m_id = id;
}

void te::st::ObservationDataSetInfo::setSpatialExtent(te::gm::Geometry* se)
{
  m_spatialExtent.reset(se);
}

void te::st::ObservationDataSetInfo::setTemporalExtent(te::dt::DateTimePeriod* te)
{
  m_temporalExtent.reset(te);
}

bool te::st::ObservationDataSetInfo::hasTimeProp() const
{
  return m_phTimePropInfo.size()>0;
}

bool te::st::ObservationDataSetInfo::hasTwoTimeProp() const
{
  return m_phTimePropInfo.size()==2;
}

bool te::st::ObservationDataSetInfo::hasTime() const
{
  return m_phTime.get()!=0;
}

bool te::st::ObservationDataSetInfo::hasVlTimeProp() const
{
  return m_validTimePropInfo.size()>0;
}

bool te::st::ObservationDataSetInfo::hasTwoVlTimeProp() const
{
  return m_validTimePropInfo.size()==2;
}

bool te::st::ObservationDataSetInfo::hasVlTime() const
{
  return m_validTime.get()!=0;
}
        
bool te::st::ObservationDataSetInfo::hasRsTimeProp() const
{
  return m_resultTimePropInfo.get()!=0;
}
        
bool te::st::ObservationDataSetInfo::hasRsTime() const
{
  return m_resultTime.get()!=0;
}
        
bool te::st::ObservationDataSetInfo::hasGeomProp() const
{
  return m_geomPropInfo.get()!=0;
}
        
bool te::st::ObservationDataSetInfo::hasGeometry() const
{
  return m_geom.get()!=0;
}
        
bool te::st::ObservationDataSetInfo::hasIdProp() const
{
  if(m_idPropIndex >= 0 || !m_idPropName.empty())
    return true;
  return false;
}
        
bool te::st::ObservationDataSetInfo::hasId() const
{
  return !m_id.empty();
}

bool te::st::ObservationDataSetInfo::hasSpatialExtent() const
{
  return (m_spatialExtent.get()!=0);
}

bool te::st::ObservationDataSetInfo::hasTemporalExtent() const
{
  return (m_temporalExtent.get()!=0);
}

const te::da::DataSourceInfo& 
te::st::ObservationDataSetInfo::getDataSourceInfo() const
{
  return m_dsInfo;
}

std::string 
te::st::ObservationDataSetInfo::getDataSetName() const
{
  return m_dsName;
}

const te::dt::DateTimeProperty* 
te::st::ObservationDataSetInfo::getBeginTimePropInfo() const
{
  if(hasTimeProp())
    return &m_phTimePropInfo[0];
  return 0;
}

const te::dt::DateTimeProperty* 
te::st::ObservationDataSetInfo::getEndTimePropInfo() const
{
  if(hasTwoTimeProp())
    return &m_phTimePropInfo[1];
  return 0;
}

const te::dt::DateTime* 
te::st::ObservationDataSetInfo::getTime() const
{
  return m_phTime.get();
}

const te::dt::DateTimeProperty* 
te::st::ObservationDataSetInfo::getVlBeginTimePropInfo() const
{
  if(hasVlTimeProp())
    return &m_validTimePropInfo[0];
  return 0;
}

const te::dt::DateTimeProperty* 
te::st::ObservationDataSetInfo::getVlEndTimePropInfo() const
{
  if(hasTwoVlTimeProp())
    return &m_validTimePropInfo[1];
  return 0;
}

const te::dt::DateTimePeriod* 
te::st::ObservationDataSetInfo::getVlTime() const
{
  return m_validTime.get();
}

const te::dt::DateTimeProperty* 
te::st::ObservationDataSetInfo::getRsTimePropInfo() const
{
  return m_resultTimePropInfo.get();
}
        
const te::dt::DateTimeInstant* 
te::st::ObservationDataSetInfo::getRsTime() const
{
  return m_resultTime.get();
}

const std::vector<int>& 
te::st::ObservationDataSetInfo::getObsPropIdxs() const
{
  return m_observedPropIdx;
}

const std::vector<std::string>& 
te::st::ObservationDataSetInfo::getObsPropNames() const
{
  return m_observedPropName;
}

const te::gm::GeometryProperty* 
te::st::ObservationDataSetInfo::getGeomPropInfo() const
{
  return m_geomPropInfo.get();
}
        
const te::gm::Geometry* 
te::st::ObservationDataSetInfo::getGeometry() const
{
  return m_geom.get();
}

int te::st::ObservationDataSetInfo::getIdPropIdx() const
{
  return m_idPropIndex;
}

std::string te::st::ObservationDataSetInfo::getIdPropName() const
{
  return m_idPropName;
}

std::string te::st::ObservationDataSetInfo::getObsId() const
{
  return m_id;
}

/*
int te::st::ObservationDataSetInfo::getBeginTimePropIdx() const
{
  if(hasTimeProp())
    return m_phTimePropInfo[0].getId();
  return -1;
}
*/

std::string
te::st::ObservationDataSetInfo::getBeginTimePropName() const
{
  if(hasTimeProp())
    return m_phTimePropInfo[0].getName();
  return "";
}

/*
int te::st::ObservationDataSetInfo::getEndTimePropIdx() const
{
  if(hasTwoTimeProp())
    return m_phTimePropInfo[1].getId();
  return -1;
}
*/

std::string
te::st::ObservationDataSetInfo::getEndTimePropName() const
{
  if(hasTwoTimeProp())
    return m_phTimePropInfo[1].getName();
  return "";
}

/*
int te::st::ObservationDataSetInfo::getGeomPropIdx() const
{
  if(hasGeomProp())
    return m_geomPropInfo->getId();
  return -1;
}
*/

std::string te::st::ObservationDataSetInfo::getGeomPropName() const
{
  if(hasGeomProp())
    return m_geomPropInfo->getName();
  return "";
}

const te::dt::DateTimePeriod* 
 te::st::ObservationDataSetInfo::getTemporalExtent() const
 {
   if(hasTemporalExtent())
     return m_temporalExtent.get();
   return 0;
 }

 const te::gm::Geometry* 
 te::st::ObservationDataSetInfo::getSpatialExtent() const
 {
   if(hasSpatialExtent())
     return m_spatialExtent.get();
   return 0;
 }

std::string te::st::ObservationDataSetInfo::getDSId() const
{
  return te::common::Convert2UCase(m_dsInfo.getId()+"&"+m_dsName);
}

te::st::ObservationDataSetInfo::~ObservationDataSetInfo()
{
}
