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
  \file terralib/rp/TiePointsLocatorSURFStrategy.h
  \brief Tie-Pointsr locator SURF strategy.
 */

#ifndef __TERRALIB_RP_INTERNAL_TIEPOINTSLOCATORSURFSTRATEGY_H
#define __TERRALIB_RP_INTERNAL_TIEPOINTSLOCATORSURFSTRATEGY_H

#include "TiePointsLocatorStrategy.h"

namespace te
{
  namespace rp
  {
    /*!
      \class TiePointsLocatorSURFStrategy
      \brief Tie-points locator SURF strategy.
     */
    class TERPEXPORT TiePointsLocatorSURFStrategy : public TiePointsLocatorStrategy
    {
      friend class TiePointsLocator;
      
      public:
        
        ~TiePointsLocatorSURFStrategy();
        
      protected :
        
        bool m_isInitialized; //!< true if this instance is initialized.
        
        te::rp::TiePointsLocatorInputParameters m_inputParameters; //!< Input parameters
        
        TiePointsLocatorSURFStrategy();
            
        //overload
        bool initialize( 
          const te::rp::TiePointsLocatorInputParameters& inputParameters );
          
        //overload
        void reset();
        
        //overload
        bool findTiePoints( 
          std::vector< te::gm::GTParameters::TiePoint >& tiePoints,
          std::vector< double >& tiePointsWeights );
    };

  } // end namespace rp
}   // end namespace te

#endif

