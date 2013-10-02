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
  \file Utils.h
   
  \brief Utility functions for ST module.  
 */

//TerraLib
#include "../datatype/AbstractData.h"
#include "../datatype/Enums.h"
#include "../datatype/SimpleData.h"
#include "../dataaccess/dataset/DataSetType.h"
#include "../datatype/Property.h"
#include "../geometry/GeometryProperty.h"
#include "../common/STLUtils.h"

//ST
#include "Utils.h"
#include "core/trajectory/Trajectory.h"
#include "core/trajectory/TrajectoryIterator.h"
#include "core/trajectory/TrajectoryDataSetInfo.h"
#include "core/trajectory/TrajectoryDataSetType.h"
#include "core/observation/ObservationDataSetInfo.h"
#include "core/observation/ObservationDataSetType.h"
#include "core/timeseries/TimeSeriesDataSetInfo.h"
#include "core/timeseries/TimeSeriesDataSetType.h"

#include <stdlib.h>

te::st::ObservationDataSetType te::st::GetType(const ObservationDataSetInfo& info)
{
  return ObservationDataSetType(info.getTimePropIdxs(), info.getObsPropIdxs(), info.getGeomPropIdx(), 
                                info.getVlTimePropIdxs(), info.getRsTimePropIdx());
}

te::st::TimeSeriesDataSetType te::st::GetType(const TimeSeriesDataSetInfo& info)
{
  return TimeSeriesDataSetType(GetType(info.getObservationDataSetInfo()), info.getValuePropIdxs(), 
                               info.getIdPropIdx(), info.getId());
}

te::st::TrajectoryDataSetType te::st::GetType(const TrajectoryDataSetInfo& info)
{
  return TrajectoryDataSetType(GetType(info.getObservationDataSetInfo()), info.getIdPropIdx(), info.getId());
}
    
/*     
bool LessOrEqual(te::dt::AbstractData* v1, te::dt::AbstractData* v2)
{
  if(v1->getTypeCode()==te::dt::INT16_TYPE && v2->getTypeCode()==te::dt::INT16_TYPE)
  {
    int16_t v1_int16 = static_cast<te::dt::Int16*>(v1)->getValue();
    int16_t v2_int16 = static_cast<te::dt::Int16*>(v2)->getValue();
    return v1_int16 <= v2_int16;
  }

  if(v1->getTypeCode()==te::dt::UINT16_TYPE && v2->getTypeCode()==te::dt::UINT16_TYPE)
  {
    uint16_t v1_uint16 = static_cast<te::dt::UInt16*>(v1)->getValue();
    uint16_t v2_uint16 = static_cast<te::dt::UInt16*>(v2)->getValue();
    return v1_uint16 <= v2_uint16;
  }

  if(v1->getTypeCode()==te::dt::INT32_TYPE && v2->getTypeCode()==te::dt::INT32_TYPE)
  {
    int32_t v1_int32 = static_cast<te::dt::Int32*>(v1)->getValue();
    int32_t v2_int32 = static_cast<te::dt::Int32*>(v2)->getValue();
    return v1_int32 <= v2_int32;
  }

  if(v1->getTypeCode()==te::dt::UINT32_TYPE && v2->getTypeCode()==te::dt::UINT32_TYPE)
  {
    uint32_t v1_uint32 = static_cast<te::dt::UInt32*>(v1)->getValue();
    uint32_t v2_uint32 = static_cast<te::dt::UInt32*>(v2)->getValue();
    return v1_uint32 <= v2_uint32;
  }

  if(v1->getTypeCode()==te::dt::INT64_TYPE && v2->getTypeCode()==te::dt::INT64_TYPE)
  {
    int64_t v1_int64 = static_cast<te::dt::Int64*>(v1)->getValue();
    int64_t v2_int64 = static_cast<te::dt::Int64*>(v2)->getValue();
    return v1_int64 <= v2_int64;
  }

  if(v1->getTypeCode()==te::dt::UINT64_TYPE && v2->getTypeCode()==te::dt::UINT64_TYPE)
  {
    uint64_t v1_uint64 = static_cast<te::dt::UInt64*>(v1)->getValue();
    uint64_t v2_uint64 = static_cast<te::dt::UInt64*>(v2)->getValue();
    return v1_uint64 <= v2_uint64;
  }

  if(v1->getTypeCode()==te::dt::FLOAT_TYPE && v2->getTypeCode()==te::dt::FLOAT_TYPE)
  {
    float v1_float = static_cast<te::dt::Float*>(v1)->getValue();
    float v2_float = static_cast<te::dt::Float*>(v2)->getValue();
    return v1_float <= v2_float;
  }

  if(v1->getTypeCode()==te::dt::DOUBLE_TYPE && v2->getTypeCode()==te::dt::DOUBLE_TYPE)
  {
    double v1_double = static_cast<te::dt::Double*>(v1)->getValue();
    double v2_double = static_cast<te::dt::Double*>(v2)->getValue();
    return v1_double <= v2_double;
  }
  return false;
}
*/
     
/*
void AdjustDataSetType( te::da::DataSetType* dtTo, const te::da::DataSetType* dtFrom, 
                  te::st::ObservationSetType* obstTo, const te::st::ObservationSetType* obstFrom, 
                  const std::vector<te::dt::Property*>& properties)
{
  //clone properties
  te::common::Clone(properties, dtTo->getProperties());

  //Get the default geometry property
  te::gm::GeometryProperty* geomPropFrom = dtFrom->getDefaultGeomProperty();
  
  //Get the time properties
  int phBegTimeIdx = obstFrom->getBeginningTimeIdx();
  std::string phBegTimeName = "";
  if(phBegTimeIdx!=-1)
    phBegTimeName = dtFrom->getPropertyById(phBegTimeIdx)->getName();
  
  int phEndTimeIdx = obstFrom->getEndTimeIdx();
  std::string phEndTimeName = "";
  if(phEndTimeIdx!=-1)
    phEndTimeName = dtFrom->getPropertyById(phEndTimeIdx)->getName();

  int vlBegTimeIdx = obstFrom->getValidTimeIdx()[0];
  std::string vlBegTimeName = "";
  if(vlBegTimeIdx!=-1)
    vlBegTimeName = dtFrom->getPropertyById(vlBegTimeIdx)->getName();

  int vlEndTimeIdx = obstFrom->getValidTimeIdx()[1];
  std::string vlEndTimeName = "";
  if(vlEndTimeIdx!=-1)
    vlEndTimeName = dtFrom->getPropertyById(vlEndTimeIdx)->getName();

  int rsTimeIdx = obstFrom->getResultTimeIdx();
  std::string rsTimeName = "";
  if(rsTimeIdx!=-1)
    rsTimeName = dtFrom->getPropertyById(rsTimeIdx)->getName();
  
  for(std::size_t c=0; c<properties.size(); ++c)
  {
    //verify if it is in the properties vector
    std::string name = properties[c]->getName();
    if(geomPropFrom!=0 && (name == geomPropFrom->getName()))
    {
      te::gm::GeometryProperty* geomPropTo =  static_cast<te::gm::GeometryProperty*>(dtTo->getPropertyById(c));
      dtTo->setDefaultGeomProperty(geomPropTo);
    }

    //Adjust ObservationSetType
    if(name == phBegTimeName)
      obstTo->setBeginningTimeIdx(c);
    else if(name == phEndTimeName)
      obstTo->setEndTimeIdx(c);
    else if(name == vlBegTimeName)
      obstTo->setVlBeginningTimeIdx(c);
    else if(name == vlEndTimeName)
      obstTo->setVlEndTimeIdx(c);
    else if(name == rsTimeName)
      obstTo->setResultTimeIdx(c);    
  }
}
*/
         
te::st::TimeSeries* te::st::Normalize(TimeSeries* /*ts*/, te::dt::DateTimeResolution /*tRes*/)
{
  return 0;
}
    
te::st::TimeSeries* te::st::Normalize(TimeSeries* /*ts*/, te::dt::DateTimeResolution /*tRes*/, te::dt::AbstractData* /*defvalue*/)
{
  return 0;
}
