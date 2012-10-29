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
\file terralib/idl/rp/Segmenter.cpp

\brief  Raster segmenters.
*/ 

#include "Segmenter.h"
#include "../Functions.h"

#include <terralib/raster/Band.h>
#include <terralib/memory/Raster.h>
#include <terralib/common/StringUtils.h>
#include <terralib/raster/RasterFactory.h>
#include <terralib/rp/Segmenter.h>
#include <terralib/rp/SegmenterRegionGrowingStrategy.h>
#include <terralib/raster/BandProperty.h>

#include <memory>

namespace te
{
  namespace idl
  {
    namespace rp
    {
      void dummyFunction( void* )
      {
      }

      IDL_VPTR RegionGrowingSegmenter(int argc, IDL_VPTR *argv, char *argk)
      {
        IDL_VPTR returnValue;
        
        // getting input parameters 
        
        IDL_VPTR inputArray = argv[ 0 ];
        IDL_ENSURE_ARRAY( inputArray );
        
        IDL_VPTR minSegmentSize = IDL_CvtUInt( 1, &argv[ 1 ] );
        IDL_ENSURE_SCALAR( minSegmentSize );
          
        IDL_VPTR segmentsSimilarityThreshold = IDL_CvtDbl( 1, &argv[ 2 ] );
        IDL_ENSURE_SCALAR( segmentsSimilarityThreshold ); 
        
        // creating the input raster
        
        const unsigned int nBands = (unsigned int)inputArray->value.arr->n_dim;
        const unsigned int nLines = (unsigned int)inputArray->value.arr->dim[ 2 ];
        const unsigned int nCols = (unsigned int)inputArray->value.arr->dim[ 1 ];
        const int tlDataType = idl2TerralibType( inputArray->type );
        
//        std::cout << std::endl << nBands << std::endl;
//        std::cout << std::endl << nLines << std::endl;
//        std::cout << std::endl << nCols << std::endl;
//        std::cout << std::endl << tlDataType << std::endl;        
        
        std::map<std::string, std::string> inputRasterInfo;
        inputRasterInfo["MEM_IS_DATA_BUFFER"] = "TRUE";
        inputRasterInfo["MEM_BUFFER_NCOLS"] = te::common::Convert2String( nCols );
        inputRasterInfo["MEM_BUFFER_NROWS"] = te::common::Convert2String( nLines );
        inputRasterInfo["MEM_BUFFER_DATATYPE"] = te::common::Convert2String( tlDataType );
        inputRasterInfo["MEM_BUFFER_NBANDS"] = te::common::Convert2String( nBands );  
        
        std::auto_ptr< te::rst::Raster > inputRasterPtr( te::rst::RasterFactory::make(
          "MEM", inputRasterInfo, inputArray->value.arr->data, dummyFunction ) );
          
        // Creating the algorithm parameters
        
        te::rp::SegmenterRegionGrowingStrategy::Parameters strategyParameters;
        
        strategyParameters.m_minSegmentSize = minSegmentSize->value.ui;
//        std::cout << std::endl << strategyParameters.m_minSegmentSize << std::endl;        
        
        strategyParameters.m_segmentsSimilarityThreshold = segmentsSimilarityThreshold->value.d;
//        std::cout << std::endl << strategyParameters.m_segmentsSimilarityThreshold << std::endl;        
        
        te::rp::Segmenter::InputParameters algoInputParams;
        
        algoInputParams.m_inputRasterPtr = inputRasterPtr.get();
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
          IDL_Message(IDL_M_NAMED_GENERIC, IDL_MSG_LONGJMP,
            "Segmenter initialization error" );
          return returnValue;
        }
        
        IDL_Message(IDL_M_NAMED_GENERIC, IDL_MSG_LONGJMP,
          "Segmenter initialized" );        
        
        if( ! algorithmInstance.execute( algoOutputParams ) )
        {
          IDL_Message(IDL_M_NAMED_GENERIC, IDL_MSG_LONGJMP,
            "Segmenter execution error" );
          return returnValue;
        }
        
        IDL_Message(IDL_M_NAMED_GENERIC, IDL_MSG_LONGJMP,
          "Segmentation finished" );          
          
        // creating the output array
        
        const UCHAR outArrayDataType = terralib2IdlType( 
          algoOutputParams.m_outputRasterPtr->getBand( 0 )->getProperty()->getType() );
          
        IDL_ARRAY_DIM outArrayDims;
        outArrayDims[ 0 ] = 1;
        outArrayDims[ 1 ] = (IDL_MEMINT)nCols;
        outArrayDims[ 2 ] = (IDL_MEMINT)nLines;
     
        if( IDL_MakeTempArray( outArrayDataType, 1, outArrayDims,
          IDL_ARR_INI_NOP, &returnValue ) == 0 )
        {
          IDL_Message(IDL_M_NAMED_GENERIC, IDL_MSG_LONGJMP,
            "Output array allocation error" );
          return returnValue;
        }
        
        algoOutputParams.m_outputRasterPtr->getBand( 0 )->read( 0, 0, 
          (void*)returnValue->value.arr->data );
        
        return returnValue;
      }
      
    } // namespace rp
  } // namespace idl
} // namespace te
