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
\file terralib/idl/rp/Segmenter.cpp

\brief  Raster segmenters.
*/ 

#include "Segmenter.h"
#include "../Functions.h"
#include "../IdlRaster.h"

//#include <terralib/memory/Raster.h>

#include <terralib/common/StringUtils.h>

#include <terralib/rp/Segmenter.h>
#include <terralib/rp/SegmenterRegionGrowingStrategy.h>

#include <terralib/raster/BandProperty.h>
#include <terralib/raster/RasterFactory.h>
#include <terralib/raster/Band.h>
#include <terralib/raster/Utils.h>

#include <memory>

namespace te
{
  namespace idl
  {
    namespace rp
    {
      IDL_VPTR RegionGrowingSegmenter(int argc, IDL_VPTR *argv, char *argk)
      {
        // getting input parameters 
        
        IDL_VPTR inputArray = argv[ 0 ];
        IDL_ENSURE_ARRAY( inputArray );
        
        IDL_VPTR minSegmentSize = IDL_CvtUInt( 1, &argv[ 1 ] );
        IDL_ENSURE_SCALAR( minSegmentSize );
          
        IDL_VPTR segmentsSimilarityThreshold = IDL_CvtDbl( 1, &argv[ 2 ] );
        IDL_ENSURE_SCALAR( segmentsSimilarityThreshold ); 
        
        // creating the input raster
        
        te::idl::IdlRaster inputRaster( inputArray, false );
        
        const unsigned int nBands = inputRaster.getNumberOfBands();
        const unsigned int nLines = inputRaster.getNumberOfRows();
        const unsigned int nCols = inputRaster.getNumberOfColumns();
          
//        te::rst::CreateCopy( inputRaster, "inputRaster.tif" );
          
        // Creating the algorithm parameters
        
        te::rp::SegmenterRegionGrowingStrategy::Parameters strategyParameters;
        
        strategyParameters.m_minSegmentSize = minSegmentSize->value.ui;
//        std::cout << std::endl << strategyParameters.m_minSegmentSize << std::endl;        
        
        strategyParameters.m_segmentsSimilarityThreshold = segmentsSimilarityThreshold->value.d;
//        std::cout << std::endl << strategyParameters.m_segmentsSimilarityThreshold << std::endl;        
        
        te::rp::Segmenter::InputParameters algoInputParams;
        
        algoInputParams.m_inputRasterPtr = &inputRaster;
        for( unsigned int band = 0 ; band < nBands ; ++band )
          algoInputParams.m_inputRasterBands.push_back( band );
        
        algoInputParams.m_strategyName = "RegionGrowing";
        
        algoInputParams.setSegStrategyParams( strategyParameters );
        
        te::rp::Segmenter::OutputParameters algoOutputParams;
        algoOutputParams.m_rType = "MEM";  
                
        // Executing the algorithm
        
        te::rp::Segmenter algorithmInstance;
        
        if( ! algorithmInstance.initialize( algoInputParams ) )
        {
          IDL_Message(IDL_M_NAMED_GENERIC, IDL_MSG_LONGJMP ,
            "Terralib segmenter initialization error" );
        }
        
        IDL_Message(IDL_M_NAMED_GENERIC, IDL_MSG_INFO,
          "Terralib segmenter initialized" );        
        
        if( ! algorithmInstance.execute( algoOutputParams ) )
        {
          IDL_Message(IDL_M_NAMED_GENERIC, IDL_MSG_LONGJMP,
            "Terralib segmenter execution error" );
        }
        
        IDL_Message(IDL_M_NAMED_GENERIC, IDL_MSG_INFO,
          "Terralib call finished" );          
          
        // creating the output array
        
        const UCHAR outArrayDataType = terralib2IdlType( 
          algoOutputParams.m_outputRasterPtr->getBand( 0 )->getProperty()->getType() );
          
        IDL_ARRAY_DIM outArrayDims;
        outArrayDims[ 0 ] = (IDL_MEMINT)nCols;
        outArrayDims[ 1 ] = (IDL_MEMINT)nLines;
     
        IDL_VPTR outArray;
        if( IDL_MakeTempArray( outArrayDataType, 2, outArrayDims,
          IDL_ARR_INI_NOP, &outArray ) == 0 )
        {
          IDL_Message(IDL_M_NAMED_GENERIC, IDL_MSG_LONGJMP,
            "Output array allocation error" );
        }
        
        algoOutputParams.m_outputRasterPtr->getBand( 0 )->read( 0, 0, 
          (void*)outArray->value.arr->data );
          
        IDL_Message(IDL_M_NAMED_GENERIC, IDL_MSG_INFO,
          "Segmentation finished" );               
        
        return outArray;
      }
      
    } // namespace rp
  } // namespace idl
} // namespace te
