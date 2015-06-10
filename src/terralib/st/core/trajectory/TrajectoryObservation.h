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
  \file TrajectoryObservation.h

  \brief This file contains a typedef that defines observations of trajectory.
 */

#ifndef __TERRALIB_ST_INTERNAL_TRAJECTORYOBSERVATION_H
#define __TERRALIB_ST_INTERNAL_TRAJECTORYOBSERVATION_H

//TerraLib
#include "../../../datatype/DateTime.h"
#include "../../../geometry/Geometry.h"

//ST
#include "../../Config.h"
#include "../../Utils.h"

//STL
#include <vector>
#include <map>

// Boost
#include <boost/shared_ptr.hpp>

// Forward declarations
namespace te { namespace dt { class DateTime; class DateTimePeriod; } }
namespace te { namespace gm { class Geometry; class Envelope; } }

namespace te
{
  namespace st
  {
    // Typedef
    typedef std::pair<te::dt::DateTimeShrPtr, te::gm::GeometryShrPtr>           TrajectoryObservation;
    typedef std::map<te::dt::DateTimeShrPtr, te::gm::GeometryShrPtr, CompareShrDateTime>        TrajectoryObservationSet;   

   } // end namespace st
}   // end namespace te

#endif  // __TERRALIB_ST_INTERNAL_TRAJECTORYOBSERVATION_H

