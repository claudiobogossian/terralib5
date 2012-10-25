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
\file terralib/idl/Functions.cpp

\brief IDL module functions.
*/ 

#include "Functions.h"

#include <terralib/datatype/Enums.h>

int idl2TerralibType( const UCHAR& idlType  )
{
  switch( idlType )
  {
    case IDL_TYP_BYTE :
    {
      return te::dt::CHAR_TYPE;
      break;
    }
    case IDL_TYP_INT :
    {
      return te::dt::INT16_TYPE;
      break;
    }     
    case IDL_TYP_UINT :
    {
      return te::dt::UINT16_TYPE;
      break;
    }     
    case IDL_TYP_LONG :
    {
      return te::dt::INT32_TYPE;
      break;
    }      
    case IDL_TYP_ULONG :
    {
      return te::dt::UINT32_TYPE;
      break;
    }      
    case IDL_TYP_LONG64 :
    {
      return te::dt::INT64_TYPE;
      break;
    }      
    case IDL_TYP_ULONG64 :
    {
      return te::dt::UINT64_TYPE;
      break;
    }     
    case IDL_TYP_FLOAT :
    {
      return te::dt::FLOAT_TYPE;
      break;
    }      
    case IDL_TYP_DOUBLE :
    {
      return te::dt::DOUBLE_TYPE;
      break;
    }      
    case IDL_TYP_COMPLEX :
    {
      return te::dt::CFLOAT_TYPE;
      break;
    }      
    case IDL_TYP_DCOMPLEX :
    {
      return te::dt::CFLOAT_TYPE;
      break;
    }   
    case IDL_TYP_STRING :
    {
      return te::dt::STRING_TYPE;
      break;
    } 
    case IDL_TYP_PTR :
    {
      return te::dt::VOID_TYPE;
      break;
    } 
    default:
    {
      return te::dt::UNKNOWN_TYPE;
      break;
    }      
  }
}
