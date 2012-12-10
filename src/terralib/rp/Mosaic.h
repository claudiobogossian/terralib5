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
  \file terralib/rp/Mosaic.h
  \brief Create a mosaic from a set of rasters.
 */

#ifndef __TERRALIB_RP_INTERNAL_MOSAIC_H
#define __TERRALIB_RP_INTERNAL_MOSAIC_H

#include "Algorithm.h"
#include "FeederRaster.h"
#include "../geometry/GTParameters.h"
#include "../raster/Interpolator.h"

#include <vector>
#include <string>
#include <map>

namespace te
{
  namespace rp
  {
    /*!
      \class Mosaic
      \brief Create a mosaic from a set of rasters.
      \ingroup RPAlgorithms
     */
    class TERPEXPORT Mosaic : public Algorithm
    {
      public:
        
        /*!
          \class InputParameters
          \brief Mosaic input parameters
         */        
        class TERPEXPORT InputParameters : public AlgorithmInputParameters
        {
          public:
            
            FeederRaster* m_feederRasterPtr;
            
            std::vector< std::vector< unsigned int > > m_inputRastersBands; //!< Bands to process for each input raster.
            
            std::vector< te::gm::GTParameters::TiePoint > m_tiePoints; //!< Tie-points between each raster pair or an empty vector if only the geographical coordinates must be used (te::gm::GTParameters::TiePoint::first are raster 1 line/column indexes, te::gm::GTParameters::TiePoint::second are raster 2 line/column indexes).
            
            std::string m_geomTransfName; //!< The name of the geometric transformation used if tie-points are supplied (see each te::gm::GTFactory inherited classes to find each factory key/name, default:Affine).
            
            te::rst::Interpolator::Method m_interpMethod; //!< The raster interpolator method (default:NearestNeighbor).
            
            double m_noDataValue; //!< The pixel value used where no raster data is avaliable (defaul:0).
            
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
          \brief Mosaic output parameters
         */        
        class TERPEXPORT OutputParameters : public AlgorithmOutputParameters
        {
          public:
            
            std::string m_rType; //!< Output raster data source type (as described in te::raster::RasterFactory ).
            
            std::map< std::string, std::string > m_rInfo; //!< The necessary information to create the output rasters (as described in te::raster::RasterFactory). 
            
            mutable boost::shared_ptr< te::rst::Raster > m_outputRasterPtr; //!< The generated output mosaic raster.
            
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

        Mosaic();
        
        ~Mosaic();
       
        //overload
        bool execute( AlgorithmOutputParameters& outputParams ) throw( te::rp::Exception );
        
        //overload
        void reset() throw( te::rp::Exception );
        
        //overload
        bool initialize( const AlgorithmInputParameters& inputParams ) throw( te::rp::Exception );
        
        bool isInitialized() const;

      protected:
        
        Mosaic::InputParameters m_inputParameters; //!< TiePointsLocator input execution parameters.
        
        bool m_isInitialized; //!< Tells if this instance is initialized.        

    };

  } // end namespace rp
}   // end namespace te

#endif

