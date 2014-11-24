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
  \file TsMatrix.cpp

  \brief A test suit for the DataSource interface.
 */

#include "TsMatrix.h"
#include "../Config.h"

#include <terralib/common/PlatformUtils.h>

CPPUNIT_TEST_SUITE_REGISTRATION( TsMatrix );

void TsMatrix::genericTests()
{
  /* Checking basic matrix creation and access */

  te::rp::Matrix< double > m1;
  CPPUNIT_ASSERT( m1.reset( 2, 2,
    te::rp::Matrix< double >::RAMMemPol ) );
  CPPUNIT_ASSERT( ( ! m1.isEmpty() ) );
  CPPUNIT_ASSERT( m1.getMemPolicy() == te::rp::Matrix< double >::RAMMemPol );

  m1( 0 , 0 ) = 0.0;
  m1( 0 , 1 ) = 1.0;
  m1( 1 , 0 ) = 2.0;
  m1( 1 , 1 ) = 3.0;

  CPPUNIT_ASSERT( m1.getLinesNumber() == 2 );
  CPPUNIT_ASSERT( m1.getColumnsNumber() == 2 );

  CPPUNIT_ASSERT( m1( 0 , 0 ) == 0.0 );
  CPPUNIT_ASSERT( m1( 0 , 1 ) == 1.0 );
  CPPUNIT_ASSERT( m1( 1 , 0 ) == 2.0 );
  CPPUNIT_ASSERT( m1( 1 , 1 ) == 3.0 );

  CPPUNIT_ASSERT( m1[ 0 ][ 0 ] == 0.0 );
  CPPUNIT_ASSERT( m1[ 0 ][ 1 ] == 1.0 );
  CPPUNIT_ASSERT( m1[ 1 ][ 0 ] == 2.0 );
  CPPUNIT_ASSERT( m1[ 1 ][ 1 ] == 3.0 );

  /* Reset test */

  CPPUNIT_ASSERT( m1.reset( 2, 2 ) );
  CPPUNIT_ASSERT( m1.getMemPolicy() == te::rp::Matrix< double >::RAMMemPol );

  m1( 0 , 0 ) = 0.0;
  m1( 0 , 1 ) = 1.0;
  m1( 1 , 0 ) = 2.0;
  m1( 1 , 1 ) = 3.0;

  CPPUNIT_ASSERT( m1.getLinesNumber() == 2 );
  CPPUNIT_ASSERT( m1.getColumnsNumber() == 2 );

  CPPUNIT_ASSERT( m1( 0 , 0 ) == 0.0 );
  CPPUNIT_ASSERT( m1( 0 , 1 ) == 1.0 );
  CPPUNIT_ASSERT( m1( 1 , 0 ) == 2.0 );
  CPPUNIT_ASSERT( m1( 1 , 1 ) == 3.0 );

  /* Checking copy operator */

  te::rp::Matrix< double > m2;
  CPPUNIT_ASSERT( m2.isEmpty() );
  CPPUNIT_ASSERT( m2.getMemPolicy() == te::rp::Matrix< double >::RAMMemPol );

  CPPUNIT_ASSERT( m2.getLinesNumber() == 0 );
  CPPUNIT_ASSERT( m2.getColumnsNumber() == 0 );

  m2 = m1;

  CPPUNIT_ASSERT( m2.getLinesNumber() == 2 );
  CPPUNIT_ASSERT( m2.getColumnsNumber() == 2 );

  CPPUNIT_ASSERT( m2( 0 , 0 ) == 0.0 );
  CPPUNIT_ASSERT( m2( 0 , 1 ) == 1.0 );
  CPPUNIT_ASSERT( m2( 1 , 0 ) == 2.0 );
  CPPUNIT_ASSERT( m2( 1 , 1 ) == 3.0 );

  /* Checking copy constructor */

  te::rp::Matrix< double > m3( m1 );
  CPPUNIT_ASSERT( m3.getMemPolicy() == te::rp::Matrix< double >::RAMMemPol );

  CPPUNIT_ASSERT( m3.getLinesNumber() == 2 );
  CPPUNIT_ASSERT( m3.getColumnsNumber() == 2 );

  CPPUNIT_ASSERT( m3( 0 , 0 ) == 0.0 );
  CPPUNIT_ASSERT( m3( 0 , 1 ) == 1.0 );
  CPPUNIT_ASSERT( m3( 1 , 0 ) == 2.0 );
  CPPUNIT_ASSERT( m3( 1 , 1 ) == 3.0 );
}

void TsMatrix::rAMMemPolTest()
{
  const unsigned int max_ram  = (unsigned int)
    ( 0.20 * (double)MIN( ( te::common::GetTotalVirtualMemory() / 2 ) - 
    te::common::GetUsedVirtualMemory(), te::common::GetTotalPhysicalMemory() ) );

  unsigned long int lines = (unsigned long int)(
    ( (double)( max_ram ) ) /
    ( ( (double)sizeof( double ) ) * 1000.00 ) );
  unsigned long int cols = 1000;

  te::rp::Matrix< double > m1;
  CPPUNIT_ASSERT( m1.reset( lines, cols, te::rp::Matrix< double >::RAMMemPol ) );

  CPPUNIT_ASSERT( m1.getMemPolicy() == te::rp::Matrix< double >::RAMMemPol );

  double counter = 0.0;

  unsigned int line = 0;
  unsigned int col = 0;

  for( line = 0 ; line < lines ; ++line ) {
    for( col = 0 ; col < cols ; ++col ) {
      m1( line , col ) = counter;

      ++counter;
    }
  }

  counter = 0.0;

  for( line = 0 ; line < lines ; ++line ) {
    for( col = 0 ; col < cols ; ++col ) {
      CPPUNIT_ASSERT( ( m1( line , col ) == counter ) );

      ++counter;
    }
  }
  
  m1.reset();

  return;
}

void TsMatrix::autoMemPolTest()
{
  const unsigned long int max_ram  = te::common::GetTotalPhysicalMemory() / 10;

  unsigned long int lines = (unsigned long int)
    (
      ( 2 * max_ram ) 
      /
      ( sizeof( double ) * 1000 ) 
    );
  unsigned long int cols = 1000;

  te::rp::Matrix< double > m1;
  CPPUNIT_ASSERT( m1.reset( lines, cols, te::rp::Matrix< double >::AutoMemPol, 10 ) );

  CPPUNIT_ASSERT( m1.getMemPolicy() == te::rp::Matrix< double >::AutoMemPol );

  double counter = 0.0;

  unsigned int line = 0;
  unsigned int col = 0;

  for( line = 0 ; line < lines ; ++line ) {
    for( col = 0 ; col < cols ; ++col ) {
      m1( line , col ) = counter;

      ++counter;
    }
  }

  counter = 0.0;

  for( line = 0 ; line < lines ; ++line ) {
    for( col = 0 ; col < cols ; ++col ) {
      CPPUNIT_ASSERT( ( m1( line , col ) == counter ) );

      ++counter;
    }
  }
  
  m1.reset();

  return;
}

void TsMatrix::diskMemPolTest()
{
  unsigned long int lines = 3000;
  unsigned long int cols = 4000;

  te::rp::Matrix< double > m1;
  CPPUNIT_ASSERT( m1.reset( lines, cols, 
    te::rp::Matrix< double >::DiskMemPol ) );

  CPPUNIT_ASSERT( m1.getMemPolicy() == te::rp::Matrix< double >::DiskMemPol );
  CPPUNIT_ASSERT( m1.getLinesNumber() == lines );
  CPPUNIT_ASSERT( m1.getColumnsNumber() == cols );

  unsigned int line = 0;
  unsigned int col = 0;
  double counter = 0.0;

  for( line = 0 ; line < lines ; ++line ) {
    for( col = 0 ; col < cols ; ++col ) {
      m1( line , col ) = counter;
      ++counter;
    }
  }

  counter = 0.0;

  for( line = 0 ; line < lines ; ++line ) {
    for( col = 0 ; col < cols ; ++col ) {
      CPPUNIT_ASSERT( ( m1( line , col ) == counter ) );
      ++counter;
    }
  }
  
  m1.reset();
  
  return;
}
