/*  Copyright (C) 2008-2013 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/geometry/WKBWriter.cpp

  \brief A class that serializes a geometry to the WKB format.
*/

// TerraLib
#include "../common/ByteSwapUtils.h"
#include "../common/Globals.h"
#include "../common/Translator.h"
#include "Exception.h"
#include "GeometryCollection.h"
#include "LinearRing.h"
#include "MultiLineString.h"
#include "MultiPoint.h"
#include "MultiPolygon.h"
#include "PointM.h"
#include "PointZ.h"
#include "PointZM.h"
#include "Polygon.h"
#include "PolyhedralSurface.h"
#include "WKBWriter.h"

// STL
#include <cstring>

namespace te
{
  namespace gm
  {

    /*!
      \brief It encodes the line to WKB.

      This is a special helper function that writes just the number of points in the line and its coordinates.
      No byte order or geometry type is written to the buffer.

      \param l          The line to be serialized to a wkb.
      \param nPts       The number of points in the line to be encoded in the WKB..
      \param wkb        The buffer to serialize the line number of points and coordinates.
      \param byteOrder  The byte order used to encode the wkb.
    */
    inline char* Write2WKB(const LineString& l, unsigned int nPts, char* wkb, te::common::MachineByteOrder byteOrder)
    {
      char* aux = wkb;

      memcpy(wkb, &nPts, 4);

      wkb += 4; // skip nPts

      Coord2D* coords = l.getCoordinates();

      switch(l.getGeomTypeId())
      {
        case LineStringType :
          memcpy(wkb, coords, 16 * nPts);
          wkb += (16 * nPts);
        break;
    
        case LineStringZType :
          for(unsigned int i = 0; i < nPts; ++i)
          {
            memcpy(wkb, &(coords[i]), 16);
            memcpy(wkb + 16, &(l.getZ()[i]), 8);
            wkb += 24;
          }
        break;

        case LineStringMType :
          for(unsigned int i = 0; i < nPts; ++i)
          {
            memcpy(wkb, &(coords[i]), 16);
            memcpy(wkb + 16, &(l.getM()[i]), 8);
            wkb += 24;
          }
        break;

        case LineStringZMType :
          for(unsigned int i = 0; i < nPts; ++i)
          {
            memcpy(wkb, &(coords[i]), 16);
            memcpy(wkb + 16, &(l.getZ()[i]), 8);
            memcpy(wkb + 24, &(l.getM()[i]), 8);
            wkb += 32;
          }
        break;

        default:
          throw Exception(TE_TR("Invalid geometry type to encode to a WKB!"));
      }

      if(te::common::Globals::sm_machineByteOrder != byteOrder)
      {
        te::common::Swap4Bytes(aux);

        aux += 4;

        int dim = 2;

        if(l.getZ())
          ++dim;

        if(l.getM())
          ++dim;

        std::size_t nStep = nPts * dim;

        for(std::size_t i = 0; i < nStep; ++i)
        {
          te::common::Swap8Bytes(aux);
          aux += 8;
        }
      }

      return wkb;
    }
  } // end namespace gm
}   // end namespace te

te::gm::WKBWriter::WKBWriter(char* wkb, te::common::MachineByteOrder byteOrder)
  : m_wkb(wkb),
    m_byteOrder(byteOrder)
{
}

te::gm::WKBWriter::~WKBWriter()
{
}

void te::gm::WKBWriter::write(const Geometry* geom)
{
  geom->accept(*this);
}

void te::gm::WKBWriter::write(const Geometry* geom, char* wkb, te::common::MachineByteOrder byteOrder)
{
  WKBWriter w(wkb, byteOrder);

  geom->accept(w);
}

void te::gm::WKBWriter::reset(char* wkb, te::common::MachineByteOrder byteOrder)
{
  m_wkb = wkb;
  m_byteOrder = byteOrder;
}

void te::gm::WKBWriter::visit(const GeometryCollection& visited)
{
  memcpy(m_wkb, &m_byteOrder, 1);

  unsigned int gType = static_cast<unsigned int>(visited.getGeomTypeId());  
  unsigned int nGeoms = static_cast<unsigned int>(visited.getNumGeometries());  // please, don't swap this value!
  unsigned int nGeomsAux = nGeoms;

  if(te::common::Globals::sm_machineByteOrder != m_byteOrder)
  {
    te::common::SwapBytes(nGeomsAux);
    te::common::SwapBytes(gType);
  }

  memcpy(m_wkb + 1, &gType, 4);
  memcpy(m_wkb + 5, &nGeomsAux, 4);

  m_wkb += 9;

  for(unsigned int i = 0; i < nGeoms; ++i)
    visited.getGeometryN(i)->accept(*this);
}

void te::gm::WKBWriter::visit(const LinearRing& visited) 
{
  m_wkb = Write2WKB(visited, static_cast<unsigned int>(visited.size()), m_wkb, m_byteOrder);
}

void te::gm::WKBWriter::visit(const LineString & visited) 
{
  unsigned int gType = static_cast<unsigned int>(visited.getGeomTypeId());
  unsigned int nPts = static_cast<unsigned int>(visited.size());

  memcpy(m_wkb, &m_byteOrder, 1);
  memcpy(m_wkb + 1, &gType, 4);

  if(te::common::Globals::sm_machineByteOrder != m_byteOrder)
    te::common::Swap4Bytes(m_wkb + 1);

  m_wkb = Write2WKB(visited, nPts, m_wkb + 5, m_byteOrder);
}

void te::gm::WKBWriter::visit(const MultiLineString& visited)
{
  visit((const GeometryCollection&)visited);
}

void te::gm::WKBWriter::visit(const MultiPoint& visited)
{
  visit((const GeometryCollection&)visited);
}

void te::gm::WKBWriter::visit(const MultiPolygon& visited)
{
  visit((const GeometryCollection&)visited);
}

void te::gm::WKBWriter::visit(const Point& visited) 
{
  unsigned int gType = static_cast<unsigned int>(visited.getGeomTypeId());  
  
  memcpy(m_wkb, &m_byteOrder, 1);
  memcpy(m_wkb + 1, &gType, 4);
  memcpy(m_wkb + 5, &(visited.getX()), 8);  
  memcpy(m_wkb + 13, &(visited.getY()), 8); 

  if(te::common::Globals::sm_machineByteOrder != m_byteOrder)
  {
    te::common::Swap4Bytes(m_wkb + 1);
    te::common::Swap8Bytes(m_wkb + 5);
    te::common::Swap8Bytes(m_wkb + 13);
  }

  m_wkb += 21;
}

void te::gm::WKBWriter::visit(const PointM& visited) 
{
  unsigned int gType = static_cast<unsigned int>(visited.getGeomTypeId());  
  
  memcpy(m_wkb, &m_byteOrder, 1);
  memcpy(m_wkb + 1, &gType, 4);
  memcpy(m_wkb + 5, &(visited.getX()), 8);  
  memcpy(m_wkb + 13, &(visited.getY()), 8); 
  memcpy(m_wkb + 21, &(visited.getM()), 8); 

  if(te::common::Globals::sm_machineByteOrder != m_byteOrder)
  {
    te::common::Swap4Bytes(m_wkb + 1);
    te::common::Swap8Bytes(m_wkb + 5);
    te::common::Swap8Bytes(m_wkb + 13);
    te::common::Swap8Bytes(m_wkb + 21);
  }

  m_wkb += 29;
}

void te::gm::WKBWriter::visit(const PointZ& visited) 
{
  unsigned int gType = static_cast<unsigned int>(visited.getGeomTypeId());  
  
  memcpy(m_wkb, &m_byteOrder, 1);
  memcpy(m_wkb + 1, &gType, 4);
  memcpy(m_wkb + 5, &(visited.getX()), 8);  
  memcpy(m_wkb + 13, &(visited.getY()), 8); 
  memcpy(m_wkb + 21, &(visited.getZ()), 8); 

  if(te::common::Globals::sm_machineByteOrder != m_byteOrder)
  {
    te::common::Swap4Bytes(m_wkb + 1);
    te::common::Swap8Bytes(m_wkb + 5);
    te::common::Swap8Bytes(m_wkb + 13);
    te::common::Swap8Bytes(m_wkb + 21);
  }

  m_wkb += 29;
}

void te::gm::WKBWriter::visit(const PointZM& visited) 
{
  unsigned int gType = static_cast<unsigned int>(visited.getGeomTypeId());  
  
  memcpy(m_wkb, &m_byteOrder, 1);
  memcpy(m_wkb + 1, &gType, 4);
  memcpy(m_wkb + 5, &(visited.getX()), 8);  
  memcpy(m_wkb + 13, &(visited.getY()), 8); 
  memcpy(m_wkb + 21, &(visited.getZ()), 8); 
  memcpy(m_wkb + 29, &(visited.getM()), 8); 

  if(te::common::Globals::sm_machineByteOrder != m_byteOrder)
  {
    te::common::Swap4Bytes(m_wkb + 1);
    te::common::Swap8Bytes(m_wkb + 5);
    te::common::Swap8Bytes(m_wkb + 13);
    te::common::Swap8Bytes(m_wkb + 21);
    te::common::Swap8Bytes(m_wkb + 29);
  }

  m_wkb += 37;
}

void te::gm::WKBWriter::visit(const Polygon& visited) 
{
  unsigned int nRings = static_cast<unsigned int>(visited.getNumRings());
  unsigned int gType = static_cast<unsigned int>(visited.getGeomTypeId());

  memcpy(m_wkb, &m_byteOrder, 1);
  memcpy(m_wkb + 1, &gType, 4);
  memcpy(m_wkb + 5, &nRings, 4);

  if(te::common::Globals::sm_machineByteOrder != m_byteOrder)
  {
    te::common::Swap4Bytes(m_wkb + 1);
    te::common::Swap4Bytes(m_wkb + 5);
  }

  m_wkb += 9;

  for(unsigned int i = 0; i < nRings; ++i)
    visited.getRingN(i)->accept(*this);
}

void te::gm::WKBWriter::visit(const PolyhedralSurface& visited) 
{
  unsigned int nPols = static_cast<unsigned int>(visited.getNumPatches());
  unsigned int gType = static_cast<unsigned int>(visited.getGeomTypeId());

  memcpy(m_wkb, &m_byteOrder, 1);
  memcpy(m_wkb + 1, &gType, 4);
  memcpy(m_wkb + 5, &nPols, 4);

  if(te::common::Globals::sm_machineByteOrder != m_byteOrder)
  {
    te::common::Swap4Bytes(m_wkb + 1);
    te::common::Swap4Bytes(m_wkb + 5);
  }

  m_wkb += 9;

  for(unsigned int i = 0; i < nPols; ++i)
    visited.getPatchN(i)->accept(*this);
}

void te::gm::WKBWriter::visit(const TIN& visited)
{
  visit((const PolyhedralSurface&)visited);
}

void te::gm::WKBWriter::visit(const Triangle& visited)
{
  visit((const Polygon&)visited);
}

