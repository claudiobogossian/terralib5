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
  \file terralib/rp/Skeleton.h
  \brief Creation of skeleton imagems.
 */

#ifndef __TERRALIB_RP_INTERNAL_SKELETON_H
#define __TERRALIB_RP_INTERNAL_SKELETON_H

#include "Algorithm.h"
#include "Matrix.h"
#include "../raster/Raster.h"
#include "../raster/BandProperty.h"
#include "../raster/RasterFactory.h"
#include "../raster/Grid.h"
#include "../common/progress/TaskProgress.h"

#include <boost/thread.hpp>

#include <memory>
#include <vector>
#include <map>
#include <string>

#include <climits>
#include <cfloat>

namespace te
{
  namespace rp
  {
    /*!
      \class Skeleton
      
      \brief Creation of skeleton images.
      
      \details The skeleton of a region may be defined via the media axis transformation MAT
      proposed by Blum [1967]. The MAT of a region R with border B is as follows:
      For each point p in R, we find its closest neightbor in B. If p has more than one
      such neighbor, it is said to belong to the medial axis (skeleton) of R.
      
      \note Reference: Blum, H. 1967. A transformation for extracting new descriptors of form.
      
      \note Reference: Normalized Gradient Vector Diffusion and Image Segmentation, Zeyun Yu, Chandrajit Bajaj.
      
      \note Reference: Image Segmentation Using Gradient Vector Diffusion and Region Merging, Zeyun Yu, Chandrajit Bajaj.
      
      \note Reference: A Segmentation-Free Approach for Skeletonization of Gray-Scale Images via Anisotropic Vector Diffusion, Zeyun Yu and Chandrajit Bajaj.
      
      \ingroup rp_gen
     */
    class TERPEXPORT Skeleton : public Algorithm
    {
      public:
        
        /*!
          \class InputParameters
          \brief Skeleton input parameters
         */        
        class TERPEXPORT InputParameters : public AlgorithmInputParameters
        {
          public:
            
            te::rst::Raster const* m_inputRasterPtr; //!< Input raster.
            
            unsigned int m_inputRasterBand; //!< Bands to process from the input raster.
            
            te::rst::Raster const* m_inputMaskRasterPtr; //!< A pointer to an input raster (where pixels with zero velues will be ignored) or an null pointer if no input mask raster must be used.
            
            double m_diffusionThreshold; //!< A threshold over the residue from one iteration to another, if the residue drops below this value the process is finished - valid range: (0,1] positive values, lower values will cause more iterations to be performed.
            
            double m_diffusionRegularization; //!< A regularization parameter to control the variation from one iteration to the next one (higher values can reduce the number of iterations but can bring the system to an unstable state, valid range: (0,1] positive values ).
            
            unsigned int m_diffusionMaxIterations; //!< The maximum number of iterations to perform (zero: no iterations limit).
            
            bool m_enableMultiThread; //!< Enable (true) the use of threads.
            
            double m_skeletonThreshold; //!< A threshold to select those pixels as being part of the final skeleton - valid range [0,1].
            
            bool m_enableProgress; //!< Enable/Disable the progress interface (default:false).
            
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
          \brief Skeleton output parameters
         */        
        class TERPEXPORT OutputParameters : public AlgorithmOutputParameters
        {
          public:
            
            std::string m_rType; //!< Output raster data source type (as described in te::raster::RasterFactory ).
            
            std::map< std::string, std::string > m_rInfo; //!< The necessary information to create the output rasters (as described in te::raster::RasterFactory). 
            
            std::auto_ptr< te::rst::Raster > m_outputRasterPtr; //!< The generated output registered raster.
            
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

        Skeleton();
        
        ~Skeleton();
       
        //overload
        bool execute( AlgorithmOutputParameters& outputParams ) throw( te::rp::Exception );
        
        //overload
        void reset() throw( te::rp::Exception );
        
        //overload
        bool initialize( const AlgorithmInputParameters& inputParams ) throw( te::rp::Exception );
        
        bool isInitialized() const;

      protected:
        
        /*!
          \class ApplyRansacThreadEntryThreadParams

          \brief Parameters used by the GTFilter::applyRansacThreadEntry method.
        */
        class ApplyVecDiffusionThreadParams
        {
          public:
            
            te::rp::Matrix< double > const * m_initialXBufPtr; //!< A pointer to the input buffer initial X component.
            te::rp::Matrix< double > const * m_initialYBufPtr; //!< A pointer to the input buffer initial Y component.  
            te::rp::Matrix< double > const * m_inputMagBufPtr; //!< A pointer to the input magnitude buffer.
            te::rp::Matrix< double > const * m_inputBufXPtr; //!< A pointer to the input buffer X component.
            te::rp::Matrix< double > const * m_inputBufYPtr; //!< A pointer to the input buffer Y component.
            te::rp::Matrix< double > * m_outputMagBufPtr; //!< A pointer to the output magnitude buffer.
            te::rp::Matrix< double > * m_outputBufXPtr; //!< A pointer to the output buffer X component.
            te::rp::Matrix< double > * m_outputBufYPtr; //!< A pointer to the output buffer X component.
            boost::mutex* m_mutexPtr; //!< A pointer to the sync mutex.
            unsigned int m_firstRowIdx; //!< First row to process.
            unsigned int m_lastRowIdx; //!< Last row to process.
            double* m_currentIterationResiduePtr; //!< A pointer the the current iteration residue;
            double m_diffusionRegularization; //!< The diffusion regularization parameter.

            ApplyVecDiffusionThreadParams() {};
            
            ApplyVecDiffusionThreadParams( const ApplyVecDiffusionThreadParams& other )
            {
              operator=( other );
            };

            ~ApplyVecDiffusionThreadParams() {};
            
            ApplyVecDiffusionThreadParams& operator=( const ApplyVecDiffusionThreadParams& other )
            {
              m_initialXBufPtr = other.m_initialXBufPtr;
              m_initialYBufPtr = other.m_initialYBufPtr;
              m_inputMagBufPtr = other.m_inputMagBufPtr;
              m_inputBufXPtr = other.m_inputBufXPtr;
              m_inputBufYPtr = other.m_inputBufYPtr;
              m_outputMagBufPtr = other.m_outputMagBufPtr;
              m_outputBufXPtr = other.m_outputBufXPtr;
              m_outputBufYPtr = other.m_outputBufYPtr;
              m_mutexPtr = other.m_mutexPtr;
              m_firstRowIdx = other.m_firstRowIdx;
              m_lastRowIdx = other.m_lastRowIdx;
              m_currentIterationResiduePtr = other.m_currentIterationResiduePtr;
              m_diffusionRegularization = other.m_diffusionRegularization;
              
              return *this;
            };
        };        
        
        Skeleton::InputParameters m_inputParameters; //!< Input execution parameters.
        
        bool m_isInitialized; //!< Tells if this instance is initialized.
        
        /*!
          \brief Load data from the input raster.
          \param rasterData The loaded data buffer.
          \return true if OK, false on errors.
         */          
        bool loadData( te::rp::Matrix< double >& rasterData ) const;        
        
        /*!
          \brief Create an gradient maps from the input image.
          \param inputData The input data.
          \param unitVectors If true, unit vectors will be created.
          \param gradXMap The created gradient X vectors map.
          \param gradYMap The created gradient Y vectors map.
          \return true if OK, false on errors.
         */          
        bool getGradientMaps( 
          const te::rp::Matrix< double >& inputData,
          const bool unitVectors,
          te::rp::Matrix< double >& gradXMap,
          te::rp::Matrix< double >& gradYMap ) const;
          
        /*!
          \brief Apply a mean filter.
          \param input The input data.
          \param output The output data.
          \return true if OK, false on errors.
         */  
        template< typename MatrixElementT >
        bool applyMeanSmooth( 
          const te::rp::Matrix< MatrixElementT >& input,
          te::rp::Matrix< MatrixElementT >& output ) const
        {
          const unsigned int nRows = input.getLinesNumber();
          const unsigned int nCols = input.getColumnsNumber();
          
          if( ! output.reset( nRows,nCols ) )
            return false;          
          
          const unsigned int lastRowIdx = nRows - 1;
          const unsigned int lastColIdx = nCols - 1;
          unsigned int row = 0;
          unsigned int col = 0;          
          unsigned int nextRow = 0;
          unsigned int nextCol = 0;
          unsigned int prevRow = 0;
          unsigned int prevCol = 0;   
          
          for( row = 0 ; row < nRows ; ++row )
          {
            output[ row ][ 0 ] = input[ row ][ 0 ];
            output[ row ][ lastColIdx ] = input[ row ][ lastColIdx ];
          }
          
          for( col = 0 ; col < nCols ; ++col )
          {
            output[ 0 ][ col ] = input[ 0 ][ col ];
            output[ lastRowIdx ][ col ] = input[ lastRowIdx ][ col ];
          }           
          
          for( row = 1 ; row < lastRowIdx ; ++row )
          {
            prevRow = row - 1;
            nextRow = row + 1;
            
            for( col = 1 ; col < lastColIdx ; ++col )
            {
              prevCol = col - 1;
              nextCol = col + 1;
              
              output[ row ][ col ] =
                (
                  input[ row ][ prevCol ] 
                  + input[ row ][ nextCol ]
                  + input[ prevRow ][ prevCol ] 
                  + input[ prevRow ][ col ]
                  + input[ prevRow ][ nextCol ] 
                  + input[ nextRow ][ prevCol ] 
                  + input[ nextRow ][ col ]
                  + input[ nextRow ][ nextCol ]
                ) / 8.0;
            }
          }
          
          return true;
        }
        
        /*!
          \brief Generate the magnitude map from the input vectors.
          \param xMap The input data X component.
          \param yMap The input data Y component.
          \param magnitude The magnitude output data.
          \return true if OK, false on errors.
         */ 
        template< typename MatrixElementT >
        bool getMagnitude( 
          const te::rp::Matrix< double >& xMap,
          const te::rp::Matrix< double >& yMap,
          te::rp::Matrix< MatrixElementT >& magnitude ) const
        {
          assert( xMap.getColumnsNumber() == yMap.getColumnsNumber() );
          assert( xMap.getLinesNumber() == yMap.getLinesNumber() );
          
          const unsigned int nRows = xMap.getLinesNumber();
          const unsigned int nCols = xMap.getColumnsNumber();
          
          if( ( magnitude.getColumnsNumber() != nCols ) &&
            ( magnitude.getLinesNumber() != nRows ) )
          {
            if( ! magnitude.reset( nRows,nCols ) )
              return false;          
          }
          
          unsigned int row = 0;
          unsigned int col = 0; 
          double xValue = 0;
          double yValue = 0;
          
          for( row = 0 ; row < nRows ; ++row )
          {
            for( col = 0 ; col < nCols ; ++col )
            {
              xValue = xMap[ row ][ col ];
              yValue = yMap[ row ][ col ];
              
              magnitude[ row ][ col ] = std::sqrt( ( xValue * xValue ) + 
                ( yValue * yValue ) );
            }
          }
          
          return true;
        }
          
        /*!
          \brief Create an Edge strenght Map from the input data.
          \param inputMap The input map.
          \param edgeStrengthMap The edge strength map (values between 0 and 1).
          \return true if OK, false on errors.
          \details The edge map have the property that the values are large near image boundaries and small within homogeneous regions.
         */          
        bool getEdgeStrengthMap( 
          const te::rp::Matrix< double >& inputMap,
          te::rp::Matrix< double >& edgeStrengthMap ) const;       
        
        /*!
          \brief Create a tiff file from a vector field.
          \param inputVecFieldX The vector decomposed X component;
          \param inputVecFieldY The vector decomposed Y component;
          \param backGroundMapPtr An optional background image (0 means no background image).
          \param vecPixelStep The step between each vector.
          \param tifFileName Tif file name.
        */             
        void createTifFromVecField( 
          const te::rp::Matrix< double >& inputVecFieldX, 
          const te::rp::Matrix< double >& inputVecFieldY,
          te::rp::Matrix< double > const * const backGroundMapPtr,
          const unsigned int vecPixelStep,
          const std::string& tifFileName ) const;   

        /*!
          \brief Apply a vector diffusion over the given vector field.
          \param inputX The vector decomposed X component;
          \param inputY The vector decomposed Y component;
          \param backGroundMapPtr An optional background image (0 means no background image).
          \param progressPtr A pointer to a progress interface or an NULL pointer.
          \param outputX The diffused X component.
          \param outputX The diffused Y component.
        */                       
        bool applyVecDiffusion( 
          const te::rp::Matrix< double >& inputX, 
          const te::rp::Matrix< double >& inputY,
          te::rp::Matrix< double > const * const backgroundDataPtr, 
          te::common::TaskProgress* progressPtr,
          te::rp::Matrix< double >& outputX, 
          te::rp::Matrix< double >& outputY ) const;     
          
        /*!
          \brief Create a skeleton strength map.
          \details The skeleton strength map is a scalar map defined on every pixel and indicates the likelihood of each pixel being on the skeletons.
          \param inputX The vector decomposed X component;
          \param inputY The vector decomposed Y component;
          \param edgeStrengthMap The edge strength map (values between 0 and 1).
          \param skelSMap The skeleton map (values between 0 and 1).
          \return true if ok, false on errors.
        */            
        bool createSkeletonStrengthMap( 
          const te::rp::Matrix< double >& inputX, 
          const te::rp::Matrix< double >& inputY,
          const te::rp::Matrix< double >& edgeStrengthMap,
          te::rp::Matrix< double >& skelMap ) const;        
          
        /*! 
          \brief Vector diffusion thread entry.

          \param paramsPtr A pointer to the thread parameters.
        */
        static void applyVecDiffusionThreadEntry( ApplyVecDiffusionThreadParams* paramsPtr);          

    };

  } // end namespace rp
}   // end namespace te

#endif

