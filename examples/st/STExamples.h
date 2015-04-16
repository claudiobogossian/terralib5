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
  \file STExamples.h

  \brief Examples on how to load/manipulate st types.
 */

#ifndef __TERRALIB_EXAMPLES_STEXAMPLES_INTERNAL_STEXAMPLES_H
#define __TERRALIB_EXAMPLES_STEXAMPLES_INTERNAL_STEXAMPLES_H

// TerraLib
#include <terralib/common_fw.h>
#include <terralib/dataaccess_fw.h>
#include <terralib/geometry_fw.h>
#include <terralib/st_fw.h>

// STL
#include <vector>
#include <string>

// Boost
#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/shared_ptr.hpp>


//ST examples
#include "Config.h"

/*! 
  \brief It loads the TerraLib modules. 
*/
void LoadModules();

/*! 
  \brief It creates a new DataSource and put it into the DataSource manager, using a random id. 
*/
void CreateDataSourceAndUpdateManager(te::da::DataSourceInfo& dsinfo);

/*! 
  \brief It groups the examples with trajectories (TrajectoryExamples.cpp). 
*/
void TrajectoryExamples();

/*! 
  \brief It calculates the distance between two trajectories (TrajectoryDistance.cpp). 
*/
void TrajectoryDistance(te::st::Trajectory* tj1, te::st::Trajectory* tj2);

/*! 
  \brief It calculates the intersection between a trajectory and a geometry (TrajectoryInstersection.cpp). 
*/
void TrajectoryIntersection(te::st::Trajectory* tj, te::gm::Geometry* geom);

/*! 
  \brief It loads trajectory data set from KML file
*/
void LoadTrajectoryDataSetFromKML(boost::ptr_vector<te::st::TrajectoryDataSet>& output);

/*! 
  \brief It groups the examples with TimeSeries (TimeSeriesExamples.cpp). 
*/
void TimeSeriesExamples();

/*! 
  \brief It loads a Time Series data set from a PostGIS database
*/
void LoadTimeSeriesDataSetFromPGIS(boost::ptr_vector<te::st::TimeSeries>& output);

/*! 
  \brief It loads a coverage series from a set of geotif files
*/
std::auto_ptr<te::st::CoverageSeries> LoadCoverageSeriesFromGeotif();

/*! 
  \brief It contains examples about coverage
*/
void CoverageSeriesExamples();

/*! 
  \brief It prints information about the trajectory data set: its spatial and temporal extent (PrintTrajectoryDataSet.cpp)
*/
void PrintTrajectoryDataSetInfo(boost::ptr_vector<te::st::TrajectoryDataSet>& output);
void PrintTrajectoryDataSetInfo(te::st::TrajectoryDataSet* output);

/*! 
  \brief It prints all observations of the trajectory data set (PrintTrajectoryDataSet.cpp)
*/
void PrintTrajectoryDataSet(boost::ptr_vector<te::st::TrajectoryDataSet>& output);
void PrintTrajectoryDataSet(te::st::TrajectoryDataSet* output);

/*! 
  \brief It prints information about the trajectory: its spatial and temporal extent (PrintTrajectory.cpp) 
*/
void PrintTrajectoryInfo(boost::ptr_vector<te::st::Trajectory>& output);
void PrintTrajectoryInfo(te::st::Trajectory* output);

/*! 
  \brief It prints all observations of the trajectory (PrintTrajectory.cpp) 
*/
void PrintTrajectory(boost::ptr_vector<te::st::Trajectory>& output);
void PrintTrajectory(te::st::Trajectory* output);
void PrintTrajectoryPatch(te::st::TrajectoryPatch& tjPatch);

/*! 
  \brief It prints information about the time series: its spatial and temporal extent (PrintTimeSeries.cpp) 
*/
void PrintTimeSeriesInfo(boost::ptr_vector<te::st::TimeSeries>& output);
void PrintTimeSeriesInfo(te::st::TimeSeries* output);

/*! 
  \brief It prints all observations of the time series (PrintTimeSeries.cpp) 
*/
void PrintTimeSeries(boost::ptr_vector<te::st::TimeSeries>& output);
void PrintTimeSeries(te::st::TimeSeries* output);

/*! 
  \brief It prints information about a coverage series (PrintCoverageSeries.cpp) 
*/
void PrintCoverageSeriesInfo(te::st::CoverageSeries* output);

#endif  // __TERRALIB_EXAMPLES_STEXAMPLES_INTERNAL_STEXAMPLES_H

