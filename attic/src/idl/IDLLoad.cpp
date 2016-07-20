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
  \file terralib/idl/IDLLoad.c

  \brief IDL module entry point.
 */

#include "IDLLoad.h" 

#include "rp/Segmenter.h"
#include "Functions.h"

#include <terralib/common/TerraLib.h>

#define TERRALIB_IDL_FUNCTIONS_NUMBER 1
 
int IDL_Load(void)
{
  IDL_ExitRegister( IDL_ExitHandler );
  
  TerraLib::getInstance().initialize();
  
  te::idl::loadTerralibModules();
  
  static IDL_SYSFUN_DEF2 moduleFunctionsAddrs[ TERRALIB_IDL_FUNCTIONS_NUMBER ];
  
  moduleFunctionsAddrs[ 0 ].arg_max =
    moduleFunctionsAddrs[ 0 ].arg_min = 3;
  moduleFunctionsAddrs[ 0 ].extra = 0;
  moduleFunctionsAddrs[ 0 ].flags = 0;
  moduleFunctionsAddrs[ 0 ].name = (char*)"TERRALIB_RP_SEGMENTER_RG";
  moduleFunctionsAddrs[ 0 ].funct_addr.fun = te::idl::rp::RegionGrowingSegmenter;
  
  int returnValue = IDL_SysRtnAdd( moduleFunctionsAddrs, TRUE, 
    TERRALIB_IDL_FUNCTIONS_NUMBER );
  
  return returnValue;  
}

void IDL_ExitHandler(void)
{
  TerraLib::getInstance().finalize();
}