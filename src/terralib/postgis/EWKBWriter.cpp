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
  \file terralib/postgis/EWKBWriter.cpp
   
  \brief An utility class for writing a PostGIS EWKB.
*/

// TerraLib
#include "../common/ByteSwapUtils.h"
#include "../common/Globals.h"
#include "../geometry.h"
#include "../srs/Config.h"
#include "EWKBWriter.h"
#include "Utils.h"

// STL
#include <cassert>
#include <cstring>
#include <typeinfo>

inline char* Write2EWKB(char* ewkb, const te::gm::LineString* l)
{
  unsigned int nPts = static_cast<unsigned int>(l->getNPoints());
  te::gm::Coord2D* coords = l->getCoordinates();
  double* zA = l->getZ();
  double* mA = l->getM();

  memcpy(ewkb, &nPts, 4);
  ewkb += 4;

  switch(l->getGeomTypeId())
  {
    case te::gm::LineStringType :
      memcpy(ewkb, coords, 16 * nPts);
      ewkb += (nPts * 16);
      break;
    
    case te::gm::LineStringZType :
      for(size_t i = 0; i < nPts; ++i)
      {
        memcpy(ewkb, &(coords[i]), 16);
        memcpy(ewkb + 16, &(zA[i]), 8);
        ewkb += 24;
      }
      break;

    case te::gm::LineStringMType :
      for(size_t i = 0; i < nPts; ++i)
      {
        memcpy(ewkb, &(coords[i]), 16);
        memcpy(ewkb + 16, &(mA[i]), 8);
        ewkb += 24;
      }
      break;

    case te::gm::LineStringZMType :
      for(size_t i = 0; i < nPts; ++i)
      {
        memcpy(ewkb, &(coords[i]), 16);
        memcpy(ewkb + 16, &(zA[i]), 8);
        memcpy(ewkb + 24, &(mA[i]), 8);
        ewkb += 32;
      }
      break;

    default:
      assert(false);
      break;
  }

  return ewkb;
}

te::pgis::EWKBWriter::EWKBWriter(char* ewkb, te::common::MachineByteOrder byteOrder)
  : m_ewkb(ewkb),
    m_byteOrder(byteOrder),
    m_outputSRID(true)
{
// ewkb must be at least = g->getWkbSize() + 4
}

te::pgis::EWKBWriter::~EWKBWriter()
{
}

void te::pgis::EWKBWriter::write(const te::gm::Geometry* geom)
{
  geom->accept(*this);
}

void te::pgis::EWKBWriter::write(const te::gm::Geometry* geom, char* wkb, te::common::MachineByteOrder byteOrder)
{
  EWKBWriter w(wkb, byteOrder);

  geom->accept(w);
}

void te::pgis::EWKBWriter::reset(char* ewkb, te::common::MachineByteOrder byteOrder)
{
  m_ewkb = ewkb;
  m_byteOrder = byteOrder;
  m_outputSRID = true;
}

void te::pgis::EWKBWriter::visit(const te::gm::GeometryCollection& visited)
{
  memcpy(m_ewkb, &te::common::Globals::sm_machineByteOrder, 1);

  unsigned int gType = static_cast<unsigned int>(visited.getGeomTypeId());
  unsigned int nGeoms = static_cast<unsigned int>(visited.getNumGeometries());
  int srid = visited.getSRID();

  if(srid <= TE_UNKNOWN_SRS)
    srid = PGIS_UNKNOWN_SRS;

  Convert2PostGISWKBType(gType);

  if(m_outputSRID)
  {
    gType |= TE_EWKB_SRID_FLAG;
    
    memcpy(m_ewkb + 1, &gType, 4);
    memcpy(m_ewkb + 5, &srid, 4);
    memcpy(m_ewkb + 9, &nGeoms, 4);
    
    m_ewkb += 13;

    m_outputSRID = false;
  }
  else
  {
    memcpy(m_ewkb + 1, &gType, 4);
    memcpy(m_ewkb + 5, &nGeoms, 4);
    m_ewkb += 9;
  }

  for(unsigned int i = 0; i < nGeoms; ++i)
    visited.getGeometryN(i)->accept(*this);
}

void te::pgis::EWKBWriter::visit(const te::gm::LinearRing& visited)
{
  m_ewkb = Write2EWKB(m_ewkb, &visited);
}

void te::pgis::EWKBWriter::visit(const te::gm::LineString& visited)
{
  memcpy(m_ewkb, &te::common::Globals::sm_machineByteOrder, 1);

  unsigned int gType = static_cast<unsigned int>(visited.getGeomTypeId());
  int srid = visited.getSRID();

  if(srid <= TE_UNKNOWN_SRS)
    srid = PGIS_UNKNOWN_SRS;

  Convert2PostGISWKBType(gType);

  if(m_outputSRID)
  {
    gType |= TE_EWKB_SRID_FLAG;

    memcpy(m_ewkb + 1, &gType, 4);
    memcpy(m_ewkb + 5, &srid, 4);

    m_ewkb += 9;

    m_outputSRID = false;
  }  
  else
  {
    memcpy(m_ewkb + 1, &gType, 4);
    m_ewkb += 5;
  }

  m_ewkb = Write2EWKB(m_ewkb, &visited);
}

void te::pgis::EWKBWriter::visit(const te::gm::MultiLineString& visited)
{
  visit((const te::gm::GeometryCollection&)visited);
}

void te::pgis::EWKBWriter::visit(const te::gm::MultiPoint& visited)
{
  visit((const te::gm::GeometryCollection&)visited);
}

void te::pgis::EWKBWriter::visit(const te::gm::MultiPolygon& visited)
{
  visit((const te::gm::GeometryCollection&)visited);
}

void te::pgis::EWKBWriter::visit(const te::gm::Point& visited)
{
  memcpy(m_ewkb, &te::common::Globals::sm_machineByteOrder, 1);

  unsigned int gType = static_cast<unsigned int>(visited.getGeomTypeId());
  int srid = visited.getSRID();

  if(srid <= TE_UNKNOWN_SRS)
    srid = PGIS_UNKNOWN_SRS;

  Convert2PostGISWKBType(gType);

  if(m_outputSRID)
  {
    gType |= TE_EWKB_SRID_FLAG;

    memcpy(m_ewkb + 1, &gType, 4);
    memcpy(m_ewkb + 5, &srid, 4);

    m_ewkb += 9;

    m_outputSRID = false;
  }
  else
  {
    memcpy(m_ewkb + 1, &gType, 4);
    m_ewkb += 5;
  }  
  
  memcpy(m_ewkb, &(visited.getX()), 8);       // copy x
  memcpy(m_ewkb + 8, &(visited.getY()), 8);   // copy y

  int size = 16;
  
  if(gType & TE_EWKB_Z_OFFSET)
  {
    memcpy(m_ewkb + size, &(visited.getZ()), 8); // copy z
    size += 8;
  }

  if(gType & TE_EWKB_M_OFFSET)
  {
    memcpy(m_ewkb + size, &(visited.getM()), 8); // copy m
    size += 8;
  }

  m_ewkb += size;
}

void te::pgis::EWKBWriter::visit(const te::gm::PointM& visited)
{
  visit((const te::gm::Point&)visited);
}

void te::pgis::EWKBWriter::visit(const te::gm::PointZ& visited)
{
  visit((const te::gm::Point&)visited);
}

void te::pgis::EWKBWriter::visit(const te::gm::PointZM& visited)
{
  visit((const te::gm::Point&)visited);
}

void te::pgis::EWKBWriter::visit(const te::gm::Polygon& visited)
{
  memcpy(m_ewkb, &te::common::Globals::sm_machineByteOrder, 1);

  unsigned int gType = static_cast<unsigned int>(visited.getGeomTypeId());
  unsigned int nRings = static_cast<unsigned int>(visited.getNumRings());
  int srid = visited.getSRID();

  if(srid <= TE_UNKNOWN_SRS)
    srid = PGIS_UNKNOWN_SRS;

  Convert2PostGISWKBType(gType);

  if(m_outputSRID)
  {
    gType |= TE_EWKB_SRID_FLAG;

    memcpy(m_ewkb + 1, &gType, 4);  
    memcpy(m_ewkb + 5, &srid, 4);
    memcpy(m_ewkb + 9, &nRings, 4);

    m_ewkb += 13;

    m_outputSRID = false;
  }
  else
  {
    memcpy(m_ewkb + 1, &gType, 4);  
    memcpy(m_ewkb + 5, &nRings, 4);

    m_ewkb += 9;
  }  

  for(unsigned int i = 0; i < nRings; ++i)
    visited.getRingN(i)->accept(*this);
}
