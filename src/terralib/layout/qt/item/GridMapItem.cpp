/*  Copyright (C) 2001-2014 National Institute For Space Research (INPE) - Brazil.

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
  \file GridMapItem.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "GridMapItem.h"
#include "../../core/pattern/mvc/ItemController.h"
#include "../../core/AbstractScene.h"
#include "../../core/pattern/mvc/Observable.h"
#include "../../../color/RGBAColor.h"
#include "../../../qt/widgets/Utils.h"
#include "../../../geometry/Envelope.h"
#include "../../../common/STLUtils.h"
#include "../../item/GridMapModel.h"

//Qt
#include <QStyleOptionGraphicsItem>

te::layout::GridMapItem::GridMapItem( ItemController* controller, Observable* o ) :
  ObjectItem(controller, o),
  m_maxWidthTextMM(0),
  m_maxHeigthTextMM(0)
{  
  m_nameClass = std::string(this->metaObject()->className());
}

te::layout::GridMapItem::~GridMapItem()
{

}

void te::layout::GridMapItem::paint( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget /*= 0 */ )
{
  ObjectItem::paint(painter, option, widget);

  Q_UNUSED( option );
  Q_UNUSED( widget );
  if ( !painter )
  {
    return;
  }

  drawBackground(painter);
  
  painter->save();
  
  QRectF parentBound = boundingRect();

  if(parentItem())
  {
    parentBound = parentItem()->boundingRect();
  }

  QPainterPath gridMapPath;
  gridMapPath.setFillRule(Qt::WindingFill);

  int heightRect = (int)parentBound.height();
  int widgetRect = (int)parentBound.width();

  painter->setPen(QPen(Qt::black, 0, Qt::SolidLine));

  QFont font("Arial", 12);
  painter->setFont(font);
  
  double mm = 0.3527777778;
  m_maxHeigthTextMM = mm * font.pointSize();

  QString text = "A";

  for (int i = 0; i <= heightRect; i+=10)
  {
    QLineF lineOne = QLineF(parentBound.topLeft().x(), parentBound.topLeft().y() + i, parentBound.topRight().x(), parentBound.topRight().y() + i);
    
    QPointF pointInit(parentBound.topLeft().x() - (heightRect*.01), parentBound.topLeft().y() + i - (m_maxHeigthTextMM/2)); //esquerda
    drawText(pointInit, painter, text.toStdString(), true);
    QPointF pointFinal(parentBound.topRight().x() + (heightRect*.01), parentBound.topRight().y() + i  - (m_maxHeigthTextMM/2)); //direita
    drawText(pointFinal, painter, text.toStdString());
           
    painter->drawLine(lineOne);

    for (int j = 0; j <= widgetRect; j+=10)
    {
      QLineF lineTwo = QLineF(parentBound.topLeft().x() + j, parentBound.topLeft().y(), parentBound.bottomLeft().x() + j, parentBound.bottomLeft().y());

      QPointF pointInit(parentBound.topLeft().x() + j + (m_maxWidthTextMM/2), boundingRect().topLeft().y() /*- (widgetRect*.01)*/); //inferior
      drawText(pointInit, painter, text.toStdString(), true);
      QPointF pointFinal(parentBound.bottomLeft().x() + j  - (m_maxWidthTextMM/2), parentBound.bottomLeft().y() + (widgetRect*.01)); //superior
      drawText(pointFinal, painter, text.toStdString());

      painter->drawLine(lineTwo);
    }    
  }
    
  painter->restore();

  //Draw Selection
  if (option->state & QStyle::State_Selected)
  {
    drawSelection(painter);
  }
}

QRectF te::layout::GridMapItem::boundingRect()
{
  if(parentItem())
  {
    if(parentItem()->boundingRect().isValid())
    {
      m_rect = parentItem()->boundingRect();
      m_rect.setWidth(m_rect.width() + m_maxWidthTextMM);
      m_rect.setX(m_rect.x() - m_maxWidthTextMM);
      m_rect.setHeight(m_rect.height() + m_maxHeigthTextMM);
      m_rect.setY(m_rect.y() - m_maxHeigthTextMM);
      return m_rect;
    }    
  }
  return m_rect;
}

void te::layout::GridMapItem::drawText( QPointF point, QPainter* painter, std::string text, bool displacementLeft /*= false*/, bool displacementRight /*= false*/ )
{
  painter->save();

  QTransform t = painter->transform();
  QPointF p = t.map(point);

  double zoomFactor = Context::getInstance().getZoomFactor();

  QFont ft = painter->font();
  ft.setPointSize(ft.pointSize() * zoomFactor);
  painter->setFont(ft);

  QFontMetrics fm(ft);
  int width = fm.width(text.c_str());

  QPointF newPoint (p);

  if(displacementLeft)
  {
    newPoint.setX(newPoint.x() - width);
  }

  if(displacementRight)
  {
    newPoint.setX(newPoint.x() + width);
  }

  QTransform copyT = painter->transform().inverted();
  QPointF copyP = copyT.map(newPoint);
  double widthMM = point.x() - copyP.x();

  if(widthMM > m_maxWidthTextMM)
  {
    m_maxWidthTextMM = widthMM;
  }

  //Keeps the size of the text.(Aspect)
  painter->setMatrixEnabled(false);
  painter->drawText(newPoint, text.c_str());
  painter->setMatrixEnabled(true);

  painter->restore();
}
