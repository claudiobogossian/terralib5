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
  \file terralib/classification/core/Enums.h

  \brief Enumerations related to the Classification module.
*/


#ifndef __TERRALIB_CLASSIFICATION_CORE_INTERNAL_ENUMS_H
#define __TERRALIB_CLASSIFICATION_CORE_INTERNAL_ENUMS_H

namespace te
{
  namespace cl
  {
    /*!
      \enum StatisticalSummary

      \brief Define grouping functions type.
    *
    enum StatisticalSummary
    {
      MIN_VALUE,          //!< Minimum value.
      MAX_VALUE,          //!< Maximum value.
      MEAN,               //!< Mean.
      SUM,                //!< Sum of values.
      COUNT,              //!< Total number of values.
      VALID_COUNT,        //!< Total not null values.
      STANDARD_DEVIATION, //!< Standard deviation.
      VARIANCE,           //!< Variance.
      SKEWNESS,           //!< Skewness.
      KURTOSIS,           //!< Kurtosis.
      AMPLITUDE,          //!< Amplitude.
      MEDIAN,             //!< Median.
      VAR_COEFF,          //!< Coefficient variation.
      MODE                //!< Mode.
    };*/
  }
}

#endif // __TERRALIB_CLASSIFICATION_CORE_INTERNAL_ENUMS_H
