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
  \file terralib/rp/TiePointsLocator.h
  \brief TiePointsLocator locator.
 */

#ifndef __TERRALIB_RP_INTERNAL_TIEPOINTSLOCATOR_H
#define __TERRALIB_RP_INTERNAL_TIEPOINTSLOCATOR_H

#include "Algorithm.h"
#include "TiePointsLocatorInputParameters.h"
#include "TiePointsLocatorStrategy.h"
#include "../geometry/GeometricTransformation.h"
#include "../geometry/GTParameters.h"

#include <vector>
#include <memory>

namespace te
{
  namespace rp
  {
    /*!
      \class TiePointsLocator
      \brief Tie points locator.
      \details Designed to find tie-points inside raster areas.
      \ingroup rp_match
     */
    class TERPEXPORT TiePointsLocator : public Algorithm
    {
      public:
        
        /*!
          \class InputParameters
          \brief TiePointsLocator input parameters
         */     
        typedef TiePointsLocatorInputParameters InputParameters;
        
        /*!
          \class OutputParameters
          \brief TiePointsLocator output parameters
         */        
        class TERPEXPORT OutputParameters : public AlgorithmOutputParameters
        {
          public:
            
            std::vector< te::gm::GTParameters::TiePoint > m_tiePoints; //!< The generated tie-points (te::gm::GTParameters::TiePoint::first are raster 1 line/column indexes, te::gm::GTParameters::TiePoint::second are raster 2 line/column indexes).
            
            std::auto_ptr< te::gm::GeometricTransformation > m_transformationPtr; //!< The generated geometric transformation with the base mininum required tie-points set ( depending on the tie-points, a valid transformation may not exist, in this case, this pointer will be null).
            
            OutputParameters();
            
            OutputParameters( const OutputParameters& );
            
            ~OutputParameters();
            
            //overload
            void reset() throw( te::rp::Exception );
            
            //overload
            const  OutputParameters& operator=( const OutputParameters& params );
            
            //overload
            AbstractParameters* clone() const;
        };        

        TiePointsLocator();
        
        ~TiePointsLocator();
       
        //overload
        bool execute( AlgorithmOutputParameters& outputParams ) throw( te::rp::Exception );
        
        //overload
        void reset() throw( te::rp::Exception );
        
        //overload
        bool initialize( const AlgorithmInputParameters& inputParams ) throw( te::rp::Exception );
        
        bool isInitialized() const;

      protected:
        
        TiePointsLocator::InputParameters m_inputParameters; //!< TiePointsLocator input execution parameters.
        
        bool m_isInitialized; //!< Tells if this instance is initialized.
        
        /*!
          \brief Convert matched interest points to tie-points
          \param matchedInterestPoints The matched interest points.
          \param tiePoints The generated tie-points.
          \param tiePointsWeights The generated tie-points weights.
         */        
        void convertMatchedInterestPoints2TiePoints(
          const te::rp::TiePointsLocatorStrategy::MatchedInterestPointsSetT& matchedInterestPoints,
          std::vector< te::gm::GTParameters::TiePoint >& tiePoints,
          std::vector< double >& tiePointsWeights );

    };

  } // end namespace rp
}   // end namespace te

#endif

