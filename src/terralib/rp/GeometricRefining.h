/*  Copyright (C) 2001-2009 National Institute For Space Research (INPE) - Brazil.

    This file is part of the TerraLib - a Framework for building GIS enabled applications.
Mul
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
  \file terralib/rp/GeometricRefining.h
  \brief Geometric (and positioning) correction of a already geo-referenced raster using a set of reference rasters.
 */

#ifndef __TERRALIB_RP_INTERNAL_GEOMETRICREFINING_H
#define __TERRALIB_RP_INTERNAL_GEOMETRICREFINING_H

#include "Algorithm.h"
#include "FeedersRaster.h"
#include "TiePointsLocator.h"
#include "../raster/Raster.h"
#include "../raster/Interpolator.h"
#include "../geometry/GeometricTransformation.h"
#include "../geometry/Polygon.h"

#include <vector>
#include <string>
#include <memory>

namespace te
{
  namespace rp
  {
    /*!
      \class GeometricRefining
      \brief Geometric (and positioning) correction/refining of a already geo-referenced raster using a set of small reference rasters.
      \ingroup rp_gen
      \note Reference: CASTEJON, E. F. ; FONSECA, L. M. G. ; ARCANJO, J. S. . Melhoria da geometria e posicionamento de imagens orbitais de média resolução - Um experimento com dados CBERS-CCD. In: XVI Simpósio Brasileiro de Sensoriamento Remoto - SBSR, 2013, Foz do Iguaçu, PR - Brasil. Anais do XVI Simpósio Brasileiro de Sensoriamento Remoto - SBSR, 2013. 
     */
    class TERPEXPORT GeometricRefining : public Algorithm
    {
      public:
        
        /*!
          \class InputParameters
          \brief Input parameters
         */        
        class TERPEXPORT InputParameters : public AlgorithmInputParameters
        {
          public:
            
            te::rst::Raster const* m_inRasterPtr; //!< Input raster pointer.
            
            std::vector< unsigned int > m_inRasterBands2Process; //!< Bands to process from the input raster.
            
            std::vector< unsigned int > m_inRasterTPLocationBands; //!< Input raster bands used for tie-points location.
            
            FeederConstRaster* m_referenceRastersPtr; //!< A feeder of reference rasters.
            
            std::vector< std::vector< unsigned int > > m_referenceTPLocationBands; //!< Reference rasters bands used for tie-points location.
            
            bool m_enableMultiThread; //!< Enable/Disable the use of threads (default:true).
            
            bool m_enableProgress; //!< Enable/Disable the progress interface (default:false).
            
            te::rst::Interpolator::Method m_interpMethod; //!< The raster interpolator method (default:NearestNeighbor).
            
            te::rp::TiePointsLocator::InputParameters m_locatorParams; //!< The parameters used by the tie-points locator when matching each raster (feeder) against the input raster (m_RasterPtr),leave untouched to use the default values.
            
            double m_minInRasterCoveredAreaPercent; //!< The mininumum required area percent (from the input raster ) covered by tie-points - valid range [0,100] (default:25).
            
            double m_minrReferenceRasterCoveredAreaPercent; //!< The mininumum required area percent (from each reference raster ) covered by tie-points - valid range [0,100] (default:25).
            
            unsigned int m_inRasterSubSectorsFactor; //!< A positive factor used to devide the input raster area into sectors ,(efault value: 3 ( 3 x 3 = 9 sub-sectors).
            
            unsigned int m_inRasterExpectedRowError; //!< The expected row position error for the given input raster (pixels units), default value:10.
            
            unsigned int m_inRasterExpectedColError; //!< The expected column position error for the given input raster (pixels units), default value:10.
            
            int m_inRasterExpectedRowDisplacement; //!< The expected input raster row displacement (pixel units, default:0)
            
            int m_inRasterExpectedColDisplacement; //!< The expected input raster row displacement (pixel units, default:0)
            
            bool m_processAllReferenceRasters; //!< If true, all reference rasters will be processed, if false the matching can finish when minimum quality criterias are achieved (default:true).
            
            bool m_enableRasterCache; //!< If true, a internal raster data cache will be used (defaul:true).
            
            std::string m_geomTransfName; //!< The name of the geometric transformation used to ensure tie-points consistency (see each te::gm::GTFactory inherited classes to find each factory key/name, default:affine).
            
            double m_geomTransfMaxTiePointError; //!< The maximum allowed tie-point error (pixels unit, default: 2);
            
            double m_outliersRemotionAssurance; //!< The error-free selection assurance - valid range (0-1) - Use Lower values for good tie-points sets - Higher values may increase the number of iterations. Use 0-zero to let this number be automatically found (default:0.1). 
            
            unsigned int m_outliersRemotionMaxIterations; //!< The maximum number of iterations (Use 0-zero to let this number be automatically found, default:0). 
            
            double m_outputNoDataValue; //!< The pixel value used where no output raster data is avaliable (defaul:0).
            
            InputParameters();
            
            InputParameters( const InputParameters& );            
            
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
          \brief Output parameters
         */        
        class TERPEXPORT OutputParameters : public AlgorithmOutputParameters
        {
          public:
            
            std::string m_rType; //!< Output raster data source type (as described in te::raster::RasterFactory ).
            
            std::map< std::string, std::string > m_rInfo; //!< The necessary information to create the raster (as described in te::raster::RasterFactory). 
            
            std::auto_ptr< te::rst::Raster > m_outputRasterPtr; //!< A pointer the ge generated output raster (label image).
            
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

        GeometricRefining();
        
        ~GeometricRefining();
       
        //overload
        bool execute( AlgorithmOutputParameters& outputParams ) throw( te::rp::Exception );
        
        //overload
        void reset() throw( te::rp::Exception );
        
        //overload
        bool initialize( const AlgorithmInputParameters& inputParams ) throw( te::rp::Exception );
        
        bool isInitialized() const;

      protected:
        
        /*!
          \brief Reference rasters matching info.
        */  
        struct MatchingInfo
        {
          unsigned int m_referenceRasterIndex; //!< Reference raster index.
          std::vector< te::gm::GTParameters::TiePoint > m_tiePoints; //!< The tie-points generated by matching (first: input rasters lines/cols, second:world coords).
          double m_convexHullAreaPercent; //!< The tie points covered area percent [0,1].
        };
        
        GeometricRefining::InputParameters m_inputParameters; //!< Input execution parameters.
        
        bool m_isInitialized; //!< Tells if this instance is initialized.
        
        /*!
          \brief Convesion from matching infor tie-points to a vector of tie-points
          
          \param inTiePoints Input tie-points.
          
          \param outTiePoints Output tie-points.
        */          
        void convert( 
          const std::vector< MatchingInfo >& inTiePoints,
          std::vector< te::gm::GTParameters::TiePoint >& outTiePoints ) const;         
        
        /*!
          \brief Returns the tie points converx hull area.
          
          \param tiePoints Input tie-points.
          
          \param useTPSecondCoordPair If true the sencond tie-point component (te::gm::GTParameters::TiePoint::second) will be used for the area calcule, otherwize the first component will be used.
          
          \return Returns the tie points converx hull area.
        */          
        double getTPConvexHullArea( 
          const std::vector< te::gm::GTParameters::TiePoint >& tiePoints,
          const bool useTPSecondCoordPair ) const;        
          
        /*!
          \brief Returns the tie points converx hull.
          
          \param tiePoints Input tie-points.
          
          \param useTPSecondCoordPair If true the sencond tie-point component (te::gm::GTParameters::TiePoint::second) will be used for the area calcule, otherwize the first component will be used.
          
          \return Returns true if ok. false on errors.
        */          
        bool getTPConvexHull( 
          const std::vector< te::gm::GTParameters::TiePoint >& tiePoints,
          const bool useTPSecondCoordPair,
          std::auto_ptr< te::gm::Polygon >& convexHullPtr ) const;    
          
        /*!
          \brief Try to instantiate a valid geometric transformation following the user parameters.
          
          \param inTiePoints Input tie-points.
          
          \param baseGeometricTransformPtr The output base geometric transformation (with the minumim required tie-points).
          
          \param baseTransAgreementTiePoints The filtered output tie-points (non-outliers) in agreenment with the generated base transformation.
          
          \return true if OK, false on errors.
        */              
        bool getTransformation( const std::vector< MatchingInfo >& inTiePoints,
          std::auto_ptr< te::gm::GeometricTransformation >& baseGeometricTransformPtr,
          std::vector< te::gm::GTParameters::TiePoint >& baseTransAgreementTiePoints ) const;
    };

  } // end namespace rp
}   // end namespace te

#endif

