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
  \file terralib/sa/Enums.h

  \brief Enumerations related to Spatial Analysis module.
*/


#ifndef __TERRALIB_SA_ENUMS_H
#define __TERRALIB_SA_ENUMS_H

namespace te
{
  namespace sa
  {
    /*!
      \enum GPMConstructorStrategyType

      \brief Strategies to construc a GPM.
    */
    enum GPMConstructorStrategyType
    {
      AdjacencyStrategy,
      DistanceStrategy
    };

    /*!
      \enum GPMWeightsStrategyType

      \brief Strategies to calculate the weight for a GPM.
    */
    enum GPMWeightsStrategyType
    {
      NoWeightStrategy,
      InverseDistanceStrategy,
      SquaredInverseDistanceStrategy
    };

    /*!
      \enum KernelFunction

      \brief Types of kernel function.
    */
    enum KernelFunctionType
    {
      Quartic,
      Normal,
      Triangular,
      Uniform,
      Negative_Exp
    };

    /*!
      \enum KernelFunction

      \brief Types of kernel estimation.
    */
    enum KernelEstimationType
    {
      Density,
      Spatial_Moving_Average,
      Probability
    };

    /*!
      \enum KernelCombinationType

      \brief Types of kernel combination.
    */
    enum KernelCombinationType
    {
      Ratio,
      Log_Ratio,
      Abs_Difference,
      Relative_Difference,
      Abs_Sum,
      Relative_Sum
    };
    
    /*!
      \enum KernelOutputType

      \brief Defines the kernel result storage mode.
    */
    enum KernelOutputType
    {
      Grid,
      Attribute
    };
  }
}

#endif // __TERRALIB_SA_ENUMS_H
