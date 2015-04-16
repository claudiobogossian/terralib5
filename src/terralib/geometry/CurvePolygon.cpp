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
  \file terralib/geometry/CurvePolygon.cpp

  \brief CurvePolygon is a planar surface defined by 1 exterior boundary and 0 or more interior boundaries.
*/

// TerraLib
#include "terralib_config.h"
#include "../common/STLUtils.h"
#include "../common/Translator.h"
#include "Config.h"
#include "Coord2D.h"
#include "Curve.h"
#include "CurvePolygon.h"
#include "Envelope.h"
#include "GEOSWriter.h"
#include "Point.h"

#ifdef TERRALIB_GEOS_ENABLED
// GEOS
#include <geos/algorithm/CentroidArea.h>
#include <geos/geom/Geometry.h>
#include <geos/geom/IntersectionMatrix.h>
#include <geos/operation/buffer/OffsetCurveBuilder.h>
#include <geos/util/GEOSException.h>
#endif

const std::string te::gm::CurvePolygon::sm_typeName("CurvePolygon");

te::gm::CurvePolygon::CurvePolygon(std::size_t nRings, GeomType t, int srid, Envelope* mbr)
  : Surface(t, srid, mbr),
    m_rings(nRings)
{
}

te::gm::CurvePolygon::CurvePolygon(const CurvePolygon& rhs)
  : Surface(rhs)
{
  te::common::Clone(rhs.m_rings, m_rings);
}

te::gm::CurvePolygon::~CurvePolygon()
{
  te::common::FreeContents(m_rings);
}

te::gm::CurvePolygon& te::gm::CurvePolygon::operator=(const CurvePolygon& rhs)
{
  if(this != &rhs)
  {
    Surface::operator=(rhs);

    te::common::FreeContents(m_rings);

    m_rings.clear();

    te::common::Clone(rhs.m_rings, m_rings);
  }

  return *this;
}

te::dt::AbstractData* te::gm::CurvePolygon::clone() const
{
  return new CurvePolygon(*this);
}

te::gm::Curve* te::gm::CurvePolygon::getExteriorRing() const
{
  assert(m_rings.size() > 0);
  return m_rings[0];
}

std::size_t te::gm::CurvePolygon::getNumInteriorRings() const
{
  return (m_rings.size() > 0) ?  (m_rings.size() - 1) : 0;
}

void te::gm::CurvePolygon::setNumRings(std::size_t size)
{
  if(size < m_rings.size())
  {
    std::size_t oldSize = m_rings.size();
    for(std::size_t i = size; i < oldSize; ++i)
      delete m_rings[i];
  }

  m_rings.resize(size);
}

te::gm::Curve* te::gm::CurvePolygon::getInteriorRingN(std::size_t i) const
{
  assert(i < getNumInteriorRings());
  return m_rings[i + 1];
}

void te::gm::CurvePolygon::setRingN(std::size_t i, Curve* r)
{
  assert(i < m_rings.size());
  delete m_rings[i];
  m_rings[i] = r;
}

void te::gm::CurvePolygon::removeRingN(std::size_t i)
{
  assert(i < m_rings.size());
  delete m_rings[i];
  m_rings.erase(m_rings.begin() + i);
}

void te::gm::CurvePolygon::clear()
{
  te::common::FreeContents(m_rings);
  m_rings.clear();
}

double te::gm::CurvePolygon::getArea() const
{
#ifdef TERRALIB_GEOS_ENABLED
  std::auto_ptr<geos::geom::Geometry> g(GEOSWriter::write(this));

  return g->getArea();

#else
  throw Exception(TE_TR("getArea routine is supported by GEOS! Please, enable the GEOS support."));
#endif  
}

te::gm::Point* te::gm::CurvePolygon::getCentroid() const
{
#ifdef TERRALIB_GEOS_ENABLED
  std::auto_ptr<geos::geom::Geometry> thisGeom(GEOSWriter::write(this));

  geos::algorithm::CentroidArea c;
  
  c.add(thisGeom.get());

  geos::geom::Coordinate coord;

  if(c.getCentroid(coord))
  {
    Point* pt = new Point(coord.x, coord.y, m_srid, 0);

    return pt;
  }

  return 0;

#else
  throw te::common::Exception(TE_TR("getCentroid routine is supported by GEOS! Please, enable the GEOS support."));
#endif
}

te::gm::Coord2D* te::gm::CurvePolygon::getCentroidCoord() const
{
  te::gm::Point * p = getCentroid();

  if(p)
  {
    te::gm::Coord2D* coord = new te::gm::Coord2D(p->getX(), p->getY());

    delete p;

    return coord;
  }

  return 0;
}

te::gm::Point* te::gm::CurvePolygon::getPointOnSurface() const
{
  return 0;
}

te::gm::Coord2D* te::gm::CurvePolygon::getCoordOnSurface() const
{
  return 0;
}

double te::gm::CurvePolygon::getPerimeter() const
{
#ifdef TERRALIB_GEOS_ENABLED
  std::auto_ptr<geos::geom::Geometry> g(GEOSWriter::write(this));

  return g->getLength();

#else
  throw Exception(TE_TR("getLength routine is supported by GEOS! Please, enable the GEOS support."));
#endif 
}

const std::string& te::gm::CurvePolygon::getGeometryType() const throw()
{
  return sm_typeName;
}

void te::gm::CurvePolygon::setSRID(int srid) throw()
{
  std::size_t n = m_rings.size();

  for(std::size_t i = 0; i < n; ++i)
    m_rings[i]->setSRID(srid);

  m_srid = srid;
}

void te::gm::CurvePolygon::transform(int srid) throw(te::common::Exception)
{
#ifdef TERRALIB_MOD_SRS_ENABLED
  if(srid == m_srid)
    return;

  std::size_t n = m_rings.size();

  for(std::size_t i = 0; i < n; ++i)
    m_rings[i]->transform(srid);

  if(m_mbr)
    computeMBR(true);  // just update the polygon MBR

  m_srid = srid;
#else
  throw Exception(TE_TR("transform method is not supported!"));
#endif // TERRALIB_MOD_SRS_ENABLED
}

void te::gm::CurvePolygon::computeMBR(bool cascade) const throw()
{
  if(m_mbr == 0)
    m_mbr = new Envelope;
  else
    m_mbr->makeInvalid();

  std::size_t n = m_rings.size();

  if(n == 0)
    return;

  if(cascade)
    m_rings[0]->computeMBR(true);

  *m_mbr = *(m_rings[0]->getMBR());

  if(cascade)
    for(std::size_t i = 1; i < n; ++i)
      m_rings[i]->computeMBR(true);
}

std::size_t te::gm::CurvePolygon::getNPoints() const throw()
{
  std::size_t n = m_rings.size();

  std::size_t sum = 0;

  for(std::size_t i = 0; i < n; ++i)
    sum += m_rings[i]->getNPoints();

  return sum;
}

