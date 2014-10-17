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
  \file terralib/rp/TiePointsLocatorInputParameters.h
  \brief TiePointsLocator input parameters.
 */

#ifndef __TERRALIB_RP_INTERNAL_TIEPOINTSLOCATORINPUTPARAMETERS_H
#define __TERRALIB_RP_INTERNAL_TIEPOINTSLOCATORINPUTPARAMETERS_H

#include "Config.h"
#include "Exception.h"
#include "AlgorithmInputParameters.h"
#include "../raster/Raster.h"
#include "../raster/Interpolator.h"

#include <vector>
#include <string>

namespace te
{
  namespace rp
  {
    /*!
      \class TiePointsLocatorInputParameters
      \brief TiePointsLocator input parameters
     */        
    class TERPEXPORT TiePointsLocatorInputParameters : public AlgorithmInputParameters
    {
      public:
        
        /**
        * \name Global parameters
        */
        /**@{*/            
        
        /*! \enum The strategy used to locate tie points.*/
        enum InteresPointsLocationStrategyType
        {
          InvalidStrategyT = 0, /*!< Invalid strategy. */
          MoravecStrategyT = 1, /*!<  Modified Moravec Interest Operator based image area matching - Reference: Extration of GCP chips from GeoCover using Modified Moravec Interest Operator (MMIO) algorithm, The United States Geological Survey. */
          SurfStrategyT = 2 /*!<  SURF based image area matching - Reference: SURF: Speeded Up Robust Features, Herbert Bay. */
        };

        InteresPointsLocationStrategyType m_interesPointsLocationStrategy; //!< The strategy used to locate interest points (default:SurfStrategyT).
        
        te::rst::Raster const* m_inRaster1Ptr; //!< Input raster 1.
        
        te::rst::Raster const* m_inMaskRaster1Ptr; //!< Optional one band input mask raster 1 (tie-points will not be generated inside mask image areas marked with zeroes).
        
        std::vector< unsigned int > m_inRaster1Bands; //!< Bands to be used from the input raster 1.
        
        unsigned int m_raster1TargetAreaLineStart; //!< The first line of the raster 1 target area to process (default:0 - The entire raster will be considered).
        
        unsigned int m_raster1TargetAreaColStart; //!< The first column of the raster 2 target area to process (default:0 - The entire raster will be considered).

        unsigned int m_raster1TargetAreaWidth; //!< The raster 1 target area width (default:0 - The entire raster will be considered).
        
        unsigned int m_raster1TargetAreaHeight; //!< The raster 1 target area height (default:0 - The entire raster will be considered).
        
        te::rst::Raster const* m_inRaster2Ptr; //!< Input raster 2.
        
        te::rst::Raster const* m_inMaskRaster2Ptr; //!< Optional one band input mask raster 2 (tie-points will not be generated inside mask image areas marked with zeroes).
        
        std::vector< unsigned int > m_inRaster2Bands; //!< Bands to be used from the input raster 2.
        
        unsigned int m_raster2TargetAreaLineStart; //!< The first line of the raster 2 target area to process (default:0 - The entire raster will be considered).
        
        unsigned int m_raster2TargetAreaColStart; //!< The first column of the raster 2 target area to process (default:0 - The entire raster will be considered).

        unsigned int m_raster2TargetAreaWidth; //!< The raster 2 target area width (default:0 - The entire raster will be considered).
        
        unsigned int m_raster2TargetAreaHeight; //!< The raster 2 target area height (default:0 - The entire raster will be considered).
        
        bool m_enableMultiThread; //!< Enable/Disable the use of multi-threads (default:true).
        
        bool m_enableProgress; //!< Enable/Disable the progress interface (default:false).
        
        unsigned int m_maxTiePoints; //!< The maximum number of tie-points to generate (default=0 - Automatically found).
        
        double m_pixelSizeXRelation; //!< The pixel resolution relation m_pixelSizeXRelation = raster1_pixel_res_x / raster2_pixel_res_x (default=1.0).
        
        double m_pixelSizeYRelation; //!< The pixel resolution relation m_pixelSizeYRelation = raster1_pixel_res_y / raster2_pixel_res_y (default=1.0).
        
        std::string m_geomTransfName; //!< The name of the geometric transformation used to ensure tie-points consistency (see each te::gm::GTFactory inherited classes to find each factory key/name, default:Affine).
        
        double m_geomTransfMaxError; //!< The maximum allowed transformation error (pixel units, default:2).
        
        unsigned int m_maxR1ToR2Offset; //!< The maximum offset (pixels units) between a raster 1 point end the respective raster 2 point (default:0 - no offset restriction).
        
        bool m_enableGeometryFilter; //!< Enable/disable the geometry filter/outliers remotion (default:true).
        
        double m_geometryFilterAssurance; //!< Geometry assurance (the error-free selection percent assurance) - valid range (0-1) - default:0.1 - Use 0-zero to let this number be automatically found.
        
        double m_rastersRescaleFactor; //!< Global rescale factor to apply to all input rasters (default:1, valid range: non-zero positive values).
        
        te::rst::Interpolator::Method m_interpMethod; //!< The raster interpolator method (default:NearestNeighbor).
        
        //@}
        
        /**
        * \name Moravec parameters
        */
        /**@{*/            
        
        unsigned int m_moravecCorrelationWindowWidth; //!< The correlation window width used to correlate points between the images (minimum 3, default: 11).
        
        unsigned int m_moravecWindowWidth; //!< The Moravec window width used to locate canditate tie-points (minimum 3, default: 5 ).
        
        unsigned int m_moravecGaussianFilterIterations; //!< The number of noise Gaussin iterations, when applicable (used to remove image noise, zero will disable the Gaussian Filter, default:1).
        
        double m_moravecMinAbsCorrelation; //!< The minimum acceptable absolute correlation value when matching features (when applicable),  default:0.5, valid range: [0,1].
        
        //@}
        
        /**
        * \name SURF parameters
        */
        /**@{*/
        
        unsigned int m_surfScalesNumber; //!< The number of sub-sampling scales to generate, when applicable (default:4, minimum:3).
        
        unsigned int m_surfOctavesNumber; //!< The number of octaves to generate, when applicable (default: 3, minimum:1).
        
        double m_surfMaxNormEuclideanDist; //!< The maximum acceptable euclidean distance when matching features (when applicable),  default:0.5, valid range: [0,1].
        
        //@}
        
        TiePointsLocatorInputParameters();
        
        TiePointsLocatorInputParameters( const TiePointsLocatorInputParameters& );
        
        ~TiePointsLocatorInputParameters();
        
        //overload
        void reset() throw( te::rp::Exception );
        
        //overload
        const  TiePointsLocatorInputParameters& operator=( const TiePointsLocatorInputParameters& params );
        
        //overload
        AbstractParameters* clone() const;
    };

  } // end namespace rp
}   // end namespace te

#endif

