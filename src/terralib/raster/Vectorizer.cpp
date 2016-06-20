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

te::rst::Vectorizer::Vectorizer(Raster* r, std::size_t b, unsigned int mp)
  : m_rasterPtr(r),
    m_rasterBand(b),
    m_maxPolygons(mp),
    m_rTreePolygons()
{
  assert(b < m_rasterPtr->getNumberOfBands());

// grid parameters
  m_resX = m_rasterPtr->getGrid()->getResolutionX();
  m_resY = m_rasterPtr->getGrid()->getResolutionY();
  m_nColumns = m_rasterPtr->getNumberOfColumns();
  m_nLines = m_rasterPtr->getNumberOfRows();

// no data value parameters
  m_noDataValue = m_rasterPtr->getBand(m_rasterBand)->getProperty()->m_noDataValue;

  m_containerPolygons.clear();
  m_rTreePolygons = new te::sam::rtree::Index<unsigned int, 8, 4>();

// setting the directions
  m_directions[NORTHWEST] = te::gm::Coord2D(-1, -1);
  m_directions[NORTH]     = te::gm::Coord2D( 0, -1);
  m_directions[NORTHEAST] = te::gm::Coord2D( 1, -1);
  m_directions[EAST]      = te::gm::Coord2D( 1,  0);
  m_directions[SOUTHEAST] = te::gm::Coord2D( 1,  1);
  m_directions[SOUTH]     = te::gm::Coord2D( 0,  1);
  m_directions[SOUTHWEST] = te::gm::Coord2D(-1,  1);
  m_directions[WEST]      = te::gm::Coord2D(-1,  0);
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

/*
bool te::rst::Vectorizer::CheckParameters(
  const TePDIParameters& parameters ) const {

//Checking for general required parameters

  //parameter input_image
  TePDITypes::TePDIRasterPtrType inRaster;
  if( ! parameters.GetParameter( "rotulated_image", inRaster ) ) {

    TEAGN_LOGERR( "Missing parameter: rotulated_image" );
    return false;
  }
  if( ! inRaster.isActive() ) {

    TEAGN_LOGERR( "Invalid parameter: rotulated_image inactive" );
    return false;
  }
  if( inRaster->params().status_ == TeRasterParams::TeNotReady ) {

    TEAGN_LOGERR( "Invalid parameter: rotulated_image not ready" );
    return false;
  }

// Checking output_polsets

  TEAGN_TRUE_OR_RETURN(
    parameters.CheckParameter< TePDITypes::TePDIPolSetMapPtrType >(
    "output_polsets" ), "Missing parameter: output_polsets" );

  TePDITypes::TePDIPolSetMapPtrType output_polsets;
  parameters.GetParameter( "output_polsets", output_polsets );

  TEAGN_TRUE_OR_RETURN( output_polsets.isActive(),
    "Invalid parameter output_polsets : Inactive polygon set pointer" );

// Checking channel

  unsigned int channel = 0;

  TEAGN_TRUE_OR_RETURN(
    parameters.GetParameter( "channel", channel ),
    "Missing parameter: channel" );

  TEAGN_TRUE_OR_RETURN( ( channel < (unsigned int)inRaster->nBands() ),
    "Invalid parameter: channel" );

// checking rotulated_image data type

  for( int band = 0 ; band < inRaster->params().nBands() ; ++band ) {
    TEAGN_TRUE_OR_RETURN(
      ( inRaster->params().dataType_[ band ] != TeFLOAT ),
      "Invalid rotulated_image floating point raster data type" );
    TEAGN_TRUE_OR_RETURN(
      ( inRaster->params().dataType_[ band ] != TeDOUBLE ),
      "Invalid rotulated_image floating point raster data type" );
  }

  return true;
}
*/

bool te::rst::Vectorizer::run(std::vector<te::gm::Geometry*>& polygons,
  std::vector< double > * const polygonsValues )
{
  polygons.clear();
  if( polygonsValues ) polygonsValues->clear();
  
  clear();

// creating a new RTree
  m_rTreePolygons = new te::sam::rtree::Index<unsigned int, 8, 4> (*m_rasterPtr->getExtent());

  double val;
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
      m_rTreePolygons->search(last_line_bbox, indexVec);

      for(unsigned int indexVec_index = 0; indexVec_index < indexVec.size(); indexVec_index++)
      {
        te::rst::VectorizerPolygonStructure& curr_pol_struct =
          m_containerPolygons[indexVec[indexVec_index]];
        double curr_pol_ll_x = curr_pol_struct.m_polygonPtr->getMBR()->getLowerLeftX();
        double curr_pol_ll_y = curr_pol_struct.m_polygonPtr->getMBR()->getLowerLeftY();

        if(m_rasterPtr->getGrid()->geoToGrid(curr_pol_ll_x, curr_pol_ll_y).y < currentLine)
        {
          curr_pol_struct.m_indexerPtr.reset();
        }
      }

      // iterating through columns      
      
      for (unsigned int currentCol = 0; currentCol < m_nColumns; currentCol++)
      {
        // verifying if the current point is already inside a polygon generated before
        
        te::gm::Coord2D coordWorld = m_rasterPtr->getGrid()->gridToGeo(currentCol, currentLine);
        //te::gm::Coord2D coordWorld = m_raster->index2Coord(coordMatrix);
        te::gm::Envelope boxPoint(coordWorld.x, coordWorld.y, coordWorld.x, coordWorld.y);
        te::gm::Point pointWorld(coordWorld.x, coordWorld.y, m_rasterPtr->getSRID());

        bool exist = false;
        indexVec.clear();
        m_rTreePolygons->search(boxPoint, indexVec);
        m_rasterPtr->getValue(currentCol, currentLine, val, m_rasterBand);

        if (!indexVec.empty())
        {
          unsigned int indexVec_index = 0;

          while(indexVec_index < indexVec.size())
          {
            assert(indexVec[indexVec_index] < m_containerPolygons.size()); // Invalid cadidates vector index

            const VectorizerPolygonStructure& pol_str_ref = 
              m_containerPolygons[indexVec[indexVec_index]];

            if (val == pol_str_ref.m_value)
            {
              assert( pol_str_ref.m_indexerPtr.get() );
              if (pol_str_ref.m_indexerPtr->within_or_touches(pointWorld))
              //if( TePDIUtils::TeRelation( coordWorld, *( pol_str_ref.m_indexer ) ) == TeINSIDE )
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
            newPolPtr.reset( new te::gm::Polygon(0, te::gm::PolygonType) );
            newPolPtr->setSRID( m_rasterPtr->getSRID() );
            newPolPtr->add( newRingPtr.release() );

            // verifying if the polygon is a hole of other one
            
            unsigned int indexVec_index = 0;
            while(indexVec_index < indexVec.size())
            {
              VectorizerPolygonStructure& pol_struct_ref =
                m_containerPolygons[indexVec[indexVec_index]];
              assert( pol_struct_ref.m_indexerPtr.get() );
              
              if (pol_struct_ref.m_indexerPtr->within_or_touches(pointWorld))
              {
                pol_struct_ref.m_polygonPtr->add((te::gm::LinearRing*) (newPolPtr->getRingN(0)->clone()));
                pol_struct_ref.m_indexerPtr->addRing(pol_struct_ref.m_polygonPtr->getNumRings() - 1); // .getNumRings()

                break;
              }

              ++indexVec_index;
            }
            
            indexVec.clear();

            // inserting the new polygon into tree and container
            
            assert(newPolPtr->getMBR()->getWidth() > 0 && newPolPtr->getMBR()->getHeight() > 0);

            if (val != m_noDataValue)
            {
              ++countObjects;
            }

            VectorizerPolygonStructure dummy_ps;
            m_containerPolygons.push_back(dummy_ps);
            m_containerPolygons[m_containerPolygons.size() - 1].reset(newPolPtr.get(), 
              (unsigned int)val,
              std::max(
                  (
                    4.0 * newPolPtr->getMBR()->getHeight()
                    /
                    (
                      newPolPtr->getMBR()->getWidth()
                      *
                      ( (double) newPolPtr->operator[](0)->getNPoints() )
                    )
                  )
                  ,
                  m_rasterPtr->getResolutionY()
                )
              );
            

            m_rTreePolygons->insert(*newPolPtr->getMBR(), m_containerPolygons.size() - 1);
            
            newPolPtr.release(); // thr polygon was acquired by the container right above.
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
  catch(...)
  {
    throw Exception("Memory error - error generating polygons map");
  }  

  // cleaning the tree
  
  delete m_rTreePolygons;
  m_rTreePolygons = 0;

  // generating output data
  
  for( unsigned int containerPolygonsSize = 0 ; containerPolygonsSize <
    m_containerPolygons.size() ; ++containerPolygonsSize )
  {
    VectorizerPolygonStructure& polStructure = m_containerPolygons[ containerPolygonsSize ];
    
    if( ((double)polStructure.m_value) != m_noDataValue )
    {
      polygons.push_back( polStructure.m_polygonPtr.release() );
      if( polygonsValues ) polygonsValues->push_back( polStructure.m_value );
      polStructure.clear();
    }
  }

  return true;
}

bool te::rst::Vectorizer::detectEdge(long i, long j, te::gm::LinearRing& line)
{
  assert(startingEdgeTest(i, j)); // "Starting edge detection error at x = i line = j

  line.makeEmpty();

// current polygon pixel values
  double pol_pixels_value = 0;
  m_rasterPtr->getValue(i, j, pol_pixels_value, m_rasterBand);

// generating chain code by following the polygon borders
  short curr_dir = EAST;
  short new_dir = EAST;
  short new_test_start_dir = EAST;
  short curr_pixel_corner = NORTHWEST;

  short pinit_leaving_dir = WEST;
  bool pinit_leaving_dir_set = false;

  te::gm::Coord2D curr_chain_p = te::gm::Coord2D(0, 0);

  int curr_x_index = i;
  int curr_y_index = j;
  int next_x_index = i;
  int next_y_index = j;

  double curr_pixel_value = 0;

  bool look_for_next_pixel = true;
  bool next_pixel_found = false;
  bool must_add_curr_chain_p = false;

  line.setNumCoordinates(line.getNPoints() + 1);
  line.setPoint(line.getNPoints() - 1, curr_chain_p.x, curr_chain_p.y);

  while(look_for_next_pixel)
  {
// finding the next direction
    new_test_start_dir = (curr_dir + 6) % 8;
    new_dir = new_test_start_dir;
    next_pixel_found = false;
    do
    {
      next_x_index = curr_x_index + (int) m_directions[new_dir].x;
      next_y_index = curr_y_index + (int) m_directions[new_dir].y;

      if ((next_x_index > -1) && (next_y_index > -1) &&
         (next_x_index < (long) m_nColumns ) && (next_y_index < (long) m_nLines))
      {
        m_rasterPtr->getValue(next_x_index, next_y_index, curr_pixel_value, m_rasterBand);
        // curr_pixel_value = m_noDataValue;

        if (curr_pixel_value == pol_pixels_value)
        {
          next_pixel_found = true;
          break;
        }
      }

      new_dir = (new_dir + 2) % 8;
    } while (new_dir != new_test_start_dir);

// generating the polygon line following the current state
    if (!next_pixel_found)
    {
// there is only one point inside the polygon
// that is the last one
      curr_chain_p.x += m_directions[EAST].x;
      curr_chain_p.y += m_directions[EAST].y;
      line.setNumCoordinates(line.getNPoints() + 1);
      line.setPoint(line.getNPoints() - 1, curr_chain_p.x, curr_chain_p.y);

      curr_chain_p.x += m_directions[SOUTH].x;
      curr_chain_p.y += m_directions[SOUTH].y;
      line.setNumCoordinates(line.getNPoints() + 1);
      line.setPoint(line.getNPoints() - 1, curr_chain_p.x, curr_chain_p.y);

      curr_chain_p.x += m_directions[WEST].x;
      curr_chain_p.y += m_directions[WEST].y;
      line.setNumCoordinates(line.getNPoints() + 1);
      line.setPoint(line.getNPoints() - 1, curr_chain_p.x, curr_chain_p.y);

      curr_chain_p.x += m_directions[NORTH].x;
      curr_chain_p.y += m_directions[NORTH].y;
      line.setNumCoordinates(line.getNPoints() + 1);
      line.setPoint(line.getNPoints() - 1, curr_chain_p.x, curr_chain_p.y);

      look_for_next_pixel = false;
    }
    else if ((curr_x_index == i) && (curr_y_index == j) &&
             (pinit_leaving_dir == new_dir))
    {
// We are back to the fist pixel again (going to the same direction)
      switch (curr_pixel_corner)
      {
        case NORTHEAST:
        {
          curr_chain_p.x += m_directions[SOUTH].x;
          curr_chain_p.y += m_directions[SOUTH].y;
          line.setNumCoordinates(line.getNPoints() + 1);
          line.setPoint(line.getNPoints() - 1, curr_chain_p.x, curr_chain_p.y);

          curr_chain_p.x += m_directions[WEST].x;
          curr_chain_p.y += m_directions[WEST].y;
          line.setNumCoordinates(line.getNPoints() + 1);
          line.setPoint(line.getNPoints() - 1, curr_chain_p.x, curr_chain_p.y);

          curr_chain_p.x += m_directions[NORTH].x;
          curr_chain_p.y += m_directions[NORTH].y;
          line.setNumCoordinates(line.getNPoints() + 1);
          line.setPoint(line.getNPoints() - 1, curr_chain_p.x, curr_chain_p.y);

          break;
        }
        case SOUTHEAST:
        {
          curr_chain_p.x += m_directions[WEST].x;
          curr_chain_p.y += m_directions[WEST].y;
          line.setNumCoordinates(line.getNPoints() + 1);
          line.setPoint(line.getNPoints() - 1, curr_chain_p.x, curr_chain_p.y);

          curr_chain_p.x += m_directions[NORTH].x;
          curr_chain_p.y += m_directions[NORTH].y;
          line.setNumCoordinates(line.getNPoints() + 1);
          line.setPoint(line.getNPoints() - 1, curr_chain_p.x, curr_chain_p.y);

          break;
        }
        case SOUTHWEST:
        {
          curr_chain_p.x += m_directions[NORTH].x;
          curr_chain_p.y += m_directions[NORTH].y;
          line.setNumCoordinates(line.getNPoints() + 1);
          line.setPoint(line.getNPoints() - 1, curr_chain_p.x, curr_chain_p.y);

          break;
        }
        default:
        {
          std::cout << "Error message: Invalid pixel corner " << std::endl; // << curr_pixel_corner + curr_dir=" + curr_dir;
          break;
        }
      }

      look_for_next_pixel = false;
    }
    else
    {
// updating current chain point and current pixel corner
      if (curr_dir == new_dir)
      {
        curr_chain_p.x += m_directions[curr_dir].x;
        curr_chain_p.y += m_directions[curr_dir].y;

        must_add_curr_chain_p = true;
      }
      else
      {
        if (must_add_curr_chain_p)
        {
          line.setNumCoordinates(line.getNPoints() + 1);
          line.setPoint(line.getNPoints() - 1, curr_chain_p.x, curr_chain_p.y);

          must_add_curr_chain_p = false;
        }

        switch (new_dir)
        {
          case EAST:
          {
            switch (curr_pixel_corner)
            {
              case NORTHWEST:
              {
                curr_chain_p.x += m_directions[EAST].x;
                curr_chain_p.y += m_directions[EAST].y;
                line.setNumCoordinates(line.getNPoints() + 1);
                line.setPoint(line.getNPoints() - 1, curr_chain_p.x, curr_chain_p.y);

                break;
              }
              case NORTHEAST:
              {
                curr_pixel_corner = NORTHWEST;
                break;
              }
              case SOUTHEAST:
              {
                curr_chain_p.x += m_directions[WEST].x;
                curr_chain_p.y += m_directions[WEST].y;
                line.setNumCoordinates(line.getNPoints() + 1);
                line.setPoint(line.getNPoints() - 1, curr_chain_p.x, curr_chain_p.y);

                curr_chain_p.x += m_directions[NORTH].x;
                curr_chain_p.y += m_directions[NORTH].y;
                line.setNumCoordinates(line.getNPoints() + 1);
                line.setPoint(line.getNPoints() - 1, curr_chain_p.x, curr_chain_p.y);

                curr_chain_p.x += m_directions[EAST].x;
                curr_chain_p.y += m_directions[EAST].y;
                line.setNumCoordinates(line.getNPoints() + 1);
                line.setPoint(line.getNPoints() - 1, curr_chain_p.x, curr_chain_p.y);

                curr_pixel_corner = NORTHWEST;
                break;
              }
              case SOUTHWEST:
              {
                curr_chain_p.x += m_directions[NORTH].x;
                curr_chain_p.y += m_directions[NORTH].y;
                line.setNumCoordinates(line.getNPoints() + 1);
                line.setPoint(line.getNPoints() - 1, curr_chain_p.x, curr_chain_p.y);

                curr_chain_p.x += m_directions[EAST].x;
                curr_chain_p.y += m_directions[EAST].y;
                line.setNumCoordinates(line.getNPoints() + 1);
                line.setPoint(line.getNPoints() - 1, curr_chain_p.x, curr_chain_p.y);

                curr_pixel_corner = NORTHWEST;
                break;
              }
              default:
              {
                throw("Invalid pixel corner");
                break;
              }
            }

            break;
          }
          case SOUTH:
          {
            switch (curr_pixel_corner)
            {
              case NORTHWEST:
              {
                curr_chain_p.x += m_directions[EAST].x;
                curr_chain_p.y += m_directions[EAST].y;
                line.setNumCoordinates(line.getNPoints() + 1);
                line.setPoint(line.getNPoints() - 1, curr_chain_p.x, curr_chain_p.y);

                curr_chain_p.x += m_directions[SOUTH].x;
                curr_chain_p.y += m_directions[SOUTH].y;
                line.setNumCoordinates(line.getNPoints() + 1);
                line.setPoint(line.getNPoints() - 1, curr_chain_p.x, curr_chain_p.y);

                curr_pixel_corner = NORTHEAST;
                break;
              }
              case NORTHEAST:
              {
                curr_chain_p.x += m_directions[SOUTH].x;
                curr_chain_p.y += m_directions[SOUTH].y;
                line.setNumCoordinates(line.getNPoints() + 1);
                line.setPoint(line.getNPoints() - 1, curr_chain_p.x, curr_chain_p.y);

                break;
              }
              case SOUTHEAST:
              {
                curr_pixel_corner = NORTHEAST;
                break;
              }
              case SOUTHWEST:
              {
                curr_chain_p.x += m_directions[NORTH].x;
                curr_chain_p.y += m_directions[NORTH].y;
                line.setNumCoordinates(line.getNPoints() + 1);
                line.setPoint(line.getNPoints() - 1, curr_chain_p.x, curr_chain_p.y);

                curr_chain_p.x += m_directions[EAST].x;
                curr_chain_p.y += m_directions[EAST].y;
                line.setNumCoordinates(line.getNPoints() + 1);
                line.setPoint(line.getNPoints() - 1, curr_chain_p.x, curr_chain_p.y);

                curr_chain_p.x += m_directions[SOUTH].x;
                curr_chain_p.y += m_directions[SOUTH].y;
                line.setNumCoordinates(line.getNPoints() + 1);
                line.setPoint(line.getNPoints() - 1, curr_chain_p.x, curr_chain_p.y);

                curr_pixel_corner = NORTHEAST;
                break;
              }
              default:
              {
                throw("Invalid pixel corner");
                break;
              }
            }

            break;
          }
          case WEST:
          {
            switch (curr_pixel_corner)
            {
              case NORTHWEST:
              {
                curr_chain_p.x += m_directions[EAST].x;
                curr_chain_p.y += m_directions[EAST].y;
                line.setNumCoordinates(line.getNPoints() + 1);
                line.setPoint(line.getNPoints() - 1, curr_chain_p.x, curr_chain_p.y);

                curr_chain_p.x += m_directions[SOUTH].x;
                curr_chain_p.y += m_directions[SOUTH].y;
                line.setNumCoordinates(line.getNPoints() + 1);
                line.setPoint(line.getNPoints() - 1, curr_chain_p.x, curr_chain_p.y);

                curr_chain_p.x += m_directions[WEST].x;
                curr_chain_p.y += m_directions[WEST].y;
                line.setNumCoordinates(line.getNPoints() + 1);
                line.setPoint(line.getNPoints() - 1, curr_chain_p.x, curr_chain_p.y);

                curr_pixel_corner = SOUTHEAST;
                break;
              }
              case NORTHEAST:
              {
                curr_chain_p.x += m_directions[SOUTH].x;
                curr_chain_p.y += m_directions[SOUTH].y;
                line.setNumCoordinates(line.getNPoints() + 1);
                line.setPoint(line.getNPoints() - 1, curr_chain_p.x, curr_chain_p.y);

                curr_chain_p.x += m_directions[WEST].x;
                curr_chain_p.y += m_directions[WEST].y;
                line.setNumCoordinates(line.getNPoints() + 1);
                line.setPoint(line.getNPoints() - 1, curr_chain_p.x, curr_chain_p.y);

                curr_pixel_corner = SOUTHEAST;
                break;
              }
              case SOUTHEAST:
              {
                curr_chain_p.x += m_directions[WEST].x;
                curr_chain_p.y += m_directions[WEST].y;
                line.setNumCoordinates(line.getNPoints() + 1);
                line.setPoint(line.getNPoints() - 1, curr_chain_p.x, curr_chain_p.y);

                break;
              }
              case SOUTHWEST:
              {
                curr_pixel_corner = SOUTHEAST;
                break;
              }
              default:
              {
                throw("Invalid pixel corner");
                break;
              }
            }

            break;
          }
          case NORTH:
          {
            switch (curr_pixel_corner)
            {
              case NORTHWEST:
              {
                curr_pixel_corner = SOUTHWEST;
                break;
              }
              case NORTHEAST:
              {
                curr_chain_p.x += m_directions[SOUTH].x;
                curr_chain_p.y += m_directions[SOUTH].y;
                line.setNumCoordinates(line.getNPoints() + 1);
                line.setPoint(line.getNPoints() - 1, curr_chain_p.x, curr_chain_p.y);

                curr_chain_p.x += m_directions[WEST].x;
                curr_chain_p.y += m_directions[WEST].y;
                line.setNumCoordinates(line.getNPoints() + 1);
                line.setPoint(line.getNPoints() - 1, curr_chain_p.x, curr_chain_p.y);

                curr_chain_p.x += m_directions[NORTH].x;
                curr_chain_p.y += m_directions[NORTH].y;
                line.setNumCoordinates(line.getNPoints() + 1);
                line.setPoint(line.getNPoints() - 1, curr_chain_p.x, curr_chain_p.y);

                curr_pixel_corner = SOUTHWEST;
                break;
              }
              case SOUTHEAST:
              {
                curr_chain_p.x += m_directions[WEST].x;
                curr_chain_p.y += m_directions[WEST].y;
                line.setNumCoordinates(line.getNPoints() + 1);
                line.setPoint(line.getNPoints() - 1, curr_chain_p.x, curr_chain_p.y);

                curr_chain_p.x += m_directions[NORTH].x;
                curr_chain_p.y += m_directions[NORTH].y;
                line.setNumCoordinates(line.getNPoints() + 1);
                line.setPoint(line.getNPoints() - 1, curr_chain_p.x, curr_chain_p.y);

                curr_pixel_corner = SOUTHWEST;
                break;
              }
              case SOUTHWEST:
              {
                curr_chain_p.x += m_directions[NORTH].x;
                curr_chain_p.y += m_directions[NORTH].y;
                line.setNumCoordinates(line.getNPoints() + 1);
                line.setPoint(line.getNPoints() - 1, curr_chain_p.x, curr_chain_p.y);

                break;
              }
              default:
              {
                throw("Invalid pixel corner");
                break;
              }
            }

            break;
          }
          default:
          {
            throw("Invalid new_dir");
            break;
          }
        }
      }

// updating the leaving direction from the initial point
      if ((!pinit_leaving_dir_set) && (curr_x_index == i) &&
          (curr_y_index == j))
      {
        pinit_leaving_dir_set = true;
        pinit_leaving_dir = new_dir;
      }

// updating the current state
      curr_x_index = next_x_index;
      curr_y_index = next_y_index;

      curr_dir = new_dir;
    }
  }
// changing lines coords to world coords
  const te::gm::Coord2D chain_init_p = m_rasterPtr->getGrid()->gridToGeo(((double) i) - 0.5, ((double) j) - 0.5);
  double min_x = std::numeric_limits<double>::max();
  double min_y = std::numeric_limits<double>::max();
  double max_x = (-1.0) * std::numeric_limits<double>::max();
  double max_y = (-1.0) * std::numeric_limits<double>::max();
  double tmp_x;
  double tmp_y;

  for (unsigned int i = 0; i < line.getNPoints(); i++)
  {
    tmp_x = chain_init_p.x + (line.getX(i) * m_resX);
    tmp_y = chain_init_p.y - (line.getY(i) * m_resY);

    if (tmp_x < min_x ) min_x = tmp_x;
    if (tmp_y < min_y ) min_y = tmp_y;

    if (tmp_x > max_x ) max_x = tmp_x;
    if (tmp_y > max_y ) max_y = tmp_y;

    line.setX(i, tmp_x);
    line.setY(i, tmp_y);
  }
  
  line.setSRID( m_rasterPtr->getSRID() );
  
// updating the line box
  // te::gm::Envelope newbox(min_x, min_y, max_x, max_y);
  // line.setBox( newbox );
  line.computeMBR(false);

// closing the line, if necessary
  if (line.getNPoints() > 1)
  {
    if ((line.getStartPoint()->getX() != line.getEndPoint()->getX()) ||
        (line.getStartPoint()->getY() != line.getEndPoint()->getY()))
    {
      line.setNumCoordinates(line.getNPoints() + 1);
      line.setPoint(line.getNPoints() - 1, line.getX(0), line.getY(0));
    }
  }

  return true;
}

bool te::rst::Vectorizer::startingEdgeTest(const int& x, const int& y)
{
  int nlines = m_rasterPtr->getNumberOfRows();
  int ncols = m_rasterPtr->getNumberOfColumns();

  assert(y < nlines);
  assert(x < ncols);

  if ((x == 0) || (y == 0) ||
      (x == (ncols - 1)) || (y == (nlines - 1)))
    return true;
  else
  {
    double test_val = 0;
    double current_val = 0;

    m_rasterPtr->getValue(x, y, current_val, m_rasterBand);
    m_rasterPtr->getValue(x - 1, y, test_val, m_rasterBand);

    // assert(current_val != test_val); // "The given point isn't a starting edge [x, y]

    return true;
  }
 
}

void te::rst::Vectorizer::clear()
{
  if (m_rTreePolygons)
  {
    delete m_rTreePolygons;
    m_rTreePolygons = 0;
  }

  m_containerPolygons.clear();
}
