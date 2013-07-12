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
            
            std::vector< unsigned int > m_inputRasterBands; //!< Bands to process from the input raster.
            
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
        
        bool buildEdgeMap( te::rp::Matrix< double >& magnitudes ) const;
        
        bool buildGradientVectorField( const te::rp::Matrix< double >& magnitudes,
          te::rp::Matrix< double >& gradVecField ) const;
        

    };

  } // end namespace rp
}   // end namespace te

#endif

