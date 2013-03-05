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
  \file terralib/rp/EdgeFilter.h
  \brief Raster edges filter.
 */

#ifndef __TERRALIB_RP_INTERNAL_EDGEFILTER_H
#define __TERRALIB_RP_INTERNAL_EDGEFILTER_H

#include "Algorithm.h"
#include "../raster/Raster.h"

#include <vector>

namespace te
{
  namespace rp
  {
    /*!
      \class EdgeFilter
      \brief Raster edges filter.
      \ingroup MatchingAlgorithms
     */
    class TERPEXPORT EdgeFilter : public Algorithm
    {
      public:
        
        /*!
          \class InputParameters
          \brief EdgeFilter input parameters
         */        
        class TERPEXPORT InputParameters : public AlgorithmInputParameters
        {
          public:
            
            /*! \enum The edge filter type.*/
            enum FilterType
            {
              InvalidFilterT = 0, /*!< Invalid strategy. */
              SobelFilterT = 1, /*!<  Sobel filter type. */
              RobertsFilterT = 2 /*!<  Roberts filter type. */
            };
            
            FilterType m_filterType; //!< The edge filter type.

            te::rst::Raster const* m_inRasterPtr; //!< Input raster.
            
            std::vector< unsigned int > m_inRasterBands; //!< Bands to be used from the input raster 1.
            
            unsigned int m_iterationsNumber; //!< The number of iterations to perform (default:1).
            
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
          \brief EdgeFilter output parameters
         */        
        class TERPEXPORT OutputParameters : public AlgorithmOutputParameters
        {
          public:
            
            std::string m_rType; //!< Output raster data source type (as described in te::raster::RasterFactory ).
            
            std::map< std::string, std::string > m_rInfo; //!< The necessary information to create the raster (as described in te::raster::RasterFactory). 
            
            mutable boost::shared_ptr< te::rst::Raster > m_outputRasterPtr; //!< A pointer the ge generated output raster (label image).
            
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

        EdgeFilter();
        
        ~EdgeFilter();
       
        //overload
        bool execute( AlgorithmOutputParameters& outputParams ) throw( te::rp::Exception );
        
        //overload
        void reset() throw( te::rp::Exception );
        
        //overload
        bool initialize( const AlgorithmInputParameters& inputParams ) throw( te::rp::Exception );
        
        //overload
        bool isInitialized() const;

      protected:
        
        /*!
          \brief Type definition for a filter method pointer.
          \param srcRaster Source raster.
          \param srcBandIdx Source raster band index.
          \param dstRaster Destination raster.
          \param dstBandIdx Destination raster band index.          
         */
        typedef void (EdgeFilter::*FilterMethodPointerT)( const te::rst::Raster& srcRaster,
          const unsigned int srcBandIdx, te::rst::Raster& dstRaster,
          const unsigned int dstBandIdx );
        
        bool m_isInitialized; //!< Is this instance already initialized?
        
        EdgeFilter::InputParameters m_inputParameters; //!< Input parameters.
        
        /*!
          \brief Applay the Roberts filter over the source raster band.
          \param srcRaster Source raster.
          \param srcBandIdx Source raster band index.
          \param dstRaster Destination raster.
          \param dstBandIdx Destination raster band index.          
         */        
        void RobertsFilter( const te::rst::Raster& srcRaster,
          const unsigned int srcBandIdx, te::rst::Raster& dstRaster,
          const unsigned int dstBandIdx  );
          
        /*!
          \brief Applay the Sobel filter over the source raster band.
          \param srcRaster Source raster.
          \param srcBandIdx Source raster band index.
          \param dstRaster Destination raster.
          \param dstBandIdx Destination raster band index.          
         */     
        void SobelFilter( const te::rst::Raster& srcRaster,
          const unsigned int srcBandIdx, te::rst::Raster& dstRaster,
          const unsigned int dstBandIdx  );

    };

  } // end namespace rp
}   // end namespace te

#endif

