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
  \file terralib/rp/TiePointsLocatorStrategy.h
  \brief Tie-Pointsr locator strategy.
 */

#ifndef __TERRALIB_RP_INTERNAL_TIEPOINTSLOCATORSTRATEGY_H
#define __TERRALIB_RP_INTERNAL_TIEPOINTSLOCATORSTRATEGY_H

#include "Config.h"
#include "TiePointsLocatorInputParameters.h"
#include "../geometry/GTParameters.h"

#include <vector>

namespace te
{
  namespace rp
  {
    /*!
      \class TiePointsLocatorStrategy
      \brief Tie-points locator strategy.
     */
    class TERPEXPORT TiePointsLocatorStrategy
    {
      friend class TiePointsLocator;
      
      public:
        
        virtual ~TiePointsLocatorStrategy();
        
      protected :
            
        /*!
          \brief Initialize the strategy.
          \param inputParameters Input parameters.
          \return true if OK, false on errors.
         */
        virtual bool initialize( 
          const te::rp::TiePointsLocatorInputParameters& inputParameters )  = 0;        
          
        /*!
          \brief Clear all internal allocated resources and go back to the initial not-initialized state.
         */
        virtual void reset() = 0;
        
        /*!
          \brief Try to find tie-points.
          \param tiePoints The found tie-points.
          \param tiePointsWeights The found tie-points
          \return true if OK, false on errors.
         */        
        virtual bool findTiePoints( 
          std::vector< te::gm::GTParameters::TiePoint >& tiePoints,
          std::vector< double >& tiePointsWeights ) = 0;

        TiePointsLocatorStrategy();
        
      private:
        
        TiePointsLocatorStrategy( const TiePointsLocatorStrategy& );
        
        const TiePointsLocatorStrategy& operator=( const TiePointsLocatorStrategy& );
    };

  } // end namespace rp
}   // end namespace te

#endif

