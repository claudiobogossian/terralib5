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
#include "../geometry/MultiPolygon.h"
#include "../geometry/MultilineString.h"
#include "../geometry/MultiPoint.h"
#include "GeomReader.h"
#include "Utils.h"

// Terralib 4.x
#include <TeGeometry.h>

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

std::auto_ptr<te::gm::MultiPolygon> terralib4::GeomReader::getMultiPolygon(const TePolygonSet& polySet)
{
  std::auto_ptr<te::gm::MultiPolygon> geom(new te::gm::MultiPolygon(polySet.size(),te::gm::MultiPolygonType, polySet.srid()));

  TeComposite<TePolygon>::iterator it = polySet.begin();

  while(it != polySet.end())
  {
    geom->add(getPolygon(*it).release());

    ++it;
  }

  return geom;
}

std::auto_ptr<te::gm::MultiLineString> terralib4::GeomReader::getMultiLineString(const TeLineSet& lineSet)
{
  std::auto_ptr<te::gm::MultiLineString> geom(new te::gm::MultiLineString(lineSet.size(), te::gm::MultiLineStringType, lineSet.srid()));

  TeComposite<TeLine2D>::iterator it = lineSet.begin();

  while(it != lineSet.end())
  {
    geom->add(getLineString(*it).release());

    ++it;
  }

  return geom;
}

std::auto_ptr<te::gm::MultiPoint> terralib4::GeomReader::getMultiPoint(const TePointSet& pointSet)
{
  std::auto_ptr<te::gm::MultiPoint> geom(new te::gm::MultiPoint(pointSet.size(), te::gm::MultiPointType, pointSet.srid()));

  TeComposite<TePoint>::iterator it = pointSet.begin();

  while(it != pointSet.end())
  {
    geom->add(getPoint(*it).release());

    ++it;
  }

  return geom;
}

std::auto_ptr<te::gm::Polygon> terralib4::GeomReader::getPolygon(const TeCell& cell)
{
  std::auto_ptr<te::gm::Polygon> geom(new te::gm::Polygon(cell.size(), te::gm::PolygonType, cell.srid()));

  TeBox cellBox = cell.box();

  te::gm::LinearRing* s = new te::gm::LinearRing(5, te::gm::LineStringType);
  s->setPoint(0, cellBox.x1(), cellBox.y1());
  s->setPoint(1, cellBox.x2()-cellBox.x1(), cellBox.y1());
  s->setPoint(2, cellBox.x2(), cellBox.y2());
  s->setPoint(3, cellBox.x1(), cellBox.y2()-cellBox.y1());
  s->setPoint(4, cellBox.x1(), cellBox.y1());

  geom->add(s);

  return geom;
}