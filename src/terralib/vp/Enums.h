/*  Copyright (C) 2008-2011 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/vp/Enums.h

  \brief Enumerations related to Vector Processing module.
*/


#ifndef __TERRALIB_VP_ENUMS_H
#define __TERRALIB_VP_ENUMS_H

namespace te
{
  namespace vp
  {

    /*!
      \enum MemoryUse

      \brief Define grouping functions type.
    */
    enum GroupingFunctionsType
    {
      MIN_VALUE,          //!< Minimum value.
      MAX_VALUE,          //!< Maximum value.
      MEAN,               //!< Mean.
      SUM,                //!< Sum of values.
      COUNT,              //!< Total number of values.
      VALID_COUNT,        //!< Total not null values.
      STANDARD_DEVIATION, //!< Standard deviation.
      KERNEL,             //!< Kernel.
      VARIANCE,           //!< Variance.
      SKEWNESS,           //!< Skewness.
      KURTOSIS,           //!< Kurtosis.
      AMPLITUDE,          //!< Amplitude.
      MEDIAN,             //!< Median.
      VAR_COEFF,          //!< Coefficient variation.
      MODE                //!< Mode.
    };


    /*!
      \enum MemoryUse

      \brief Define possible states for memory use.
    */
    enum MemoryUse
    {
      WHOLE_MEM,      //!< Whole memory use.
      PARTIALLY_MEM,  //!< Partially memory use.
      LOW_MEM         //!< Low memory use.
    };
  }
}

#endif // __TERRALIB_VP_ENUMS_H