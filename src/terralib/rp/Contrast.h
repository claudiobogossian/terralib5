/*  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

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

#include <vector>
#include <string>
#include <map>
#include <memory>
#include <cmath>

namespace te
{
  namespace da
  {
    class DataSource;
  }

  namespace rst
  {
    class Raster;
    class Band;
  }

  namespace rp
  {
    /*!
      \class Contrast
      \brief Contrast enhancement.
      \details Apply contrast enhencement on the selected bands.
      \ingroup rp_enh
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
            
            /*!
              \name Global parameters
            */
            /**@{*/              
            
            /*! \enum  Allowed contrast types. */
            enum ContrastType
            {
              InvalidContrastT, /*!< Invalid contrast. */
              LinearContrastT, /*!< The histogram range will be changed to the supplied min/max range ( linear function ). */
              HistogramEqualizationContrastT, /*!< The histogram of the image will be equalized automatically. */
              SquareContrastT, /*!< The contrasted image will be created by using a square function. */
              SquareRootContrastT, /*!< The contrasted image will be created by using a square root function. */
              LogContrastT, /*!< The contrasted image will be created by using a log function. */
              MeanAndStdContrastT, /*!< The contrasted image will have a predefined mean and standard deviation normalization. */
              DecorrelationEnhancementT /*!< Decorrelation Enhancement using principal components. */
            };

            ContrastType m_type; //!< The contrast type to be applied.
            
            te::rst::Raster const* m_inRasterPtr; //!< Input raster.
            
            std::vector< unsigned int > m_inRasterBands; //!< Bands to be processed from the input raster.
            
            bool m_enableProgress; //!< Enable/Disable the progress interface (default:false).
            
            //@}
            
            /*!
              \name Linear contrast parameters
            */
            /**@{*/             
            
            std::vector< double > m_lCMinInput; //!< The contrast minimum input greyscale value of each band.
            
            std::vector< double > m_lCMaxInput; //!< The contrast maximum input greyscale value of each band.
            
            //@}
            
            /*!
              \name Histogram equalization contrast parameters
            */
            /**@{*/             
            
            std::vector< double > m_hECMaxInput; //!<  The contrast maximum input greyscale value of each band.
            
            //@}
            
            /*!
              \name Square contrast parameters
            */
            /**@{*/             
            
            std::vector< double > m_squareCMinInput; //!< The contrast minimum input greyscale value of each band.
            
            std::vector< double > m_squareCMaxInput; //!< The contrast maximum input greyscale value of each band.
            
            //@}            
            
            /*!
              \name Square root contrast parameters
            */
            /**@{*/             
            
            std::vector< double > m_squareRootCMinInput; //!< The contrast minimum input greyscale value of each band.
            
            std::vector< double > m_squareRootCMaxInput; //!< The contrast maximum input greyscale value of each band.
            
            //@}                        
            
            /*!
              \name Log contrast parameters
            */
            /**@{*/             
            
            std::vector< double > m_logCMinInput; //!< The contrast minimum input greyscale value of each band.
            
            std::vector< double > m_logCMaxInput; //!< The contrast maximum input greyscale value of each band.
            
            //@}                
            
            /*!
              \name Mean and standard deviation normalization contrast parameters
            */
            /**@{*/             
            
            std::vector< double > m_sMASCMeanInput; //!<  The mean greyscale to be applied in each band.
            
            std::vector< double > m_sMASCStdInput; //!< The standard deviation to be applied in each band.

            //@}
          
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
            
            te::rst::Raster* m_outRasterPtr; //!< A pointer to a valid initiated raster instance where the result must be written, leave NULL to create a new instance(in this case the other output parameters must be used).
            
            std::auto_ptr< te::rst::Raster > m_createdOutRasterPtr; //!< A pointer to the created output raster instance, or an empty pointer empty if the result must be written to the raster pointed m_outRasterPtr.
            
            std::vector< unsigned int > m_outRasterBands; //!< Bands to be processed from the output raster.
            
            std::string m_createdOutRasterDSType; //!< Output raster data source type (as described in te::raster::RasterFactory ), leave empty if the result must be written to the raster pointed m_outRasterPtr.
            
            std::map< std::string, std::string > m_createdOutRasterInfo; //!< The necessary information to create the raster (as described in te::raster::RasterFactory), leave empty if the result must be written to the raster pointed m_outRasterPtr.
          
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
          \brief Execute a square contrast following the internal parameters
          \return true if OK, false on errors.
         */
        bool execSquareContrast();      
        
        /*!
          \brief Execute a square root contrast following the internal parameters
          \return true if OK, false on errors.
         */
        bool execSquareRootContrast(); 

        /*!
          \brief Execute a log contrast following the internal parameters
          \return true if OK, false on errors.
         */
        bool execLogContrast();         

        /*!
          \brief Execute the histogram equalization contrast following the internal parameters
          \return true if OK, false on errors.
        */
        bool execSetMeanAndStdContrast();
        
        /*!
          \brief Execute the decorrelation enhancement contrast following the internal parameters
          \return true if OK, false on errors.
        */
        bool execDecorrelationEnhancement();

        /*!
          \brief Band gray levels remap using a remap function.
          \param inRasterBand Input raster band.
          \param outRasterBand Output raster band.
          \param remapFuncPtr The remap function pointer used.
          \param enableProgress Enable the use of a progress interface.
          \return true if OK, false on errors.
         */        
        bool remapBandLevels( const te::rst::Band& inRasterBand,
          te::rst::Band& outRasterBand, RemapFuncPtrT remapFuncPtr,
          const bool enableProgress );
          
        // Variables used by offSetGainRemap
        double m_offSetGainRemap_offset1;
        double m_offSetGainRemap_offset2;
        double m_offSetGainRemap_gain;
        
        // Variables used by offSetGainRemap
        double m_squareRemap_factor;
        
        
        // Variables used by offSetGainRemap
        double m_squareRootRemap_gain;        
        
        // Variables used by offSetGainRemap
        double m_logRemap_gain;
        double m_logRemap_offset;                
        
        /*!
          \brief Remap on gray level using an offset and gain.
          \param inValue Input gray level.
          \param outValue Output gray level.
          \note outValue = ( ( inValue + m_offSetGainRemap_offset1 ) * m_offSetGainRemap_gain ) + m_offSetGainRemap_offset2
         */         
        inline void offSetGainRemap( const double& inValue, double& outValue )
        {
          outValue = ( ( inValue + m_offSetGainRemap_offset1 ) * m_offSetGainRemap_gain )
            + m_offSetGainRemap_offset2;
        };
        
        /*!
          \brief Remap on gray level using a square.
          \param inValue Input gray level.
          \param outValue Output gray level.
          \note outValue = m_squareRemap_factor * std::pow( inValue, 2.0 )
         */         
        inline void squareRemap( const double& inValue, double& outValue )
        {
          outValue = m_squareRemap_factor * std::pow( inValue, 2.0 );
        };  
        
        /*!
          \brief Remap on gray level using a square root.
          \param inValue Input gray level.
          \param outValue Output gray level.
          \note outValue = m_squareRootRemap_gain * std::sqrt( inValue );
         */         
        inline void squareRootRemap( const double& inValue, double& outValue )
        {
          outValue = m_squareRootRemap_gain * std::sqrt( inValue );
        };       
        
        /*!
          \brief Remap on gray level using a log.
          \param inValue Input gray level.
          \param outValue Output gray level.
          \note outValue = m_logRemap_gain * std::log10( inValue + m_logRemap_offset + 1.0 );
         */         
        inline void logRemap( const double& inValue, double& outValue )
        {
          outValue = m_logRemap_gain * std::log10( inValue + m_logRemap_offset + 1.0 );
        };             
    };

  } // end namespace rp
}   // end namespace te

#endif

