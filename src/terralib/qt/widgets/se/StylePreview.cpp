/*  Copyright (C) 2011-2012 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/qt/widgets/se/StylePreview.cpp

  \brief Static methods used to generate preview of Symbology Enconding style elements.
*/

// TerraLib
#include "../../../maptools/CanvasConfigurer.h"
#include "../../../se/Rule.h"
#include "../../../se/Symbolizer.h"
#include "../canvas/Canvas.h"
#include "StylePreview.h"

// STL
#include <cassert>

QPixmap te::qt::widgets::StylePreview::build(te::se::Symbolizer* symb, te::gm::Geometry* geom, const QSize& size)
{
  assert(symb);
  assert(geom);
  assert(!size.isEmpty());

  // Creating a canvas...
  te::qt::widgets::Canvas canvas(size.width(), size.height());
  canvas.setWindow(0.0, 0.0, size.width(), size.height());
  canvas.setBackgroundColor(te::color::RGBAColor(0, 0, 0, TE_TRANSPARENT));

  // Configuring...
  te::map::CanvasConfigurer cc(&canvas);
  cc.config(symb);

  // Let's draw!
  canvas.draw(geom);

  return *canvas.getPixmap();
}

QPixmap te::qt::widgets::StylePreview::build(const std::vector<te::se::Symbolizer*>& symbs, te::gm::Geometry* geom, const QSize& size)
{
  QPixmap result(size);
  result.fill(Qt::transparent);

  // Let's draw!
  QPainter painter(&result);
  for(std::size_t i = 0; i < symbs.size(); ++i)
  {
    QPixmap pix = build(symbs[i], geom, size);
    painter.drawPixmap(0, 0, pix);
  }

  return result;
}

QPixmap te::qt::widgets::StylePreview::build(te::se::Rule* rule, te::gm::Geometry* geom, const QSize& size)
{
  assert(rule);

  return build(rule->getSymbolizers(), geom, size);
}
