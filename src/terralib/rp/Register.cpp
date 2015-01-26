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
  \file terralib/rp/Register.cpp
  \brief Create a mosaic from a set of rasters.
*/

#include "Register.h"
#include "Macros.h"
#include "Functions.h"
#include "../geometry/GeometricTransformation.h"
#include "../geometry/GTFactory.h"
#include "../raster/RasterFactory.h"
#include "../raster/Band.h"
#include "../raster/BandProperty.h"
#include "../raster/Grid.h"
#include "../raster/Utils.h"

#include <cfloat>
#include <memory>


namespace te
{
  namespace rp
  {

    Register::InputParameters::InputParameters()
    {
      reset();
    }

    Register::InputParameters::InputParameters( const InputParameters& other )
    {
      reset();
      operator=( other );
    }

    Register::InputParameters::~InputParameters()
    {
      reset();
    }

    void Register::InputParameters::reset() throw( te::rp::Exception )
    {
      m_inputRasterPtr = 0;
      m_inputRasterBands.clear();
      m_tiePoints.clear();
      m_outputSRID = 0;
      m_outputResolutionX = 1.0;
      m_outputResolutionY = 1.0;
      m_interpMethod = te::rst::NearestNeighbor;
      m_noDataValue = 0;
      m_geomTransfName = "Affine";
      m_geomTransfPtr = 0;
      m_outputBoundingBox.m_llx = m_outputBoundingBox.m_lly =
        m_outputBoundingBox.m_urx = m_outputBoundingBox.m_ury = 0.0;
    }

    const Register::InputParameters& Register::InputParameters::operator=(
      const Register::InputParameters& params )
    {
      reset();

      m_inputRasterPtr = params.m_inputRasterPtr;
      m_inputRasterBands = params.m_inputRasterBands;
      m_tiePoints = params.m_tiePoints;
      m_outputSRID = params.m_outputSRID;
      m_outputResolutionX = params.m_outputResolutionX;
      m_outputResolutionY = params.m_outputResolutionY;
      m_interpMethod = params.m_interpMethod;
      m_noDataValue = params.m_noDataValue;
      m_geomTransfName = params.m_geomTransfName;
      m_geomTransfPtr = params.m_geomTransfPtr;
      m_outputBoundingBox = params.m_outputBoundingBox;

      return *this;
    }

    te::common::AbstractParameters* Register::InputParameters::clone() const
    {
      return new InputParameters( *this );
    }

    Register::OutputParameters::OutputParameters()
    {
      reset();
    }

    Register::OutputParameters::OutputParameters( const OutputParameters& other )
    {
      reset();
      operator=( other );
    }

    Register::OutputParameters::~OutputParameters()
    {
      reset();
    }

    void Register::OutputParameters::reset() throw( te::rp::Exception )
    {
      m_rType.clear();
      m_rInfo.clear();
      m_outputRasterPtr.reset();
    }

    const Register::OutputParameters& Register::OutputParameters::operator=(
      const Register::OutputParameters& params )
    {
      reset();

      m_rType = params.m_rType;
      m_rInfo = params.m_rInfo;

      return *this;
    }

    te::common::AbstractParameters* Register::OutputParameters::clone() const
    {
      return new OutputParameters( *this );
    }

    Register::Register()
    {
      reset();
    }

    Register::~Register()
    {
    }

    bool Register::execute( AlgorithmOutputParameters& outputParams )
      throw( te::rp::Exception )
    {
      if( ! m_isInitialized ) return false;

      Register::OutputParameters* outParamsPtr = dynamic_cast<
        Register::OutputParameters* >( &outputParams );
      TERP_TRUE_OR_THROW( outParamsPtr, "Invalid paramters" );
      
      // instantiating the geometric transformation
      
      std::auto_ptr< te::gm::GeometricTransformation > transformationPtr;
      
      if( m_inputParameters.m_geomTransfPtr )
      {
        transformationPtr.reset( m_inputParameters.m_geomTransfPtr->clone() );
      }
      else
      {
        te::gm::GTParameters transfParams;
        transfParams.m_tiePoints = m_inputParameters.m_tiePoints;
        
        transformationPtr.reset( te::gm::GTFactory::make( m_inputParameters.m_geomTransfName ) );
        TERP_TRUE_OR_RETURN_FALSE( transformationPtr.get() != 0,
          "Geometric transformation instatiation error" );        
        TERP_TRUE_OR_RETURN_FALSE( transformationPtr->initialize( transfParams ),
          "Geometric transformation parameters calcule error" );
      }
      
      // guessing the output raster bounding box coords
      
      double lowerLeftX = DBL_MAX;
      double lowerLeftY = DBL_MAX;
      double upperRightX = -1.0 * DBL_MAX;
      double upperRightY = -1.0 * DBL_MAX;     
      
      if(
          ( m_inputParameters.m_outputBoundingBox.m_llx != 0.0 )
          ||
          ( m_inputParameters.m_outputBoundingBox.m_lly != 0.0 )
          ||
          ( m_inputParameters.m_outputBoundingBox.m_urx != 0.0 )
          ||
          ( m_inputParameters.m_outputBoundingBox.m_ury != 0.0 )
        )
      {
        lowerLeftX = m_inputParameters.m_outputBoundingBox.m_llx;
        lowerLeftY = m_inputParameters.m_outputBoundingBox.m_lly;
        upperRightX = m_inputParameters.m_outputBoundingBox.m_urx;
        upperRightY = m_inputParameters.m_outputBoundingBox.m_ury;
      }
      else
      {
        te::gm::LinearRing indexedDetailedExtent(te::gm::LineStringType, 0, 0);
        TERP_TRUE_OR_RETURN_FALSE( te::rp::GetIndexedDetailedExtent( 
          *m_inputParameters.m_inputRasterPtr->getGrid(), indexedDetailedExtent ),
          "Indexed extent creation error" );
        
        double mappedX = 0;
        double mappedY = 0;        
        
        for( std::size_t extentIdx = 0 ; extentIdx < indexedDetailedExtent.size() ;
          ++extentIdx )
        {
          transformationPtr->directMap( indexedDetailedExtent.getX( extentIdx ), 
            indexedDetailedExtent.getY( extentIdx ), mappedX, mappedY );
          
          lowerLeftX = std::min( lowerLeftX, mappedX );
          lowerLeftY = std::min( lowerLeftY, mappedY );
          upperRightX = std::max( upperRightX, mappedX );
          upperRightY = std::max( upperRightY, mappedY );  
        }
      }
      
      // creating the output raster
      
      {
        std::vector< te::rst::BandProperty* > bandsProperties;
        for( unsigned int inputRasterBandsIdx = 0 ; inputRasterBandsIdx <
          m_inputParameters.m_inputRasterBands.size() ; ++inputRasterBandsIdx )
        {
          bandsProperties.push_back( new te::rst::BandProperty(
            *( m_inputParameters.m_inputRasterPtr->getBand(  
            m_inputParameters.m_inputRasterBands[ inputRasterBandsIdx ] )->getProperty() ) ) );  
          bandsProperties[ inputRasterBandsIdx ]->m_noDataValue = m_inputParameters.m_noDataValue;
        }
        
        te::gm::Envelope* gridMbr = new te::gm::Envelope();
        gridMbr->m_llx = lowerLeftX;
        gridMbr->m_lly = lowerLeftY;
        gridMbr->m_urx = upperRightX;
        gridMbr->m_ury = upperRightY;
        
        outParamsPtr->m_outputRasterPtr.reset( 
          te::rst::RasterFactory::make(
            outParamsPtr->m_rType, 
            new te::rst::Grid( m_inputParameters.m_outputResolutionX,
              m_inputParameters.m_outputResolutionY, gridMbr, 
              m_inputParameters.m_outputSRID ),
            bandsProperties,
            outParamsPtr->m_rInfo,
            0,
            0 ) );
        TERP_TRUE_OR_RETURN_FALSE( outParamsPtr->m_outputRasterPtr.get(),
          "Output raster creation error" );
      }      
      
      // Rendering the output raster
      
      te::rst::Raster& outputRaster = *outParamsPtr->m_outputRasterPtr;
      const te::rst::Grid& outputGrid = *outputRaster.getGrid();
      te::rst::Interpolator interpInstance( m_inputParameters.m_inputRasterPtr,
        m_inputParameters.m_interpMethod );
      te::gm::GeometricTransformation& geomTransf = *transformationPtr;
      const unsigned int nRows = outputRaster.getNumberOfRows();
      const unsigned int nCols = outputRaster.getNumberOfColumns();
      unsigned int outRow = 0;
      unsigned int outCol = 0;
      double outX = 0;
      double outY = 0;
      double inRow = 0;
      double inCol = 0;
      std::complex< double > cValue;
        
      for( unsigned int inputRasterBandsIdx = 0 ; inputRasterBandsIdx <
        m_inputParameters.m_inputRasterBands.size() ; ++inputRasterBandsIdx )
      {
        const unsigned int inputBandIdx = m_inputParameters.m_inputRasterBands[ inputRasterBandsIdx ];
        const double inputBandNoDataValue = m_inputParameters.m_inputRasterPtr->getBand( inputBandIdx )->getProperty()->m_noDataValue;
        te::rst::Band& outputBand = *outputRaster.getBand( inputRasterBandsIdx );
        
        double outputBandRangeMin = 0;
        double outputBandRangeMax = 0;
        te::rst::GetDataTypeRanges( outputBand.getProperty()->m_type,
          outputBandRangeMin, outputBandRangeMax );        
        
        for( outRow = 0 ; outRow < nRows ; ++outRow )
        {
          for( outCol = 0 ; outCol < nCols ; ++outCol )
          {
            outputGrid.gridToGeo( (double)outCol, (double)outRow, outX, outY );
            
            geomTransf.inverseMap(  outX, outY, inCol, inRow );
            
            interpInstance.getValue( inCol, inRow, cValue, inputBandIdx );
            
            if( cValue.real() == inputBandNoDataValue )
            {
              outputBand.setValue( outCol, outRow, m_inputParameters.m_noDataValue );
            }
            else
            {
              outputBand.setValue( outCol, outRow, std::max( outputBandRangeMin, 
                std::min( outputBandRangeMax, cValue.real() ) ) );
            }
          }
        }
      }      
      

      return true;
    }

    void Register::reset() throw( te::rp::Exception )
    {
      m_inputParameters.reset();
      m_isInitialized = false;
    }

    bool Register::initialize( const AlgorithmInputParameters& inputParams )
      throw( te::rp::Exception )
    {
      reset();

      Register::InputParameters const* inputParamsPtr = dynamic_cast<
        Register::InputParameters const* >( &inputParams );
      TERP_TRUE_OR_THROW( inputParamsPtr, "Invalid paramters pointer" );

      m_inputParameters = *inputParamsPtr;

      // Checking the input raster

      TERP_TRUE_OR_RETURN_FALSE( m_inputParameters.m_inputRasterPtr,
        "Invalid m_inputRasterPtr" )

      TERP_TRUE_OR_RETURN_FALSE( 
        m_inputParameters.m_inputRasterPtr->getAccessPolicy() & te::common::RAccess, 
        "Invalid raster" );

      TERP_TRUE_OR_RETURN_FALSE( m_inputParameters.m_inputRasterBands.size() > 0,
        "Invalid raster bands number" );
        
      for( unsigned int inRasterBandsIdx = 0 ; inRasterBandsIdx < 
        m_inputParameters.m_inputRasterBands.size() ; ++inRasterBandsIdx )
      {
        TERP_TRUE_OR_RETURN_FALSE( 
          m_inputParameters.m_inputRasterBands[ inRasterBandsIdx ] <
          m_inputParameters.m_inputRasterPtr->getNumberOfBands(), 
          "Invalid raster bands" );
      }        

      // checking transformation related parameters
      
      if( m_inputParameters.m_geomTransfPtr )
      {
        TERP_TRUE_OR_RETURN_FALSE( m_inputParameters.m_geomTransfPtr->isValid(),
          "Invalid geometric transformation" );
      }
      else
      {
        std::auto_ptr< te::gm::GeometricTransformation > transformationPtr( 
        te::gm::GTFactory::make( m_inputParameters.m_geomTransfName ) );
        TERP_TRUE_OR_RETURN_FALSE( transformationPtr.get() != 0,
          "Geometric transformation instatiation error" );
          
        TERP_TRUE_OR_RETURN_FALSE( m_inputParameters.m_tiePoints.size() >=
          transformationPtr->getMinRequiredTiePoints(),
          "Invalid tie-points number" )
      }

      // checking other parameters

      TERP_TRUE_OR_RETURN_FALSE( m_inputParameters.m_outputSRID >= 0,
        "Invalid m_outputSRID" )
        
      TERP_TRUE_OR_RETURN_FALSE( m_inputParameters.m_outputResolutionX > 0.0,
        "Invalid m_outputResolutionX" )        
        
      TERP_TRUE_OR_RETURN_FALSE( m_inputParameters.m_outputResolutionY > 0.0,
        "Invalid m_outputResolutionY" )  

      m_isInitialized = true;

      return true;
    }

    bool Register::isInitialized() const
    {
      return m_isInitialized;
    }



  } // end namespace rp
}   // end namespace te

