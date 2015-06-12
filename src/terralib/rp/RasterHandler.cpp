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
    }
    
    RasterHandler::RasterHandler(
      te::da::DataSource* dataSourcePtr,
      te::da::DataSourceTransactor* transactorPtr,
      te::da::DataSet* dataSetPtr,
      te::rst::Raster* rasterPtr )
    {
      m_dataSourcePtr.reset( dataSourcePtr );
      m_transactorPtr.reset( transactorPtr );
      m_dataSetPtr.reset( dataSetPtr );
      m_rasterPtr.reset( rasterPtr );
    }    
    
    RasterHandler::RasterHandler(
      te::da::DataSourceTransactor* transactorPtr,
      te::da::DataSet* dataSetPtr,
      te::rst::Raster* rasterPtr )
    {
      m_transactorPtr.reset( transactorPtr );
      m_dataSetPtr.reset( dataSetPtr );
      m_rasterPtr.reset( rasterPtr );
    }    
    
    
    RasterHandler::RasterHandler( te::rst::Raster* rasterPtr )
    {
      m_rasterPtr.reset( rasterPtr );
    }     
    
    RasterHandler::~RasterHandler()
    {
      reset();
    }
    
    void RasterHandler::reset()
    {
      // Inverse order is requred
      m_rasterPtr.reset();
      m_dataSetPtr.reset();
      m_transactorPtr.reset();
      m_dataSourcePtr.reset();
    }    
    
    void RasterHandler::reset( 
      te::da::DataSource* dataSourcePtr,
      te::da::DataSourceTransactor* transactorPtr,
      te::da::DataSet* dataSetPtr,
      te::rst::Raster* rasterPtr )
    {
      reset();
      
      m_dataSourcePtr.reset( dataSourcePtr );
      m_transactorPtr.reset( transactorPtr );
      m_dataSetPtr.reset( dataSetPtr );
      m_rasterPtr.reset( rasterPtr );
    }     
    
    void RasterHandler::reset( 
      te::da::DataSourceTransactor* transactorPtr,
      te::da::DataSet* dataSetPtr,
      te::rst::Raster* rasterPtr )
    {
      reset();
      
      m_transactorPtr.reset( transactorPtr );
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
    
    void RasterHandler::release( 
      std::auto_ptr< te::da::DataSource >& dataSourcePtr,
      std::auto_ptr< te::da::DataSourceTransactor >& transactorPtr,
      std::auto_ptr< te::da::DataSet >& dataSetPtr,
      std::auto_ptr< te::rst::Raster >& rasterPtr )
    {
      rasterPtr.reset( m_rasterPtr.release() );
      dataSetPtr.reset( m_dataSetPtr.release() );
      transactorPtr.reset( m_transactorPtr.release() );
      dataSourcePtr.reset( m_dataSourcePtr.release() );      
    }
    
    void RasterHandler::release( RasterHandler& other )
    {
      other.m_rasterPtr.reset( m_rasterPtr.release() );
      other.m_dataSetPtr.reset( m_dataSetPtr.release() );
      other.m_transactorPtr.reset( m_transactorPtr.release() );
      other.m_dataSourcePtr.reset( m_dataSourcePtr.release() );      
    }    
    
  } // end namespace rp
}   // end namespace te    

