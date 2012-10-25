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
  \file terralib/idl/IDLLoad.c

  \brief IDL module entry point.
 */

#include "IDLLoad.h" 

#include "rp/Segmenter.h"

 
int IDL_Load(void)
{
  static IDL_SYSFUN_DEF2 regionGrowingSegmenterFunctionAddr[ 1 ];
  
  regionGrowingSegmenterFunctionAddr[ 0 ].arg_max =
    regionGrowingSegmenterFunctionAddr[ 0 ].arg_min = 3;
  regionGrowingSegmenterFunctionAddr[ 0 ].extra = 0;
  regionGrowingSegmenterFunctionAddr[ 0 ].flags = 0;
  regionGrowingSegmenterFunctionAddr[ 0 ].name = (char*)"TERRALIB_RP_SEGMENTER_RG";
  regionGrowingSegmenterFunctionAddr[ 0 ].funct_addr.fun = RegionGrowingSegmenter;
  
  int returnValue = IDL_SysRtnAdd( regionGrowingSegmenterFunctionAddr, TRUE, 1 );
  
  return returnValue;  
}

