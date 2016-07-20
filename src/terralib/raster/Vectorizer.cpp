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
  \file terralib/raster/Vectorizer.cpp

  \brief It implements the vectorizer, based on TerraLib 4 algorithm.
*/

// TerraLib
#include "../common/progress/TaskProgress.h"
#include "../core/translator/Translator.h"
#include "../geometry/Coord2D.h"
#include "../geometry/Envelope.h"
#include "../geometry/Line.h"
#include "../geometry/LinearRing.h"
#include "../geometry/Point.h"
#include "../geometry/Polygon.h"
#include "../geometry/Utils.h"
#include "Band.h"
#include "BandProperty.h"
#include "Config.h"
#include "Grid.h"
#include "Vectorizer.h"
#include "VectorizerPolygonStructure.h"
#include "Exception.h"

// STL
#include <iostream>
#include <memory>

// The 8 cardinals directions used by vectorize method
#define NORTHWEST 0
#define NORTH     1
#define NORTHEAST 2
#define EAST      3
#define SOUTHEAST 4
#define SOUTH     5
#define SOUTHWEST 6
#define WEST      7

#define APPENDPOINTINLINE( newDirection ) \
  currChainCodePoint.first += (long int)m_directions[ newDirection ].x; \
  currChainCodePoint.second += (long int)m_directions[ newDirection ].y; \
  chainCode.push_back( currChainCodePoint );

te::rst::Vectorizer::Vectorizer(Raster* r, std::size_t b, unsigned int mp,
  const bool noDataExclusion )
  : m_rasterPtr(r),
    m_rasterBand(b),
    m_maxPolygons(mp)
{
  assert(b < m_rasterPtr->getNumberOfBands());

// grid parameters
  m_resX = m_rasterPtr->getGrid()->getResolutionX();
  m_resY = m_rasterPtr->getGrid()->getResolutionY();
  m_nColumns = m_rasterPtr->getNumberOfColumns();
  m_nLines = m_rasterPtr->getNumberOfRows();

// no data value parameters
  m_noDataValue = noDataExclusion ? 
    m_rasterPtr->getBand(m_rasterBand)->getProperty()->m_noDataValue
    :
    std::numeric_limits< double >::max();

// setting the directions
  m_directions[NORTHWEST] = te::gm::Coord2D(-1, -1);/* NW - 0 */
  m_directions[NORTH]     = te::gm::Coord2D( 0, -1);/* N  - 1 */
  m_directions[NORTHEAST] = te::gm::Coord2D( 1, -1);/* EN - 2 */
  m_directions[EAST]      = te::gm::Coord2D( 1,  0);/* E  - 3 */
  m_directions[SOUTHEAST] = te::gm::Coord2D( 1,  1);/* SE - 4 */
  m_directions[SOUTH]     = te::gm::Coord2D( 0,  1);/* S  - 5 */
  m_directions[SOUTHWEST] = te::gm::Coord2D(-1,  1);/* WS - 6 */
  m_directions[WEST]      = te::gm::Coord2D(-1,  0);/* W  - 7 */
}

te::rst::Vectorizer::Vectorizer(const te::rst::Vectorizer& rhs)
{
}

te::rst::Vectorizer::~Vectorizer()
{
  clear();
}

te::rst::Vectorizer& te::rst::Vectorizer::operator=(const Vectorizer& rhs)
{
  if(this != &rhs)
  {
  }

  return *this;
}

bool te::rst::Vectorizer::run(std::vector<te::gm::Geometry*>& polygons,
  std::vector< double > * const polygonsValues )
{
  polygons.clear();
  if( polygonsValues ) polygonsValues->clear();
  
  clear();
  
  // Vector of all polygons.
  
  std::vector<VectorizerPolygonStructure> containerPolygons;

// creating a new RTree
  te::sam::rtree::Index<unsigned int, 8, 4> rTreePolygons(*m_rasterPtr->getExtent());

  double currentPixelValue;
  int countObjects = 0;
  std::vector<unsigned int> indexVec;
  te::common::TaskProgress task(TE_TR("Vectorizing"), te::common::TaskProgress::UNDEFINED, m_nLines);

// scanning rotulated image in row order to vectorize the cells
  
  te::gm::Coord2D last_line_ll_point;
  te::gm::Coord2D last_line_ll_point_indexed;
  te::gm::Coord2D last_line_lr_point;
  te::gm::Coord2D last_line_lr_point_indexed;
  std::auto_ptr< te::gm::LinearRing > newRingPtr;
  std::auto_ptr< te::gm::Polygon > newPolPtr;
  const VectorizerPolygonStructure dummy_ps;

  try
  {
    for (unsigned int currentLine = 0; currentLine < m_nLines ; currentLine++)
    {
      task.pulse();
      
      // cleaning the tileindexers that will not be used anymore
      
      last_line_ll_point_indexed.x = 0.0;
      last_line_ll_point_indexed.y = ((double) currentLine) - 1.0;

      last_line_lr_point_indexed.x = ((double) m_nColumns) - 1.0;
      last_line_lr_point_indexed.y = ((double) currentLine) - 1.0;

      last_line_ll_point = m_rasterPtr->getGrid()->gridToGeo(last_line_ll_point_indexed.x, last_line_ll_point_indexed.y);
      last_line_lr_point = m_rasterPtr->getGrid()->gridToGeo(last_line_lr_point_indexed.x, last_line_lr_point_indexed.y);

      te::gm::Envelope last_line_bbox(last_line_ll_point.x, last_line_ll_point.y,
                                      last_line_lr_point.x, last_line_lr_point.y);

      indexVec.clear();
      rTreePolygons.search(last_line_bbox, indexVec);

      for(unsigned int indexVec_index = 0; indexVec_index < indexVec.size(); indexVec_index++)
      {
        te::rst::VectorizerPolygonStructure& curr_pol_struct =
          containerPolygons[indexVec[indexVec_index]];
        const te::gm::Envelope& env = *(curr_pol_struct.m_polygonPtr->getMBR());
        
        double curr_pol_ll_x = env.getLowerLeftX();
        double curr_pol_ll_y = env.getLowerLeftY();

        if(m_rasterPtr->getGrid()->geoToGrid(curr_pol_ll_x, curr_pol_ll_y).y < ((double)currentLine) )
        {
          rTreePolygons.remove( env, indexVec[indexVec_index] );
          curr_pol_struct.m_indexerPtr.reset();
        }
      }

      // iterating through columns      
      
      for (unsigned int currentCol = 0; currentCol < m_nColumns; currentCol++)
      {
        // verifying if the current point is already inside a polygon generated before
        
        te::gm::Coord2D coordWorld = m_rasterPtr->getGrid()->gridToGeo(currentCol, currentLine);
        te::gm::Envelope boxPoint(coordWorld.x, coordWorld.y, coordWorld.x, coordWorld.y);
        te::gm::Point pointWorld(coordWorld.x, coordWorld.y, m_rasterPtr->getSRID());

        bool exist = false;
        indexVec.clear();
        rTreePolygons.search(boxPoint, indexVec);
        m_rasterPtr->getValue(currentCol, currentLine, currentPixelValue, m_rasterBand);

        if (!indexVec.empty())
        {
          unsigned int indexVec_index = 0;

          while(indexVec_index < indexVec.size())
          {
            const VectorizerPolygonStructure& pol_str_ref = 
              containerPolygons[indexVec[indexVec_index]];

            if (currentPixelValue == pol_str_ref.m_value)
            {
              if ( pol_str_ref.m_indexerPtr->within(pointWorld) )
              {
                exist = true;
                break;
              }
            }

            ++indexVec_index;
          }
        }

        if( exist == false )
        {
          // we found a new class
          
          newRingPtr.reset( new te::gm::LinearRing(0, te::gm::LineStringType) );

          if(detectEdge(currentCol, currentLine, *newRingPtr))
          {
//             std::cout << std::endl << "new polygon index " << countObjects << " :" ;
//             for( unsigned int newRingIdx = 0 ; newRingIdx < newRingPtr->size() ;
//               ++newRingIdx )
//             {
//               std::cout << " [" << newRingPtr->getPointN( newRingIdx )->getX()
//                 << " ; " << newRingPtr->getPointN( newRingIdx )->getY()
//                 << "]";
//             }
//             std::cout << std::endl;
                          
            newPolPtr.reset( new te::gm::Polygon(0, te::gm::PolygonType) );
            newPolPtr->setSRID( m_rasterPtr->getSRID() );
            newPolPtr->add( newRingPtr.get() );

            // verifying if the polygon is a hole of other one
            
            unsigned int indexVec_index = 0;
            while(indexVec_index < indexVec.size())
            {
              VectorizerPolygonStructure& pol_struct_ref =
                containerPolygons[indexVec[indexVec_index]];
              
              if( currentPixelValue != pol_struct_ref.m_value )
              {
                if (pol_struct_ref.m_indexerPtr->within(pointWorld))
                {
                  pol_struct_ref.m_polygonPtr->add((te::gm::LinearRing*) (newRingPtr->clone()));
                  pol_struct_ref.m_indexerPtr->addRing(pol_struct_ref.m_polygonPtr->getNumRings() - 1);

                  break;
                }
              }

              ++indexVec_index;
            }
            
            indexVec.clear();

            // inserting the new polygon into tree and container
            
            containerPolygons.push_back(dummy_ps);
            containerPolygons.back().reset(newPolPtr.get(), 
              currentPixelValue, m_rasterPtr->getResolutionY() );

            rTreePolygons.insert(*newPolPtr->getMBR(), containerPolygons.size() - 1);
            
            newRingPtr.release(); // was acquired by newPolPtr
            newPolPtr.release(); // thr polygon was acquired by the container right above.
            
            ++countObjects;
          }
          else
          {
            // no more polygons will be generated
            
            currentCol = m_nColumns;
            currentLine = m_nLines;
          }
          
          if(m_maxPolygons)
          {
            if(countObjects >= ((int) m_maxPolygons))
            {
              // no more polygons will be generated
              
              currentCol = m_nColumns;
              currentLine = m_nLines;
            }
          }
        }
      }
    }
  }
  catch( const te::rst::Exception& e )
  {
    throw te::rst::Exception( "Vectorizer error:" + std::string( e.what() ) );
  }  
  catch(...)
  {
    throw te::rst::Exception( "Vectorizer error" );
  }   

  // generating output data
  
  const std::size_t containerPolygonsSize = containerPolygons.size(); 
  
  for( std::size_t containerPolygonsIdx = 0 ; containerPolygonsIdx <
    containerPolygonsSize ; ++containerPolygonsIdx )
  {
    VectorizerPolygonStructure& polStructure = containerPolygons[ containerPolygonsIdx ];
    
    if( polStructure.m_value != m_noDataValue )
    {
      polygons.push_back( polStructure.m_polygonPtr.release() );
      
      if( polygonsValues )
      {
        polygonsValues->push_back( polStructure.m_value );
      }
      
      polStructure.clear();
    }
  }

  return true;
}

bool te::rst::Vectorizer::detectEdge(long segmentInitialCol, long segmentInitialRow, te::gm::LinearRing& outputLine)
{
  assert( startingEdgeTest(segmentInitialCol, segmentInitialRow) );

  outputLine.makeEmpty();

// current polygon pixel values
  double pol_pixels_value = 0;
  m_rasterPtr->getValue(segmentInitialCol, segmentInitialRow, pol_pixels_value, m_rasterBand);

// generating chain code by following the polygon borders
  
  short currentSegmentInitialLeavingDirection = EAST;
  bool currentSegmentInitialLeavingDirectionWasSet = false;
  short currentDirection = EAST;
  short currentPixelInitialNextTestDirection = EAST;
  short currentPixelNextTestDirection = EAST;
  short currentPixelCorner = NORTHWEST;
  short cornerToDirectionRotAngle = 0;
  short cornerRotationOffset = 0;
  std::pair< long int, long int > currChainCodePoint( 0, 0 );
  std::vector< std::pair< long int, long int > > chainCode;
  chainCode.push_back( currChainCodePoint );
  int currentCol = segmentInitialCol;
  int currentRow = segmentInitialRow;
  int nextCol = segmentInitialCol;
  int nextRow = segmentInitialRow;
  double currentPixelValue = 0;
  bool lookForNextPixel = true;
  bool nextPixelWasFound = false;

  while(lookForNextPixel)
  {
    // finding the next direction
    
    nextPixelWasFound = false;
    currentPixelInitialNextTestDirection = currentPixelNextTestDirection = (currentDirection + 6) % 8;
    
    do
    {
      nextCol = currentCol + (int) m_directions[currentPixelNextTestDirection].x;
      nextRow = currentRow + (int) m_directions[currentPixelNextTestDirection].y;

      if ((nextCol > -1) && (nextRow > -1) &&
         (nextCol < (int) m_nColumns ) && (nextRow < (int) m_nLines))
      {
        m_rasterPtr->getValue(nextCol, nextRow, currentPixelValue, m_rasterBand);
        // curr_pixel_value = m_noDataValue;

        if (currentPixelValue == pol_pixels_value)
        {
          nextPixelWasFound = true;
          currentDirection = currentPixelNextTestDirection;
          break;
        }
      }

      currentPixelNextTestDirection = (currentPixelNextTestDirection + 2) % 8;
    } 
    while (currentPixelInitialNextTestDirection != currentPixelNextTestDirection);
    
    // Setting the initial segment leaving direction
    
    if( !currentSegmentInitialLeavingDirectionWasSet )
    {
      currentSegmentInitialLeavingDirectionWasSet = true;
      currentSegmentInitialLeavingDirection = currentDirection;
    }
    
    // generating the polygon line following the current state
    
    if (!nextPixelWasFound)
    {
      // there is only one point inside the polygon
      // that is the last one
      
      APPENDPOINTINLINE( EAST )
      APPENDPOINTINLINE( SOUTH )
      APPENDPOINTINLINE( WEST )
      APPENDPOINTINLINE( NORTH )
      
      // Finish de process for the current line

      lookForNextPixel = false;
    }
    else if(
             ( currentCol == segmentInitialCol ) 
             && 
             ( currentRow == segmentInitialRow )
             &&
             ( currentDirection == currentSegmentInitialLeavingDirection )
             &&
             ( chainCode.size() > 1 )
           )
    {
      // We are back to the fist pixel again (going to the same direction)
      
      // Final stiching of the inital pixel to close the line 
      
      switch (currentPixelCorner)
      {
        case SOUTHEAST:
        {
          APPENDPOINTINLINE( WEST )
          APPENDPOINTINLINE( NORTH )
          break;
        }
        case SOUTHWEST:
        {
          APPENDPOINTINLINE( NORTH )
          break;
        }
        default:
        {
          throw te::common::Exception(TE_TR("Invalid final corner"));
          break;
        }
      }
      
      // Ending the process for the current edge

      lookForNextPixel = false;
    }
    else
    {
      // Stitching the current pixel border line segments following next direction
      // preparing to a junction with the next pixel

      cornerToDirectionRotAngle = ( currentDirection < currentPixelCorner ) ? 
        ( currentDirection + 8 ) : currentDirection;
      cornerToDirectionRotAngle -= currentPixelCorner;
      
      for( cornerRotationOffset = 2 ; cornerRotationOffset < cornerToDirectionRotAngle ;
        cornerRotationOffset += 2 )
      {
        currentPixelCorner = ( currentPixelCorner + 2 ) % 8;
        
        switch( currentPixelCorner )
        {
          case NORTHWEST :
            APPENDPOINTINLINE( NORTH )
            break;
          case NORTHEAST :
            APPENDPOINTINLINE( EAST )
            break;
          case SOUTHEAST :
            APPENDPOINTINLINE( SOUTH )
            break;
          case SOUTHWEST :
            APPENDPOINTINLINE( WEST )
            break;
          default :
            throw te::common::Exception(TE_TR("Invalid corner"));
            break;          
        }
      }      
      
      // Setting the next pixel corner 
      
      switch (currentDirection)
      {
        case EAST:
        {
          switch (currentPixelCorner)
          {
            case NORTHWEST:
            {
              break;
            }
            case NORTHEAST:
            {
              currentPixelCorner = NORTHWEST;
              break;
            }
            case SOUTHEAST:
            {
              currentPixelCorner = SOUTHWEST;
              break;
            }
            case SOUTHWEST:
            {
              break;
            }
            default:
            {
              throw te::common::Exception(TE_TR("Invalid corner"));
              break;
            }
          }

          break;
        }
        case SOUTH:
        {
          switch (currentPixelCorner)
          {
            case NORTHWEST:
            {
              break;
            }
            case NORTHEAST:
            {
              break;
            }
            case SOUTHEAST:
            {
              currentPixelCorner = NORTHEAST;
              break;
            }
            case SOUTHWEST:
            {
              currentPixelCorner = NORTHWEST;
              break;
            }
            default:
            {
              throw te::common::Exception(TE_TR("Invalid corner"));
              break;
            }
          }

          break;
        }
        case WEST:
        {
          switch (currentPixelCorner)
          {
            case NORTHWEST:
            {
              currentPixelCorner = NORTHEAST;
              break;
            }
            case NORTHEAST:
            {
              break;
            }
            case SOUTHEAST:
            {
              break;
            }
            case SOUTHWEST:
            {
              currentPixelCorner = SOUTHEAST;
              break;
            }
            default:
            {
              throw te::common::Exception(TE_TR("Invalid corner"));
              break;
            }
          }

          break;
        }
        case NORTH:
        {
          switch (currentPixelCorner)
          {
            case NORTHWEST:
            {
              currentPixelCorner = SOUTHWEST;
              break;
            }
            case NORTHEAST:
            {
              currentPixelCorner = SOUTHEAST;
              break;
            }
            case SOUTHEAST:
            {
              break;
            }
            case SOUTHWEST:
            {
              break;
            }
            default:
            {
              throw te::common::Exception(TE_TR("Invalid corner"));
              break;
            }
          }

          break;
        }
        default:
        {
          throw te::common::Exception(TE_TR("Invalid direction"));
          break;
        }
      }
    }

    // updating the current state
    
    currentCol = nextCol;
    currentRow = nextRow;
  }
  
  // closing the chain if necessary
  
  if( !chainCode.empty() )
  {
    if( chainCode.front() != chainCode.back() )
    {
      chainCode.push_back( chainCode.front() );
    }
  }
  
  // changing lines coords to world coords
  
  const unsigned int chainCodeSize = chainCode.size();
  
  outputLine.setNumCoordinates( chainCodeSize );
  
  double row = 0;
  double col = 0;
  double xCoord = 0;
  double yCoord = 0;
  const te::rst::Grid& grid = *m_rasterPtr->getGrid();
  
  for (unsigned int chainCodeIdx = 0; chainCodeIdx < chainCodeSize; chainCodeIdx++)
  {
    col = ( (double)segmentInitialCol ) - 0.5 + 
      ( (double)chainCode[ chainCodeIdx ].first );
    row = ( (double)segmentInitialRow ) - 0.5 + 
      ( (double)chainCode[ chainCodeIdx ].second );
    grid.gridToGeo( col, row, xCoord, yCoord );
    outputLine.setPoint( chainCodeIdx, xCoord, yCoord );
  }
  
  outputLine.setSRID( m_rasterPtr->getSRID() );
  
  // updating the line box
  
  outputLine.computeMBR(false);

  return true;
}

bool te::rst::Vectorizer::startingEdgeTest(const int& x, const int& y)
{
  double current_val = 0;
  double test_val = 0;
  m_rasterPtr->getValue(x, y, current_val, m_rasterBand);
    
  if( x > 0 )
  {
    m_rasterPtr->getValue(x - 1, y, test_val, m_rasterBand);
    if( current_val == test_val ) return false;
  }
  
  if( y > 0 )
  {
    m_rasterPtr->getValue(x, y-1, test_val, m_rasterBand);
    if( current_val == test_val ) return false;
  }  
  
  return true;
}

void te::rst::Vectorizer::clear()
{
}
