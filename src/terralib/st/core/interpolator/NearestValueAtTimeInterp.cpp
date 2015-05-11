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
  \file NearestValueAtTimeInterp.cpp

  \brief  This file contains an interpolation function that estimates the
          nearest value at time of a time series. 
*/


// TerraLib
#include "../../../datatype/DateTime.h"
#include "../../../datatype/DateTimeUtils.h"
#include "../../../datatype/AbstractData.h"

// ST
#include "../timeseries/TimeSeries.h"
#include "../timeseries/TimeSeriesObservation.h"
#include "NearestValueAtTimeInterp.h"

te::st::NearestValueAtTimeInterp::NearestValueAtTimeInterp()
{
}

te::st::NearestValueAtTimeInterp::~NearestValueAtTimeInterp()
{
}  

std::auto_ptr<te::dt::AbstractData> 
te::st::NearestValueAtTimeInterp::estimate(const te::st::TimeSeries& ts, te::dt::DateTime* time) const
{
  TimeSeriesObservationSet::const_iterator itlower, itupper, itbegin, itend;
  const TimeSeriesObservationSet& tsObs = ts.getObservations();

  TimeSeriesObservation item(static_cast<te::dt::DateTime*>(time->clone()),0);
  itlower = tsObs.lower_bound(item);
  itupper = tsObs.upper_bound(item);

  //If both iterator points to the same position (the first element after item), we have to get 
  //the first element before item
  if(itlower==itupper && itlower!=tsObs.begin())
    --itlower;
  
  if(itlower!=tsObs.end() && itupper!=tsObs.end())
  {
     long distLower = GetDistance(itlower->getTime(),time);
     long distUpper = GetDistance(itupper->getTime(),time);
     if(distLower<distUpper)
      return std::auto_ptr<te::dt::AbstractData>(itlower->getValue()->clone());
     else
      return std::auto_ptr<te::dt::AbstractData>(itupper->getValue()->clone()); 
  }
  
  if(itlower!=tsObs.end() && itupper==tsObs.end())
    return std::auto_ptr<te::dt::AbstractData>(itlower->getValue()->clone());
  
  return std::auto_ptr<te::dt::AbstractData>(0);
}


