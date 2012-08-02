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
#include "../raster/Raster.h"

namespace te
{
  namespace rp
  {
    /*!
      \class TiePointsLocator
      \brief Tie points locator.
      \details Designed to find tie-points inside raster areas.
      \ingroup MatchingAlgorithms
     */
    class TERPEXPORT TiePointsLocator : public Algorithm
    {
      public:
        
        /*!
          \class InputParameters
          \brief TiePointsLocator input parameters
         */        
        class TERPEXPORT InputParameters : public AlgorithmInputParameters
        {
          public:
            
            /*! \enum The strategy used to locate tie points.*/
            enum StrategyType
            {
              InvalidStrategyT = 0, /*!< Invalid contrast. */
              MoravecStrategyT = 1, /*!< The histogram range will be changed to the supplied min/max range ( linear function ). */
            };

            StrategyType m_strategyType; //!< The strategy used to locate tie points.
            
            te::rst::Raster const* m_inRaster1Ptr; //!< Input raster.
            
            te::rst::Raster const* m_inRaster2Ptr; //!< Input raster.
            
            std::vector< unsigned int > m_inRaster1Bands; //!< Bands to be used from the input raster 1.
            
            std::vector< unsigned int > m_inRaster2Bands; //!< Bands to be used from the input raster 2.
          
            InputParameters();
            
            ~InputParameters();
            
            //overload
            void reset() throw( te::rp::Exception );
            
            //overload
            const  InputParameters& operator=( const InputParameters& params );
            
            //overload
            AbstractParameters* clone() const;
        };
        
        /*!
          \class OutputParameters
          \brief TiePointsLocator output parameters
         */        
        class TERPEXPORT OutputParameters : public AlgorithmOutputParameters
        {
          public:
            
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
        TiePointsLocator::OutputParameters* m_outputParametersPtr; //!< TiePointsLocator input execution parameters.
        
        bool m_isInitialized; //!< Tells if this instance is initialized.

    };

  } // end namespace rp
}   // end namespace te

#endif

