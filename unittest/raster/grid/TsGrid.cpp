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
  \file terralib/unittest/raster/TsGrid.cpp
 
  \brief A test suit for the Grid class.
 */

// TerraLib
#include <terralib/raster.h>
#include <terralib/geometry.h>
#include "../Config.h"

// Boost
#include <boost/test/unit_test.hpp>
#include <boost/shared_ptr.hpp>

BOOST_AUTO_TEST_SUITE (grid_tests)

BOOST_AUTO_TEST_CASE (gridConstructor1_test)
{
  te::rst::Grid grid( 2u, 2u, new te::gm::Envelope( 1.0, 1.0, 5.0, 3.0 ), 12345 );
  
  BOOST_CHECK( grid.getNumberOfColumns() == 2 );
  BOOST_CHECK( grid.getNumberOfRows() == 2 );
  BOOST_CHECK_CLOSE( 2.0, grid.getResolutionX(), 0.0000000001 );
  BOOST_CHECK_CLOSE( 1.0, grid.getResolutionY(), 0.0000000001 );
  BOOST_CHECK( grid.getSRID() == 12345 );
  BOOST_CHECK_CLOSE( 0.0, grid.getExtent()->m_llx, 1.0000000001 );
  BOOST_CHECK_CLOSE( 0.0, grid.getExtent()->m_lly, 1.0000000001 );
  BOOST_CHECK_CLOSE( 5.0, grid.getExtent()->m_urx, 0.0000000001 );
  BOOST_CHECK_CLOSE( 3.0, grid.getExtent()->m_ury, 0.0000000001 );
  
  double x = 0;
  double y = 0;
  
  grid.gridToGeo( 0.0 , 0.0, x, y );
  BOOST_CHECK_CLOSE( 2.0, x, 0.0000000001 );
  BOOST_CHECK_CLOSE( 2.5, y, 0.0000000001 );

  grid.gridToGeo( 1.0 , 1.0, x, y );
  BOOST_CHECK_CLOSE( 4.0, x, 0.0000000001 );
  BOOST_CHECK_CLOSE( 1.5, y, 0.0000000001 );
  
  grid.geoToGrid(2.0 , 2.5, x, y );
  BOOST_CHECK_CLOSE( 0.0, x, 0.0000000001 );
  BOOST_CHECK_CLOSE( 0.0, y, 0.0000000001 );

  grid.geoToGrid( 4.0 , 1.5, x, y );
  BOOST_CHECK_CLOSE( 1.0, x, 0.0000000001 );
  BOOST_CHECK_CLOSE( 1.0, y, 0.0000000001 );
}

BOOST_AUTO_TEST_CASE (gridConstructor1NoEnv_test)
{
  te::rst::Grid grid( 2u, 2u, (te::gm::Envelope*)0, 12345 );
  
  BOOST_CHECK( grid.getNumberOfColumns() == 2 );
  BOOST_CHECK( grid.getNumberOfRows() == 2 );
  BOOST_CHECK_CLOSE( 1.0, grid.getResolutionX(), 0.0000000001 );
  BOOST_CHECK_CLOSE( 1.0, grid.getResolutionY(), 0.0000000001 );
  BOOST_CHECK( grid.getSRID() == 12345 );
  BOOST_CHECK_CLOSE( 0.0, grid.getExtent()->m_llx, 0.0000000001 );
  BOOST_CHECK_CLOSE( -2.0, grid.getExtent()->m_lly, 0.0000000001 );
  BOOST_CHECK_CLOSE( 2.0, grid.getExtent()->m_urx, 0.0000000001 );
  BOOST_CHECK_CLOSE( 0.0, grid.getExtent()->m_ury, 0.0000000001 );
  
  double x = 0;
  double y = 0;
  
  grid.gridToGeo( 0.0 , 0.0, x, y );
  BOOST_CHECK_CLOSE( 0.5, x, 0.0000000001 );
  BOOST_CHECK_CLOSE( -0.5, y, 0.0000000001 );

  grid.gridToGeo( 1.0 , 1.0, x, y );
  BOOST_CHECK_CLOSE( 1.5, x, 0.0000000001 );
  BOOST_CHECK_CLOSE( -1.5, y, 0.0000000001 );
  
  grid.geoToGrid(0.5 , -0.5, x, y );
  BOOST_CHECK_CLOSE( 0.0, x, 0.0000000001 );
  BOOST_CHECK_CLOSE( 0.0, y, 0.0000000001 );

  grid.geoToGrid( 1.5 , -1.5, x, y );
  BOOST_CHECK_CLOSE( 1.0, x, 0.0000000001 );
  BOOST_CHECK_CLOSE( 1.0, y, 0.0000000001 );
}

BOOST_AUTO_TEST_CASE (gridConstructor2_test)
{
  te::gm::Coord2D ulc;
  ulc.x = 1;
  ulc.y = 3;
  
  te::rst::Grid grid( 2u, 2u, 2.0, 1.0, &ulc, 12345 );
  
  BOOST_CHECK( grid.getNumberOfColumns() == 2 );
  BOOST_CHECK( grid.getNumberOfRows() == 2 );
  BOOST_CHECK_CLOSE( 2.0, grid.getResolutionX(), 0.0000000001 );
  BOOST_CHECK_CLOSE( 1.0, grid.getResolutionY(), 0.0000000001 );
  BOOST_CHECK( grid.getSRID() == 12345 );
  BOOST_CHECK_CLOSE( 1.0, grid.getExtent()->m_llx, 0.0000000001 );
  BOOST_CHECK_CLOSE( 1.0, grid.getExtent()->m_lly, 0.0000000001 );
  BOOST_CHECK_CLOSE( 5.0, grid.getExtent()->m_urx, 0.0000000001 );
  BOOST_CHECK_CLOSE( 3.0, grid.getExtent()->m_ury, 0.0000000001 );
  
  double x = 0;
  double y = 0;
  
  grid.gridToGeo( 0.0 , 0.0, x, y );
  BOOST_CHECK_CLOSE( 2.0, x, 0.0000000001 );
  BOOST_CHECK_CLOSE( 2.5, y, 0.0000000001 );

  grid.gridToGeo( 1.0 , 1.0, x, y );
  BOOST_CHECK_CLOSE( 4.0, x, 0.0000000001 );
  BOOST_CHECK_CLOSE( 1.5, y, 0.0000000001 );
  
  grid.geoToGrid(2.0 , 2.5, x, y );
  BOOST_CHECK_CLOSE( 0.0, x, 0.0000000001 );
  BOOST_CHECK_CLOSE( 0.0, y, 0.0000000001 );

  grid.geoToGrid( 4.0 , 1.5, x, y );
  BOOST_CHECK_CLOSE( 1.0, x, 0.0000000001 );
  BOOST_CHECK_CLOSE( 1.0, y, 0.0000000001 );
}

BOOST_AUTO_TEST_CASE (gridConstructor2NoUlc_test)
{
  te::rst::Grid grid( 2u, 2u, 1.0, 1.0, (te::gm::Coord2D*)0, 12345 );
  
  BOOST_CHECK( grid.getNumberOfColumns() == 2 );
  BOOST_CHECK( grid.getNumberOfRows() == 2 );
  BOOST_CHECK_CLOSE( 1.0, grid.getResolutionX(), 0.0000000001 );
  BOOST_CHECK_CLOSE( 1.0, grid.getResolutionY(), 0.0000000001 );
  BOOST_CHECK( grid.getSRID() == 12345 );
  BOOST_CHECK_CLOSE( 0.0, grid.getExtent()->m_llx, 0.0000000001 );
  BOOST_CHECK_CLOSE( -2.0, grid.getExtent()->m_lly, 0.0000000001 );
  BOOST_CHECK_CLOSE( 2.0, grid.getExtent()->m_urx, 0.0000000001 );
  BOOST_CHECK_CLOSE( 0.0, grid.getExtent()->m_ury, 0.0000000001 );
  
  double x = 0;
  double y = 0;
  
  grid.gridToGeo( 0.0 , 0.0, x, y );
  BOOST_CHECK_CLOSE( 0.5, x, 0.0000000001 );
  BOOST_CHECK_CLOSE( -0.5, y, 0.0000000001 );

  grid.gridToGeo( 1.0 , 1.0, x, y );
  BOOST_CHECK_CLOSE( 1.5, x, 0.0000000001 );
  BOOST_CHECK_CLOSE( -1.5, y, 0.0000000001 );
  
  grid.geoToGrid(0.5 , -0.5, x, y );
  BOOST_CHECK_CLOSE( 0.0, x, 0.0000000001 );
  BOOST_CHECK_CLOSE( 0.0, y, 0.0000000001 );

  grid.geoToGrid( 1.5 , -1.5, x, y );
  BOOST_CHECK_CLOSE( 1.0, x, 0.0000000001 );
  BOOST_CHECK_CLOSE( 1.0, y, 0.0000000001 );
}

BOOST_AUTO_TEST_CASE (gridConstructor3_test)
{
  te::rst::Grid grid( 2u, 2u, 2.0, 1.0, new te::gm::Envelope( 1.0, 1.0, 5.0, 3.0 ), 12345 );
  
  BOOST_CHECK( grid.getNumberOfColumns() == 2 );
  BOOST_CHECK( grid.getNumberOfRows() == 2 );
  BOOST_CHECK_CLOSE( 2.0, grid.getResolutionX(), 0.0000000001 );
  BOOST_CHECK_CLOSE( 1.0, grid.getResolutionY(), 0.0000000001 );
  BOOST_CHECK( grid.getSRID() == 12345 );
  BOOST_CHECK_CLOSE( 1.0, grid.getExtent()->m_llx, 0.0000000001 );
  BOOST_CHECK_CLOSE( 1.0, grid.getExtent()->m_lly, 0.0000000001 );
  BOOST_CHECK_CLOSE( 5.0, grid.getExtent()->m_urx, 0.0000000001 );
  BOOST_CHECK_CLOSE( 3.0, grid.getExtent()->m_ury, 0.0000000001 );
  
  double x = 0;
  double y = 0;
  
  grid.gridToGeo( 0.0 , 0.0, x, y );
  BOOST_CHECK_CLOSE( 2.0, x, 0.0000000001 );
  BOOST_CHECK_CLOSE( 2.5, y, 0.0000000001 );

  grid.gridToGeo( 1.0 , 1.0, x, y );
  BOOST_CHECK_CLOSE( 4.0, x, 0.0000000001 );
  BOOST_CHECK_CLOSE( 1.5, y, 0.0000000001 );
  
  grid.geoToGrid(2.0 , 2.5, x, y );
  BOOST_CHECK_CLOSE( 0.0, x, 0.0000000001 );
  BOOST_CHECK_CLOSE( 0.0, y, 0.0000000001 );

  grid.geoToGrid( 4.0 , 1.5, x, y );
  BOOST_CHECK_CLOSE( 1.0, x, 0.0000000001 );
  BOOST_CHECK_CLOSE( 1.0, y, 0.0000000001 );
}

BOOST_AUTO_TEST_CASE (gridConstructor4_test)
{
  te::rst::Grid grid( 2.0, 1.0, new te::gm::Envelope( 1.0, 1.0, 5.0, 3.0 ), 12345 );
  
  BOOST_CHECK( grid.getNumberOfColumns() == 2 );
  BOOST_CHECK( grid.getNumberOfRows() == 2 );
  BOOST_CHECK_CLOSE( 2.0, grid.getResolutionX(), 0.0000000001 );
  BOOST_CHECK_CLOSE( 1.0, grid.getResolutionY(), 0.0000000001 );
  BOOST_CHECK( grid.getSRID() == 12345 );
  BOOST_CHECK_CLOSE( 1.0, grid.getExtent()->m_llx, 0.0000000001 );
  BOOST_CHECK_CLOSE( 1.0, grid.getExtent()->m_lly, 0.0000000001 );
  BOOST_CHECK_CLOSE( 5.0, grid.getExtent()->m_urx, 0.0000000001 );
  BOOST_CHECK_CLOSE( 3.0, grid.getExtent()->m_ury, 0.0000000001 );
  
  double x = 0;
  double y = 0;
  
  grid.gridToGeo( 0.0 , 0.0, x, y );
  BOOST_CHECK_CLOSE( 2.0, x, 0.0000000001 );
  BOOST_CHECK_CLOSE( 2.5, y, 0.0000000001 );

  grid.gridToGeo( 1.0 , 1.0, x, y );
  BOOST_CHECK_CLOSE( 4.0, x, 0.0000000001 );
  BOOST_CHECK_CLOSE( 1.5, y, 0.0000000001 );
  
  grid.geoToGrid(2.0 , 2.5, x, y );
  BOOST_CHECK_CLOSE( 0.0, x, 0.0000000001 );
  BOOST_CHECK_CLOSE( 0.0, y, 0.0000000001 );

  grid.geoToGrid( 4.0 , 1.5, x, y );
  BOOST_CHECK_CLOSE( 1.0, x, 0.0000000001 );
  BOOST_CHECK_CLOSE( 1.0, y, 0.0000000001 );
}

BOOST_AUTO_TEST_CASE (gridConstructor5_test)
{
  double geoTrans[ 6 ];
  geoTrans[ 0 ] = 2.0;
  geoTrans[ 1 ] = 0.0;
  geoTrans[ 2 ] = 2.0;
  geoTrans[ 3 ] = 0.0;
  geoTrans[ 4 ] = -1.0;
  geoTrans[ 5 ] = 2.5;
  
  te::rst::Grid grid( geoTrans, 2u, 2u, 12345 );
  
  BOOST_CHECK( grid.getNumberOfColumns() == 2 );
  BOOST_CHECK( grid.getNumberOfRows() == 2 );
  BOOST_CHECK_CLOSE( 2.0, grid.getResolutionX(), 0.0000000001 );
  BOOST_CHECK_CLOSE( 1.0, grid.getResolutionY(), 0.0000000001 );
  BOOST_CHECK( grid.getSRID() == 12345 );
  BOOST_CHECK_CLOSE( 1.0, grid.getExtent()->m_llx, 0.0000000001 );
  BOOST_CHECK_CLOSE( 1.0, grid.getExtent()->m_lly, 0.0000000001 );
  BOOST_CHECK_CLOSE( 5.0, grid.getExtent()->m_urx, 0.0000000001 );
  BOOST_CHECK_CLOSE( 3.0, grid.getExtent()->m_ury, 0.0000000001 );
  
  double x = 0;
  double y = 0;
  
  grid.gridToGeo( 0.0 , 0.0, x, y );
  BOOST_CHECK_CLOSE( 2.0, x, 0.0000000001 );
  BOOST_CHECK_CLOSE( 2.5, y, 0.0000000001 );

  grid.gridToGeo( 1.0 , 1.0, x, y );
  BOOST_CHECK_CLOSE( 4.0, x, 0.0000000001 );
  BOOST_CHECK_CLOSE( 1.5, y, 0.0000000001 );
  
  grid.geoToGrid(2.0 , 2.5, x, y );
  BOOST_CHECK_CLOSE( 0.0, x, 0.0000000001 );
  BOOST_CHECK_CLOSE( 0.0, y, 0.0000000001 );

  grid.geoToGrid( 4.0 , 1.5, x, y );
  BOOST_CHECK_CLOSE( 1.0, x, 0.0000000001 );
  BOOST_CHECK_CLOSE( 1.0, y, 0.0000000001 );
}

BOOST_AUTO_TEST_SUITE_END()
