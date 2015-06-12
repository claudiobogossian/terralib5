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
  \file TsExpansibleBandBlocksManager.cpp
 
  \brief A test suit for the Cached Raster class interface.

 */

#include "TsExpansibleBandBlocksManager.h"
#include "../Config.h"

CPPUNIT_TEST_SUITE_REGISTRATION( TsExpansibleBandBlocksManager );

void TsExpansibleBandBlocksManager::ReadWriteTest()
{
  const unsigned int maxNumberRAMBlocks = 2;
  
  std::vector< unsigned int> numbersOfBlocksX;
  numbersOfBlocksX.push_back( 1 );
  numbersOfBlocksX.push_back( 2 );
  numbersOfBlocksX.push_back( 2 );
  
  std::vector< unsigned int> numbersOfBlocksY;
  numbersOfBlocksY.push_back( 1 );
  numbersOfBlocksY.push_back( 3 );
  numbersOfBlocksY.push_back( 1 );
  
  std::vector< unsigned int> blocksSizesBytes;
  blocksSizesBytes.push_back( 6 * sizeof( unsigned int ) );
  blocksSizesBytes.push_back( 2 * sizeof( unsigned char ) );
  blocksSizesBytes.push_back( 3 * sizeof( double ) );
  
  const unsigned long int maxDiskFilesSize = 1000000;  
  
  te::mem::ExpansibleBandBlocksManager manager;
  CPPUNIT_ASSERT( manager.initialize( maxNumberRAMBlocks, numbersOfBlocksX,
    numbersOfBlocksY, blocksSizesBytes, maxDiskFilesSize ) );

  // writing values
  
  ((unsigned int*)manager.getBlockPointer( 0, 0, 0))[ 0 ] = (unsigned int)(0);
  ((unsigned int*)manager.getBlockPointer( 0, 0, 0))[ 1 ] = (unsigned int)(1);
  ((unsigned int*)manager.getBlockPointer( 0, 0, 0))[ 2 ] = (unsigned int)(2);
  ((unsigned int*)manager.getBlockPointer( 0, 0, 0))[ 3 ] = (unsigned int)(3);
  ((unsigned int*)manager.getBlockPointer( 0, 0, 0))[ 4 ] = (unsigned int)(4);
  ((unsigned int*)manager.getBlockPointer( 0, 0, 0))[ 5 ] = (unsigned int)(5);
  
  ((unsigned char*)manager.getBlockPointer( 1, 0, 0))[ 0 ] = (unsigned char)(6);
  ((unsigned char*)manager.getBlockPointer( 1, 0, 1))[ 0 ] = (unsigned char)(7);
  ((unsigned char*)manager.getBlockPointer( 1, 0, 2))[ 0 ] = (unsigned char)(8);
  ((unsigned char*)manager.getBlockPointer( 1, 1, 0))[ 0 ] = (unsigned char)(9);
  ((unsigned char*)manager.getBlockPointer( 1, 1, 1))[ 0 ] = (unsigned char)(10);
  ((unsigned char*)manager.getBlockPointer( 1, 1, 2))[ 0 ] = (unsigned char)(11);
  
  ((double*)manager.getBlockPointer( 2, 0, 0))[ 0 ] = (double)(12);
  ((double*)manager.getBlockPointer( 2, 0, 0))[ 1 ] = (double)(13);
  ((double*)manager.getBlockPointer( 2, 0, 0))[ 2 ] = (double)(14);
  ((double*)manager.getBlockPointer( 2, 1, 0))[ 0 ] = (double)(15);
  ((double*)manager.getBlockPointer( 2, 1, 0))[ 1 ] = (double)(16);
  ((double*)manager.getBlockPointer( 2, 1, 0))[ 2 ] = (double)(17);
  
  // Verifying the values
  
  CPPUNIT_ASSERT( ((unsigned int*)manager.getBlockPointer( 0, 0, 0))[ 0 ] == (unsigned int)(0) );
  CPPUNIT_ASSERT( ((unsigned int*)manager.getBlockPointer( 0, 0, 0))[ 1 ] == (unsigned int)(1) );
  CPPUNIT_ASSERT( ((unsigned int*)manager.getBlockPointer( 0, 0, 0))[ 2 ] == (unsigned int)(2) );
  CPPUNIT_ASSERT( ((unsigned int*)manager.getBlockPointer( 0, 0, 0))[ 3 ] == (unsigned int)(3) );
  CPPUNIT_ASSERT( ((unsigned int*)manager.getBlockPointer( 0, 0, 0))[ 4 ] == (unsigned int)(4) );
  CPPUNIT_ASSERT( ((unsigned int*)manager.getBlockPointer( 0, 0, 0))[ 5 ] == (unsigned int)(5) );
  
  CPPUNIT_ASSERT( ((unsigned char*)manager.getBlockPointer( 1, 0, 0))[ 0 ] == (unsigned char)(6) );
  CPPUNIT_ASSERT( ((unsigned char*)manager.getBlockPointer( 1, 0, 1))[ 0 ] == (unsigned char)(7) );
  CPPUNIT_ASSERT( ((unsigned char*)manager.getBlockPointer( 1, 0, 2))[ 0 ] == (unsigned char)(8) );
  CPPUNIT_ASSERT( ((unsigned char*)manager.getBlockPointer( 1, 1, 0))[ 0 ] == (unsigned char)(9) );
  CPPUNIT_ASSERT( ((unsigned char*)manager.getBlockPointer( 1, 1, 1))[ 0 ] == (unsigned char)(10) );
  CPPUNIT_ASSERT( ((unsigned char*)manager.getBlockPointer( 1, 1, 2))[ 0 ] == (unsigned char)(11) );
  
  CPPUNIT_ASSERT( ((double*)manager.getBlockPointer( 2, 0, 0))[ 0 ] == (double)(12) );
  CPPUNIT_ASSERT( ((double*)manager.getBlockPointer( 2, 0, 0))[ 1 ] == (double)(13) );
  CPPUNIT_ASSERT( ((double*)manager.getBlockPointer( 2, 0, 0))[ 2 ] == (double)(14) );
  CPPUNIT_ASSERT( ((double*)manager.getBlockPointer( 2, 1, 0))[ 0 ] == (double)(15) );
  CPPUNIT_ASSERT( ((double*)manager.getBlockPointer( 2, 1, 0))[ 1 ] == (double)(16) );  
  CPPUNIT_ASSERT( ((double*)manager.getBlockPointer( 2, 1, 0))[ 2 ] == (double)(17) );  
}

void TsExpansibleBandBlocksManager::addBlocksTest()
{
  const unsigned int maxNumberRAMBlocks = 2;
  
  std::vector< unsigned int> numbersOfBlocksX;
  numbersOfBlocksX.push_back( 1 );
  numbersOfBlocksX.push_back( 2 );
  numbersOfBlocksX.push_back( 2 );
  
  std::vector< unsigned int> numbersOfBlocksY;
  numbersOfBlocksY.push_back( 1 );
  numbersOfBlocksY.push_back( 3 );
  numbersOfBlocksY.push_back( 1 );
  
  std::vector< unsigned int> blocksSizesBytes;
  blocksSizesBytes.push_back( 6 * sizeof( unsigned int ) );
  blocksSizesBytes.push_back( 2 * sizeof( unsigned int ) );
  blocksSizesBytes.push_back( 3 * sizeof( unsigned int ) );
  
  const unsigned long int maxDiskFilesSize = 1000000; 
  
  te::mem::ExpansibleBandBlocksManager manager;
  CPPUNIT_ASSERT( manager.initialize( maxNumberRAMBlocks, numbersOfBlocksX,
    numbersOfBlocksY, blocksSizesBytes, maxDiskFilesSize ) );

  FillBlocks< unsigned int >( manager );
  CheckValues< unsigned int >( manager );
  
  std::vector< te::mem::ExpansibleBandBlocksManager::BlockIndex3D > addedBlocksCoords;
  
  manager.addTopBlocks( 1, 0, addedBlocksCoords );
  FillBlocks< unsigned int >( manager );
  CheckValues< unsigned int >( manager );

  manager.addTopBlocks( 2, 1, addedBlocksCoords );
  FillBlocks< unsigned int >( manager );
  CheckValues< unsigned int >( manager );

  manager.addTopBlocks( 3, 2, addedBlocksCoords );
  FillBlocks< unsigned int >( manager );
  CheckValues< unsigned int >( manager );
  
  manager.addBottomBlocks( 1, 0, addedBlocksCoords );
  FillBlocks< unsigned int >( manager );
  CheckValues< unsigned int >( manager );

  manager.addBottomBlocks( 2, 1, addedBlocksCoords );
  FillBlocks< unsigned int >( manager );
  CheckValues< unsigned int >( manager );

  manager.addBottomBlocks( 3, 2, addedBlocksCoords );
  FillBlocks< unsigned int >( manager );
  CheckValues< unsigned int >( manager );  
  
  manager.addLeftBlocks( 1, 0, addedBlocksCoords );
  FillBlocks< unsigned int >( manager );
  CheckValues< unsigned int >( manager );

  manager.addLeftBlocks( 2, 1, addedBlocksCoords );
  FillBlocks< unsigned int >( manager );
  CheckValues< unsigned int >( manager );

  manager.addLeftBlocks( 3, 2, addedBlocksCoords );
  FillBlocks< unsigned int >( manager );
  CheckValues< unsigned int >( manager );  
  
  manager.addRightBlocks( 1, 0, addedBlocksCoords );
  FillBlocks< unsigned int >( manager );
  CheckValues< unsigned int >( manager );

  manager.addRightBlocks( 2, 1, addedBlocksCoords );
  FillBlocks< unsigned int >( manager );
  CheckValues< unsigned int >( manager );

  manager.addRightBlocks( 3, 2, addedBlocksCoords );
  FillBlocks< unsigned int >( manager );
  CheckValues< unsigned int >( manager );   
  
  manager.addTopBands( 1, addedBlocksCoords );
  FillBlocks< unsigned int >( manager );
  CheckValues< unsigned int >( manager );
  
  manager.addTopBands( 2, addedBlocksCoords );
  FillBlocks< unsigned int >( manager );
  CheckValues< unsigned int >( manager );  
  
  manager.addTopBands( 3, addedBlocksCoords );
  FillBlocks< unsigned int >( manager );
  CheckValues< unsigned int >( manager );    
  
  manager.addBottomBands( 1, addedBlocksCoords );
  FillBlocks< unsigned int >( manager );
  CheckValues< unsigned int >( manager );
  
  manager.addBottomBands( 2, addedBlocksCoords );
  FillBlocks< unsigned int >( manager );
  CheckValues< unsigned int >( manager );  
  
  manager.addBottomBands( 3, addedBlocksCoords );
  FillBlocks< unsigned int >( manager );
  CheckValues< unsigned int >( manager );  
}

