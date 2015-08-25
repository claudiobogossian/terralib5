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
  \file LineItem.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "LineItem.h"
#include "../../core/property/Property.h"

// STL
#include <vector>

// Qt
#include <QPointF>
#include <QGraphicsSceneMouseEvent>
#include <QStyleOptionGraphicsItem>
#include <QObject>
#include "../../core/property/Property.h"

te::layout::LineItem::LineItem( AbstractItemController* controller, AbstractItemModel* model, bool invertedMatrix ) 
  : AbstractItem<QGraphicsItem>(controller, model)
{  
  //m_nameClass = std::string(this->metaObject()->className());
}

te::layout::LineItem::~LineItem()
{

}

void te::layout::LineItem::drawItem( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget )
{
  QPolygonF poly = getQPolygon();
  if (poly.empty())
  {
    return;
  }

  const Property& pColor = m_controller->getProperty("color");
  const te::color::RGBAColor& color = pColor.getValue().toColor();
  QColor qColor(color.getRed(), color.getGreen(), color.getBlue(), color.getAlpha());

  painter->save();
  QPen penStyle = searchStyle();
  QPen pn(qColor, 0, penStyle.style());
  painter->setPen(pn);

  QPainterPath path;
  path.addPolygon(poly);

  //draws the item
  painter->drawPath(path);
  painter->restore();
}

QPen te::layout::LineItem::searchStyle()
{
  const Property& pLineStyle = m_controller->getProperty("line_style_type");
  QPen penStyle;

  if(pLineStyle.isNull() == false)
  {
    EnumLineStyleType lineStyle;

    const std::string& label = pLineStyle.getOptionByCurrentChoice().toString();
    EnumType* currentLineStyle = lineStyle.searchLabel(label);

    if(currentLineStyle == lineStyle.getStyleSolid())
    {
      penStyle.setStyle(Qt::SolidLine);
    }
    if(currentLineStyle == lineStyle.getStyleDash())
    {
      penStyle.setStyle(Qt::DashLine);
    }
    if(currentLineStyle == lineStyle.getStyleDot())
    {
      penStyle.setStyle(Qt::DotLine);
    }
    if(currentLineStyle == lineStyle.getStyleDashDot())
    {
      penStyle.setStyle(Qt::DashDotLine);
    }
    if(currentLineStyle == lineStyle.getStyleDashDotDot())
    {
      penStyle.setStyle(Qt::DashDotDotLine);
    }
    if(currentLineStyle == lineStyle.getStyleCustomDash())
    {
      penStyle.setStyle(Qt::CustomDashLine);
    }
  }
  return penStyle;
}

QPolygonF te::layout::LineItem::getQPolygon()
{
  const Property& pLine = m_controller->getProperty("geometry");

  const te::gm::GeometryShrPtr geometryPtr = pLine.getValue().toGeometry();

  te::gm::LineString const* lineString = dynamic_cast< te::gm::LineString const * > (geometryPtr.get());
  std::size_t nPoints = lineString->size();
  te::gm::Coord2D const* coordsPtr = lineString->getCoordinates();
  QPolygonF poly;
  for( std::size_t pIdx = 0 ; pIdx < nPoints ; ++pIdx )
  {
    QPointF pt(coordsPtr[pIdx].getX(), coordsPtr[pIdx].getY());
    //QPointF mlocal = mapFromScene(pt);
    poly.push_back(pt);
  }
  return poly;
}

