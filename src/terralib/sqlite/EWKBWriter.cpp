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
  \file terralib/sqlite/EWKBWriter.cpp

  \brief A class that serializes a geometry to the SpatiaLite EWKB format.
*/

// TerraLib
#include "../common/ByteSwapUtils.h"
#include "../common/Globals.h"
#include "../common/Translator.h"
#include "../dataaccess/Exception.h"
#include "../geometry/Envelope.h"
#include "../geometry/GeometryCollection.h"
#include "../geometry/LinearRing.h"
#include "../geometry/LineString.h"
#include "../geometry/MultiLineString.h"
#include "../geometry/MultiPoint.h"
#include "../geometry/MultiPolygon.h"
#include "../geometry/PointM.h"
#include "../geometry/PointZ.h"
#include "../geometry/PointZM.h"
#include "../geometry/Polygon.h"
#include "EWKBWriter.h"

// STL
#include <cstring>

// SQLite
#include <sqlite3.h>

#ifdef TE_ENABLE_SPATIALITE
// SpatiaLite
#include <spatialite/gaiageo.h>

namespace te
{
  namespace sqlite
  {

    inline unsigned char* Write2WKB(const te::gm::LineString& l, unsigned int nPts, unsigned char* ewkb, te::common::MachineByteOrder byteOrder)
    {
      char* aux = (char*)ewkb;

      memcpy(ewkb, &nPts, 4);

      ewkb += 4;

      te::gm::Coord2D* coords = l.getCoordinates();

      switch(l.getGeomTypeId())
      {
      case te::gm::LineStringType :
          memcpy(ewkb, coords, 16 * nPts);
          ewkb += (16 * nPts);
        break;

        case te::gm::LineStringZType :
          for(unsigned int i = 0; i < nPts; ++i)
          {
            memcpy(ewkb, &(coords[i]), 16);
            memcpy(ewkb + 16, &(l.getZ()[i]), 8);
            ewkb += 24;
          }
        break;

        case te::gm::LineStringMType :
          for(unsigned int i = 0; i < nPts; ++i)
          {
            memcpy(ewkb, &(coords[i]), 16);
            memcpy(ewkb + 16, &(l.getM()[i]), 8);
            ewkb += 24;
          }
        break;

        case te::gm::LineStringZMType :
          for(unsigned int i = 0; i < nPts; ++i)
          {
            memcpy(ewkb, &(coords[i]), 16);
            memcpy(ewkb + 16, &(l.getZ()[i]), 8);
            memcpy(ewkb + 24, &(l.getM()[i]), 8);
            ewkb += 32;
          }
        break;

        default:
          throw te::da::Exception(TR_COMMON("Invalid geometry type to encode to a SpatiaLIte EWKB!"));
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

      return ewkb;
    }

  } // end namespace sqlite
}   // end namespace te

te::sqlite::EWKBWriter::EWKBWriter(unsigned char* ewkb, te::common::MachineByteOrder byteOrder)
  : m_ewkb(ewkb),
    m_byteOrder(byteOrder)
{
}

te::sqlite::EWKBWriter::~EWKBWriter()
{
}

void te::sqlite::EWKBWriter::write(const te::gm::Geometry* geom)
{
  m_ewkb[0] = GAIA_MARK_START;

  m_ewkb[1] = GAIA_LITTLE_ENDIAN;

  int srid = geom->getSRID();

  te::gm::Envelope mbr(*(geom->getMBR()));

  if(te::common::Globals::sm_machineByteOrder != m_byteOrder)
  {
    te::common::SwapBytes(srid);
    te::common::SwapBytes(mbr.m_llx);
    te::common::SwapBytes(mbr.m_lly);
    te::common::SwapBytes(mbr.m_urx);
    te::common::SwapBytes(mbr.m_ury);
  }

  *((int*)(m_ewkb + 2)) = srid;  

  *((double*)(m_ewkb + 6)) = mbr.m_llx;
  *((double*)(m_ewkb + 14)) = mbr.m_lly;
  *((double*)(m_ewkb + 22)) = mbr.m_urx;
  *((double*)(m_ewkb + 30)) = mbr.m_ury;

  m_ewkb[38] = GAIA_MARK_MBR;

  m_ewkb += 39;

  geom->accept(*this);

  m_ewkb[0] = GAIA_MARK_END;
}

void te::sqlite::EWKBWriter::write(const te::gm::Geometry* geom, unsigned char* ewkb, te::common::MachineByteOrder byteOrder)
{
  EWKBWriter w(ewkb, byteOrder);

  w.write(geom);
}

void te::sqlite::EWKBWriter::reset(unsigned char* ewkb, te::common::MachineByteOrder byteOrder)
{
  m_ewkb = ewkb;
  m_byteOrder = byteOrder;
}

void te::sqlite::EWKBWriter::visit(const te::gm::GeometryCollection& visited)
{
  unsigned int gType = static_cast<unsigned int>(visited.getGeomTypeId());
  unsigned int nGeoms = static_cast<unsigned int>(visited.getNumGeometries());
  unsigned int nGeomsAux = nGeoms;

  if(te::common::Globals::sm_machineByteOrder != m_byteOrder)
  {
    te::common::SwapBytes(nGeomsAux);
    te::common::SwapBytes(gType);
  }

  memcpy(m_ewkb, &gType, 4);
  memcpy(m_ewkb + 4, &nGeomsAux, 4);

  m_ewkb += 8;

  for(unsigned int i = 0; i < nGeoms; ++i)
  {
    m_ewkb[0] = GAIA_MARK_ENTITY;

    m_ewkb += 1;

    visited.getGeometryN(i)->accept(*this);
  }
}

void te::sqlite::EWKBWriter::visit(const te::gm::LinearRing& visited) 
{
  m_ewkb = Write2WKB(visited, static_cast<unsigned int>(visited.size()), m_ewkb, m_byteOrder);
}

void te::sqlite::EWKBWriter::visit(const te::gm::LineString & visited) 
{
  unsigned int gType = static_cast<unsigned int>(visited.getGeomTypeId());
  unsigned int nPts = static_cast<unsigned int>(visited.size());

  if(te::common::Globals::sm_machineByteOrder != m_byteOrder)
    te::common::Swap4Bytes(gType);

  memcpy(m_ewkb, &gType, 4);

  m_ewkb = Write2WKB(visited, nPts, m_ewkb + 4, m_byteOrder);
}

void te::sqlite::EWKBWriter::visit(const te::gm:: MultiLineString& visited)
{
  visit((const te::gm::GeometryCollection&)visited);
}

void te::sqlite::EWKBWriter::visit(const te::gm::MultiPoint& visited)
{
  visit((const te::gm::GeometryCollection&)visited);
}

void te::sqlite::EWKBWriter::visit(const te::gm::MultiPolygon& visited)
{
  visit((const te::gm::GeometryCollection&)visited);
}

void te::sqlite::EWKBWriter::visit(const te::gm::Point& visited) 
{
  unsigned int gType = static_cast<unsigned int>(visited.getGeomTypeId());  

  double x = visited.getX();
  double y = visited.getY();
  
  if(te::common::Globals::sm_machineByteOrder != m_byteOrder)
  {
    te::common::SwapBytes(gType);
    te::common::SwapBytes(x);
    te::common::SwapBytes(y);
  }

  memcpy(m_ewkb, &gType, 4);
  memcpy(m_ewkb + 4, &x, 8);
  memcpy(m_ewkb + 12, &y, 8);

  m_ewkb += 20;
}

void te::sqlite::EWKBWriter::visit(const te::gm::PointM& visited) 
{
  unsigned int gType = static_cast<unsigned int>(visited.getGeomTypeId());  
  
  double x = visited.getX();
  double y = visited.getY();
  double m = visited.getM();
  
  if(te::common::Globals::sm_machineByteOrder != m_byteOrder)
  {
    te::common::SwapBytes(gType);
    te::common::SwapBytes(x);
    te::common::SwapBytes(y);
    te::common::SwapBytes(m);
  }

  memcpy(m_ewkb, &gType, 4);
  memcpy(m_ewkb + 4, &x, 8);
  memcpy(m_ewkb + 12, &y, 8);
  memcpy(m_ewkb + 20, &m, 8);

  m_ewkb += 28;
}

void te::sqlite::EWKBWriter::visit(const te::gm::PointZ& visited) 
{
  unsigned int gType = static_cast<unsigned int>(visited.getGeomTypeId());  
  
  double x = visited.getX();
  double y = visited.getY();
  double z = visited.getZ();
  
  if(te::common::Globals::sm_machineByteOrder != m_byteOrder)
  {
    te::common::SwapBytes(gType);
    te::common::SwapBytes(x);
    te::common::SwapBytes(y);
    te::common::SwapBytes(z);
  }

  memcpy(m_ewkb, &gType, 4);  
  memcpy(m_ewkb + 4, &x, 8);  
  memcpy(m_ewkb + 12, &y, 8); 
  memcpy(m_ewkb + 20, &z, 8); 

  m_ewkb += 28;
}

void te::sqlite::EWKBWriter::visit(const te::gm::PointZM& visited) 
{
  unsigned int gType = static_cast<unsigned int>(visited.getGeomTypeId());  
  
  double x = visited.getX();
  double y = visited.getY();
  double z = visited.getZ();
  double m = visited.getM();
  
  if(te::common::Globals::sm_machineByteOrder != m_byteOrder)
  {
    te::common::SwapBytes(gType);
    te::common::SwapBytes(x);
    te::common::SwapBytes(y);
    te::common::SwapBytes(z);
    te::common::SwapBytes(m);
  }

  memcpy(m_ewkb, &gType, 4);
  memcpy(m_ewkb + 4, &x, 8);
  memcpy(m_ewkb + 12, &y, 8);
  memcpy(m_ewkb + 20, &z, 8);
  memcpy(m_ewkb + 28, &m, 8);

  m_ewkb += 36;
}

void te::sqlite::EWKBWriter::visit(const te::gm::Polygon& visited) 
{
  unsigned int nRings = static_cast<unsigned int>(visited.getNumRings());
  unsigned int gType = static_cast<unsigned int>(visited.getGeomTypeId());

  if(te::common::Globals::sm_machineByteOrder != m_byteOrder)
  {
    te::common::SwapBytes(nRings);
    te::common::SwapBytes(gType);
  }

  memcpy(m_ewkb, &gType, 4);
  memcpy(m_ewkb + 4, &nRings, 4);

  m_ewkb += 8;

  for(unsigned int i = 0; i < nRings; ++i)
    visited.getRingN(i)->accept(*this);
}

#endif // TE_ENABLE_SPATIALITE

