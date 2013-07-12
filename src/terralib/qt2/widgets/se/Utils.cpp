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
  \file terralib/qt/widgets/se/Utils.cpp
   
  \brief This file contains utility functions for dealing with Symbology Enconding and Qt concepts.
 */

// TerraLib
#include "../../../color/RGBAColor.h"
#include "../../../maptools2/Utils.h"
#include "../../../se/Stroke.h"
#include "../../../se/SvgParameter.h"
#include "Utils.h"

// Qt
#include <QtCore/QVector>
#include <QtGui/QBrush>
#include <QtGui/QPen>

void te::qt::widgets::Config(QPen& pen, const te::se::Stroke* stroke)
{
  if(stroke == 0)
  {
    pen.setColor(Qt::transparent);
    return;
  }

  // Color
  te::color::RGBAColor rgba(TE_SE_DEFAULT_STROKE_BASIC_COLOR, TE_OPAQUE);
  te::map::GetColor(stroke, rgba);
  QColor qrgba(rgba.getRgba());
  qrgba.setAlpha(rgba.getAlpha());
  pen.setColor(qrgba);

  // Width
  const te::se::SvgParameter* width = stroke->getWidth();
  if(width)
    pen.setWidth(te::map::GetInt(width));

  // Cap Style
  const te::se::SvgParameter* linecap = stroke->getLineCap();
  if(linecap)
  {
    std::string capValue = te::map::GetString(linecap);

    Qt::PenCapStyle capStyle = Qt::FlatCap;
    capValue == TE_SE_ROUND_CAP ? capStyle = Qt::RoundCap : capStyle = Qt::SquareCap;

    pen.setCapStyle(capStyle);
  }
  
  // Join Style
  const te::se::SvgParameter* linejoin = stroke->getLineJoin();
  if(linejoin)
  {
    std::string joinValue = te::map::GetString(linejoin);

    Qt::PenJoinStyle joinStyle = Qt::MiterJoin;
    joinValue == TE_SE_ROUND_JOIN ? joinStyle = Qt::RoundJoin : joinStyle = Qt::BevelJoin;

    pen.setJoinStyle(joinStyle);
  }

  // Dash Style
  const te::se::SvgParameter* dasharray = stroke->getDashArray();
  if(dasharray)
  {
    std::string value = te::map::GetString(dasharray);
    
    std::vector<double> pattern;
    te::map::GetDashStyle(value, pattern);
    
    pen.setDashPattern(QVector<qreal>::fromStdVector(pattern));
  }

  /* TODO: Is necessary verify stroke-dashoffset, <GraphicStroke>, and <GraphicFill> elements here ?! */
}

void te::qt::widgets::Config(QBrush& brush, const te::se::Fill* fill)
{
  if(fill == 0)
  {
    brush.setColor(Qt::transparent);
    return;
  }

  te::color::RGBAColor rgba(TE_SE_DEFAULT_FILL_BASIC_COLOR, TE_OPAQUE);
  te::map::GetColor(fill, rgba);
  QColor qrgba(rgba.getRgba());
  qrgba.setAlpha(rgba.getAlpha());
  brush.setColor(qrgba);

  /* TODO: Is necessary verify <GraphicFill> element here ?! */
}
