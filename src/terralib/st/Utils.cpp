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
    

         
