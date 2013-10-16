/*  Copyright (C) 2009-2013 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/terralib4/GeomReader.cpp
   
  \brief An utility class for converting a TerraLib 4.x geometry to a TerraLib 5.
*/

// Terralib 5
#include "../geometry/Curve.h"
#include "../geometry/LinearRing.h"
#include "../geometry/LineString.h"
#include "../geometry/Point.h"
#include "../geometry/Polygon.h"
#include "GeomReader.h"
#include "Utils.h"

// Terralib 4.x
#include <terralib/kernel/TeGeometry.h>

std::auto_ptr<te::gm::Point> terralib4::GeomReader::getPoint(const TePoint& pt)
{
  std::auto_ptr<te::gm::Point> geom(new te::gm::Point(pt.box().x1(), pt.box().y1(), pt.srid()));
  return geom;
}

std::auto_ptr<te::gm::LineString> terralib4::GeomReader::getLineString(const TeLine2D& line)
{
  std::auto_ptr<te::gm::LineString> geom(new te::gm::LineString(te::gm::LineStringType, line.srid()));

  TeComposite<TeCoord2D>::iterator it = line.begin();

  int count = 0;
  while(it != line.end())
  {
    geom->setPoint(count, it->x(), it->y());

    ++count;
    ++it;
  }

  return geom;
}

std::auto_ptr<te::gm::LinearRing> terralib4::GeomReader::getLinearRing(const TeLinearRing& ring)
{
  std::auto_ptr<te::gm::LinearRing> geom(new te::gm::LinearRing(te::gm::PolygonType, ring.srid()));

  TeComposite<TeCoord2D>::iterator it = ring.begin();

  int count = 0;
  while(it != ring.end())
  {
    geom->setPoint(count, it->x(), it->y());

    ++count;
    ++it;
  }

  return geom;
}

std::auto_ptr<te::gm::Polygon> terralib4::GeomReader::getPolygon(const TePolygon& poly)
{
  std::auto_ptr<te::gm::Polygon> geom(new te::gm::Polygon(poly.size(), te::gm::PolygonType, poly.srid()));

  TeComposite<TeLinearRing>::iterator it = poly.begin();

  while(it != poly.end())
  {
    geom->push_back(getLinearRing(*it).release());

    ++it;
  }

  return geom;
}
