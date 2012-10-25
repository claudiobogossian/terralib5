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
#include "Functions.h"

#include <terralib/memory/Raster.h>
#include <terralib/common/StringUtils.h>
#include <terralib/raster/RasterFactory.h>

#include <memory>

void dummyFunction( void* )
{
 
}

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
  
  std::map<std::string, std::string> rinfo;
  rinfo["MEM_IS_DATA_BUFFER"] = "TRUE";
  rinfo["MEM_BUFFER_NROWS"] = te::common::Convert2String( 
    (int64_t)inputArray->value.arr->dim[ 1 ] );
  rinfo["MEM_BUFFER_NCOLS"] = te::common::Convert2String( 
    (int64_t)inputArray->value.arr->dim[ 2 ] );
  rinfo["MEM_BUFFER_DATATYPE"] = te::common::Convert2String( idl2TerralibType( 
    inputArray->type ) );
  rinfo["MEM_BUFFER_NBANDS"] = te::common::Convert2String( 
    inputArray->value.arr->n_dim );  
  
  std::auto_ptr< te::rst::Raster > inputRasterPtr( te::rst::RasterFactory::make(
    "MEM", rinfo, inputArray->value.arr->data, dummyFunction ) );  
  
  IDL_VPTR outputArray;
  double* outputArrayDataPtr = (double*)IDL_MakeTempArray( inputArray->type, 
    inputArray->value.arr->n_dim, inputArray->value.arr->dim,
    IDL_ARR_INI_NOP, &outputArray );
    
  UCHAR* inputPtr = inputArray->value.arr->data;
  UCHAR* outputPtr = outputArray->value.arr->data;
  
  IDL_MEMINT n = 0;
  for ( n = inputArray->value.arr->arr_len ; n--; n )
    *outputPtr++ = *inputPtr++;
  
  return( outputArray );
}

