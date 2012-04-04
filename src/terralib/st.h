/*  Copyright (C) 2001-2009 National Institute For Space Research (INPE) - Brazil.

    This file is part of the TerraLib - a Framework for building GIS enabled applications.

    TerraLib is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License,
    or (at your option) any later version.

    TerraLib is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with TerraLib. See COPYING. If not, write to
    TerraLib Team at <terralib-team@terralib.org>.
 */

/*!
  \file st.h

  \brief This file contains include headers for the TerraLib ST module.
*/

#ifndef __TERRALIB_ST_INTERNAL_H
#define __TERRALIB_ST_INTERNAL_H

// ST module
#include "st/observation/Observation.h"
#include "st/observation/ObservationSet.h"
#include "st/observation/SpatioTemporalObservationSet.h"
#include "st/observation/ObservationSetIterator.h"
#include "st/timeseries/TimeSeries.h"
#include "st/timeseries/TimeSeriesIterator.h"
#include "st/timeseries/LocatedTimeSeries.h"
#include "st/movingobject/MovingObject.h"
#include "st/movingobject/Trajectory.h"
#include "st/movingobject/TrajectoryIterator.h"
#include "st/movingobject/MovingObjectIterator.h"
#include "st/interpolator/AbstractInterpolator.h"
#include "st/interpolator/TemporalNearestNeigInterpolator.h"
#include "st/coverage/Coverage.h"
#include "st/coverage/RasterCoverage.h"

namespace te
{
  /*! \brief Namespace for the TerraLib ST module. */
  namespace st
  {
  }	// end namespace st
}   // end namespace te

#endif  // __TERRALIB_ST_INTERNAL_H

