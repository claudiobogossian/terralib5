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
  \file terralib/geometry/CompoundCurve.cpp

  \brief CompoundCurve is a curve that may have circular and linear segments.
*/

// TerraLib
#include "terralib_config.h"
#include "../common/STLUtils.h"
#include "../common/Translator.h"
#include "../srs/Converter.h"
#include "Coord2D.h"
#include "Envelope.h"
#include "Exception.h"
#include "CompoundCurve.h"
#include "Point.h"
#include "PointM.h"
#include "PointZ.h"
#include "PointZM.h"

// STL
#include <cassert>
#include <memory>

const std::string te::gm::CompoundCurve::sm_typeName("CompoundCurve");

te::gm::CompoundCurve::CompoundCurve(GeomType t, int srid, Envelope* mbr)
  : Curve(t, srid, mbr)
{
}

te::gm::CompoundCurve::CompoundCurve(std::size_t size, GeomType t, int srid, Envelope* mbr)
  : Curve(t, srid, mbr),
    m_curves(size)
{
}

te::gm::CompoundCurve::CompoundCurve(const CompoundCurve& rhs)
  : Curve(rhs)
{
  te::common::Clone(rhs.m_curves, m_curves);  
}

te::gm::CompoundCurve::~CompoundCurve()
{
  te::common::FreeContents(m_curves);
}

te::gm::CompoundCurve& te::gm::CompoundCurve::operator=(const CompoundCurve& rhs)
{
  if(this != &rhs)
  {
    Curve::operator=(rhs);

    te::common::FreeContents(m_curves);
    m_curves.clear();
    te::common::Clone(rhs.m_curves, m_curves);
  }

  return *this;
}

te::dt::AbstractData* te::gm::CompoundCurve::clone() const
{
  return new CompoundCurve(*this);
}

const std::string& te::gm::CompoundCurve::getGeometryType() const throw()
{
  return sm_typeName;
}

void te::gm::CompoundCurve::setSRID(int srid) throw()
{
  m_srid = srid;
}

void te::gm::CompoundCurve::transform(int srid) throw(te::common::Exception)
{
#ifdef TERRALIB_MOD_SRS_ENABLED
  if(srid == m_srid)
    return;

  const std::size_t size = m_curves.size();

  for(std::size_t i = 0; i < size; ++i)
    m_curves[i]->transform(srid);

  if(m_mbr)
    computeMBR(false);

  m_srid = srid;
#else
  throw Exception(TE_TR("transform method is not supported!"));
#endif // TERRALIB_MOD_SRS_ENABLED
}

void te::gm::CompoundCurve::computeMBR(bool cascade) const throw()
{
  if(m_mbr == 0)
    m_mbr = new Envelope;
  else
    m_mbr->makeInvalid();

  const std::size_t nSegs = size();

  if(nSegs == 0)
    return;

  if(cascade)
  {
    for(std::size_t i = 0; i < nSegs; ++i)
      m_curves[i]->computeMBR(true);
  }

  const Envelope* e = m_curves[0]->getMBR();

  double minx = e->m_llx;
  double miny = e->m_lly;
  double maxx = e->m_urx;
  double maxy = e->m_ury;
  
  for(std::size_t i = 1; i < nSegs; ++i)
  {
    e = m_curves[i]->getMBR();

    if(minx > e->m_llx) minx = e->m_llx;
    if(miny > e->m_lly) miny = e->m_lly;
    if(maxx < e->m_urx) maxx = e->m_urx;
    if(maxy < e->m_ury) maxy = e->m_ury;
  }

  m_mbr->m_llx = minx;
  m_mbr->m_lly = miny;
  m_mbr->m_urx = maxx;
  m_mbr->m_ury = maxy;
}

std::size_t te::gm::CompoundCurve::getNPoints() const throw()
{
  return 0;
}

te::gm::Geometry* te::gm::CompoundCurve::locateBetween(const double& /*mStart*/, const double& /*mEnd*/) const throw(Exception)
{
  return 0;
}

double te::gm::CompoundCurve::getLength() const
{
  return 0.0;
}

te::gm::Point* te::gm::CompoundCurve::getStartPoint() const
{
  assert(size() > 1);
  return 0;
}

te::gm::Point* te::gm::CompoundCurve::getEndPoint() const
{
  assert(size() > 1);
  return 0;
}

bool te::gm::CompoundCurve::isClosed() const
{
  assert(size() >= 2);
  return false;
}

void te::gm::CompoundCurve::makeEmpty()
{
  te::common::FreeContents(m_curves);
  m_curves.clear();
}

te::gm::Curve* te::gm::CompoundCurve::getCurve(std::size_t i) const
{
  assert(i < size());

  return m_curves[i];
}

void te::gm::CompoundCurve::add(Curve* c)
{
  m_curves.push_back(c);
}