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
  \file terralib/rp/IHSFusion.h
  \brief Creation of skeleton imagems.
 */

#ifndef __TERRALIB_RP_INTERNAL_IHSFUSION_H
#define __TERRALIB_RP_INTERNAL_IHSFUSION_H

#include "Algorithm.h"
#include "Matrix.h"
#include "../raster/Raster.h"
#include "../raster/Interpolator.h"

#include <vector>
#include <string>
#include <map>

namespace te
{
  namespace rp
  {
    /*!
      \class IHSFusion
      
      \brief Fusion of a low-resolution multi-band image with a high resolution image using the IHS method.
      
      \details The IHS is one of the widespread image fusion methods in the remote sensing community and has been employed as a standard procedure in many commercial packages; IHS is the intrinsic method when the intensity channel is replaced by the high-resolution Pan image.
      
      \note Reference: Te-Ming Tu, Shun-Chi Su, Hsuen-Chyun Shyu, Ping S. Huang, A new look at IHS-like image fusion methods, Information Fusion, Volume 2, Issue 3, September 2001, Pages 177-186, ISSN 1566-2535.
      
      \note Reference: W.J. Carper, T.M. Lillesand, R.W. Kiefer, The use of intensity-hue-saturation transformations for merging SPOT panchromatic and multispectral image data Photogramm. Eng. Remote Sensing, 56 (1990), pp. 459–467.

      \ingroup rp_fus
     */
    class TERPEXPORT IHSFusion : public Algorithm
    {
      public:
        
        /*!
          \class InputParameters
          \brief IHSFusion input parameters
         */        
        class TERPEXPORT InputParameters : public AlgorithmInputParameters
        {
          public:
            
            te::rst::Raster const* m_lowResRasterPtr; //!< Input low-resolution multi-band raster.
            
            unsigned int m_lowResRasterRedBandIndex; //!< The low-resolution raster red band index (default:0).
            
            unsigned int m_lowResRasterGreenBandIndex; //!< The low-resolution raster green band index (default:1).
            
            unsigned int m_lowResRasterBlueBandIndex; //!< The low-resolution raster blue band index (default:2).
            
            te::rst::Raster const* m_highResRasterPtr; //!< Input high-resolution raster.
            
            unsigned int m_highResRasterBand; //!< Band to process from the high-resolution raster.
            
            bool m_enableProgress; //!< Enable/Disable the progress interface (default:false).
            
            te::rst::Interpolator::Method m_interpMethod; //!< The raster interpolator method (default:NearestNeighbor).
            
            double m_RGBMin; //!< The used RGB minimum value (default:0 - leave zero for automatic detection based on the input images).
            
            double m_RGBMax; //!< The used RGB maximum value (default:0 - leave zero for automatic detection based on the input images).            
            
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
          \brief IHSFusion output parameters
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

        IHSFusion();
        
        ~IHSFusion();
       
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
          \brief Get the minimum and maximum values from the RGB input image.
          \param rgbMin RGB minimum value.
          \param rgbMax RGB maximum value.
          \return true if ok, false on errors.
         */        
        bool getRGBRange( double& rgbMin, double& rgbMax ) const;
        
        /*!
          \brief Get statistics from the given matrix.
          \param matrix Input matrix.
          \param mean Mean value.
          \param variance Variance value.
          \return true if ok, false on errors.
         */        
        bool getStatistics( const te::rp::Matrix< float >& matrix, float& mean, 
          float& variance ) const;        
        
        /*!
          \brief Load resampled IHS data from the input image.
          \param rgbMin RGB minimum value.
          \param rgbMax RGB maximum value.
          \param intensityData Intensity channel data.
          \param hueData Hue channel data.
          \param saturationData Saturation channel data.
          \return true if ok, false on errors.
          \note IHS data with the following channels ranges: I:[0,1] H:[0,2pi] (radians) S:[0,1].
         */
        bool loadIHSData( const double& rgbMin, const double rgbMax, te::rp::Matrix< float >& intensityData,
          te::rp::Matrix< float >& hueData, te::rp::Matrix< float >& saturationData ) const;
          
        /*!
          \brief Swap the intensity data by the high resolution image data.
          \param intensityData Intensity channel data.
          \return true if ok, false on errors.
         */
        bool swapIntensity( te::rp::Matrix< float >& intensityData );   
        
        /*!
          \brief Save resampled IHS data as RGB data to the output image.
          \param rgbMin RGB minimum value.
          \param rgbMax RGB maximum value.
          \param intensityData Intensity channel data.
          \param hueData Hue channel data.
          \param saturationData Saturation channel data.
          \return true if ok, false on errors.
         */
        bool saveIHSData( const double& rgbMin, const double rgbMax, const te::rp::Matrix< float >& intensityData,
          const te::rp::Matrix< float >& hueData, const te::rp::Matrix< float >& saturationData,
          const std::string& rType, const std::map< std::string, std::string >& rInfo,
          std::auto_ptr< te::rst::Raster >& outputRasterPtr ) const;        
    };

  } // end namespace rp
}   // end namespace te

#endif

