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
  \file TsExpansibleBandBlocksManager.h
 
  \brief A test suit for the Expansible Band Blocks Manager Class.
 */

#ifndef __TERRALIB_UNITTEST_MEMORY_EXPANSIBLEBANDBLOCKSMANAGER_INTERNAL_H
#define __TERRALIB_UNITTEST_MEMORY_EXPANSIBLEBANDBLOCKSMANAGER_INTERNAL_H

// cppUnit
#include <cppunit/extensions/HelperMacros.h>

#include <terralib/memory.h>

/*!
  \class TsExpansibleBandBlocksManager

  \brief A test suit for the Expansible Band Blocks Manager Class.
 */
class TsExpansibleBandBlocksManager : public CPPUNIT_NS::TestFixture 
{
  CPPUNIT_TEST_SUITE( TsExpansibleBandBlocksManager );
  
  CPPUNIT_TEST( ReadWriteTest );
  
  CPPUNIT_TEST( addBlocksTest );
  
  CPPUNIT_TEST_SUITE_END();

  protected :
    
    template< typename DataType >
    void FillBlocks( te::mem::ExpansibleBandBlocksManager& manager )
    {
      DataType counter = 0;
      const unsigned int bandsNumber = manager.getNumberOfBands();
      
      for( unsigned int bandIdx = 0 ; bandIdx < bandsNumber ; ++bandIdx )
      {
        const unsigned int numberOfBlocksX = manager.getNumberOfBlocksX( bandIdx );
        const unsigned int numberOfBlocksY = manager.getNumberOfBlocksY( bandIdx );
        const unsigned int blocksSizesBytes = manager.getBlockSizeBytes();
        const unsigned int elementsNumber = blocksSizesBytes / 
          ((unsigned int)sizeof( DataType ));
          
        for( unsigned int blockX = 0 ; blockX < numberOfBlocksX ; ++blockX )
          for( unsigned int blockY = 0 ; blockY < numberOfBlocksY ; ++blockY )
          {
            DataType* dataPtr = (DataType*)manager.getBlockPointer( bandIdx, blockX, blockY );
            assert( dataPtr != 0 );
            
            for( unsigned int elemIdx = 0 ; elemIdx < elementsNumber ; ++elemIdx )
            {
              dataPtr[ elemIdx ] = counter;
              ++counter;
            }
          }
      }
    }
    
    template< typename DataType >
    void CheckValues( te::mem::ExpansibleBandBlocksManager& manager )
    {
      DataType counter = 0;
      const unsigned int bandsNumber = manager.getNumberOfBands();
      
      for( unsigned int bandIdx = 0 ; bandIdx < bandsNumber ; ++bandIdx )
      {
        const unsigned int numberOfBlocksX = manager.getNumberOfBlocksX( bandIdx );
        const unsigned int numberOfBlocksY = manager.getNumberOfBlocksY( bandIdx );
        const unsigned int blocksSizesBytes = manager.getBlockSizeBytes();
        const unsigned int elementsNumber = blocksSizesBytes / 
          ((unsigned int)sizeof( DataType ));
          
        for( unsigned int blockX = 0 ; blockX < numberOfBlocksX ; ++blockX )
          for( unsigned int blockY = 0 ; blockY < numberOfBlocksY ; ++blockY )
          {
            DataType* dataPtr = (DataType*)manager.getBlockPointer( bandIdx, blockX, blockY );
            assert( dataPtr != 0 );
            
            for( unsigned int elemIdx = 0 ; elemIdx < elementsNumber ; ++elemIdx )
            {
              CPPUNIT_ASSERT( dataPtr[ elemIdx ] == counter );
              ++counter;
            }
          }
      }
    }    
    
    void ReadWriteTest();
    
    void addBlocksTest();
};

#endif  // __TERRALIB_UNITTEST_MEMORY_EXPANSIBLEBANDBLOCKSMANAGER_INTERNAL_H

