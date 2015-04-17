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
  \file terralib/geometry/LineString.cpp

  \brief LineString is a curve with linear interpolation between points.
*/

// TerraLib
#include "terralib_config.h"
#include "../common/Translator.h"
#include "../srs/Converter.h"
#include "Config.h"
#include "Coord2D.h"
#include "Envelope.h"
#include "Exception.h"
#include "GEOSWriter.h"
#include "LineString.h"
#include "Point.h"
#include "PointM.h"
#include "PointZ.h"
#include "PointZM.h"

// STL
#include <cassert>
#include <cstdlib>
#include <cstring>
#include <memory>

#ifdef TERRALIB_GEOS_ENABLED
// GEOS
#include <geos/geom/Geometry.h>
#include <geos/util/GEOSException.h>
#endif

const std::string te::gm::LineString::sm_typeName("LineString");

te::gm::LineString::LineString(GeomType t, int srid, Envelope* mbr)
  : Curve(t, srid, mbr),
    m_coords(0),
    m_zA(0),
    m_mA(0),
    m_nPts(0)
{
}

te::gm::LineString::LineString(std::size_t size, GeomType t, int srid, Envelope* mbr)
  : Curve(t, srid, mbr),
    m_zA(0),
    m_mA(0),
    m_nPts(size)
{
  m_coords = static_cast<Coord2D*>(malloc(16 * size));

  if((m_gType & 0xF00) == 0x300)
     m_zA = static_cast<double*>(malloc(8 * size));

  if((m_gType & 0xF00) == 0x700)
     m_mA = static_cast<double*>(malloc(8 * size));

  if((m_gType & 0xF00) == 0xB00)
  {
    assert(m_zA == 0);
    assert(m_mA == 0);

    m_zA = static_cast<double*>(malloc(8 * size));
    m_mA = static_cast<double*>(malloc(8 * size));
  }
}

te::gm::LineString::LineString(const LineString& rhs)
  : Curve(rhs),
    m_zA(0),
    m_mA(0),
    m_nPts(0)
{
  m_nPts = rhs.m_nPts;

  if(rhs.m_coords)
  {
    m_coords = static_cast<Coord2D*>(malloc(16 * rhs.m_nPts));
    memcpy(m_coords, rhs.m_coords, 16 * rhs.m_nPts);
  }

  if(rhs.m_zA)
  {
    m_zA = static_cast<double*>(malloc(8 * rhs.m_nPts));
    memcpy(m_zA, rhs.m_zA, 8 * rhs.m_nPts);
  }

  if(rhs.m_mA)
  {
    m_mA = static_cast<double*>(malloc(8 * rhs.m_nPts));
    memcpy(m_mA, rhs.m_mA, 8 * rhs.m_nPts);
  }
}

te::gm::LineString::~LineString()
{
  free(m_coords);
  free(m_zA);
  free(m_mA);
}

te::gm::LineString& te::gm::LineString::operator=(const LineString& rhs)
{
  if(this != &rhs)
  {
    Curve::operator=(rhs);

    makeEmpty();

    if( rhs.m_coords )
    {
      m_coords = static_cast<Coord2D*>(malloc(sizeof( Coord2D ) * rhs.m_nPts));
      memcpy( m_coords, rhs.m_coords, sizeof( Coord2D ) * rhs.m_nPts );
    }
    else
    {
      m_coords = 0;
    }

    if( rhs.m_zA )
    {
      m_zA = static_cast<double*>(malloc(sizeof( double ) * rhs.m_nPts));
      memcpy( m_zA, rhs.m_zA, sizeof( double ) * rhs.m_nPts );
    }
    else
    {
      m_zA = 0;
    }

    if( rhs.m_mA )
    {
      m_mA = static_cast<double*>(malloc(sizeof( double ) * rhs.m_nPts));
      memcpy( m_mA, rhs.m_mA, sizeof( double ) * rhs.m_nPts);
    }
    else
    {
      m_mA = 0;
    }
    
    m_nPts = rhs.m_nPts;
  }

  return *this;
}

te::dt::AbstractData* te::gm::LineString::clone() const
{
  return new LineString(*this);
}

const std::string& te::gm::LineString::getGeometryType() const throw()
{
  return sm_typeName;
}

void te::gm::LineString::setSRID(int srid) throw()
{
  m_srid = srid;
}

void te::gm::LineString::transform(int srid) throw(te::common::Exception)
{
#ifdef TERRALIB_MOD_SRS_ENABLED
  if(srid == m_srid)
    return;

  std::auto_ptr<te::srs::Converter> converter(new te::srs::Converter());

  converter->setSourceSRID(getSRID());

  converter->setTargetSRID(srid);

  double* pt = (double*)(m_coords);

  converter->convert(pt, &(pt[1]), static_cast<long>(size()), 2);

  if(m_mbr)
    computeMBR(false);

  m_srid = srid;
#else
  throw Exception(TE_TR("transform method is not supported!"));
#endif // TERRALIB_MOD_SRS_ENABLED
}

void te::gm::LineString::computeMBR(bool /*cascade*/) const throw()
{
  if(m_mbr == 0)
    m_mbr = new Envelope;
  else
    m_mbr->makeInvalid();

  const std::size_t nPts = size();

  if(nPts == 0)
    return;

  double minx = m_coords[0].x;
  double miny = m_coords[0].y;
  double maxx = m_coords[0].x;
  double maxy = m_coords[0].y;
  
  for(std::size_t i = 1; i < nPts; ++i)
  {
    if(minx > m_coords[i].x) minx = m_coords[i].x;
    if(miny > m_coords[i].y) miny = m_coords[i].y;
    if(maxx < m_coords[i].x) maxx = m_coords[i].x;
    if(maxy < m_coords[i].y) maxy = m_coords[i].y;
  }

  m_mbr->m_llx = minx;
  m_mbr->m_lly = miny;
  m_mbr->m_urx = maxx;
  m_mbr->m_ury = maxy;
}

te::gm::Geometry* te::gm::LineString::locateBetween(const double& /*mStart*/, const double& /*mEnd*/) const throw(Exception)
{
  return 0;
}

//double te::gm::LineString::getLength() const
//{
//  return 0.0;
//}

te::gm::Point* te::gm::LineString::getStartPoint() const
{
  assert(size() > 1);
  return getPointN(0);
}

te::gm::Point* te::gm::LineString::getEndPoint() const
{
  assert(size() > 1);
  return getPointN(m_nPts - 1);
}

bool te::gm::LineString::isClosed() const
{
  assert(size() >= 2);
  return m_coords[0] == m_coords[m_nPts - 1];
}

void te::gm::LineString::setNumCoordinates(std::size_t size)
{
  if(size < m_nPts)
  { // just decrease the known capacity... realloc just if needed!
    m_nPts = size;
    return;
  }

  Coord2D* coords = static_cast<Coord2D*>(malloc(16 * size));
  memcpy(coords, m_coords, (m_nPts < size ? m_nPts * 16 : size * 16));
  free(m_coords);
  m_coords = coords;

  if((m_gType & 0xF00) == 0x300)
  {
    double* zA = static_cast<double*>(malloc(8 * size));
    memcpy(zA, m_zA, (m_nPts < size ? m_nPts : size));
    free(m_zA);
    m_zA = zA;
  }

  if((m_gType & 0xF00) == 0x700)
  {
    double* mA = static_cast<double*>(malloc(8 * size));
    memcpy(mA, m_mA, (m_nPts < size ? m_nPts : size));
    free(m_mA);
    m_mA = mA;
  }

  if((m_gType & 0xF00) == 0xB00)
  {
    double* zA = static_cast<double*>(malloc(8 * size));
    memcpy(zA, m_zA, (m_nPts < size ? m_nPts : size));
    free(m_zA);
    m_zA = zA;

    double* mA = static_cast<double*>(malloc(8 * size));
    memcpy(mA, m_mA, (m_nPts < size ? m_nPts : size));
    free(m_mA);
    m_mA = mA;
  }

  m_nPts = size;
}

void te::gm::LineString::makeEmpty()
{
  free(m_coords);
  free(m_zA);
  free(m_mA);

  m_coords = 0;
  m_zA = 0;
  m_mA = 0;

  m_nPts = 0;
}

te::gm::Point* te::gm::LineString::getPointN(std::size_t i) const
{
  assert(i < m_nPts);
  
  if((m_gType & 0xF00) == 0x000)
    return new Point(m_coords[i].x, m_coords[i].y, m_srid, 0);

  if((m_gType & 0xF00) == 0x300)
    return new PointZ(m_coords[i].x, m_coords[i].y, m_zA[i], m_srid, 0);

  if((m_gType & 0xF00) == 0x700)
    return new PointM(m_coords[i].x, m_coords[i].y, m_mA[i], m_srid, 0);

  return new PointZM(m_coords[i].x, m_coords[i].y, m_zA[i], m_mA[i], m_srid, 0);
}

void te::gm::LineString::setPointN(std::size_t i, const Point& p)
{
  assert(i < m_nPts);

  m_coords[i].x = p.getX();
  m_coords[i].y = p.getY();

  if(m_zA)
    m_zA[i] = p.getZ();

  if(m_mA)
    m_mA[i] = p.getM();
}

void te::gm::LineString::setPoint(std::size_t i, const double& x, const double& y)
{
  assert(i < size());
  m_coords[i].x = x;
  m_coords[i].y = y;
}

void te::gm::LineString::setPointZ(std::size_t i, const double& x, const double& y, const double& z)
{
  assert((i < m_nPts) && (m_zA != 0));
  m_coords[i].x = x;
  m_coords[i].y = y;
  m_zA[i] = z;
}

void te::gm::LineString::setPointM(std::size_t i, const double& x, const double& y, const double& m)
{
  assert((i < m_nPts) && (m_mA != 0));
  m_coords[i].x = x;
  m_coords[i].y = y;
  m_mA[i] = m;
}

void te::gm::LineString::setPointZM(std::size_t i, const double& x, const double& y, const double& z, const double& m)
{
  assert((i < m_nPts) && (m_zA != 0) && (m_mA != 0));
  m_coords[i].x = x;
  m_coords[i].y = y;
  m_zA[i] = z;
  m_mA[i] = m;
}

const double& te::gm::LineString::getX(std::size_t i) const
{
  assert(i < size());
  return m_coords[i].x;
}

const double& te::gm::LineString::getY(std::size_t i) const
{
  assert(i < size());
  return m_coords[i].y;
}

const double& te::gm::LineString::getZ(std::size_t i) const
{
  assert((i < m_nPts) && (m_zA != 0));
  return m_zA[i];
}

const double& te::gm::LineString::getM(std::size_t i) const
{
  assert((i < m_nPts) && (m_mA != 0));
  return m_mA[i];
}


void te::gm::LineString::setX(std::size_t i, const double& x)
{
  assert(i < size());
  m_coords[i].x = x;
}

void te::gm::LineString::setY(std::size_t i, const double& y)
{
  assert(i < size());
  m_coords[i].y = y;
}

void te::gm::LineString::setZ(std::size_t i, const double& z)
{
  assert((i < m_nPts) && (m_zA != 0));
  m_zA[i] = z;
}

void te::gm::LineString::setM(std::size_t i, const double& m)
{
  assert((i < m_nPts) && (m_mA != 0));
  m_mA[i] = m;
}

