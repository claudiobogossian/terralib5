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
    class Observation;
    class ObservationSet;
    class SpatioTemporalObservationSet;
    class ObservationSetIterator;
    class TimeSeries;
    class TimeSeriesIterator;
    class LocatedTimeSeries;
    class MovingObject;
    class MovingObjectIterator;
    class AbstractInterpolator;
    class TemporalNearestNeigInterpolator;
    class Trajectory;
	  class TrajectoryIterator;
    class Coverage;
    class RasterCoverage;
        
  } // end namespace st
}   // end namespace te

#endif // __TERRALIB_ST_INTERNAL_FW_H

