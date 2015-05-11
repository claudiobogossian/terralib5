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
  \file AbstractTimeSeriesInterp.h

  \brief  This file contains an abstract class for interpolation functions 
          or interpolators that estimate values at non-observaved times of a
          time series.
*/

#ifndef __TERRALIB_ST_INTERNAL_ABSTRACTTIMESERIESINTERP_H
#define __TERRALIB_ST_INTERNAL_ABSTRACTTIMESERIESINTERP_H

// ST
#include "../../Config.h"

//STL
#include <memory>

// Forward declarations
namespace te { namespace dt { class DateTime; class AbstractData; } }
namespace te { namespace common { class AbstractParameters; } }

namespace te
{
  namespace st
  {
     // Forward declarations
     class TimeSeries;
     
     /*!
      \class AbstractTimeSeriesInterp

      \brief  An abstract class for an interpolation function or interpolator 
              that estimate a value at non-observaved times of a time series.

      This is an abstract class for every class that implements
      interpolation functions that estimate value at non-observaved times
      of a time series.
     
      \ingroup st
      
      \sa Time Series
    */
    class TESTEXPORT AbstractTimeSeriesInterp 
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
          \brief It estimates a value at a given non-observed time of a time series.

          \param ts       A time series used in the estimation. 
          \param time     The given non-observed time.  
          
          \return The estimated value.  
          
          \note It will NOT take the ownership of the given pointer.
          \note The caller will take the ownership of the output pointer.     
        */
        virtual std::auto_ptr<te::dt::AbstractData> estimate(const TimeSeries& ts, te::dt::DateTime* time) const = 0;
        
        /*! \brief Virtual destructor. */
        virtual ~AbstractTimeSeriesInterp(); 

      protected:

        /*! \brief Constructor. */
        AbstractTimeSeriesInterp();             
     };

  } // end namespace st
}   // end namespace te

#endif  // __TERRALIB_ST_INTERNAL_ABSTRACTTIMESERIESINTERP_H

