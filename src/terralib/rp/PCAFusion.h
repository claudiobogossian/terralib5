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
  \file terralib/rp/PCAFusion.h
  \brief Creation of skeleton imagems.
 */

#ifndef __TERRALIB_RP_INTERNAL_PCAFUSION_H
#define __TERRALIB_RP_INTERNAL_PCAFUSION_H

#include "Algorithm.h"
#include "../raster/Raster.h"
#include "../raster/Band.h"
#include "../raster/Interpolator.h"

#include <vector>
#include <string>
#include <map>
#include <memory>

namespace te
{
  namespace rp
  {
    /*!
      \class PCAFusion
      \brief Fusion of a low-resolution multi-band image with a high resolution image using the PCA (Principal components analysis) method.
      \details The PCA performs image fusion where the first principal component of the multi-spectral image is replaced by the histogram matched panchromatic imagery.
      \note Reference: Tania Stathaki, "Image Fusion: Algorithms and Applications", Elsevier, First edition 2008.
      \ingroup rp_fus
     */
    class TERPEXPORT PCAFusion : public Algorithm
    {
      public:
        
        /*!
          \class InputParameters
          \brief PCAFusion input parameters
         */        
        class TERPEXPORT InputParameters : public AlgorithmInputParameters
        {
          public:
            
            te::rst::Raster const* m_lowResRasterPtr; //!< Input low-resolution multi-band raster.
            
            std::vector< unsigned int > m_lowResRasterBands; //!< The low-resolution raster band indexes.
            
            te::rst::Raster const* m_highResRasterPtr; //!< Input high-resolution raster.
            
            unsigned int m_highResRasterBand; //!< Band to process from the high-resolution raster.
            
            bool m_enableProgress; //!< Enable/Disable the progress interface (default:false).
            
            bool m_enableThreadedProcessing; //!< If true, threaded processing will be performed (best with  multi-core or multi-processor systems (default:true).
            
            te::rst::Interpolator::Method m_interpMethod; //!< The raster interpolator method (default:NearestNeighbor).
            
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
          \brief PCAFusion output parameters
         */        
        class TERPEXPORT OutputParameters : public AlgorithmOutputParameters
        {
          public:
            
            std::string m_rType; //!< Output raster data source type (as described in te::raster::RasterFactory ).
            
            std::map< std::string, std::string > m_rInfo; //!< The necessary information to create the output rasters (as described in te::raster::RasterFactory). 
            
            std::auto_ptr< te::rst::Raster > m_outputRasterPtr; //!< The generated output fused raster.
            
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

        PCAFusion();
        
        ~PCAFusion();
       
        //overload
        bool execute( AlgorithmOutputParameters& outputParams ) throw( te::rp::Exception );
        
        //overload
        void reset() throw( te::rp::Exception );
        
        //overload
        bool initialize( const AlgorithmInputParameters& inputParams ) throw( te::rp::Exception );
        
        //overload
        bool isInitialized() const;

      protected:
        
        InputParameters m_inputParameters; //!< Input execution parameters.
        
        bool m_isInitialized; //!< Tells if this instance is initialized.
        
        /*!
          \brief Load resampled data from the input image.
          \param ressampledRasterPtr The loaded and ressampled raster data.
          \return true if ok, false on errors.
         */
        bool loadRessampledRaster( std::auto_ptr< te::rst::Raster >& ressampledRasterPtr ) const;         
        
        /*!
          \brief Swap the band values by the normalized high resolution raster data.
          \param pcaRaster The PCA raster.
          \param pcaRasterBandIdx The band index where the values will be swapped.
          \return true if ok, false on errors.
         */
        bool swapBandByHighResRaster( te::rst::Raster& pcaRaster, const unsigned int pcaRasterBandIdx );          
       
    };

  } // end namespace rp
}   // end namespace te

#endif

