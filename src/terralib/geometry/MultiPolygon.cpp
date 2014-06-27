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
  \file terralib/geometry/MultiPolygon.cpp

  \brief MultiPolygon is a MultiSurface whose elements are Polygons.
*/

// TerraLib
#include "GEOSWriter.h"
#include "MultiPolygon.h"
#include "Point.h"

#ifdef TERRALIB_GEOS_ENABLED
// GEOS
#include <geos/algorithm/CentroidArea.h>
#include <geos/geom/MultiPolygon.h>
#endif

// STL
#include <cassert>

const std::string te::gm::MultiPolygon::sm_typeName("MultiPolygon");

te::gm::MultiPolygon::MultiPolygon(std::size_t nGeom, GeomType t, int srid, Envelope* mbr)
  : MultiSurface(nGeom, t, srid, mbr)
{
  assert(t == MultiPolygonType || t == MultiPolygonZType || t == MultiPolygonMType || t == MultiPolygonZMType);
}

te::gm::MultiPolygon::MultiPolygon(const MultiPolygon& rhs)
  : MultiSurface(rhs) 
{
}

te::gm::Point* te::gm::MultiPolygon::getCentroid() const
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

te::gm::Coord2D* te::gm::MultiPolygon::getCentroidCoord() const
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

te::gm::MultiPolygon& te::gm::MultiPolygon::operator=(const MultiPolygon& rhs)
{
  MultiSurface::operator=(rhs);

  return *this;
}

te::dt::AbstractData* te::gm::MultiPolygon::clone() const
{
  return new MultiPolygon(*this);
}

const std::string& te::gm::MultiPolygon::getGeometryType() const throw()
{
  return sm_typeName;
}

