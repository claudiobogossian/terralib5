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
  \file AbstractTrajectoryInterp.h

  \brief  This file contains an abstract class for interpolation function 
          or interpolator that estimate geometries at non-observed times
          of a trajectory.
*/

#ifndef __TERRALIB_ST_INTERNAL_ABSTRACTTRAJECTORYINTERP_H
#define __TERRALIB_ST_INTERNAL_ABSTRACTTRAJECTORYINTERP_H

// ST
#include "../../Config.h"

//STL
#include <memory>

// Forward declarations
namespace te { namespace dt { class DateTime; } }
namespace te { namespace gm { class Geometry; } }
namespace te { namespace common { class AbstractParameters; } }

namespace te
{
  namespace st
  {
     // Forward declarations
     class Trajectory;
     
     /*!
      \class AbstractTrajectoryInterp

      \brief  An abstract class for an interpolation function or interpolator 
              that estimates geometries at non-observed times of a trajectory.

      This is an abstract class for every class that implements
      interpolation functions that estimate geometries at non-observaved times
      of a trajectory.
     
      \ingroup st

      \sa Trajectory
    */
    class TESTEXPORT AbstractTrajectoryInterp 
    {
      
      public:
        
        /*!
          \brief It gets the parameters associated to the interpolator.

          \return It returns the parameters associated to the interpolator.

          \note The caller will not take the ownership of the returned pointer.
        */
        virtual te::common::AbstractParameters* getParams() = 0;

        /*!
          \brief It sets the parameters associated to the interpolator.

          \param p The parameters to be associated to the interpolator.

          \note It will take the ownership of the given pointer.
        */
        virtual void setParams(te::common::AbstractParameters* p) = 0;

        /*!
          \brief It estimates a geometry of a trajectory at a given non-observed time .

          \param tj     A trajectory used in the estimation. 
          \param time   The given non-observed time.  
          
          \return The estimated geometry.  
          
          \note It will NOT take the ownership of the given pointer.
          \note The caller will take the ownership of the output Geometry pointer.     
        */
        virtual std::auto_ptr<te::gm::Geometry> estimate(const Trajectory& tj, te::dt::DateTime* time) const = 0;
        
        /*! \brief Virtual destructor. */
        virtual ~AbstractTrajectoryInterp(); 

      protected:

        /*! \brief Constructor. */
        AbstractTrajectoryInterp();             
     };

  } // end namespace st
}   // end namespace te

#endif  // __TERRALIB_ST_INTERNAL_ABSTRACTTRAJECTORYINTERP_H

