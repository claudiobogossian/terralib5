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
  \file PointCoverageObservation.h

  \brief This file contains a class to represent an observation of a PointCoverage.

  TO DO: indexar o AbstractData também!...
*/

#ifndef __TERRALIB_ST_INTERNAL_POINTCOVERAGEOBSERVATION_H
#define __TERRALIB_ST_INTERNAL_POINTCOVERAGEOBSERVATION_H

//TerraLib
#include "../../../geometry/Point.h"
#include "../../../datatype/AbstractData.h"

//ST
#include "../../Config.h"

//STL
#include <vector>
#include <map>

// Boost
#include <boost/shared_ptr.hpp>
#include <boost/ptr_container/ptr_vector.hpp>

namespace te
{
  namespace st
  {
    
    //Typedef
    typedef std::pair<te::gm::Point, boost::ptr_vector<te::dt::AbstractData> >   PointCoverageItem;
    typedef boost::shared_ptr< PointCoverageItem >                               PointCoverageObservation;
    typedef std::vector< PointCoverageObservation >                              PointCoverageObservationSet;
   
  } // end namespace st
}   // end namespace te

#endif  // __TERRALIB_ST_INTERNAL_POINTCOVERAGEOBSERVATION_H

