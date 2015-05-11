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
  \file terralib/geometry/Surface.cpp

  \brief Surface is an abstract class that represents a 2-dimensional geometric objects.
*/

// TerraLib
#include "../common/Translator.h"
#include "Surface.h"
#include "GEOSWriter.h"

#ifdef TERRALIB_GEOS_ENABLED
// GEOS
#include <geos/geom/Geometry.h>
#include <geos/util/GEOSException.h>
#endif

te::gm::Surface::Surface(GeomType t, int srid, Envelope* mbr)
  : Geometry(t, srid, mbr)
{
}

te::gm::Surface::Surface(const Surface& rhs)
  : Geometry(rhs)
{
}

double te::gm::Surface::getPerimeter() const
{
#ifdef TERRALIB_GEOS_ENABLED
  std::auto_ptr<geos::geom::Geometry> g(GEOSWriter::write(this));

  return g->getLength();

#else
  throw Exception(TE_TR("getLength routine is supported by GEOS! Please, enable the GEOS support."));
#endif  
}

te::gm::Surface& te::gm::Surface::operator=(const Surface& rhs)
{
  Geometry::operator=(rhs);

  return *this;
}

te::gm::Dimensionality te::gm::Surface::getDimension() const throw()
{
  return te::gm::A;
}

