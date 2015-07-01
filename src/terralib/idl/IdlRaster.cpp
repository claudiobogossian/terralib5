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
  \file terralib/idl/IdlRaster.cpp

  \brief A adaptor to an external existent raster in a idl array form.
*/

#include "Exception.h"
#include "IdlRaster.h"
#include "IdlBand.h"
#include "Functions.h"

#include "../common/Translator.h"
#include "../raster/Utils.h"

te::idl::IdlRaster::IdlRaster()
{
  throw te::idl::Exception( "Not implemented" );
}

te::idl::IdlRaster::IdlRaster( te::rst::Grid* grid, te::common::AccessPolicy p )
: te::rst::Raster( grid, p )
{
  throw te::idl::Exception( "Not implemented" );
}

te::idl::IdlRaster::IdlRaster( const te::rst::Raster& rhs )
: te::rst::Raster( rhs )
{
  throw te::idl::Exception( "Not implemented" );
}

te::idl::IdlRaster::IdlRaster( const IDL_VPTR& idlValuePointer,
  const bool takeBufferOwnership )
  : m_takeBufferOwnership( takeBufferOwnership )
{
  if( ! ( idlValuePointer->flags & IDL_V_ARR) )
  {
    throw te::idl::Exception( "Invalid IDL value pointer" );
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
      throw te::idl::Exception( "Invalid IDL value pointer" );
    }
    
    m_dataType = te::idl::idl2TerralibType( idlValuePointer->type );
    m_allBandsDataPtr = (unsigned char*)idlValuePointer->value.arr->data;
    
    m_grid = new te::rst::Grid( nCols, nLines );
    
    m_policy = te::common::RAccess;
    
    for( unsigned int bandIdx = 0 ; bandIdx < nBands ; ++bandIdx )
    {
      m_bands.push_back( new te::idl::IdlBand( this, m_allBandsDataPtr,
        bandIdx, m_dataType, nBands, nLines, nCols  ) );
    }
  }
}

te::idl::IdlRaster::~IdlRaster()
{
  if( m_takeBufferOwnership ) delete[] m_allBandsDataPtr;
  
  std::vector< te::idl::IdlBand* >::iterator bandsIt = m_bands.begin();
  std::vector< te::idl::IdlBand* >::iterator bandsItEnd = m_bands.end();
  
  while( bandsIt != bandsItEnd )
  {
    delete (*bandsIt);
    ++bandsIt;
  }
}

void te::idl::IdlRaster::open(const std::map<std::string, std::string>& rinfo, 
  te::common::AccessPolicy p)
{
  throw te::idl::Exception( "Not implemented" );
}

std::map<std::string, std::string> te::idl::IdlRaster::getInfo() const
{
  throw te::idl::Exception( "Not implemented" );
};        


te::dt::AbstractData* te::idl::IdlRaster::clone() const
{
  throw te::idl::Exception( "Not implemented" );
}

