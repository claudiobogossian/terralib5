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
  \file NearestGeometryAtTimeInterp.h

  \brief  This file contains an interpolation function that estimates the
          nearest geometry at time of a trajectory. 
*/

#ifndef __TERRALIB_ST_INTERNAL_NEARESTGEOMETRYATTIMEINTERP_H
#define __TERRALIB_ST_INTERNAL_NEARESTGEOMETRYATTIMEINTERP_H

// Common
#include "../../../common/Singleton.h"

// ST
#include "../../Config.h"
#include "AbstractTrajectoryInterp.h"

namespace te
{
  namespace st
  {
        
     /*!
      \class NearestGeometryAtTimeInterp

      \brief  It is an interpolation function the estimates the nearest geometry 
              at a given non-observed time of a trajectory. 

      \sa AbstractTrajectoryInterp
    */
    class TESTEXPORT NearestGeometryAtTimeInterp: public AbstractTrajectoryInterp, public te::common::Singleton<NearestGeometryAtTimeInterp>
    {
      friend class te::common::Singleton<NearestGeometryAtTimeInterp>;
      
      public:
        
        te::common::AbstractParameters* getParams()  { return 0; }

        void setParams(te::common::AbstractParameters* p) { return; } 

        std::auto_ptr<te::gm::Geometry> estimate(const Trajectory& tj, te::dt::DateTime* time) const;
        
        /*! \brief Virtual destructor. */
        virtual ~NearestGeometryAtTimeInterp(); 

      protected:

        /*! \brief Constructor. */
        NearestGeometryAtTimeInterp();             
     };

  } // end namespace st
}   // end namespace te

#endif  // __TERRALIB_ST_INTERNAL_NEARESTGEOMETRYATTIMEINTERP_H

