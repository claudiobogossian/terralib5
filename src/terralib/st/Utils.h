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
  \file Utils.h
   
  \brief Utility functions for ST module.  
 */

#ifndef __TERRALIB_ST_INTERNAL_UTILS_H
#define __TERRALIB_ST_INTERNAL_UTILS_H

// TerraLib
#include "../datatype/Enums.h"

// ST
#include "Config.h"

// Forward declarations
namespace te { namespace dt { class AbstractData; } }

namespace te
{
  namespace st
  {
    // Forward declarations
    class TimeSeries;
    
    /*!
      \brief It normalizes a given time series, considering a given date and time resolution.
          
      During the normalization, it uses the time series internal interpolator in order to 
      estimate values to be associated to non-observed date and time.
          
      \param ts     The given time series.
      \param tRes   The date and time resolution to be considered in the normalization.

      \return A new normalized time series.
         
      \note The caller WILL take the ownership of the returned pointer.
    */
    /*TESTEXPORT*/ te::st::TimeSeries* Normalize(TimeSeries* ts, te::dt::DateTimeResolution tRes);

    /*!
      \brief It normalizes a given time series, considering a given date and time resolution.
          
      During the normalization, it uses a default value, passed by parameter, 
      to be associated to non-observed date and time.
          
      \param ts       The given time series.
      \param tRes     The date and time resolution to be considered in the normalization.
      \param defvalue The default value to be used at non-observed date and time.

      \return A new normalized time series.
         
      \note The caller WILL take the ownership of the returned pointer.
    */
    /*TESTEXPORT*/ te::st::TimeSeries* Normalize(TimeSeries* ts, te::dt::DateTimeResolution tRes, te::dt::AbstractData* defvalue);    

  } // end namespace st
}   // end namespace te

#endif  // __TERRALIB_ST_INTERNAL_UTILS_H
