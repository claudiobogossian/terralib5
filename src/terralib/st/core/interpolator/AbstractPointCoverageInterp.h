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
  \file AbstractPointCoverageInterp.h

  \brief  This file contains an abstract class for interpolation function 
          or interpolator that estimate values at non-observaved location within
          a PointCoverage.
*/

#ifndef __TERRALIB_ST_INTERNAL_ABSTRACTPOINTCOVERAGEINTERP_H
#define __TERRALIB_ST_INTERNAL_ABSTRACTPOINTCOVERAGEINTERP_H

// ST
#include "../../Config.h"

//STL
#include <memory>

// Forward declarations
namespace te { namespace dt { class AbstractData; } }
namespace te { namespace common { class AbstractParameters; } }
namespace te { namespace gm { class Point; } }

namespace te
{
  namespace st
  {
     // Forward declarations
    class PointCoverage;
    
     /*!
      \class AbstractPointCoverageInterp

      \brief  An abstract class for interpolation function 
              or interpolator that estimate values at non-observaved location within
              a PointCoverage.

      \ingroup st

      \sa PointCoverage
    */
    class TESTEXPORT AbstractPointCoverageInterp 
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
          \brief It estimates a value at a given non-observed location, in a PointCoverage.

          \param cv  A PointCoverage 
          \param p   The index of the PointCoverage property to be estimated 
          \param l   A given non-observed location.  
          
          \return The estimated value.  
          
          \note The caller will take the ownership of the output Geometry pointer.     
        */
        virtual std::auto_ptr<te::dt::AbstractData> estimate(const PointCoverage& cv, unsigned int p, const te::gm::Point& l) = 0;
        

        /*! \brief Virtual destructor. */
        virtual ~AbstractPointCoverageInterp(); 

      protected:

        /*! \brief Constructor. */
        AbstractPointCoverageInterp();             
     };

  } // end namespace st
}   // end namespace te

#endif // __TERRALIB_ST_INTERNAL_ABSTRACTPOINTCOVERAGEINTERP_H

