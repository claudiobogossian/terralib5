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
  \file NearestCoverageAtTimeInterp.cpp

  \brief This file contains an interpolation function that estimates the
          nearest coverage at time. 
*/

// TerraLib
#include "../../../datatype/DateTime.h"
#include "../../../datatype/DateTimeUtils.h"

// ST
#include "../coverage/Coverage.h"
#include "../coverage/CoverageSeries.h"
#include "NearestCoverageAtTimeInterp.h"

te::st::NearestCoverageAtTimeInterp::NearestCoverageAtTimeInterp()
{
}

te::st::NearestCoverageAtTimeInterp::~NearestCoverageAtTimeInterp()
{
}  

std::auto_ptr<te::st::Coverage> 
te::st::NearestCoverageAtTimeInterp::estimate(const CoverageSeries& cvs, te::dt::DateTime* time) const
{
  CoverageSeriesObservationSet::const_iterator itlower, itupper, itbegin, itend;
  const CoverageSeriesObservationSet& cvObs = cvs.getObservations();

  te::dt::DateTimeShrPtr dtshr(static_cast<te::dt::DateTime*>(time->clone()));
  itlower = cvObs.lower_bound(dtshr);
  itupper = cvObs.upper_bound(dtshr);

  //If both iterator points to the same position (the first element after dtshr), we have to get 
  //the first element before dtshr
  if(itlower==itupper && itlower!=cvObs.begin())
    --itlower;
  
  if(itlower!=cvObs.end() && itupper!=cvObs.end())
  {
     long distLower = GetDistance(itlower->first.get(),time);
     long distUpper = GetDistance(itupper->first.get(),time);
     if(distLower<distUpper)
      return std::auto_ptr<te::st::Coverage>(itlower->second->clone());
     else
      return std::auto_ptr<te::st::Coverage>(itupper->second->clone()); 
  }
  
  if(itlower!=cvObs.end() && itupper==cvObs.end())
    return std::auto_ptr<te::st::Coverage>(itlower->second->clone());
  
  return std::auto_ptr<te::st::Coverage>(0);
}

