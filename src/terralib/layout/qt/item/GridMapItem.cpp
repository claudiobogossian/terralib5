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
  \file GridMapItem.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "GridMapItem.h"
#include "../../core/pattern/mvc/ItemController.h"
#include "../../core/AbstractScene.h"
#include "../../core/pattern/mvc/ItemModelObservable.h"
#include "../../../color/RGBAColor.h"
#include "../../../qt/widgets/Utils.h"
#include "../../../geometry/Envelope.h"
#include "../../../common/STLUtils.h"
#include "../../item/GridMapModel.h"
#include "../../core/WorldTransformer.h"
#include "MapItem.h"
#include "../../item/MapModel.h"

// Qt
#include <QStyleOptionGraphicsItem>

te::layout::GridMapItem::GridMapItem( ItemController* controller, Observable* o ) :
  ObjectItem(controller, o),
  m_maxWidthTextMM(0),
  m_maxHeigthTextMM(0),
  m_onePointMM(0.3527777778),
  m_changeSize(false)
{  
  m_nameClass = std::string(this->metaObject()->className());

  //The text size or length that exceeds the sides will be cut
  setFlag(QGraphicsItem::ItemClipsToShape);
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
  
  GridMapModel* model = dynamic_cast<GridMapModel*>(m_model);
  if(model)
  {
    if(model->isVisible() && parentItem())
    {
      drawGrid(painter);
    }
    else
    {
      drawDefaultGrid(painter);
    }
  }

  //Draw Selection
  if (option->state & QStyle::State_Selected)
  {
    drawSelection(painter);
  }
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
    m_changeSize = true;
  }

  //Keeps the size of the text.(Aspect)
  painter->setMatrixEnabled(false);
  painter->drawText(newPoint, text.c_str());
  painter->setMatrixEnabled(true);

  painter->restore();
}

QRectF te::layout::GridMapItem::boundingRect() const
{
  if(parentItem())
  {
    return parentItem()->boundingRect();
  }
  return m_rect;
}

void te::layout::GridMapItem::recalculateBoundingRect()
{
  if(!m_changeSize)
    return;

  if(parentItem())
  {    
    QRectF parentBoundRect = parentItem()->boundingRect();
    if(parentBoundRect.isValid())
    {
      QRectF boundRect = boundingRect();
      double w = parentBoundRect.width() + (m_maxWidthTextMM*2);
      double h = parentBoundRect.height() + (m_maxHeigthTextMM*2);
      if(boundRect.width() != w || boundRect.height() != h)
      {
        prepareGeometryChange();
        QRectF rect(0., 0., w, h);
        setRect(rect);
        
        //update model
        te::gm::Envelope box(m_model->getBox());
        box.m_urx = box.m_llx + w;
        box.m_ury = box.m_lly + h;
        m_controller->setBox(box);
      }
    } 
  }
  m_changeSize = false;
}

QVariant te::layout::GridMapItem::itemChange( QGraphicsItem::GraphicsItemChange change, const QVariant & value )
{
  if(change == QGraphicsItem::ItemParentHasChanged)
  {
    GridMapModel* model = dynamic_cast<GridMapModel*>(m_model);
    if(model)
    {
      if(parentItem())
      {
        MapItem* item = dynamic_cast<MapItem*>(parentItem());
        if(item)
        {
          if(item->getModel())
          {
            model->setMapName(item->getModel()->getName());
          }
        }
      }
    }
  }
  return QGraphicsItem::itemChange(change, value);
}

void te::layout::GridMapItem::drawGrid( QPainter* painter )
{

}

void te::layout::GridMapItem::drawVerticalLines()
{

}

void te::layout::GridMapItem::drawHorizontalLines()
{

}

bool te::layout::GridMapItem::hasLayer()
{
  bool result = false;

  MapItem* item = dynamic_cast<MapItem*>(parentItem());
  if(!item)
  {
    return result;    
  }

  MapModel* mapModel = dynamic_cast<MapModel*>(item->getModel());
  if(!mapModel)
  {
    return result;    
  }

  if(!mapModel->isLoadedLayer())
  {
    return result;
  }

  return true;
}

void te::layout::GridMapItem::configPainter( QPainter* painter )
{
  GridMapModel* model = dynamic_cast<GridMapModel*>(m_model);
  if(!model)
  {
    return;
  }

  QPen pen;

  EnumLineStyleType* lineStyle = Enums::getInstance().getEnumLineStyleType();
  if(model->getLineStyle() == lineStyle->getStyleSolid())
  {
    pen.setStyle(Qt::SolidLine);
  }
  else if(model->getLineStyle() == lineStyle->getStyleDot())
  {
    pen.setStyle(Qt::DotLine);
  }
  else if(model->getLineStyle() == lineStyle->getStyleDash())
  {
    pen.setStyle(Qt::DashLine);
  }
  else if(model->getLineStyle() == lineStyle->getStyleDashDot())
  {
    pen.setStyle(Qt::DashDotLine);
  }
  else if(model->getLineStyle() == lineStyle->getStyleDashDotDot())
  {
    pen.setStyle(Qt::DashDotDotLine);
  }

  te::color::RGBAColor lineColor = model->getLineColor();
  QColor clrLine = rgbaToQColor(lineColor);
  pen.setColor(clrLine);

  double lineWidth = model->getLineWidth();
  pen.setWidth(0);

  painter->setPen(pen);  
}

void te::layout::GridMapItem::configTextPainter( QPainter* painter )
{
  GridMapModel* model = dynamic_cast<GridMapModel*>(m_model);
  if(!model)
  {
    return;
  }

  QPen pen;

  int pointTextSize = model->getTextPointSize();
  std::string family = model->getFontFamily();

  QFont ft(family.c_str(), pointTextSize);
  te::color::RGBAColor textColor = model->getTextColor();
  
  painter->setFont(ft);

  QColor clrText = rgbaToQColor(textColor);
  pen.setColor(clrText);

  painter->setPen(pen);
}

void te::layout::GridMapItem::drawDefaultGrid( QPainter* painter )
{
  if(parentItem())
  {
    return;
  }

  GridMapModel* model = dynamic_cast<GridMapModel*>(m_model);
  if(!model)
  {
    return;
  }

  painter->save();

  QRectF parentBound = boundingRect();
  
  QPainterPath gridMapPath;
  gridMapPath.setFillRule(Qt::WindingFill);

  int heightRect = (int)parentBound.height();
  int widgetRect = (int)parentBound.width();
    
  QFont ft(model->getFontFamily().c_str(), model->getTextPointSize());
  painter->setFont(ft);

  // PostScript to mm
  m_maxHeigthTextMM = m_onePointMM * ft.pointSize();

  QString text = "GRID";

  for (int i = 0; i <= heightRect; i+=10)
  {
    QLineF lineOne = QLineF(parentBound.topLeft().x(), parentBound.topLeft().y() + i, parentBound.topRight().x(), parentBound.topRight().y() + i);

    configTextPainter(painter);

    QPointF pointInit(parentBound.topLeft().x(), parentBound.topLeft().y() + i - (m_maxHeigthTextMM/2)); //left
    drawText(pointInit, painter, text.toStdString(), true);
    QPointF pointFinal(parentBound.topRight().x(), parentBound.topRight().y() + i  - (m_maxHeigthTextMM/2)); //right
    drawText(pointFinal, painter, text.toStdString());

    configPainter(painter);

    painter->drawLine(lineOne);

    for (int j = 0; j <= widgetRect; j+=10)
    {
      QLineF lineTwo = QLineF(parentBound.topLeft().x() + j, parentBound.topLeft().y(), parentBound.bottomLeft().x() + j, parentBound.bottomLeft().y());

      configTextPainter(painter);

      QPointF pointInit(parentBound.topLeft().x() + j + (m_maxWidthTextMM/2), boundingRect().topLeft().y() + (m_maxHeigthTextMM)); //lower
      drawText(pointInit, painter, text.toStdString(), true);
      QPointF pointFinal(parentBound.bottomLeft().x() + j  - (m_maxWidthTextMM/2), parentBound.bottomLeft().y()); //upper
      drawText(pointFinal, painter, text.toStdString());

      configPainter(painter);

      painter->drawLine(lineTwo);
    }    
  }

  painter->restore();
}




















