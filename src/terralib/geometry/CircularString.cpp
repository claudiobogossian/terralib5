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
  \file terralib/geometry/CircularString.cpp

  \brief CircularString is a curve with circular interpolation between points.
*/

// TerraLib
#include "../BuildConfig.h"
#include "../common/Translator.h"
#include "../srs/Converter.h"
#include "Coord2D.h"
#include "Envelope.h"
#include "Exception.h"
#include "CircularString.h"
#include "Point.h"
#include "PointM.h"
#include "PointZ.h"
#include "PointZM.h"

// STL
#include <cassert>
#include <memory>

const std::string te::gm::CircularString::sm_typeName("CircularString");

te::gm::CircularString::CircularString(GeomType t, int srid, Envelope* mbr)
  : Curve(t, srid, mbr)
{
}

te::gm::CircularString::CircularString(std::size_t size, GeomType t, int srid, Envelope* mbr)
  : Curve(t, srid, mbr),
    m_coords(size)
{
  if((m_gType & 0xF00) == 0x300)
    m_zA.resize(size);
  else if((m_gType & 0xF00) == 0x700)
     m_mA.resize(size);
  else if((m_gType & 0xF00) == 0xB00)
  {
    assert(m_zA.size() == 0);
    assert(m_mA.size() == 0);

    m_zA.resize(size);
    m_mA.resize(size);
  }
}

te::gm::CircularString::CircularString(const CircularString& rhs)
  : Curve(rhs),
    m_coords(rhs.m_coords),
    m_zA(rhs.m_zA),
    m_mA(rhs.m_mA)
{
}

te::gm::CircularString::~CircularString()
{
}

te::gm::CircularString& te::gm::CircularString::operator=(const CircularString& rhs)
{
  if(this != &rhs)
  {
    Curve::operator=(rhs);

    m_coords = rhs.m_coords;
    m_zA = rhs.m_zA;
    m_mA = rhs.m_mA;
  }

  return *this;
}

te::dt::AbstractData* te::gm::CircularString::clone() const
{
  return new CircularString(*this);
}

const std::string& te::gm::CircularString::getGeometryType() const throw()
{
  return sm_typeName;
}

void te::gm::CircularString::setSRID(int srid) throw()
{
  m_srid = srid;
}

void te::gm::CircularString::transform(int srid) throw(te::common::Exception)
{
#ifdef TERRALIB_MOD_SRS_ENABLED
  if(srid == m_srid)
    return;

  std::auto_ptr<te::srs::Converter> converter(new te::srs::Converter());

  converter->setSourceSRID(getSRID());

  converter->setTargetSRID(srid);

  double* pt = (double*)(&m_coords);

  converter->convert(pt, &(pt[1]), static_cast<long>(size()), 2);

  if(m_mbr)
    computeMBR(false);

  m_srid = srid;
#else
  throw Exception(TE_TR("transform method is not supported!"));
#endif // TERRALIB_MOD_SRS_ENABLED
}

void te::gm::CircularString::computeMBR(bool /*cascade*/) const throw()
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

te::gm::Geometry* te::gm::CircularString::locateBetween(const double& /*mStart*/, const double& /*mEnd*/) const throw(Exception)
{
  return 0;
}

double te::gm::CircularString::getLength() const
{
  return 0.0;
}

te::gm::Point* te::gm::CircularString::getStartPoint() const
{
  assert(size() > 1);
  return getPointN(0);
}

te::gm::Point* te::gm::CircularString::getEndPoint() const
{
  assert(size() > 1);
  return getPointN(size() - 1);
}

bool te::gm::CircularString::isClosed() const
{
  assert(size() >= 2);
  return m_coords[0] == m_coords[size() - 1];
}

void te::gm::CircularString::setNumCoordinates(std::size_t size)
{
  m_coords.resize(size);

  if((m_gType & 0xF00) == 0x300)
    m_zA.resize(size);
  else if((m_gType & 0xF00) == 0x700)
     m_mA.resize(size);
  else if((m_gType & 0xF00) == 0xB00)
  {
    m_zA.resize(size);
    m_mA.resize(size);
  }
}

void te::gm::CircularString::makeEmpty()
{
  m_coords.clear();
  m_zA.clear();
  m_mA.clear();
}

te::gm::Point* te::gm::CircularString::getPointN(std::size_t i) const
{
  assert(i < size());
  
  if((m_gType & 0xF00) == 0x000)
    return new Point(m_coords[i].x, m_coords[i].y, m_srid, 0);

  if((m_gType & 0xF00) == 0x300)
    return new PointZ(m_coords[i].x, m_coords[i].y, m_zA[i], m_srid, 0);

  if((m_gType & 0xF00) == 0x700)
    return new PointM(m_coords[i].x, m_coords[i].y, m_mA[i], m_srid, 0);

  return new PointZM(m_coords[i].x, m_coords[i].y, m_zA[i], m_mA[i], m_srid, 0);
}

void te::gm::CircularString::setPointN(std::size_t i, const Point& p)
{
  assert(i < size());

  m_coords[i].x = p.getX();
  m_coords[i].y = p.getY();

  if((m_gType & 0xF00) == 0x300)
    m_zA[i] = p.getZ();
  else if((m_gType & 0xF00) == 0x300)
    m_mA[i] = p.getM();
  else if((m_gType & 0xF00) == 0x700)
  {
    m_zA[i] = p.getZ();
    m_mA[i] = p.getM();
  }
}

void te::gm::CircularString::setPoint(std::size_t i, const double& x, const double& y)
{
  assert(i < size());
  m_coords[i].x = x;
  m_coords[i].y = y;
}

void te::gm::CircularString::setPointZ(std::size_t i, const double& x, const double& y, const double& z)
{
  assert((i < size()) && (m_zA.empty() == false));
  m_coords[i].x = x;
  m_coords[i].y = y;
  m_zA[i] = z;
}

void te::gm::CircularString::setPointM(std::size_t i, const double& x, const double& y, const double& m)
{
  assert((i < size()) && (m_mA.empty() == false));
  m_coords[i].x = x;
  m_coords[i].y = y;
  m_mA[i] = m;
}

void te::gm::CircularString::setPointZM(std::size_t i, const double& x, const double& y, const double& z, const double& m)
{
  assert((i < size()) && (m_zA.empty() == false) && (m_mA.empty() == false));
  m_coords[i].x = x;
  m_coords[i].y = y;
  m_zA[i] = z;
  m_mA[i] = m;
}

const double& te::gm::CircularString::getX(std::size_t i) const
{
  assert(i < size());
  return m_coords[i].x;
}

const double& te::gm::CircularString::getY(std::size_t i) const
{
  assert(i < size());
  return m_coords[i].y;
}

const double& te::gm::CircularString::getZ(std::size_t i) const
{
  assert((i < size()) && (m_zA.empty() == false));
  return m_zA[i];
}

const double& te::gm::CircularString::getM(std::size_t i) const
{
  assert((i < size()) && (m_mA.empty() == false));
  return m_mA[i];
}


void te::gm::CircularString::setX(std::size_t i, const double& x)
{
  assert(i < size());
  m_coords[i].x = x;
}

void te::gm::CircularString::setY(std::size_t i, const double& y)
{
  assert(i < size());
  m_coords[i].y = y;
}

void te::gm::CircularString::setZ(std::size_t i, const double& z)
{
  assert((i < size()) && (m_zA.empty() == false));
  m_zA[i] = z;
}

void te::gm::CircularString::setM(std::size_t i, const double& m)
{
  assert((i < size()) && (m_mA.empty() == false));
  m_mA[i] = m;
}

