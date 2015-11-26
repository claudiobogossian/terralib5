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
  \file terralib/rp/TiePointsLocatorStrategyParameters.h
  \brief Tie Points Locator strategy parameters.
 */

#ifndef __TERRALIB_RP_INTERNAL_TIEPOINTSLOCATORSTRATEGYPARAMETERS_H
#define __TERRALIB_RP_INTERNAL_TIEPOINTSLOCATORSTRATEGYPARAMETERS_H

#include "AlgorithmInputParameters.h"

namespace te
{
  namespace rp
  {
    /*!
      \class TiePointsLocatorStrategyParameters
      \brief TiePointsLocator strategy parameters
     */        
    class TERPEXPORT TiePointsLocatorStrategyParameters : public AlgorithmInputParameters
    {
      public:

        virtual ~TiePointsLocatorStrategyParameters();
        
      protected :
        
        TiePointsLocatorStrategyParameters();
        
        TiePointsLocatorStrategyParameters( const TiePointsLocatorStrategyParameters& );
    };

  } // end namespace rp
}   // end namespace te

#endif

