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

//// ST module

//Coverage
#include "st/core/coverage/Coverage.h"
#include "st/core/coverage/PointCoverage.h"
#include "st/core/coverage/RasterCoverage.h"
#include "st/core/coverage/CoverageSeries.h"
#include "st/core/coverage/CoverageSeriesDataSetInfo.h"
#include "st/core/coverage/CoverageSeriesObservation.h"
#include "st/core/coverage/CoverageDataSetInfo.h"
#include "st/core/coverage/PointCoverageDataSetInfo.h"
#include "st/core/coverage/RasterCoverageDataSetInfo.h"
#include "st/core/coverage/CoverageSeriesIterator.h"
#include "st/core/coverage/PointCoverageIterator.h"
#include "st/core/coverage/PointCoverageObservation.h"

//Event and Object
#include "st/core/event/Event.h"
#include "st/core/event/Object.h"

//Interpolators
#include "st/core/interpolator/AbstractCoverageSeriesInterp.h"
#include "st/core/interpolator/AbstractPointCoverageInterp.h"
#include "st/core/interpolator/AbstractTimeSeriesInterp.h"
#include "st/core/interpolator/AbstractTrajectoryInterp.h"
#include "st/core/interpolator/IDWInterpolator.h"
#include "st/core/interpolator/NearestCoverageAtTimeInterp.h"
#include "st/core/interpolator/NearestGeometryAtTimeInterp.h"
#include "st/core/interpolator/NearestValueAtTimeInterp.h"
    
//Observations
#include "st/core/observation/Observation.h"
#include "st/core/observation/ObservationDataSet.h"
#include "st/core/observation/ObservationDataSetInfo.h"
#include "st/core/observation/ObservationDataSetType.h"

//Time Series
#include "st/core/timeseries/TimeSeries.h"
#include "st/core/timeseries/TimeSeriesDataSet.h"
#include "st/core/timeseries/TimeSeriesDataSetInfo.h"
#include "st/core/timeseries/TimeSeriesIterator.h"
#include "st/core/timeseries/TimeSeriesObservation.h"

//Trajectory
#include "st/core/trajectory/Trajectory.h"
#include "st/core/trajectory/TrajectoryDataSet.h"
#include "st/core/trajectory/TrajectoryDataSetInfo.h"
#include "st/core/trajectory/TrajectoryIterator.h"
#include "st/core/trajectory/TrajectoryObservation.h"

//Loader
#include "st/loader/STDataLoader.h"
#include "st/loader/STDataLoaderFromDS.h"
#include "st/loader/STDataLoaderFromDSFactory.h"
#include "st/loader/STDataLoaderFromMemDS.h"
#include "st/loader/STDataLoaderFromMemDSFactory.h"
#include "st/loader/STDataLoaderImpl.h"
#include "st/loader/STDataLoaderImplFactory.h"  

/*!
  \defgroup st Spatiotemporal Module

  \brief The st module provides classes and functions to load and deal with spatiotemporal data.  
  
  This module provides data types or classes to represent and process 
  spatiotemporal information, such as observations, trajectory, time series, 
  coverage and coverage series.
  Besides that, it provides a set of functions to load spatiotemporal information
  from different data sources. 
  
  The main classes/concepts in this module are listed here. The namespace associated to the ST module is te::st.
  To know more about it, see the te::st namespace documentation.
*/

namespace te
{
  /*! \brief Namespace for the TerraLib ST module. */
  namespace st
  {
  }	// end namespace st
}   // end namespace te

#endif  // __TERRALIB_ST_INTERNAL_H

