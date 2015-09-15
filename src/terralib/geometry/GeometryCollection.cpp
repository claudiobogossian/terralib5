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
  \file terralib/geometry/GeometryCollection.cpp

  \brief It is a collection of other geometric objects.
*/

// TerraLib
#include "../BuildConfig.h"
#include "../common/STLUtils.h"
#include "../common/Translator.h"
#include "Envelope.h"
#include "Exception.h"
#include "GeometryCollection.h"

// STL
#include <cassert>

const std::string te::gm::GeometryCollection::sm_typeName("GeometryCollection");

te::gm::GeometryCollection::GeometryCollection(std::size_t nGeom, GeomType t, int srid, Envelope* mbr)
  : te::gm::Geometry(t, srid, mbr),
    m_geometries(nGeom)
{
}

te::gm::GeometryCollection::GeometryCollection(const GeometryCollection& rhs)
  : Geometry(rhs)
{
  te::common::Clone(rhs.m_geometries, m_geometries);
}

te::gm::GeometryCollection::~GeometryCollection()
{
  te::common::FreeContents(m_geometries);
}

te::gm::GeometryCollection& te::gm::GeometryCollection::operator=(const GeometryCollection& rhs)
{
  if(this != &rhs)
  {
    Geometry::operator=(rhs);

    te::common::FreeContents(m_geometries);

    m_geometries.clear();

    te::common::Clone(rhs.m_geometries, m_geometries);
  }

  return *this;
}

te::dt::AbstractData* te::gm::GeometryCollection::clone() const
{
  return new GeometryCollection(*this);
}

te::gm::Dimensionality te::gm::GeometryCollection::getDimension() const throw()
{
  Dimensionality maxDim = P;

  std::size_t size = m_geometries.size();

  for(std::size_t i = 0; i < size; ++i)
  {
    if(maxDim < m_geometries[i]->getDimension())
      maxDim = m_geometries[i]->getDimension();
  }

  return maxDim;
}

const std::string& te::gm::GeometryCollection::getGeometryType() const throw()
{
  return sm_typeName;
}

void te::gm::GeometryCollection::setSRID(int srid) throw()
{
  std::size_t n = m_geometries.size();

  for(std::size_t i = 0; i < n; ++i)
    m_geometries[i]->setSRID(srid);

  m_srid = srid;
}

void te::gm::GeometryCollection::transform(int srid) throw(te::common::Exception)
{
#ifdef TERRALIB_MOD_SRS_ENABLED
  if(srid == m_srid)
    return;

  std::size_t nGeoms = m_geometries.size();

  for(std::size_t i = 0; i < nGeoms; ++i)
    m_geometries[i]->transform(srid);

  m_srid = srid;

  if(m_mbr)
    computeMBR(false);  // the above transform will do the job for the parts, so don't compute mbr again!
#else
  throw Exception(TE_TR("transform method is not supported!"));
#endif // TERRALIB_MOD_SRS_ENABLED
}

void te::gm::GeometryCollection::computeMBR(bool cascade) const throw()
{
  if(m_mbr == 0)
    m_mbr = new Envelope;
  else
    m_mbr->makeInvalid();

  std::size_t nGeoms = m_geometries.size();

  if(nGeoms == 0)
    return;

  if(cascade)
    m_geometries[0]->computeMBR(true);

  *m_mbr = *(m_geometries[0]->getMBR());

  for(std::size_t i = 1; i < nGeoms; ++i)
  {
    if(cascade)
      m_geometries[i]->computeMBR(true);

    m_mbr->Union(*(m_geometries[i]->getMBR()));
  }
}

std::size_t te::gm::GeometryCollection::getNPoints() const throw()
{
  std::size_t n = m_geometries.size();

  std::size_t sum = 0;

  for(std::size_t i = 0; i < n; ++i)
  {
    assert(m_geometries[i] != 0);
    sum += m_geometries[i]->getNPoints();
  }

  return sum;
}

void te::gm::GeometryCollection::setNumGeometries(std::size_t size)
{
  if(size < m_geometries.size())
  {
    std::size_t oldSize = m_geometries.size();
    for(std::size_t i = size; i < oldSize; ++i)
      delete m_geometries[i];
  }

  m_geometries.resize(size);
}

te::gm::Geometry* te::gm::GeometryCollection::getGeometryN(std::size_t i) const
{
  assert(i < m_geometries.size());
  return m_geometries[i];
}

te::gm::Geometry* te::gm::GeometryCollection::getGeometryN(std::size_t i)
{
  assert(i < m_geometries.size());
  return m_geometries[i];         
}

void te::gm::GeometryCollection::setGeometryN(std::size_t i, Geometry* g)
{
  assert((i < m_geometries.size()) && (m_geometries[i] == 0));
  delete m_geometries[i];
  m_geometries[i] = g;
}

void te::gm::GeometryCollection::removeGeometryN(std::size_t i)
{
  assert(i < m_geometries.size());
  delete m_geometries[i];
  m_geometries.erase(m_geometries.begin() + i);
}

void te::gm::GeometryCollection::add(Geometry* g)
{
  m_geometries.push_back(g);
}

void te::gm::GeometryCollection::clear()
{
  te::common::FreeContents(m_geometries);
  m_geometries.clear();
}

