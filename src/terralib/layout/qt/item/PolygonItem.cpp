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
  \file PolygonItem.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "PolygonItem.h"
#include "../../../color/RGBAColor.h"
#include "../../item/PolygonModel.h"
#include "AbstractItem.h"

// Qt
#include <QPointF>
#include <QGraphicsSceneMouseEvent>
#include <QStyleOptionGraphicsItem>
#include <QObject>

te::layout::PolygonItem::PolygonItem(AbstractItemController* controller, bool invertedMatrix)
  : LineItem(controller, invertedMatrix)
{ 
  
}

te::layout::PolygonItem::~PolygonItem()
{

}

void te::layout::PolygonItem::drawItem( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget )
{
  QPolygonF poly = getQPolygon();

  if (poly.empty())
  {
    return;
  }

  const Property& pColor = m_controller->getProperty("contour_color");
  const Property& pFillColor = m_controller->getProperty("fill_color");

  const te::color::RGBAColor& fillColor = pFillColor.getValue().toColor();
  const te::color::RGBAColor& color = pColor.getValue().toColor();

  QColor qFillColor(fillColor.getRed(), fillColor.getGreen(), fillColor.getBlue(), fillColor.getAlpha());
  QColor qColor(color.getRed(), color.getGreen(), color.getBlue(), color.getAlpha());

  QBrush brush(qFillColor);
  QPen pen(qColor, 0, Qt::SolidLine);

  painter->save();

  painter->setPen(pen);
  painter->setBrush(brush);

  //draws the item
  painter->drawPolygon(poly);

  painter->restore();
}
