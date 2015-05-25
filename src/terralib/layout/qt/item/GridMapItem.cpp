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
  
  drawGrid(painter);

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

QColor te::layout::GridMapItem::rgbaToQColor( te::color::RGBAColor color )
{
  QColor clr;
  clr.setRed(color.getRed());
  clr.setGreen(color.getGreen());
  clr.setBlue(color.getBlue());
  clr.setAlpha(color.getAlpha());

  return clr;
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




















