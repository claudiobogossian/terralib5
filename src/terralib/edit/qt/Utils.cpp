/*  Copyright (C) 2001-2009 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/edit/qt/Utils.cpp
   
  \brief Utility Qt functions for TerraLib Edit module.
*/

// TerraLib
#include "../../geometry/Geometry.h"
#include "../../geometry/LineString.h"
#include "../../geometry/Point.h"
#include "../../qt/widgets/canvas/Canvas.h"
#include "../../qt/widgets/Utils.h"
#include "../../srs/Config.h"
#include "../Utils.h"
#include "Utils.h"

// Qt
#include <QColor>

// STL
#include <cassert>

QPointF te::edit::GetPosition(QMouseEvent* e)
{
#if QT_VERSION >= 0x050000
  return e->localPos();
#else
  return e->posF();
#endif
}

void te::edit::DrawGeometry(te::qt::widgets::Canvas* canvas, te::gm::Geometry* geom, int srid)
{
  assert(canvas);
  assert(geom);

  switch(geom->getGeomTypeId())
  {
    case te::gm::PolygonType:
    case te::gm::PolygonZType:
    case te::gm::PolygonMType:
    case te::gm::PolygonZMType:
    case te::gm::MultiPolygonType:
    case te::gm::MultiPolygonZType:
    case te::gm::MultiPolygonMType:
    case te::gm::MultiPolygonZMType:
    {
      te::qt::widgets::Config2DrawPolygons(canvas, QColor(0, 255, 0, 80), Qt::black, 2);
    }
    break;

    case te::gm::LineStringType:
    case te::gm::LineStringZType:
    case te::gm::LineStringMType:
    case te::gm::LineStringZMType:
    case te::gm::MultiLineStringType:
    case te::gm::MultiLineStringZType:
    case te::gm::MultiLineStringMType:
    case te::gm::MultiLineStringZMType:
    {
      te::qt::widgets::Config2DrawLines(canvas, QColor(0, 0, 0, 80), 5);
    }
    break;
  }

  if((geom->getSRID() != TE_UNKNOWN_SRS) && (srid != TE_UNKNOWN_SRS) && (geom->getSRID() != srid))
    geom->transform(srid);

  canvas->draw(geom);
}

void te::edit::DrawVertexes(te::qt::widgets::Canvas* canvas, const std::vector<te::gm::Geometry*>& geom, int srid)
{
  if(geom.empty())
    return;

  assert(canvas);

  for(std::size_t i = 0; i < geom.size(); ++i)
    DrawVertexes(canvas, geom[i], srid);
}

void te::edit::DrawVertexes(te::qt::widgets::Canvas* canvas, te::gm::Geometry* geom, int srid)
{
  assert(canvas);
  assert(geom);

  std::vector<te::gm::LineString*> lines;
  GetLines(geom, lines);

  DrawVertexes(canvas, lines, srid);
}

void te::edit::DrawVertexes(te::qt::widgets::Canvas* canvas, const std::vector<te::gm::LineString*>& lines, int srid)
{
  if(lines.empty())
    return;

  assert(canvas);

  for(std::size_t i = 0; i < lines.size(); ++i)
    DrawVertexes(canvas, lines[i], srid);
}

void te::edit::DrawVertexes(te::qt::widgets::Canvas* canvas, te::gm::LineString* line, int srid)
{
  assert(canvas);
  assert(line);

  if((line->getSRID() != TE_UNKNOWN_SRS) && (srid != TE_UNKNOWN_SRS) && (line->getSRID() != srid))
    line->transform(srid);

  for(std::size_t j = 0; j < line->getNPoints(); ++j)
  {
    std::auto_ptr<te::gm::Point> point(line->getPointN(j));
    canvas->draw(point.get());
  }
}

void te::edit::DrawVertexes(te::qt::widgets::Canvas* canvas, const std::vector<te::gm::Coord2D>& coords, int fromSRID, int toSRID)
{
  if(coords.empty())
    return;

  assert(canvas);

  for(std::size_t i = 0; i < coords.size(); ++i)
  {
    std::auto_ptr<te::gm::Point> p(new te::gm::Point(coords[i].x, coords[i].y, fromSRID));

    if((fromSRID != TE_UNKNOWN_SRS) && (toSRID != TE_UNKNOWN_SRS) && (fromSRID != toSRID))
      p->transform(toSRID);

    canvas->draw(p.get());
  }
}
