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
  \file NearestGeometryAtTimeInterp.cpp

  \brief  This file contains an interpolation function that estimates the
          nearest geometry at time of a trajectory. 
*/

// TerraLib
#include "../../../datatype/DateTime.h"
#include "../../../datatype/DateTimeUtils.h"
#include "../../../geometry/Geometry.h"

// ST
#include "../trajectory/Trajectory.h"
#include "NearestGeometryAtTimeInterp.h"

te::st::NearestGeometryAtTimeInterp::NearestGeometryAtTimeInterp()
{
}

te::st::NearestGeometryAtTimeInterp::~NearestGeometryAtTimeInterp()
{
}  

std::auto_ptr<te::gm::Geometry> 
te::st::NearestGeometryAtTimeInterp::estimate( const Trajectory& tj, 
                                               te::dt::DateTime* time) const
{
  TrajectoryObservationSet::const_iterator itlower, itupper, itbegin, itend;
  const TrajectoryObservationSet& tjObs = tj.getObservations();
  
  te::dt::DateTimeShrPtr dtshr(static_cast<te::dt::DateTime*>(time->clone()));
  itlower = tjObs.lower_bound(dtshr);
  itupper = tjObs.upper_bound(dtshr);

  //If both iterator points to the same position (the first element after dtshr), we have to get 
  //the first element before dtshr
  if(itlower==itupper && itlower!=tjObs.begin())
    --itlower;
  
  if(itlower!=tjObs.end() && itupper!=tjObs.end())
  {
     long distLower = GetDistance(itlower->first.get(),time);
     long distUpper = GetDistance(itupper->first.get(),time);
     if(distLower<distUpper)
      return std::auto_ptr<te::gm::Geometry>(static_cast<te::gm::Geometry*>(itlower->second->clone()));
     else
      return std::auto_ptr<te::gm::Geometry>(static_cast<te::gm::Geometry*>(itupper->second->clone())); 
  }
  
  if(itlower!=tjObs.end() && itupper==tjObs.end())
    return std::auto_ptr<te::gm::Geometry>(static_cast<te::gm::Geometry*>(itlower->second->clone()));
  
  return std::auto_ptr<te::gm::Geometry>(0);
}


