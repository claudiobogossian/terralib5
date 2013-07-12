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
  \file terralib/rp/RasterHandler.cpp
  \brief Raster tuple.
*/

#include "RasterHandler.h"

namespace te
{
  namespace rp
  {
    RasterHandler::RasterHandler()
    {
    };

    RasterHandler::RasterHandler(
      const boost::shared_ptr< te::da::DataSource >& dataSourcePtr,
      const boost::shared_ptr< te::da::DataSet >& dataSetPtr,
      const boost::shared_ptr< te::rst::Raster >& rasterPtr )
    {
      m_dataSourcePtr = dataSourcePtr;
      m_dataSetPtr = dataSetPtr;
      m_rasterPtr = rasterPtr;
    };
    
    RasterHandler::RasterHandler(
      te::da::DataSource* dataSourcePtr,
      te::da::DataSet* dataSetPtr,
      te::rst::Raster* rasterPtr )
    {
      m_dataSourcePtr.reset( dataSourcePtr );
      m_dataSetPtr.reset( dataSetPtr );
      m_rasterPtr.reset( rasterPtr );
    };    
    
    RasterHandler::RasterHandler( te::rst::Raster* rasterPtr )
    {
      m_rasterPtr.reset( rasterPtr );
    };     
    
    RasterHandler::~RasterHandler()
    {
      reset();
    };
    
    const RasterHandler& RasterHandler::operator=( const RasterHandler& ext )
    {
      reset();
      
      m_rasterPtr = ext.m_rasterPtr;     
      m_dataSetPtr = ext.m_dataSetPtr;
      m_dataSourcePtr = ext.m_dataSourcePtr;
      
      return *this;
    }
    
    void RasterHandler::reset()
    {
      // Inverse order is requred
      m_rasterPtr.reset();
      m_dataSetPtr.reset();
      m_dataSourcePtr.reset();
    };    
    
    void RasterHandler::reset( 
      const boost::shared_ptr< te::da::DataSource >& dataSourcePtr,
      const boost::shared_ptr< te::da::DataSet >& dataSetPtr,
      const boost::shared_ptr< te::rst::Raster >& rasterPtr )
    {
      reset();
      
      m_rasterPtr = rasterPtr;
      m_dataSetPtr = dataSetPtr;
      m_dataSourcePtr = dataSourcePtr;
    }    
    
    void RasterHandler::reset( 
      te::da::DataSource* dataSourcePtr,
      te::da::DataSet* dataSetPtr,
      te::rst::Raster* rasterPtr )
    {
      reset();
      
      m_dataSourcePtr.reset( dataSourcePtr );
      m_dataSetPtr.reset( dataSetPtr );
      m_rasterPtr.reset( rasterPtr );
    }     
    
    void RasterHandler::reset( te::rst::Raster* rasterPtr )
    {
      reset();
      m_rasterPtr.reset( rasterPtr );
    }       
    
    te::rst::Raster* RasterHandler::getRasterPtr()
    {
      return m_rasterPtr.get();
    }
    
  } // end namespace rp
}   // end namespace te    

