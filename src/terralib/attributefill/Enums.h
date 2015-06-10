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
  \file terralib/attributefill/core/Enums.h

  \brief Enumerations related to Attribute Fill module.
*/


#ifndef __TERRALIB_ATTRIBUTEFILL_CORE_INTERNAL_ENUMS_H
#define __TERRALIB_ATTRIBUTEFILL_CORE_INTERNAL_ENUMS_H

namespace te
{
  namespace attributefill
  {
    /*!
      \enum OperationType

      \brief Define grouping operations type.
    */
    enum OperationType
    {
      VALUE,
      MIN_VALUE,
      MAX_VALUE,
      MEAN,
      SUM,
      COUNT,
      VALID_COUNT,
      STANDARD_DEVIATION,
      VARIANCE,
      SKEWNESS,
      KURTOSIS,
      AMPLITUDE,
      MEDIAN,
      VAR_COEFF,
      MODE,
      HIGHEST_OCCURRENCE,
      HIGHEST_INTERSECTION,
      PERCENT_CLASS,
      MIN_DISTANCE,
      PRESENCE,
      WEIGHTED,
      WEIGHTED_SUM,
      PERCENT_EACH_CLASS,
      PERCENT_TOTAL_AREA
    };
  }
}

#endif // __TERRALIB_ATTRIBUTEFILL_CORE_INTERNAL_ENUMS_H