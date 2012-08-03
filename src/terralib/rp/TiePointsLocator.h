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
#include "../geometry/GTParameters.h"

#include <vector>
#include <string>

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
            enum InteresPointsLocationStrategyType
            {
              InvalidStrategyT = 0, /*!< Invalid strategy. */
              MoravecStrategyT = 1, /*!<  Modified Moravec Interest Operator based image area matching. */
            };

            InteresPointsLocationStrategyType m_interesPointsLocationStrategy; //!< The strategy used to locate interest points (default:MoravecStrategyT).
            
            te::rst::Raster const* m_inRaster1Ptr; //!< Input raster 1.
            
            te::rst::Raster const* m_inMaskRaster1Ptr; //!< Optional input mask raster 1 (tie-points will not be generated inside mask image areas marked with zeroes).
            
            std::vector< unsigned int > m_inRaster1Bands; //!< Bands to be used from the input raster 1.
            
            unsigned int m_raster1TargetAreaLineStart; //!< The first target rectangle line (default:0 - The entire raster will be considered).
            
            unsigned int m_raster1TargetAreaColStart; //!< The first target rectangle column (default:0 - The entire raster will be considered).

            unsigned int m_raster1TargetAreaWidth; //!< The target rectangle width (default:0 - The entire raster will be considered).
            
            unsigned int m_raster1TargetAreaHeight; //!< The target rectangle height (default:0 - The entire raster will be considered).
            
            te::rst::Raster const* m_inRaster2Ptr; //!< Input raster 2.
            
            te::rst::Raster const* m_inMaskRaster2Ptr; //!< Optional input mask raster 2 (tie-points will not be generated inside mask image areas marked with zeroes).
            
            std::vector< unsigned int > m_inRaster2Bands; //!< Bands to be used from the input raster 2.
            
            unsigned int m_raster2TargetAreaLineStart; //!< The first target rectangle line (default:0 - The entire raster will be considered).
            
            unsigned int m_raster2TargetAreaColStart; //!< The first target rectangle column (default:0 - The entire raster will be considered).

            unsigned int m_raster2TargetAreaWidth; //!< The target rectangle width (default:0 - The entire raster will be considered).
            
            unsigned int m_raster2TargetAreaHeight; //!< The target rectangle height (default:0 - The entire raster will be considered).
            
            bool m_enableMultiThread; //!< Enable/Disable the use of multi-threads (default:true).
            
            unsigned int m_maxTiePoints; //!< The maximum number of tie-points to generate (default=1000).
            
            double m_pixelSizeXRelation; //!< The pixel resolution relation m_pixelSizeXRelation = raster1_pixel_res_x / raster2_pixel_res_x (default=1.0).
            
            double m_pixelSizeYRelation; //!< The pixel resolution relation m_pixelSizeYRelation = raster1_pixel_res_y / raster2_pixel_res_y (default=1.0).
            
            std::string m_geomTransfName; //!< The name of the geometric transformation used to ensure tie-points consistency (default:Affine).
            
            double m_geomTransfMaxError; //!< The maximum allowed transformation error (pixel units, default:1).
            
            unsigned int m_correlationWindowWidth; //!< The correlation window width used to correlate points between the images (Must be an odd number, minimum 3, default: 21).
            
            unsigned int m_moravecWindowWidth; //!< The Moravec window width used to locate canditate tie-points (Must be an odd number, minimum 11, default: 11 ).
          
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
            
            std::vector< te::gm::GTParameters::TiePoint > m_tiePoints; //!< The generated tie-pionts (te::gm::GTParameters::TiePoint::first are raster 1 line/column indexes, te::gm::GTParameters::TiePoint::second are raster 2 line/column indexes).
            
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

