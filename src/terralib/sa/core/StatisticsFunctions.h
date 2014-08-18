/*  Copyright (C) 2008-2013 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/sa/core/StatisticsFunctions.h

  \brief Functions used in statistics operations.

  \reference Methods adapted from TerraLib4.
*/

#ifndef __TERRALIB_SA_INTERNAL_STATISTICSFUNCTIONS_H
#define __TERRALIB_SA_INTERNAL_STATISTICSFUNCTIONS_H

// TerraLib
#include "../Config.h"
#include "KernelFunctions.h"

// STL
#include <string>

namespace te
{
  namespace sa
  {
    // Forward declaration
    class GeneralizedProximityMatrix;

    /*!
      \brief Function used to calculate sum of a specific attribute from a gpm

      \param gpm  Pointer to the gpm
      \param attrIdx Attribute index used to calculate the sum.
      
      \return Return the sum value.
    */
    TESAEXPORT double Sum(te::sa::GeneralizedProximityMatrix* gpm, int attrIdx);

    /*!
      \brief Function used to calculate sum of the intensity value from a kernel map

      \param kMap  Referente to a kernel map.
      
      \return Return the sum value.
    */
    TESAEXPORT double Sum(te::sa::KernelMap& kMap);

    /*!
      \brief Function used to calculate mean (first moment)  of a specific attribute from a gpm

      \param gpm  Pointer to the gpm
      \param attrIdx Attribute index used to calculate the mean.
      
      \return Return the mean value.
    */
    TESAEXPORT double FirstMoment(te::sa::GeneralizedProximityMatrix* gpm, int attrIdx);

    /*!
      \brief Function used to calculate variance (second moment)  of a specific attribute from a gpm

      \param gpm  Pointer to the gpm
      \param attrIdx Attribute index used to calculate the variance.
      
      \return Return the variance value.
    */
    TESAEXPORT double SecondMoment(te::sa::GeneralizedProximityMatrix* gpm, int attrIdx, double mean);

  } // end namespace sa
}   // end namespace te

#endif  // __TERRALIB_SA_INTERNAL_STATISTICSFUNCTIONS_H

