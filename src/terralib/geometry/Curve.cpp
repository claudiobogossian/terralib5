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
  \file terralib/geometry/Curve.cpp

  \brief Curve is an abstract class that represents 1-dimensional geometric objects stored as a sequence of coordinates.
*/

// TerraLib
#include "../common/STLUtils.h"
#include "../common/Translator.h"
#include "Config.h"
#include "Curve.h"
#include "GEOSWriter.h"

#ifdef TERRALIB_GEOS_ENABLED
// GEOS
#include <geos/geom/Geometry.h>
#include <geos/util/GEOSException.h>
#endif

te::gm::Curve::Curve(GeomType t, int srid, Envelope* mbr)
  : Geometry(t, srid, mbr)
{
}

te::gm::Curve::Curve(const Curve& rhs)
  : Geometry(rhs)
{
}

double te::gm::Curve::getLength() const
{
#ifdef TERRALIB_GEOS_ENABLED
  std::auto_ptr<geos::geom::Geometry> g(GEOSWriter::write(this));

  return g->getLength();

#else
  throw Exception(TR_TR("getLength routine is supported by GEOS! Please, enable the GEOS support."));
#endif  
}

te::gm::Curve& te::gm::Curve::operator=(const Curve& rhs)
{
  Geometry::operator=(rhs);

  return *this;
}

bool te::gm::Curve::isRing() const
{
  return (isClosed() && isSimple());
}

te::gm::Dimensionality te::gm::Curve::getDimension() const throw()
{
  return te::gm::L;
}

