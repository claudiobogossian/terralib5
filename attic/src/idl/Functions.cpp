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
\file terralib/idl/Functions.cpp

\brief IDL module functions.
*/ 

#include "Functions.h"

#include <terralib/datatype/Enums.h>

#include <terralib/plugin.h>
#include <terralib/common.h>
#include <terralib/raster/RasterFactory.h>

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
    
    void loadTerralibModules()
    {
      {
        te::plugin::PluginInfo info; 
        info.m_name = "te.da.ogr";
        info.m_displayName = "OGR DataSource Driver";
        info.m_description = "This data source driver supports spatial data managed by OGR";
        info.m_engine = "C++";
//        info.m_folder = PLUGINS_PATH;
        
        std::pair<std::string, std::string> rsc("SharedLibraryName", "terralib_ogr");
        
        info.m_resources.push_back(rsc);
        
        te::plugin::PluginManager::getInstance().load(info);
      }

      {
        std::pair<std::string, std::string> rsc("SharedLibraryName", "terralib_gdal");
        
        te::plugin::PluginInfo info;
        info.m_name = "te.da.gdal";
        info.m_displayName = "GDAL DataSource Driver";
        info.m_description = "This data source driver supports spatial data managed by GDAL";
        info.m_engine = "C++";
//        info.m_folder = PLUGINS_PATH;
        info.m_resources.push_back(rsc);

        te::plugin::PluginManager::getInstance().load(info);
      }    
    }
    
    bool createRasterFromIdlArray( IDL_VPTR idlValuePointer, 
      const bool takeBufferOwnership,
      boost::shared_ptr< te::rst::Raster >& outRasterPtr )
    {
      if( ! ( idlValuePointer->flags & IDL_V_ARR) )
      {
        return false;
      }
      else
      {
        unsigned int nLines = 0;
        unsigned int nCols = 0;  
        unsigned int nBands = 0;
        
        if( idlValuePointer->value.arr->n_dim == 2 )
        {
          nLines = (unsigned int)idlValuePointer->value.arr->dim[ 1 ];
          nCols = (unsigned int)idlValuePointer->value.arr->dim[ 0 ];
          nBands = 1;
        }
        else if( idlValuePointer->value.arr->n_dim == 3 )
        {
          nLines = (unsigned int)idlValuePointer->value.arr->dim[ 2 ];
          nCols = (unsigned int)idlValuePointer->value.arr->dim[ 1 ];
          nBands = (unsigned int)idlValuePointer->value.arr->dim[ 0 ];
        }
        else
        {
          return false;
        }
        
        const int tlDataType = idl2TerralibType( idlValuePointer->type );
        
        std::map<std::string, std::string> rasterInfo;
        rasterInfo["MEM_IS_DATA_BUFFER"] = "TRUE";
        rasterInfo["MEM_BUFFER_NCOLS"] = te::common::Convert2String( nCols );
        rasterInfo["MEM_BUFFER_NROWS"] = te::common::Convert2String( nLines );
        rasterInfo["MEM_BUFFER_DATATYPE"] = te::common::Convert2String( tlDataType );
        rasterInfo["MEM_BUFFER_NBANDS"] = te::common::Convert2String( nBands );  
        
        outRasterPtr.reset( te::rst::RasterFactory::make(
            "MEM", 
            rasterInfo, 
            idlValuePointer->value.arr->data, 
            takeBufferOwnership ? te::idl::deleteArray : te::idl::dummyFunction ) );        
            
        if( outRasterPtr.get() )
        {
          return true;
        }
        else
        {
          outRasterPtr.reset();
          return false;
        }
        
      }
    }
    
  } // namespace idl
} // namespace te  
  
