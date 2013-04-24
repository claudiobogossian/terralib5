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
  \file TsExpansibleBandBlocksManager.cpp
 
  \brief A test suit for the Cached Raster class interface.

 */

#include "TsExpansibleBandBlocksManager.h"
#include "../Config.h"

#include <terralib/memory/ExpansibleBandBlocksManager.h>

CPPUNIT_TEST_SUITE_REGISTRATION( TsExpansibleBandBlocksManager );

void TsExpansibleBandBlocksManager::ReadWriteTest()
{
  const unsigned int maxNumberRAMBlocks = 2;
  
  std::vector< unsigned int> numbersOfBlocksX;
  numbersOfBlocksX.push_back( 1 );
  numbersOfBlocksX.push_back( 2 );
  numbersOfBlocksX.push_back( 1 );
  
  std::vector< unsigned int> numbersOfBlocksY;
  numbersOfBlocksY.push_back( 1 );
  numbersOfBlocksY.push_back( 2 );
  numbersOfBlocksY.push_back( 1 );
  
  std::vector< unsigned int> blocksSizesBytes;
  blocksSizesBytes.push_back( 4 * sizeof( unsigned int ) );
  blocksSizesBytes.push_back( sizeof( unsigned char ) );
  blocksSizesBytes.push_back( 4 * sizeof( double ) );
  
  const unsigned long int maxDiskFilesSize = 1000000;  
  
  te::mem::ExpansibleBandBlocksManager manager;
  CPPUNIT_ASSERT( manager.initialize( maxNumberRAMBlocks, numbersOfBlocksX,
    numbersOfBlocksY, blocksSizesBytes, maxDiskFilesSize ) );

  // writing values
  
  ((unsigned int*)manager.getBlockPointer( 0, 0, 0))[ 0 ] = (unsigned int)(0);
  ((unsigned int*)manager.getBlockPointer( 0, 0, 0))[ 1 ] = (unsigned int)(1);
  ((unsigned int*)manager.getBlockPointer( 0, 0, 0))[ 2 ] = (unsigned int)(2);
  ((unsigned int*)manager.getBlockPointer( 0, 0, 0))[ 3 ] = (unsigned int)(3);
  
  ((unsigned char*)manager.getBlockPointer( 1, 0, 0))[ 0 ] = (unsigned char)(4);
  ((unsigned char*)manager.getBlockPointer( 1, 0, 1))[ 0 ] = (unsigned char)(5);
  ((unsigned char*)manager.getBlockPointer( 1, 1, 0))[ 0 ] = (unsigned char)(6);
  ((unsigned char*)manager.getBlockPointer( 1, 1, 1))[ 0 ] = (unsigned char)(7);
  
  ((double*)manager.getBlockPointer( 2, 0, 0))[ 0 ] = (double)(8);
  ((double*)manager.getBlockPointer( 2, 0, 0))[ 1 ] = (double)(9);
  ((double*)manager.getBlockPointer( 2, 0, 0))[ 2 ] = (double)(10);
  ((double*)manager.getBlockPointer( 2, 0, 0))[ 3 ] = (double)(11);
  
  // Verifying the values
  
  CPPUNIT_ASSERT( ((unsigned int*)manager.getBlockPointer( 0, 0, 0))[ 0 ] == (unsigned int)(0) );
  CPPUNIT_ASSERT( ((unsigned int*)manager.getBlockPointer( 0, 0, 0))[ 1 ] == (unsigned int)(1) );
  CPPUNIT_ASSERT( ((unsigned int*)manager.getBlockPointer( 0, 0, 0))[ 2 ] == (unsigned int)(2) );
  CPPUNIT_ASSERT( ((unsigned int*)manager.getBlockPointer( 0, 0, 0))[ 3 ] == (unsigned int)(3) );
  
  CPPUNIT_ASSERT( ((unsigned char*)manager.getBlockPointer( 1, 0, 0))[ 0 ] == (unsigned char)(4) );
  CPPUNIT_ASSERT( ((unsigned char*)manager.getBlockPointer( 1, 0, 1))[ 0 ] == (unsigned char)(5) );
  CPPUNIT_ASSERT( ((unsigned char*)manager.getBlockPointer( 1, 1, 0))[ 0 ] == (unsigned char)(6) );
  CPPUNIT_ASSERT( ((unsigned char*)manager.getBlockPointer( 1, 1, 1))[ 0 ] == (unsigned char)(7) );
  
  CPPUNIT_ASSERT( ((double*)manager.getBlockPointer( 2, 0, 0))[ 0 ] == (double)(8) );
  CPPUNIT_ASSERT( ((double*)manager.getBlockPointer( 2, 0, 0))[ 1 ] == (double)(9) );
  CPPUNIT_ASSERT( ((double*)manager.getBlockPointer( 2, 0, 0))[ 2 ] == (double)(10) );
  CPPUNIT_ASSERT( ((double*)manager.getBlockPointer( 2, 0, 0))[ 3 ] == (double)(11) );  
}


