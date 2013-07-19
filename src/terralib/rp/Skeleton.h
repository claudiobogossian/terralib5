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
#include "../raster/Raster.h"

#include "Matrix.h"

#include <memory>
#include <vector>
#include <map>
#include <string>

namespace te
{
  namespace rp
  {
    /*!
      \class Skeleton
      
      \brief Creation of skeleton imagems.
      
      \details The skeleton of a region may be defined via the media axis transformation MAT
      proposed by Blum [1967]. The MAT of a region R with border B is as follows:
      For each point p in R, we find its closest neightbor in B. If p has more than one
      such neighbor, it is said to belong to the medial axis (skeleton) of R.
      
      \note Reference: Skeletons - Gonzalez R. C, "Digital Image Processing" - Second Edition.
      
      \note Reference: Blum, H. "A transformation for Extracting New Descriptors of Shape" In Models
      for the Perception of Speech and Visual Form, Wathen-Dunn, W. (ed.) MIT Press, Cambridge, Mass.
        
      \note Reference Segmentation-Free Approach for Skeletonization of Gray-Scale Images via 
      Anisotropic Vector Diffusion - Zeyun Yu and Chandrajit Bajaj.
      
      \ingroup RPAlgorithms
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
            
            double m_finiteDifferencesThreshold; //!< A threshold for the finite differences iterative diffusion - valid range [0,1].
            
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
        
        Skeleton::InputParameters m_inputParameters; //!< Input execution parameters.
        
        bool m_isInitialized; //!< Tells if this instance is initialized.
        
        /*!
          \brief Create an Edge Strenght Map from the input image.
          \param edgeIntensity The created edge intesity map;
          \param edgeX The vector decomposed X component;
          \param edgeY The vector decomposed Y component;
          \return true if OK, false on errors.
         */          
        bool getEdgeStrengthMap( 
          te::rp::Matrix< double >& edgeIntensity,
          te::rp::Matrix< double >& edgeX, 
          te::rp::Matrix< double >& edgeY ) const;
        
        /*!
          \brief Create the initial vector field from the edge intensity map.
          \param edgeIntensity The created edge intesity map;
          \param edgeX The vector decomposed X component;
          \param edgeY The vector decomposed Y component;
          \param vecFieldX The vector decomposed X component;
          \param vecFieldY The vector decomposed Y component;
          \return true if OK, false on errors.
         */           
        bool getInitialVectorField( 
          const te::rp::Matrix< double >& edgeIntensity,
          const te::rp::Matrix< double >& edgeX, 
          const te::rp::Matrix< double >& edgeY,
          te::rp::Matrix< double >& vecFieldX, 
          te::rp::Matrix< double >& vecFieldY ) const;
          
        /*!
          \brief Create the diffused vector field.
          \param inputVecFieldMagnitudes The input vector field intesity map;
          \param inputVecFieldX The vector decomposed X component;
          \param inputVecFieldY The vector decomposed Y component;
          \param diffusedVecFieldMagnitudes The created diffused vector field intesity map;
          \param diffurseVecFieldX The vector decomposed X component;
          \param diffusedVecFieldY The vector decomposed Y component;          
          \return true if OK, false on errors.
         */            
        bool getDiffusedVectorField( 
          const te::rp::Matrix< double >& inputVecFieldX, 
          const te::rp::Matrix< double >& inputVecFieldY,
          te::rp::Matrix< double >& diffurseVecFieldX, 
          te::rp::Matrix< double >& diffusedVecFieldY) const;  
          
        /*!
          \brief Create a tiff file from a matrix.
          \param matrix The matrix.
          \param normalize Enable/disable pixel normalization;
          \param tifFileName Tif file name.
        */             
        void createTifFromMatrix( 
          const te::rp::Matrix< double >& matrix,
          const bool normalize,
          const std::string& tifFileName ) const;  
          
        /*!
          \brief Create the skeleton stregth map.
          \param diffusedVecFieldX The vector decomposed X component;
          \param diffusedVecFieldY The vector decomposed Y component;
          \param strenghMap The skeleton stregth map.  
          \return true if OK, false on errors.
         */            
        bool getSkeletonStrengthMap( 
          const te::rp::Matrix< double >& diffusedVecFieldX, 
          const te::rp::Matrix< double >& diffusedVecFieldY,
          te::rp::Matrix< double >& strenghMap ) const;
        

    };

  } // end namespace rp
}   // end namespace te

#endif

