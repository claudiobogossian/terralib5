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
#include "../geometry/GeometricTransformation.h"

#include <vector>
#include <string>

namespace te
{
  namespace rp
  {
    /*!
      \class GeometricRefining
      \brief Geometric (and positioning) correction/refining of a already geo-referenced raster using a set of reference rasters.
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
            
            te::rst::Raster const* m_RasterPtr; //!< Input raster pointer.
            
            std::vector< unsigned int > m_inRasterBands2Process; //!< Bands to process from the input raster.
            
            std::vector< unsigned int > m_inRasterTPLocationBands; //!< Input raster bands used for tie-points location.
            
            FeederConstRaster* m_referenceRastersPtr; //!< A feeder of reference rasters.
            
            std::vector< std::vector< unsigned int > > m_referenceTPLocationBands; //!< Reference rasters bands used for tie-points location.
            
            bool m_enableMultiThread; //!< Enable/Disable the use of threads (default:true).
            
            bool m_enableProgress; //!< Enable/Disable the progress interface (default:false).
            
            te::rp::TiePointsLocator::InputParameters m_locatorParams; //!< The parameters used by the tie-points locator when matching each raster (feeder) against the input raster (m_RasterPtr),leave untouched to use the default values.
            
            double m_minRequiredTiePointsCoveredAreaPercent; //!< The mininumum required area percent (from the input raster ) covered by tie-points - valid range [0,100] (default:0).
            
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
            
            std::vector< te::gm::GTParameters::TiePoint > m_tiePoints; //!< The generated tie-points (te::gm::GTParameters::TiePoint::first are raster lines/columns ) and their respective coordinates under the chosen SRS (te::gm::GTParameters::TiePoint::second)
            
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
        
        GeometricRefining::InputParameters m_inputParameters; //!< Input execution parameters.
        
        bool m_isInitialized; //!< Tells if this instance is initialized.
        
        /*!
          \brief Returns the tie points converx hull area.
          
          \param tiePoints Input tie-points.
          
          \param useTPSecondCoordPair If true the sencond tie-point component (te::gm::GTParameters::TiePoint::second) will be used for the area calcule, otherwize the first component will be used.
          
          \return Returns the tie points converx hull area.
        */          
        double getTPConvexHullArea( 
          const std::vector< te::gm::GTParameters::TiePoint >& tiePoints,
          const bool useTPSecondCoordPair ) const;        
    };

  } // end namespace rp
}   // end namespace te

#endif

