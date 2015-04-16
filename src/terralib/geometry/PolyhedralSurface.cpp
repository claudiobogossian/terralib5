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
  \file terralib/geometry/PolyhedralSurface.cpp

  \brief PolyhedralSurface is a contiguous collection of polygons, which share common boundary segments.
*/

// TerraLib
#include "terralib_config.h"
#include "../common/STLUtils.h"
#include "../common/Translator.h"
#include "Envelope.h"
#include "Exception.h"
#include "MultiPolygon.h"
#include "Polygon.h"
#include "PolyhedralSurface.h"

const std::string te::gm::PolyhedralSurface::sm_typeName("PolyhedralSurface");

te::gm::PolyhedralSurface::PolyhedralSurface(std::size_t nPolygons, GeomType t, int srid, Envelope* mbr)
  : Surface(t, srid, mbr),
    m_polygons(nPolygons)
{
}

te::gm::PolyhedralSurface::PolyhedralSurface(const PolyhedralSurface& rhs)
  : Surface(rhs)
{
  te::common::Clone(rhs.m_polygons, m_polygons);
}

te::gm::PolyhedralSurface::~PolyhedralSurface()
{
  te::common::FreeContents(m_polygons);
}

te::gm::PolyhedralSurface& te::gm::PolyhedralSurface::operator=(const PolyhedralSurface& rhs)
{
  if(this != &rhs)
  {
    Surface::operator=(rhs);
    te::common::FreeContents(m_polygons);
    m_polygons.clear();
    te::common::Clone(rhs.m_polygons, m_polygons);
  }

  return *this;
}

te::dt::AbstractData* te::gm::PolyhedralSurface::clone() const
{
  return new PolyhedralSurface(*this);
}

void te::gm::PolyhedralSurface::setNumPatches(std::size_t size)
{
  if(size < m_polygons.size())
  {
    std::size_t oldSize = m_polygons.size();
    for(std::size_t i = size; i < oldSize; ++i)
      delete m_polygons[i];
  }

  m_polygons.resize(size);
}

te::gm::Polygon* te::gm::PolyhedralSurface::getPatchN(std::size_t i) const
{
  assert(i < m_polygons.size());
  return m_polygons[i];
}

void te::gm::PolyhedralSurface::setPatchN(std::size_t i, Polygon* p)
{
  assert((i < m_polygons.size()) && (m_polygons[i] == 0));
  delete m_polygons[i];
  m_polygons[i] = p;
}

te::gm::MultiPolygon* te::gm::PolyhedralSurface::getBoundingPolygons(const Polygon* /*p*/) const
{
  return 0;
}

double te::gm::PolyhedralSurface::getPerimeter() const
{
  return 0.0;
}

const std::string& te::gm::PolyhedralSurface::getGeometryType() const throw()
{
  return sm_typeName;
}

void te::gm::PolyhedralSurface::setSRID(int srid) throw()
{
  std::size_t n = m_polygons.size();

  for(std::size_t i = 0; i < n; ++i)
    m_polygons[i]->setSRID(srid);

  m_srid = srid;
}

void te::gm::PolyhedralSurface::transform(int srid) throw(te::common::Exception)
{
#ifdef TERRALIB_MOD_SRS_ENABLED
  if (srid == m_srid)
    return;

  std::size_t n = m_polygons.size();

  for(std::size_t i = 0; i < n; ++i)
   m_polygons[i]->transform(srid);

  if(m_mbr)
    computeMBR(false);

  m_srid = srid;
#else
  throw Exception(TE_TR("transform method is not supported!"));
#endif // TERRALIB_MOD_SRS_ENABLED
}

void te::gm::PolyhedralSurface::computeMBR(bool cascade) const throw()
{
  if(m_mbr == 0)
    m_mbr = new Envelope;
  else
    m_mbr->makeInvalid();

  std::size_t nPols = m_polygons.size();

  if(nPols == 0)
    return;

  if(cascade)
    m_polygons[0]->computeMBR(true);

  *m_mbr =  *(m_polygons[0]->getMBR());

  for(std::size_t i = 1; i < nPols; ++i)
  {
    if(cascade)
      m_polygons[i]->computeMBR(true);

    m_mbr->Union(*(m_polygons[i]->getMBR()));
  }
}

std::size_t te::gm::PolyhedralSurface::getNPoints() const throw()
{
  std::size_t n = m_polygons.size();

  std::size_t sum = 0;

  for(std::size_t i = 0; i < n; ++i)
    sum += m_polygons[i]->getNPoints();

  return sum;
}

