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
  \file terralib/rp/TiePointsLocatorInputParameters.cpp
  \brief Tie points locator input parameters.
*/

#include "TiePointsLocatorInputParameters.h"
#include "TiePointsLocatorMoravecStrategy.h"

namespace te
{
  namespace rp
  {
    TiePointsLocatorInputParameters::TiePointsLocatorInputParameters()
    {
      reset();
    }
    
    TiePointsLocatorInputParameters::TiePointsLocatorInputParameters( const TiePointsLocatorInputParameters& other )
    {
      reset();
      operator=( other );
    }    

    TiePointsLocatorInputParameters::~TiePointsLocatorInputParameters()
    {
      reset();
    }

    void TiePointsLocatorInputParameters::reset() throw( te::rp::Exception )
    {
      m_interesPointsLocationStrategyName = "Moravec";
      m_inRaster1Ptr = 0;
      m_inMaskRaster1Ptr = 0;
      m_inRaster1Bands.clear();
      m_raster1TargetAreaLineStart = 0;
      m_raster1TargetAreaColStart = 0;
      m_raster1TargetAreaWidth = 0;
      m_raster1TargetAreaHeight = 0;
      m_inRaster2Ptr = 0;
      m_inMaskRaster2Ptr = 0;
      m_inRaster2Bands.clear();
      m_raster2TargetAreaLineStart = 0;
      m_raster2TargetAreaColStart = 0;
      m_raster2TargetAreaWidth = 0;
      m_raster2TargetAreaHeight = 0;
      m_enableMultiThread = true;
      m_enableProgress = false;
      m_maxTiePoints = 2500;
      m_pixelSizeXRelation = 1;
      m_pixelSizeYRelation = 1;
      m_geomTransfName = "Affine";
      m_geomTransfMaxError = 2;
      m_enableGeometryFilter = true;
      m_geometryFilterAssurance = 0.75;
      m_subSampleOptimizationRescaleFactor = 1.0;
      m_subSampleOptimizationMinTPAreaCoverage = 25;
      m_subSampleOptimizationMinTPNumberFactor = 2;
      m_interpMethod = te::rst::NearestNeighbor;
      m_tiePointsSubSectorsSplitFactor = 3;
      
      m_specStratParamsPtr.reset( new te::rp::TiePointsLocatorMoravecStrategy::Parameters() );
    }

    const TiePointsLocatorInputParameters& TiePointsLocatorInputParameters::operator=(
      const TiePointsLocatorInputParameters& params )
    {
      reset();
      
      m_interesPointsLocationStrategyName = params.m_interesPointsLocationStrategyName;
      m_inRaster1Ptr = params.m_inRaster1Ptr;
      m_inMaskRaster1Ptr = params.m_inMaskRaster1Ptr;
      m_inRaster1Bands = params.m_inRaster1Bands;
      m_raster1TargetAreaLineStart = params.m_raster1TargetAreaLineStart;
      m_raster1TargetAreaColStart = params.m_raster1TargetAreaColStart;
      m_raster1TargetAreaWidth = params.m_raster1TargetAreaWidth;
      m_raster1TargetAreaHeight = params.m_raster1TargetAreaHeight;
      m_inRaster2Ptr = params.m_inRaster2Ptr;
      m_inMaskRaster2Ptr = params.m_inMaskRaster2Ptr;
      m_inRaster2Bands = params.m_inRaster2Bands;
      m_raster2TargetAreaLineStart = params.m_raster2TargetAreaLineStart;
      m_raster2TargetAreaColStart = params.m_raster2TargetAreaColStart;
      m_raster2TargetAreaWidth = params.m_raster2TargetAreaWidth;
      m_raster2TargetAreaHeight = params.m_raster2TargetAreaHeight;
      m_enableMultiThread = params.m_enableMultiThread;
      m_enableProgress = params.m_enableProgress;
      m_maxTiePoints = params.m_maxTiePoints;
      m_pixelSizeXRelation = params.m_pixelSizeXRelation;
      m_pixelSizeYRelation = params.m_pixelSizeYRelation;
      m_geomTransfName = params.m_geomTransfName;
      m_geomTransfMaxError = params.m_geomTransfMaxError;
      m_enableGeometryFilter = params.m_enableGeometryFilter;
      m_geometryFilterAssurance = params.m_geometryFilterAssurance;
      m_subSampleOptimizationRescaleFactor = params.m_subSampleOptimizationRescaleFactor;
      m_subSampleOptimizationMinTPAreaCoverage = params.m_subSampleOptimizationMinTPAreaCoverage;
      m_subSampleOptimizationMinTPNumberFactor = params.m_subSampleOptimizationMinTPNumberFactor;
      m_interpMethod = params.m_interpMethod;
      m_tiePointsSubSectorsSplitFactor = params.m_tiePointsSubSectorsSplitFactor;
      
      if( params.m_specStratParamsPtr.get() )
      {
        m_specStratParamsPtr.reset( (TiePointsLocatorStrategyParameters*) 
          params.m_specStratParamsPtr->clone() );
      }
      else
      {
        m_specStratParamsPtr.reset();
      }

      return *this;
    }

    te::common::AbstractParameters* TiePointsLocatorInputParameters::clone() const
    {
      return new TiePointsLocatorInputParameters( *this );
    }
    
    void TiePointsLocatorInputParameters::setSpecStrategyParams( 
      const TiePointsLocatorStrategyParameters& specStratParams )
    {
      m_specStratParamsPtr.reset( (TiePointsLocatorStrategyParameters*)
        specStratParams.clone() );
    }

    TiePointsLocatorStrategyParameters const* TiePointsLocatorInputParameters::getSpecStrategyParams() const
    {
      return m_specStratParamsPtr.get();      
    }
    
    TiePointsLocatorStrategyParameters* TiePointsLocatorInputParameters::getSpecStrategyParams()
    {
      return m_specStratParamsPtr.get();      
    }    

  } // end namespace rp
}   // end namespace te

