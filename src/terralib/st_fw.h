/*  Copyright (C) 2008-2011 National Institute For Space Research (INPE) - Brazil.

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
  \file st_fw.h
   
  \brief Forward declarations for the ST module.
*/

#ifndef __TERRALIB_ST_INTERNAL_FW_H
#define __TERRALIB_ST_INTERNAL_FW_H

// Forward declarations
namespace te
{
  namespace st
  {
    //Coverage
    class Coverage;
    class PointCoverage;
    class RasterCoverage;
    class CoverageSeries;
    class CoverageDataSetInfo;
    class PointCoverageDataSetInfo;
    class RasterCoverageDataSetInfo;
    class CoverageSeriesIterator;
    class PointCoverageIterator;

    //Event and Object
    class Event;
    class Object;

    //Interpolators
    class AbstractCoverageSeriesInterp;
    class AbstractPointCoverageInterp;
    class AbstractTimeSeriesInterp;
    class AbstractTrajectoryInterp;
    class IDWInterpolator;
    class NearestCoverageAtTimeInterp;
    class NearestGeometryAtTimeInterp;
    class NearestValueAtTimeInterp;
    
    //Observations
    class Observation;
    class ObservationDataSet;
    class ObservationDataSetInfo;
    class ObservationDataSetType;

    //Time Series
    class TimeSeries;
    class TimeSeriesDataSet;
    class TimeSeriesDataSetInfo;
    class TimeSeriesDataSetType;
    class TimeSeriesIterator;
    class TimeSeriesObservation;

    //Trajectory
    class Trajectory;
    class TrajectoryDataSet;
    class TrajectoryDataSetInfo;
    class TrajectoryDataSetType;
    class TrajectoryIterator;
    struct TrajectoryPatch;

    //Loader
    class STDataLoader;
    class STDataLoaderFromDS;
    class STDataLoaderFromDSFactory;
    class STDataLoaderFromMemDS;
    class STDataLoaderFromMemDSFactory;
    class STDataLoaderImpl;
    class STDataLoaderImplFactory;    
        
  } // end namespace st
}   // end namespace te

#endif // __TERRALIB_ST_INTERNAL_FW_H

