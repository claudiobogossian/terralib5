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
  \file TemporalNearestNeigInterpolator.cpp

  \brief This file contains a temporal nearest neighbors interpolator.
*/


// TerraLib
#include "../../datatype/DateTime.h"
#include "../../datatype/DateTimeUtils.h"
#include "../../common/STLUtils.h"

// ST
#include "../observation/ObservationSet.h"
#include "../observation/Observation.h"

#include "TemporalNearestNeigInterpolator.h"

void
te::st::TemporalNearestNeigInterpolator::getValues(ObservationSet* obSet, te::dt::AbstractData* position, 
                                                     std::vector<te::dt::AbstractData*>& values)
{
  te::dt::DateTime* dt = dynamic_cast<te::dt::DateTime*>(position);
  TimeToObservationMap::const_iterator itlower, itupper, itbegin;
  itbegin = obSet->getObservations().begin();
  itlower = obSet->getObservations().upper_bound(dt);
  itupper = obSet->getObservations().upper_bound(dt);

  //The temporal nearest neighbor is the last observation of the set
  if(itlower==obSet->getObservations().end())
  {
    --itlower;
    te::common::Clone(itlower->second->getObservedValues(), values); 
    return; 
  }
  
  //Get the lower  
  while((*itlower->first)>(*dt) && itlower!=itbegin)
    --itlower;

  long distLower = GetDistance(itlower->first, dt);
  long distUpper = GetDistance(dt, itupper->first);
    
  if(distLower<distUpper)
    te::common::Clone(itlower->second->getObservedValues(), values); 
  else
    te::common::Clone(itupper->second->getObservedValues(), values);
      
  return; 
}


