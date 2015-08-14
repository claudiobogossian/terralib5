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
  \file ImageItem.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "ImageItem.h"
#include "../../core/pattern/mvc/ItemController.h"
#include "../../core/AbstractScene.h"
#include "../../core/pattern/mvc/Observable.h"
#include "../../../color/RGBAColor.h"
#include "../../../qt/widgets/Utils.h"
#include "../../../geometry/Envelope.h"
#include "../../../common/STLUtils.h"
#include "../../item/ImageModel.h"

// Qt
#include <QStyleOptionGraphicsItem>
#include "AbstractItem.h"

te::layout::ImageItem::ImageItem( AbstractItemController* controller, AbstractItemModel* model, bool invertedMatrix ) 
  : AbstractItem<QGraphicsItem>(controller, model)
{
}

te::layout::ImageItem::~ImageItem()
{

}

void te::layout::ImageItem::drawItem( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget )
{
  painter->save();

  const Property& pContourColor = m_controller->getProperty("contour_color");
  const Property& pBackgroundColor = m_controller->getProperty("background_color");

  const te::color::RGBAColor& contourColor = pContourColor.getValue().toColor();
  const te::color::RGBAColor& backgroundColor = pBackgroundColor.getValue().toColor();

  QColor qContourColor(contourColor.getRed(), contourColor.getGreen(), contourColor.getBlue(), contourColor.getAlpha());
  QColor qBackgroundColor(backgroundColor.getRed(), backgroundColor.getGreen(), backgroundColor.getBlue(), backgroundColor.getAlpha());
  
  QPen pen(qContourColor, 0, Qt::SolidLine);
  QBrush brush(qBackgroundColor);

  painter->setPen(pen);
  painter->setBrush(brush);
  painter->setRenderHint( QPainter::Antialiasing, true );

  if(m_image.isNull() == true)
  {
    return;
  }

  painter->save();

  QRectF boundRect = boundingRect();
  QRectF sourceRect(0, 0, m_image.width(), m_image.height());

  //draws the item
  painter->drawImage(boundRect, m_image, sourceRect);
  painter->restore();
}
