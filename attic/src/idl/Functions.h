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
\file terralib/idl/Functions.h

\brief IDL module functions.
*/ 
 
#ifndef __TERRALIB_IDL_INTERNAL_FUNCTIONS_H
#define __TERRALIB_IDL_INTERNAL_FUNCTIONS_H

#include "Config.h"

#include <boost/shared_ptr.hpp>

#include <terralib/raster/Raster.h>

#include <stdio.h>

#include <idl_export.h>

namespace te
{
  namespace idl
  {
    /*!
      \brief Convert IDL data types to Terralib data types.
      
      \param idlType IDL data type.
      
      \return Terralib data type.
      
      \sa terralib/datetype/Enums.h for more details.
      */
    int idl2TerralibType( const UCHAR& idlType );

    /*!
      \brief Convert Terralib data types to IDL data types.
      
      \param idlType IDL data type.
      
      \return IDL data type.
      
      \sa terralib/datetype/Enums.h for more details.
      */
    UCHAR terralib2IdlType( const int& tlType );
    
    /*!
      \brief A dummy function for internal use.
      \details It does nothing
      */
    inline void dummyFunction( void* ) {};
    
    /*!
      \brief Delete an array of elements.
      */
    inline void deleteArray( void* arrayPtr )
    {
      delete[] (unsigned char*)arrayPtr;
    };    
    
    /*!
      \brief Load the required terralib modules.
      */
    void loadTerralibModules();
    
    bool createRasterFromIdlArray( IDL_VPTR idlValuePointer, const bool takeBufferOwnership,
      boost::shared_ptr< te::rst::Raster >& outRasterPtr );
    
    
  }; // namespace idl
}; // namespace te

#endif // __TERRALIB_IDL_INTERNAL_FUNCTIONS_H
