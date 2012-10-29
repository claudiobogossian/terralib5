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

#include <terralib/plugin/PluginManager.h>

namespace te
{
  namespace idl
  {
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
  
    UCHAR terralib2IdlType( const int& tlType )
    {
      switch( tlType )
      {
        case te::dt::CHAR_TYPE :
        {
          return IDL_TYP_BYTE;
          break;
        }
        case te::dt::INT16_TYPE :
        {
          return IDL_TYP_INT;
          break;
        }     
        case te::dt::UINT16_TYPE :
        {
          return IDL_TYP_UINT;
          break;
        }     
        case te::dt::INT32_TYPE :
        {
          return IDL_TYP_LONG;
          break;
        }      
        case te::dt::UINT32_TYPE :
        {
          return IDL_TYP_ULONG;
          break;
        }      
        case te::dt::INT64_TYPE :
        {
          return IDL_TYP_LONG64;
          break;
        }      
        case te::dt::UINT64_TYPE :
        {
          return IDL_TYP_ULONG64;
          break;
        }     
        case te::dt::FLOAT_TYPE :
        {
          return IDL_TYP_FLOAT;
          break;
        }      
        case te::dt::DOUBLE_TYPE :
        {
          return IDL_TYP_DOUBLE;
          break;
        }      
        case te::dt::CFLOAT_TYPE :
        {
          return IDL_TYP_COMPLEX;
          break;
        }      
        case te::dt::CDOUBLE_TYPE :
        {
          return IDL_TYP_DCOMPLEX;
          break;
        }   
        case te::dt::STRING_TYPE :
        {
          return IDL_TYP_STRING;
          break;
        } 
        case te::dt::VOID_TYPE :
        {
          return IDL_TYP_PTR;
          break;
        } 
        default:
        {
          return IDL_TYP_UNDEF;
          break;
        }      
      }
    }  
    
    void dummyFunction( void* )
    {
    }
    
    void loadTerralibModules()
    {
      {
        #if TE_USE_OGR == 1
          te::plugin::PluginInfo info;
          info.m_name = "OGR DataSource Driver";
          info.m_description = "This data source driver supports...";

          #if TE_PLATFORM == TE_PLATFORMCODE_MSWINDOWS
            info.m_type = "dll";
            #ifdef NDEBUG
                  info.m_mainFile = "terralib_ogr.dll";
            #else
                  info.m_mainFile = "terralib_ogr_d.dll";
            #endif
          #elif TE_PLATFORM == TE_PLATFORMCODE_LINUX
            #ifdef NDEBUG
              info.m_type = "s.o.";
              info.m_mainFile = "libterralib_ogr.so";
            #else
              info.m_type = "s.o.";
              info.m_mainFile = "libterralib_ogr_d.so";
            #endif
          #elif TE_PLATFORM == TE_PLATFORMCODE_APPLE
            info.m_type = "dylib";
            info.m_mainFile = "libterralib_ogr.dylib";
          #else
            #error "Platform not supported yet"
          #endif

          te::plugin::PluginManager::getInstance().loadPlugin(info);
        #endif
      }

      {
        #if TE_USE_GDAL
          te::plugin::PluginInfo info;
          info.m_name = "GDAL DataSource Driver";
          info.m_description = "This data source driver supports...";

          #if TE_PLATFORM == TE_PLATFORMCODE_MSWINDOWS
            info.m_type = "dll";
            #ifdef NDEBUG
                  info.m_mainFile = "terralib_gdal.dll";
            #else
                  info.m_mainFile = "terralib_gdal_d.dll";
            #endif            
          #elif TE_PLATFORM == TE_PLATFORMCODE_LINUX
            #ifdef NDEBUG
              info.m_type = "s.o.";
              info.m_mainFile = "libterralib_gdal.so";
            #else
              info.m_type = "s.o.";
              info.m_mainFile = "libterralib_gdal_d.so";
            #endif
          #elif TE_PLATFORM == TE_PLATFORMCODE_APPLE
            info.m_type = "dylib";
            info.m_mainFile = "libterralib_gdal.dylib";
          #else
            #error "Platform not supported yet"
          #endif

          te::plugin::PluginManager::getInstance().loadPlugin(info);
        #endif
      }    
    }
    
  } // namespace idl
} // namespace te  
  
