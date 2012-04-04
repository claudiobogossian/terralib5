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
  \file AbstractInterpolator.h

  \brief This file contains an abstract class for interpolation function or interpolator.
*/

#ifndef __TERRALIB_ST_INTERNAL_ABSTRACTINTERPOLATOR_H
#define __TERRALIB_ST_INTERNAL_ABSTRACTINTERPOLATOR_H

// ST
#include "../Config.h"

// Forward declarations
namespace te { namespace dt { class AbstractData; } }

namespace te
{
  namespace st
  {
     // Forward declarations
     class AbstractInterpolatorParams;
     class ObservationSet;
     
     /*!
      \class AbstractInterpolator

      \brief An abstract class for an interpolation function or interpolator.

      This is an abstract class for every class that implements
      interpolation functions or interpolators. 
      An interpolation function is a procedure that, given a set of discrete 
      observations and some specific parameters, is able to estimate a value in any 
      non-observed position in time, in space or in space and time.  

      \sa AbstractInterpolatorParams
    */
    class TESTEXPORT AbstractInterpolator 
    {
      
      public:
        
        /*!
          \brief It gets the parameters associated to the interpolator.

          \return It returns the parameters associated to the interpolator.

          \note The caller will not take the ownership of the returned pointer.
        */
        virtual AbstractInterpolatorParams* getParams() = 0;

        /*!
          \brief It sets the parameters associated to the interpolator.

          \param p The parameters to be associated to the interpolator.

          \note It will take the ownership of the given pointer.
        */
        virtual void setParams(AbstractInterpolatorParams* p) = 0;

        /*!
          \brief It estimates the values associated to a given non-observed position, in an observation set.

          \param obSet    An observation set used in the estimation. 
          \param position A non-observed position. 
          \param values   The output estimated values. (output)  
          
          \note The caller will take the ownership of the output pointer (values).     
        */
        virtual void getValues(ObservationSet* obSet, te::dt::AbstractData* position, std::vector<te::dt::AbstractData*>& values) = 0;
        

        /*! \brief Virtual destructor. */
        virtual ~AbstractInterpolator() {}; 

      protected:

        /*! \brief Constructor. */
        AbstractInterpolator() { };             
     };

  } // end namespace st
}   // end namespace te

#endif  // __TERRALIB_ST_INTERNAL_ABSTRACTINTERPOLATOR_H

