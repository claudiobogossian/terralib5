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
  \file terralib/unittest/raster/TsRaster.cpp
 
  \brief A test suit for the Cached Raster class.
 */

// TerraLib
#include <terralib/dataaccess.h>
#include <terralib/datatype.h>
#include <terralib/raster.h>
#include <terralib/memory/CachedRaster.h>
#include "../Config.h"

// Boost
#include <boost/test/unit_test.hpp>
#include <boost/shared_ptr.hpp>

BOOST_AUTO_TEST_SUITE ( raster_tests )

bool loadSHPFile( const std::string& shpFileName,
  std::vector< te::gm::Geometry* >& geomPtrs )
{
  std::map<std::string, std::string> connInfo;
  connInfo["URI"] = shpFileName;
  std::auto_ptr< te::da::DataSource > ogrDataSourcePtr(
    te::da::DataSourceFactory::make("OGR") );
  BOOST_CHECK( ogrDataSourcePtr.get() != 0 );
  
  ogrDataSourcePtr->setConnectionInfo(connInfo);
  ogrDataSourcePtr->open();
  
  BOOST_CHECK( ogrDataSourcePtr->isOpened() );
  
  std::vector<std::string> dataSetNames = ogrDataSourcePtr->getDataSetNames();
  std::auto_ptr< te::da::DataSourceTransactor > transactorPtr =
    ogrDataSourcePtr->getTransactor();
  
  for( unsigned int dataSetNamesIdx = 0 ; dataSetNamesIdx < dataSetNames.size() ;
    ++dataSetNamesIdx )
  {
    std::auto_ptr<te::da::DataSet> datasetPtr = transactorPtr->getDataSet(
      dataSetNames[ dataSetNamesIdx ] );
    
    const std::size_t nProperties = datasetPtr->getNumProperties();
    
    BOOST_CHECK( datasetPtr->moveBeforeFirst() );
    
    while( datasetPtr->moveNext() )
    {
      for( std::size_t propIdx = 0 ; propIdx < nProperties ; ++propIdx )
      {
        if( datasetPtr->getPropertyDataType( propIdx ) == te::dt::GEOMETRY_TYPE )
        {
          std::auto_ptr< te::gm::Geometry > geomPtr = datasetPtr->getGeometry(
            propIdx );
          
          if( geomPtr->getGeomTypeId() == te::gm::PolygonType )
          {
            geomPtrs.push_back( geomPtr.release() );
          }
          else if( geomPtr->getGeomTypeId() == te::gm::MultiPolygonType )
          {
            te::gm::MultiPolygon const * const mPolPtr = (te::gm::MultiPolygon const *)
              geomPtr.get();
              
            for( std::size_t gIdx = 0 ; gIdx < mPolPtr->getNumGeometries() ;
              ++gIdx )
            {
              geomPtrs.push_back( new te::gm::Polygon( *( (te::gm::Polygon*)
                mPolPtr->getGeometryN( gIdx ) ) ) );
            }
          }
        }
      }
    }
  }
  
  return true;
}

BOOST_AUTO_TEST_CASE (rasterConstructor_test)
{
}

BOOST_AUTO_TEST_CASE (rasterGrid_test)
{
}

BOOST_AUTO_TEST_CASE (rasterCopyConstructor_test)
{
}

BOOST_AUTO_TEST_CASE (rasterize_test)
{
  std::vector< te::gm::Geometry* > geomPtrs;
  BOOST_CHECK( loadSHPFile( TERRALIB_DATA_DIR "/shp/PoligonoUnico.shp",
    geomPtrs ) );
  
  std::vector<te::rst::BandProperty*> vecBandProp;
  vecBandProp.push_back( new te::rst::BandProperty( 0, te::dt::UCHAR_TYPE ) );
  vecBandProp[ 0 ]->m_blkh = 100;
  vecBandProp[ 0 ]->m_blkw = 100;
  vecBandProp[ 0 ]->m_nblocksx = 1;
  vecBandProp[ 0 ]->m_nblocksy = 1;
  vecBandProp[ 0 ]->m_noDataValue = 0;
  vecBandProp[ 0 ]->m_type = te::dt::UCHAR_TYPE;
  
  std::map<std::string, std::string> dsinfo;
  dsinfo["URI"] = "TsRastertcRasterize.tif";
  
  te::rst::Grid* grid = new te::rst::Grid( (unsigned int)
    vecBandProp[ 0 ]->m_blkw, (unsigned int)vecBandProp[ 0 ]->m_blkh,
    new te::gm::Envelope( *geomPtrs[ 0 ]->getMBR() ), geomPtrs[ 0 ]->getSRID() );
  
  std::auto_ptr<te::rst::Raster> rst(te::rst::RasterFactory::make("GDAL", grid,
    vecBandProp, dsinfo));
  
  std::vector< double > values;
  values.push_back( 1.0 );
  
  rst->rasterize( geomPtrs, values, 0 );
}

BOOST_AUTO_TEST_SUITE_END ()
