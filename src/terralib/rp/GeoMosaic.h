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
  \file terralib/rp/GeoMosaic.h
  \brief Create a mosaic from a set of geo-referenced rasters.
 */

#ifndef __TERRALIB_RP_INTERNAL_GEOMOSAIC_H
#define __TERRALIB_RP_INTERNAL_GEOMOSAIC_H

#include "Algorithm.h"
#include "FeedersRaster.h"
#include "Blender.h"
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
      \class GeoMosaic
      \brief Create a mosaic from a set of geo-referenced rasters.
      \note The first raster will always be taken as reference to define the mosaic resolution and SRS.
      \ingroup rp_mos
     */
    class TERPEXPORT GeoMosaic : public Algorithm
    {
      public:
        
        /*!
          \class InputParameters
          \brief GeoMosaic input parameters
         */        
        class TERPEXPORT InputParameters : public AlgorithmInputParameters
        {
          public:
            
            FeederConstRaster* m_feederRasterPtr; //!< Input rasters feeder.
            
            std::vector< std::vector< unsigned int > > m_inputRastersBands; //!< Bands to process for each input raster.
            
            te::rst::Interpolator::Method m_interpMethod; //!< The raster interpolator method (default:NearestNeighbor).
            
            double m_noDataValue; //!< The pixel value used where no raster data is avaliable (defaul:0).
            
            bool m_forceInputNoDataValue; //!< If true, m_noDataValue will be used as the no-data value for input rasters (defalt:false).
            
            te::rp::Blender::BlendMethod m_blendMethod; //!< The pixel blending method (default: NoBlendMethod).
            
            bool m_autoEqualize; //!< Auto equalization will be performed using the overlaped image areas (default:true).
            
            bool m_useRasterCache; //!< Enable(true) or disable the use of raster caching (default:true).
            
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
          \brief GeoMosaic output parameters
         */        
        class TERPEXPORT OutputParameters : public AlgorithmOutputParameters
        {
          public:
            
            std::string m_rType; //!< Output raster data source type (as described in te::raster::RasterFactory ).
            
            std::map< std::string, std::string > m_rInfo; //!< The necessary information to create the output rasters (as described in te::raster::RasterFactory). 
            
            std::auto_ptr< te::rst::Raster > m_outputRasterPtr; //!< The generated output mosaic raster.
            
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

        GeoMosaic();
        
        ~GeoMosaic();
       
        //overload
        bool execute( AlgorithmOutputParameters& outputParams ) throw( te::rp::Exception );
        
        //overload
        void reset() throw( te::rp::Exception );
        
        //overload
        bool initialize( const AlgorithmInputParameters& inputParams ) throw( te::rp::Exception );
        
        bool isInitialized() const;

      protected:
        
        GeoMosaic::InputParameters m_inputParameters; //!< Input execution parameters.
        
        bool m_isInitialized; //!< Tells if this instance is initialized.
        
        /*!
          \brief Execute a mosaic of georeferenced images.
          \param outputParams The algorithm execution parameters.
          \return true if ok, false on errors.
        */
        bool executeGeoMosaic( GeoMosaic::OutputParameters& outputParams );
          
          
        /*!
          \brief Raster band statistics calcule.
          \param band Input raster band.
          \param forceNoDataValue Force the noDataValue to be used as the band no-data value.
          \param noDataValue The no-data value to use.
          \param mean Pixels mean.
          \param variance Pixels variance.
        */
        static void calcBandStatistics( const te::rst::Band& band,
          const bool& forceNoDataValue,
          const double& noDataValue,
          double& mean, 
          double& variance );

    };

  } // end namespace rp
}   // end namespace te

#endif

