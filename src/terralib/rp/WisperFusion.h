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
  \file terralib/rp/WisperFusion.h
  \brief WiSpeR fusion.
 */

#ifndef __TERRALIB_RP_INTERNAL_WISPERFUSION_H
#define __TERRALIB_RP_INTERNAL_WISPERFUSION_H

#include "Algorithm.h"
#include "Matrix.h"
#include "../raster/Raster.h"
#include "../raster/Interpolator.h"
#include "Functions.h"
#include "SpectralResponseFunctions.h"

#include <vector>
#include <map>

namespace te
{
  namespace rp
  {
    /*!
      \class WisperFusion
      
      \brief Fusion of a low-resolution multi-band image with a high resolution image using the WiSpeR method.
      
      \note This algorithm expects both images to be aligned over the same geographic region. No reprojection or crop operations are performed.
      
      \note Reference: OTAZU, X.; GONZALEZ-AUDICANA, M.; FORS, O.; NUNEZ, J. Introduction of sensor spectral response into image fusion methods. application to wavelet-based methods. IEEE Transactions on Geoscience and Remote Sensing, v. 43, n. 10, p. 2376{2385, Oct. 2005. 21, 26, 36, 40.
        
      \note Reference: GONZALEZ-AUDICANA, M.; OTAZU, X.; FORS, O.; SECO, A. Comparison between Mallat's and the 'a trous' discrete wavelet transform based algorithms for the fusion of multispectral and panchromatic images. International Journal of Remote Sensing, Taylor & Francis, v. 26, n. 3, p. 595{614, 2005. 33.
        
      \note The spectral response functions must be in the form std::pair< double, double >( wavelength , response );
        
      \ingroup rp_fus
     */
    class TERPEXPORT WisperFusion : public Algorithm
    {
      public:
        
        /*!
          \class InputParameters
          \brief WisperFusion input parameters
         */        
        class TERPEXPORT InputParameters : public AlgorithmInputParameters
        {
          public:
            
            te::rst::Raster const* m_lowResRasterPtr; //!< Input low-resolution multi-band raster.
            
            std::vector< unsigned int > m_lowResRasterBands; //!< Bands to processe from the low resolution raster.
            
            std::vector< te::rp::srf::SensorType > m_lowResRasterBandSensors; //!< The low resolution bands sensors.
            
            std::vector< std::map< double, double > > m_lowResRasterBandsSRFs; //!< An optional vector of user supplied Spectral Response Functions for each low resolution band (The wavelength unit: Nanometers (nm) ).
            
            te::rst::Raster const* m_highResRasterPtr; //!< Input high-resolution raster.
            
            unsigned int m_highResRasterBand; //!< Band to process from the high-resolution raster.
            
            te::rp::srf::SensorType m_hiResRasterBandSensor; //!< The high resolution band sensor.
            
            std::map< double, double > m_hiResRasterBandsSRFs; //!< An optional vector of user supplied Spectral Response Functions for the high resolution band (The wavelength unit: Nanometers (nm) ).            
            
            unsigned int m_hiResRasterWaveletLevels; //!< The number of wavelet decomposition levels to use (0-automatically found, default:0).
            
            bool m_enableProgress; //!< Enable/Disable the progress interface (default:false).
            
            te::rst::Interpolator::Method m_interpMethod; //!< The raster interpolator method (default:NearestNeighbor).
            
            te::rp::WaveletAtrousFilterType m_waveletFilterType; //!< The wavelet filter type to use (default: TriangleWAFilter);
            
            boost::numeric::ublas::matrix< double > const* m_userWaveletFilterPtr; //!< An optional pointer to an user filter.
            
            bool m_enableMultiThread; //!< Enable/Disable the use of multi-threads (default:true).
            
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
          \brief WisperFusion output parameters
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

        WisperFusion();
        
        ~WisperFusion();
       
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
    };

  } // end namespace rp
}   // end namespace te

#endif

