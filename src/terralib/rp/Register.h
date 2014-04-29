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
  \file terralib/rp/Register.h
  \brief Performs raster data registering into a SRS using a set of tie points.
 */

#ifndef __TERRALIB_RP_INTERNAL_REGISTER_H
#define __TERRALIB_RP_INTERNAL_REGISTER_H

#include "Algorithm.h"
#include "../raster/Raster.h"
#include "../raster/Interpolator.h"
#include "../geometry/Envelope.h"
#include "../geometry/GTParameters.h"
#include "../geometry/GeometricTransformation.h"

#include <vector>
#include <string>
#include <memory>

namespace te
{
  namespace rp
  {
    /*!
      \class Register
      \brief Performs raster data registering into a SRS using a set of tie points.
      \ingroup rp_gen
    */
    class TERPEXPORT Register : public Algorithm
    {
      public:
        
        /*!
          \class InputParameters
          \brief Register input parameters
         */        
        class TERPEXPORT InputParameters : public AlgorithmInputParameters
        {
          public:
            
            te::rst::Raster const* m_inputRasterPtr; //!< Input raster.
            
            std::vector< unsigned int > m_inputRasterBands; //!< Bands to process from the input raster.
            
            std::vector< te::gm::GTParameters::TiePoint > m_tiePoints; //!< Tie-points between each raster point (te::gm::GTParameters::TiePoint::first are raster lines/columns ) and their respective coordinates under the chosen SRS (te::gm::GTParameters::TiePoint::second).
            
            int m_outputSRID; //!< The output raster SRID (default:0).
            
            double m_outputResolutionX; //!< The output raster X axis resolution (default:1).
            
            double m_outputResolutionY; //!< The output raster Y axis resolution (default:1).
            
            te::rst::Interpolator::Method m_interpMethod; //!< The raster interpolator method (default:NearestNeighbor).
            
            double m_noDataValue; //!< The pixel value used where no raster data is avaliable (defaul:0).
            
            std::string m_geomTransfName; //!< The name of the geometric transformation used (see each te::gm::GTFactory inherited classes to find each factory key/name, default:Affine).
            
            te::gm::GeometricTransformation const* m_geomTransfPtr; //!< An optional pointer to a valid geometric transformation instance (direct mapping raster lines/columns to and their respective coordinates under the chosen SRS) that will be used by the register process (if this pionter is used, m_tiePoints and m_geomTransfName will be ignored).
            
            te::gm::Envelope m_outputBoundingBox; //!< An optional bounding box (under the chosen SRS) to use for the generated output raster (default: automatically calculated).
            
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
          \brief Register output parameters
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

        Register();
        
        ~Register();
       
        //overload
        bool execute( AlgorithmOutputParameters& outputParams ) throw( te::rp::Exception );
        
        //overload
        void reset() throw( te::rp::Exception );
        
        //overload
        bool initialize( const AlgorithmInputParameters& inputParams ) throw( te::rp::Exception );
        
        bool isInitialized() const;

      protected:
        
        Register::InputParameters m_inputParameters; //!< Input execution parameters.
        
        bool m_isInitialized; //!< Tells if this instance is initialized.
        

    };

  } // end namespace rp
}   // end namespace te

#endif

