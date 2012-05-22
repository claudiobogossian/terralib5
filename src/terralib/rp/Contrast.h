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
  \file terralib/rp/Contrast.h
  \brief Contrast enhancement.
 */

#ifndef __TERRALIB_RP_INTERNAL_CONTRAST_H
#define __TERRALIB_RP_INTERNAL_CONTRAST_H

#include "Algorithm.h"

#include <boost/shared_ptr.hpp>

#include <vector>
#include <string>
#include <map>

namespace te
{
  namespace da
  {
    class DataSource;
  };

  namespace rst
  {
    class Raster;
    class Band;
  };

  namespace rp
  {
    /*!
      \class Contrast
      \brief Contrast enhancement.
      \details Apply contrast enhencement on the selected bands.
      \ingroup RPAlgorithms
     */
    class TERPEXPORT Contrast : public Algorithm
    {
      public:
        
        /*!
          \class InputParameters
          \brief Contrast input parameters
         */        
        class TERPEXPORT InputParameters : public AlgorithmInputParameters
        {
          public:
            
            /*! \enum  Allowed contrast types. */
            enum ContrastType
            {
              InvalidContrastT = 0, /*!< Invalid contrast. */
              LinearContrastT = 1, /*!< The histogram range will be changed to the supplied min/max range ( linear function ). */
              HistogramEqualizationContrastT = 2, /*!< The histogram of the image will be equalized automatically. */
              SetMeanAndStdContrastT = 3, /*!< The contrasted image will have a predefined mean and standard deviation. */
            };

            ContrastType m_type; //!< The contrast type to be applied.
            
            double m_lCMinInput; //!< The contrast minimum input greyscale value.
            
            double m_lCMaxInput; //!< The contrast maximum input greyscale value.
            
            double m_hECMaxInput; //!<  The contrast maximum input greyscale value.
            
            double m_sMASCMeanInput; //!<  The mean greyscale to be applied in the contrast image.
            
            double m_sMASCStdInput; //!< The standard deviation to be applied in the contrast image.

            te::rst::Raster* m_inRasterPtr; //!< Input raster.
            
            std::vector< unsigned int > m_inRasterBands; //!< Bands to be processed from the input raster.
          
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
          \brief Contrast output parameters
          \details The result will be written to the raster instance pointed 
          by m_outRasterPtr (in this case the output bands must also be 
          passed by m_outRasterBands ); or written to a new raster instance 
          created inside the given data source pointed by m_outDataSourcePtr 
          (in this case the data set name must be supplied - m_outDataSetName ).
         */        
        class TERPEXPORT OutputParameters : public AlgorithmOutputParameters
        {
          public:
            
            mutable std::auto_ptr< te::rst::Raster > m_outRasterPtr; //!< A pointer to a valid (initialized ) output raster instance or an empty pointer.
            
            std::vector< unsigned int > m_outRasterBands; //!< Bands to be processed from the output raster.
            
            std::string m_rType; //!< Output raster data source type (as described in te::raster::RasterFactory ).
            
            std::map< std::string, std::string > m_rInfo; //!< The necessary information to create the raster (as described in te::raster::RasterFactory). 
          
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

        Contrast();
        
        ~Contrast();
       
        //overload
        bool execute( AlgorithmOutputParameters& outputParams ) throw( te::rp::Exception );
        
        //overload
        void reset() throw( te::rp::Exception );
        
        //overload
        bool initialize( const AlgorithmInputParameters& inputParams ) throw( te::rp::Exception );
        
        bool isInitialized() const;

      protected:
        
        /*!
          \brief Type definition for a remapping function pointer.
         */        
        typedef void (Contrast::*RemapFuncPtrT)( const double& inValue, 
          double& outValue );

        Contrast::InputParameters m_inputParameters; //!< Contrast input execution parameters.
        Contrast::OutputParameters* m_outputParametersPtr; //!< Contrast input execution parameters.
        
        bool m_isInitialized; //!< Tells if this instance is initialized.

        /*!
          \brief Execute a linear contrast following the internal parameters
          \return true if OK, false on errors.
         */
        bool execLinearContrast();

        /*!
          \brief Execute the histogram equalization contrast following the internal parameters
          \return true if OK, false on errors.
        */
        bool execHistogramEqualizationContrast();

        /*!
          \brief Execute the histogram equalization contrast following the internal parameters
          \return true if OK, false on errors.
        */
        bool execSetMeanAndStdContrast();

        /*!
          \brief Band gray levels remap using a remap function.
          \param inRasterBand Input raster band.
          \param outRasterBand Output raster band.
          \param remapFuncPtr The remap function pointer used.
          \return true if OK, false on errors.
         */        
        bool remapBandLevels( te::rst::Band& inRasterBand,
          te::rst::Band& outRasterBand, RemapFuncPtrT remapFuncPtr );
          
        // Variables used by offSetGainRemap
        double m_offSetGainRemap_offset;
        double m_offSetGainRemap_gain;
        
        /*!
          \brief Remap on gray level using an offset 
          (Contrast::m_offSetGainRemap_offset) and a gain value
          (Contrast::m_offSetGainRemap_gain.
          \param inValue Input gray level.
          \param outValue Output gray level.
         */         
        inline void offSetGainRemap( const double& inValue, double& outValue )
        {
          outValue = ( inValue + m_offSetGainRemap_offset ) * m_offSetGainRemap_gain;
        };

    };

  } // end namespace rp
}   // end namespace te

#endif

