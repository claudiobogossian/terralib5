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
  \file ItemGroup.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "PaperItem.h"
#include "../../../color/RGBAColor.h"

te::layout::PaperItem::PaperItem( AbstractItemController* controller, AbstractItemModel* model ) 
  : AbstractItem<QGraphicsItem>(controller, model)
{  

  this->setFlags(QGraphicsItem::ItemSendsGeometryChanges);
  //m_canChangeGraphicOrder = false;
}

te::layout::PaperItem::~PaperItem()
{

}

void te::layout::PaperItem::drawItem( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget )
{
  const Property& pPaperColor = m_controller->getProperty("paper_color");
  const Property& pShadowColor = m_controller->getProperty("shadow_color");
  const Property& pFrameColor = m_controller->getProperty("frame_color");
  const Property& pPaperWidth = m_controller->getProperty("paper_width");
  const Property& pPaperHeight = m_controller->getProperty("paper_height");
  const Property& pShadowPadding = m_controller->getProperty("shadow_padding");

  const te::color::RGBAColor& paperColor = pPaperColor.getValue().toColor();
  const te::color::RGBAColor& shadowColor = pShadowColor.getValue().toColor();
  const te::color::RGBAColor& frameColor = pFrameColor.getValue().toColor();
  double paperWidth = pPaperWidth.getValue().toDouble();
  double paperHeight = pPaperHeight.getValue().toDouble();
  double shadowPadding = pShadowPadding.getValue().toDouble();

  QColor qPaperColor(paperColor.getRed(), paperColor.getGreen(), paperColor.getBlue(), paperColor.getAlpha());
  QColor qShadowColor(shadowColor.getRed(), shadowColor.getGreen(), shadowColor.getBlue(), shadowColor.getAlpha());
  QColor qFrameColor(frameColor.getRed(), frameColor.getGreen(), frameColor.getBlue(), frameColor.getAlpha());

  QRectF boundRect;
  boundRect = boundingRect();

  painter->save();

  QBrush bsh(qShadowColor);
  painter->setBrush(bsh);
  QPen pen(qFrameColor);
  painter->setPen(pen);

  QRectF boxShadow(boundRect.x() + shadowPadding, boundRect.y() - shadowPadding, boundRect.width() - shadowPadding, boundRect.height() - shadowPadding);
  painter->drawRect(boxShadow);
  
  bsh.setColor(qPaperColor);
  painter->setBrush(bsh);

  QRectF boxPaper = QRectF(boundRect.x(), boundRect.y(), boundRect.width() - shadowPadding, boundRect.height() - shadowPadding);
  painter->drawRect(boxPaper);

  painter->restore();
}
