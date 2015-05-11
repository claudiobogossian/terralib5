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
\file terralib/idl/rp/Segmenter.h

\brief  Raster segmenters.
*/ 
 
#ifndef __TERRALIB_IDL_INTERNAL_RP_SEGMENTER_H
#define __TERRALIB_IDL_INTERNAL_RP_SEGMENTER_H

#include "../Config.h"

#include <stdio.h>

#include <idl_export.h>

namespace te
{
  namespace idl
  {
    namespace rp
    {    
      /*!
        \brief Raster region growing segmentation.
        
        \details Raster segmentation following the region growing strategy.
        
        \param argc The number of elements in argv.
        
        \param argv An array of IDL_VPTRs. These point to the IDL_VARIABLEs which comprise the
        arguments to the function.
        
        \return A IDL_VPTR pointing to the function output (always must be a temporary object).
        
        \note The parameters order: argv[ 0 ]-Input array, argv[ 1 ]-minSegmentSize,
        argv[ 2 ]-segmentsSimilarityThreshold
        */
      IDL_VPTR RegionGrowingSegmenter(int argc, IDL_VPTR *argv, char *argk );
    }
  }; // namespace idl
}; // namespace te

#endif // __TERRALIB_IDL_INTERNAL_RP_SEGMENTER_H
