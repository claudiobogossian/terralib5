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
  \file TsGrid.cpp
 
  \brief A test suit for the Grid class.
 */

#include "TsGrid.h"
#include "../Config.h"

#include <terralib/raster/Grid.h>
#include <terralib/geometry/Envelope.h>
#include <terralib/geometry/Coord2D.h>

//#include <terralib/dataaccess/datasource/DataSourceFactory.h>

#include <boost/shared_ptr.hpp>

CPPUNIT_TEST_SUITE_REGISTRATION( TsGrid );

void TsGrid::tcGridConstructor1()
{
  te::rst::Grid grid( 2u, 2u, new te::gm::Envelope( 0.0, 0.0, 2.0, 2.0 ), 12345 );
  
  CPPUNIT_ASSERT( grid.getNumberOfColumns() == 2 );
  CPPUNIT_ASSERT( grid.getNumberOfRows() == 2 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0, grid.getResolutionX(), 0.0000000001 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0, grid.getResolutionY(), 0.0000000001 );
  CPPUNIT_ASSERT( grid.getSRID() == 12345 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0, grid.getExtent()->m_llx, 0.0000000001 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0, grid.getExtent()->m_lly, 0.0000000001 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 2.0, grid.getExtent()->m_urx, 0.0000000001 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 2.0, grid.getExtent()->m_ury, 0.0000000001 ); 
  
  double x = 0;
  double y = 0;
  
  grid.gridToGeo( 0.0 , 0.0, x, y );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.5, x, 0.0000000001 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.5, y, 0.0000000001 ); 

  grid.gridToGeo( 1.0 , 1.0, x, y );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.5, x, 0.0000000001 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.5, y, 0.0000000001 );  
  
  grid.geoToGrid(1.5 , 0.5, x, y );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0, x, 0.0000000001 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0, y, 0.0000000001 ); 

  grid.geoToGrid( 0.5 , 1.5, x, y );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0, x, 0.0000000001 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0, y, 0.0000000001 );    
}

void TsGrid::tcGridConstructor1NoEnv()
{
  te::rst::Grid grid( 2u, 2u, (te::gm::Envelope*)0, 12345 );
  
  CPPUNIT_ASSERT( grid.getNumberOfColumns() == 2 );
  CPPUNIT_ASSERT( grid.getNumberOfRows() == 2 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0, grid.getResolutionX(), 0.0000000001 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0, grid.getResolutionY(), 0.0000000001 );
  CPPUNIT_ASSERT( grid.getSRID() == 12345 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0, grid.getExtent()->m_llx, 0.0000000001 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( -2.0, grid.getExtent()->m_lly, 0.0000000001 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 2.0, grid.getExtent()->m_urx, 0.0000000001 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0, grid.getExtent()->m_ury, 0.0000000001 ); 
  
  double x = 0;
  double y = 0;
  
  grid.gridToGeo( 0.0 , 0.0, x, y );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.5, x, 0.0000000001 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( -0.5, y, 0.0000000001 ); 

  grid.gridToGeo( 1.0 , 1.0, x, y );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.5, x, 0.0000000001 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( -1.5, y, 0.0000000001 );  
  
  grid.geoToGrid(0.5 , -0.5, x, y );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0, x, 0.0000000001 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0, y, 0.0000000001 ); 

  grid.geoToGrid( 1.5 , -1.5, x, y );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0, x, 0.0000000001 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0, y, 0.0000000001 );    
}


void TsGrid::tcGridConstructor2()
{
  te::gm::Coord2D ulc;
  ulc.x = 0;
  ulc.y = 2;
  
  te::rst::Grid grid( 2u, 2u, 1.0, 1.0, &ulc, 12345 );
  
  CPPUNIT_ASSERT( grid.getNumberOfColumns() == 2 );
  CPPUNIT_ASSERT( grid.getNumberOfRows() == 2 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0, grid.getResolutionX(), 0.0000000001 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0, grid.getResolutionY(), 0.0000000001 );
  CPPUNIT_ASSERT( grid.getSRID() == 12345 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0, grid.getExtent()->m_llx, 0.0000000001 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0, grid.getExtent()->m_lly, 0.0000000001 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 2.0, grid.getExtent()->m_urx, 0.0000000001 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 2.0, grid.getExtent()->m_ury, 0.0000000001 ); 
  
  double x = 0;
  double y = 0;
  
  grid.gridToGeo( 0.0 , 0.0, x, y );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.5, x, 0.0000000001 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.5, y, 0.0000000001 ); 

  grid.gridToGeo( 1.0 , 1.0, x, y );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.5, x, 0.0000000001 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.5, y, 0.0000000001 );  
  
  grid.geoToGrid(1.5 , 0.5, x, y );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0, x, 0.0000000001 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0, y, 0.0000000001 ); 

  grid.geoToGrid( 0.5 , 1.5, x, y );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0, x, 0.0000000001 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0, y, 0.0000000001 );  
}

void TsGrid::tcGridConstructor2NoUlc()
{
  te::rst::Grid grid( 2u, 2u, 1.0, 1.0, (te::gm::Coord2D*)0, 12345 );
  
  CPPUNIT_ASSERT( grid.getNumberOfColumns() == 2 );
  CPPUNIT_ASSERT( grid.getNumberOfRows() == 2 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0, grid.getResolutionX(), 0.0000000001 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0, grid.getResolutionY(), 0.0000000001 );
  CPPUNIT_ASSERT( grid.getSRID() == 12345 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0, grid.getExtent()->m_llx, 0.0000000001 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( -2.0, grid.getExtent()->m_lly, 0.0000000001 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 2.0, grid.getExtent()->m_urx, 0.0000000001 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0, grid.getExtent()->m_ury, 0.0000000001 ); 
  
  double x = 0;
  double y = 0;
  
  grid.gridToGeo( 0.0 , 0.0, x, y );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.5, x, 0.0000000001 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( -0.5, y, 0.0000000001 ); 

  grid.gridToGeo( 1.0 , 1.0, x, y );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.5, x, 0.0000000001 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( -1.5, y, 0.0000000001 );  
  
  grid.geoToGrid(0.5 , -0.5, x, y );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0, x, 0.0000000001 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0, y, 0.0000000001 ); 

  grid.geoToGrid( 1.5 , -1.5, x, y );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0, x, 0.0000000001 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0, y, 0.0000000001 );  
}

void TsGrid::tcGridConstructor3()
{
  te::rst::Grid grid( 1.0, 1.0, new te::gm::Envelope( 0.0, 0.0, 2.0, 2.0 ), 12345 );
  
  CPPUNIT_ASSERT( grid.getNumberOfColumns() == 2 );
  CPPUNIT_ASSERT( grid.getNumberOfRows() == 2 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0, grid.getResolutionX(), 0.0000000001 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0, grid.getResolutionY(), 0.0000000001 );
  CPPUNIT_ASSERT( grid.getSRID() == 12345 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0, grid.getExtent()->m_llx, 0.0000000001 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0, grid.getExtent()->m_lly, 0.0000000001 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 2.0, grid.getExtent()->m_urx, 0.0000000001 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 2.0, grid.getExtent()->m_ury, 0.0000000001 ); 
  
  double x = 0;
  double y = 0;
  
  grid.gridToGeo( 0.0 , 0.0, x, y );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.5, x, 0.0000000001 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.5, y, 0.0000000001 ); 

  grid.gridToGeo( 1.0 , 1.0, x, y );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.5, x, 0.0000000001 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.5, y, 0.0000000001 );  
  
  grid.geoToGrid(1.5 , 0.5, x, y );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0, x, 0.0000000001 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0, y, 0.0000000001 ); 

  grid.geoToGrid( 0.5 , 1.5, x, y );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0, x, 0.0000000001 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0, y, 0.0000000001 );      
}


